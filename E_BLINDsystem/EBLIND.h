#pragma once
#include<iostream>
#include<memory>
class EBLIND
{
public:
	EBLIND();
	~EBLIND();
	void embedWM(std::string srcpath,std::string wmpath); // ����Դ�ļ���ˮӡ�ļ��Ĵ���
	void readAndShow(std::string filepath);  // open and read image fi
	void GenerateWaterMark(int size);        //����ʵ����Ҫ��Ԥ������256*256 �� 512*512��refermce pattern
	void embedWM(std::string filepath); //����ˮӡ�Ĳ�����ˮӡ������Ԥ�����ɺõ�40��ˮӡ�е�һ��
	void SetParamters(); 
private:
	int a, m; // ���ڽ�
	std::string filepath;
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

};