#pragma once
#include<iostream>
#include<cv.h>
#include<memory>
#include<highgui\highgui.hpp>

class EBLIND
{
public:
	EBLIND();
	~EBLIND();
	void embedWM(std::string srcpath,std::string wmpath); // 给出源文件和水印文件的处理
	void readAndShow(std::string filepath);  // open and read image fi
	void GenerateWaterMark(int size);        //根据实验需要先预先生成256*256 和 512*512的refermce pattern
	void embedWM(std::string filepath); //加上水印的操作，水印可能是预先生成好的40张水印中的一张
	void SetParamters(int aa, int mm, double zz){ 
		a = aa;
		m = mm;
		zlc = zz;
	}
	void setSave(bool temp)
	{
		saveOrNot = temp;
	}
	int checkWM(std::string srcpath,std::string wmpath); //根据检测值和阈值的比较，返回0或者1，或者-1表示没有水印
	int checkWM(const cv::Mat &src, const cv::Mat &wm);
protected:
	void SaveImg(const cv::Mat &src, const std::string srcName, const std::string wmName);
private:
	bool saveOrNot;   //加完水印之后的图片要不要保存
	int a, m; // 用于将
	std::string filepath;
	double zlc;
};

class ErrorInfo
{
public:
	ErrorInfo(std::string str);
	~ErrorInfo();


	std::string getInfo()
	{
		return info;
	}
private:
	std::string info;
	std::shared_ptr<std::string> infoptr;
	double bound;

};