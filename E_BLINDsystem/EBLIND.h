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
	void embedWM(std::string srcpath,std::string wmpath); // ����Դ�ļ���ˮӡ�ļ��Ĵ���
	void readAndShow(std::string filepath);  // open and read image fi
	void GenerateWaterMark(int size);        //����ʵ����Ҫ��Ԥ������256*256 �� 512*512��refermce pattern
	void embedWM(std::string filepath); //����ˮӡ�Ĳ�����ˮӡ������Ԥ�����ɺõ�40��ˮӡ�е�һ��
	void SetParamters(int aa, int mm, double zz){ 
		a = aa;
		m = mm;
		zlc = zz;
	}
	void setSave(bool temp)
	{
		saveOrNot = temp;
	}
	int checkWM(std::string srcpath,std::string wmpath); //���ݼ��ֵ����ֵ�ıȽϣ�����0����1������-1��ʾû��ˮӡ
	int checkWM(const cv::Mat &src, const cv::Mat &wm);
protected:
	void SaveImg(const cv::Mat &src, const std::string srcName, const std::string wmName);
private:
	bool saveOrNot;   //����ˮӡ֮���ͼƬҪ��Ҫ����
	int a, m; // ���ڽ�
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