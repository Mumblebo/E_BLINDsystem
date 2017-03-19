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
void EBLIND::GenerateWaterMark(int size)             //����ʵ����Ҫ��Ԥ������256*256 �� 512*512��refermce pattern
{
	int div[] = { size, size }; //ʵ���и��ĳ���256*256����512*512 
	Mat wm1(2, div, CV_8UC1, Scalar::all(0)); //��ʼ��һ��256*256�Ĵ�С�Ҷ�ֵȫ��Ϊ0��ͼƬ
	imwrite("256-1.bmp", wm1);  // ȫ��0��ˮӡ
	std::string filename("");
	auto channels = wm1.channels();
	std::stringstream ss;   //����intת��Ϊstring
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
		ss.clear(); //���ԭ���Ѿ�������ַ�
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
	// ����ˮӡ
}