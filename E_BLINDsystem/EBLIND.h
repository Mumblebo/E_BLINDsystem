#pragma once
#include<iostream>
#include<memory>
class EBLIND
{
public:
	EBLIND();
	~EBLIND();
	void embedWM(std::string srcpath,std::string wmpath); // 给出源文件和水印文件的处理
	void readAndShow(std::string filepath);  // open and read image fi
	void GenerateWaterMark(int size);        //根据实验需要先预先生成256*256 和 512*512的refermce pattern
	void embedWM(std::string filepath); //加上水印的操作，水印可能是预先生成好的40张水印中的一张
	void SetParamters(); 
private:
	int a, m; // 用于将
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