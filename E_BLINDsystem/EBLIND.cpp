#include "EBLIND.h"
#include<iostream>
#include<cv.h>
#include<Windows.h>
#include<time.h>
#include<random>
#include <functional>
#include<fstream>
#include<highgui\highgui.hpp>
using namespace std;
using namespace cv;


ErrorInfo::ErrorInfo(std::string str)
{
	this->info = str;
}

ErrorInfo::~ErrorInfo()
{

}

EBLIND::EBLIND()
{
	this->eightbitCut = true;  //默认采用8bit截断方式
	this->SavePath = "./result/";  //默认保存加完水印之后的图片路径
	this->a = 1; // 默认的嵌入强度为1
}

EBLIND::~EBLIND()
{

}

void EBLIND::readAndShow(std::string filepath)
{
	Mat src = imread(filepath, CV_LOAD_IMAGE_UNCHANGED);
	// no data
	if (src.empty())
	{
		cout << "pleass give the leagle file path" << endl;
		return;
	}
	cout << src.rows << " " << src.cols << endl;
	uchar *p = src.data;
	int channels = src.channels();
	int nRows = src.rows;
	int nCols = src.cols * channels;
	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	int i, j;
	for (i = 0; i < nRows; ++i)
	{
		p = src.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			cout << (int)p[j] << ",";
		}
	}
	namedWindow("1", WINDOW_AUTOSIZE);
	imshow("1", src);
	waitKey(0);
	return;
}
/*
Function: generate WaterMark images with certain 
author: @Qiuxl
date: 2017/03/19
*/
void EBLIND::GenerateWaterMark(int size)             //根据实验需要先预先生成256*256 和 512*512的refermce pattern
{
	int div[] = { size, size }; //实验中给的除了256*256就是512*512 
	Mat wm1(2, div, CV_8UC1, Scalar::all(0)); //初始化一张256*256的大小灰度值全部为0的图片
	imwrite("256-1.bmp", wm1);  // 全是0的水印
	std::string filename("");
	auto channels = wm1.channels();
	std::stringstream ss;   //用于int转化为string
	float sigma = 42.666; // 作为正态分布的方差
	int miu = 0, dotpix;        //记录正态分布的均值
	string temp("");
	string filesize;
	ss << size;
	ss >> filesize;
	auto rows = wm1.rows;
	auto cols = wm1.cols * channels;
	if (wm1.isContinuous())
	{
		cols *= rows;
 		rows = 1;
	}
	uchar *p = wm1.data;

	for (auto k = 0; k < 40; k++)
	{
		std::default_random_engine generator(time(NULL));
		std::uniform_int_distribution<int> dis(30, 40); //保证值均落在 -128 到 128 之间 
		auto dice = std::bind(dis, generator);
		sigma = dice();  //随机产生一个数作为方差
		std::normal_distribution<double> nids(miu, 1.0*sigma);
		auto nordice = std::bind(nids, generator);
		for (auto i = 0; i < rows; i++)
		{
			p = wm1.ptr<uchar>(i);
			for (auto j = 0; j < cols; j++)
			{
				while (1)
				{
					dotpix = (int)nordice();
					if (dotpix >= -128 && dotpix <= 127)
						break;
				}
				p[j] = 127 - dotpix;  //对于 0 -127之间的灰度值，还是正数，对于-128 到 -1之间的采用 128到255保存
			}
		}
		cout << "hello, 高斯分布平均值: "<< miu <<  " fangcha :" <<  sigma <<endl;
		if (!temp.empty())
			temp.clear();
		if (k>0)
		{
			filename.clear();
		}
		ss.clear(); //清除原来已经输入的字符
		ss.str("");
		ss << k;
		ss >> temp;
		filename.append("-" + temp + ".bmp");
		imwrite("./waterMark/" + filesize +filename, wm1);
		Sleep(2000);
	}
}

void EBLIND::embedWM(std::string srcpath, std::string wmpath)
{
	Mat src = imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
	Mat wm = imread(wmpath, CV_LOAD_IMAGE_UNCHANGED);
//	imshow("picture1", src);
//	imshow("picture2", wm);
	waitKey(0);
	auto nrows = src.rows;
	auto ncols = src.cols;
	auto mrows = wm.rows;
	auto mcols = wm.cols;
	int tempWat;
	int MaxSrc = -1, MinSrc = 256, MaxWm = -1, MinWm = 256;
	int MaxVal = -256, MinVal = 511, tmpval, Gap;
	if (nrows != mrows || mcols != ncols)  // 图片与给定的大小不同
	{
		throw new ErrorInfo("watermark and picture have diffierent size!");
	}
	// 加上水印
	// m = 1证明是加算，这个时候需要考虑最大和最小值之间的差距，规格化图片

	uchar *pSrc = src.data;
	uchar *pWm = wm.data;

	if (pSrc == NULL || pWm == NULL)
		throw new ErrorInfo("one of the given picture is empty!");
	//第一次遍历是为了得到两张图片合成之后图片的灰度值中的最大和最小值

	int size[] = { mrows, mcols };
	Mat Target(2, size, CV_8UC1, Scalar::all(0));
	uchar *pTar = Target.data;
	for (auto i = 0; i < nrows; ++i)
	{
		pSrc = src.ptr<uchar>(i);
		pWm = wm.ptr<uchar>(i);
		pTar = Target.ptr<uchar>(i);
		for (auto j = 0; j < ncols; ++j)
		{
			tempWat = (int)pWm[j];

			tempWat = -(tempWat - 127);

		//	tempWat /= this->scale;
			if (this->m == 1)
				tmpval = (int)pSrc[j] + a *tempWat;
			else //m =0 的情况
				tmpval = (int)pSrc[j] - a *tempWat;
			if (tmpval > MaxVal)
				MaxVal = tmpval;
			if (tmpval < MinVal)
				MinVal = tmpval;
			if ((MinVal >= 0 && MaxVal <= 255) || this->eightbitCut == true)
			{
				pTar[j] = (uchar)tmpval;
			}
		}
	}
	Gap = MaxVal - MinVal; // 
	if (MaxVal <= 255 && MinVal >= 0 || this->eightbitCut)
	{
		// 不采取任何操作
	}
	else
	{  //必定是MAX > 255，超过了灰度值的上限
		// 这里不考虑两者相等并且都超过了255的情况，对于符合高斯分布的水印和有局部特征的图片，在256*256像素点的情况下几乎不可能出现这种情况
		for (auto i = 0; i < nrows; ++i)
		{
			pSrc = src.ptr<uchar>(i);
			pWm = wm.ptr<uchar>(i);
			pTar = Target.ptr<uchar>(i);
			for (auto j = 0; j < ncols; ++j)
			{
				tempWat = (int)pWm[j];

				tempWat = -(tempWat - 127);

				tempWat /= this->scale;
				if (this->m == 1)
					tmpval = (int)pSrc[j] + a *tempWat;
				else
					tmpval = (int)pSrc[j] - a *tempWat;
;
					//以最最小值为基准，等比例放大
				pTar[j] = (uchar)((tmpval - MinVal)*255 / Gap);
			}
		}
		//加水印完毕，保存图片
	}
	string srcName, wmName;
	if (this->saveOrNot)
	{
		auto index1 = srcpath.rfind('/');
		auto index2 = wmpath.rfind('/');
		if (index1 != std::string::npos)
		{
			srcName.assign(srcpath, index1 + 1, srcpath.length() - index1 - 1);
		//	cout << srcName << endl;
			
		}
		else{
			srcName = srcpath;
			
		}
		if (index2 != std::string::npos)
		{
			wmName.assign(wmpath, index2 + 1, wmpath.length() - index2 - 1);
		//	cout << wmName << endl;
		}
		else
		{
			wmName = wmpath;
		}
		SaveImg(Target, srcName, wmName);
	}
	
}



int EBLIND::checkWM(const cv::Mat &src, const cv::Mat &wm)
{

	auto srows = src.rows;
	auto scols = src.cols;
	auto wrows = wm.rows;
	auto wcols = wm.cols;
	int tempWat = 0;
	fstream fout;
	fout.open("detectValueForPart4.txt", ios::out | ios::app);
	if (!(srows == wrows && scols == wcols))
	{
		throw new ErrorInfo("source image and watermark have different size!");
	}
	if (!(srows == wrows && scols == wcols))
	{
		throw new ErrorInfo("source image and watermark have different size!");
	}
	int temp = 0, middle, count = 0;
	double result;
	const uchar *pSrc = src.data;
	const uchar *pWm = wm.data;
	if (pSrc == NULL || pWm == NULL)
	{
		throw new ErrorInfo("one(or both) of the picture is empty");
	}
	for (int i = 0; i < srows; i++)
	{
		for (int j = 0; j < scols; j++)
		{
			pSrc = src.ptr<uchar>(i);
			pWm = wm.ptr<uchar>(i);
			tempWat = (int)pWm[j];
			tempWat = -(tempWat - 127);
			tempWat /= scale;
			middle = (int)pSrc[j] * tempWat;
			if (middle <= 1)
			{
				count++;
			}
			temp += middle;
		}
	}
//	cout << "temp: " << temp << endl;
	result = temp*1.0 / (srows * scols);
//	cout << "smaller than 1: " << count << endl;
	cout << "result: " << result << endl;
	fout << result << endl;
	fout.close();
	if (result > this->zlc)
	{
		return 1;
	}
	else if (result < -zlc)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int EBLIND::checkWM(std::string srcpath, std::string wmpath)
{
	// 这里应该是直接用两张图进行计算相似性，不是用简化后的公式
	Mat src = imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
	Mat wm = imread(wmpath, CV_LOAD_IMAGE_UNCHANGED);
	cout << "result for " << srcpath << " " << wmpath << endl;
	return checkWM(src, wm);
}

void EBLIND::SaveImg(const cv::Mat &src, const std::string srcName, const std::string wmName)
{
	auto index = srcName.find('.');
	string part1, part2;
	part1.assign(srcName, 0, index);
	index = wmName.find('.');
	part2.assign(wmName, 0, index);

	string finalName = SavePath;
	finalName.append(part1);
	finalName.append("+");
	finalName.append(part2);
	finalName.append(".bmp");

	imwrite(finalName, src);
}
