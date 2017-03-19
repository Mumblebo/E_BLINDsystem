#include "EBLIND.h"
#include<iostream>
#include<cv.h>
#include<Windows.h>
#include<time.h>
#include<highgui\highgui.hpp>
using namespace std;
using namespace cv;

EBLIND::EBLIND()
{

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
		srand(time(0));
		for (auto i = 0; i < rows; i++)
		{
			p = wm1.ptr<uchar>(i);
			for (auto j = 0; j < cols; j++)
			{
				p[j] = (int)rand() % 256;
			}
		}
		cout << "hello" << endl;
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
		Sleep(1000);
	}
}

void EBLIND::embedWM(std::string srcpath,std::string wmpath)
{
	Mat src = imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
	Mat wm = imread(wmpath, CV_LOAD_IMAGE_UNCHANGED);
	auto nrows = src.rows;
	auto ncols = src.cols;
	auto mrows = wm.rows;
	auto mcols = wm.cols;
	if (!(nrows == 256 && ncols == 256 || nrows == 512 && ncols == 512))
	{
		cout << "size of the given file is illeagle" << endl;
		return;
	}
	// 加上水印
}