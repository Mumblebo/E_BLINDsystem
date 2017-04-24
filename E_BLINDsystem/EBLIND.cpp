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
	this->eightbitCut = true;  //Ĭ�ϲ���8bit�ضϷ�ʽ
	this->SavePath = "./result/";  //Ĭ�ϱ������ˮӡ֮���ͼƬ·��
	this->a = 1; // Ĭ�ϵ�Ƕ��ǿ��Ϊ1
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
void EBLIND::GenerateWaterMark(int size)             //����ʵ����Ҫ��Ԥ������256*256 �� 512*512��refermce pattern
{
	int div[] = { size, size }; //ʵ���и��ĳ���256*256����512*512 
	Mat wm1(2, div, CV_8UC1, Scalar::all(0)); //��ʼ��һ��256*256�Ĵ�С�Ҷ�ֵȫ��Ϊ0��ͼƬ
	imwrite("256-1.bmp", wm1);  // ȫ��0��ˮӡ
	std::string filename("");
	auto channels = wm1.channels();
	std::stringstream ss;   //����intת��Ϊstring
	float sigma = 42.666; // ��Ϊ��̬�ֲ��ķ���
	int miu = 0, dotpix;        //��¼��̬�ֲ��ľ�ֵ
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
		std::uniform_int_distribution<int> dis(30, 40); //��ֵ֤������ -128 �� 128 ֮�� 
		auto dice = std::bind(dis, generator);
		sigma = dice();  //�������һ������Ϊ����
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
				p[j] = 127 - dotpix;  //���� 0 -127֮��ĻҶ�ֵ����������������-128 �� -1֮��Ĳ��� 128��255����
			}
		}
		cout << "hello, ��˹�ֲ�ƽ��ֵ: "<< miu <<  " fangcha :" <<  sigma <<endl;
		if (!temp.empty())
			temp.clear();
		if (k>0)
		{
			filename.clear();
		}
		ss.clear(); //���ԭ���Ѿ�������ַ�
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
	if (nrows != mrows || mcols != ncols)  // ͼƬ������Ĵ�С��ͬ
	{
		throw new ErrorInfo("watermark and picture have diffierent size!");
	}
	// ����ˮӡ
	// m = 1֤���Ǽ��㣬���ʱ����Ҫ����������Сֵ֮��Ĳ�࣬���ͼƬ

	uchar *pSrc = src.data;
	uchar *pWm = wm.data;

	if (pSrc == NULL || pWm == NULL)
		throw new ErrorInfo("one of the given picture is empty!");
	//��һ�α�����Ϊ�˵õ�����ͼƬ�ϳ�֮��ͼƬ�ĻҶ�ֵ�е�������Сֵ

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
			else //m =0 �����
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
		// ����ȡ�κβ���
	}
	else
	{  //�ض���MAX > 255�������˻Ҷ�ֵ������
		// ���ﲻ����������Ȳ��Ҷ�������255����������ڷ��ϸ�˹�ֲ���ˮӡ���оֲ�������ͼƬ����256*256���ص������¼��������ܳ����������
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
					//������СֵΪ��׼���ȱ����Ŵ�
				pTar[j] = (uchar)((tmpval - MinVal)*255 / Gap);
			}
		}
		//��ˮӡ��ϣ�����ͼƬ
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
	// ����Ӧ����ֱ��������ͼ���м��������ԣ������ü򻯺�Ĺ�ʽ
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
