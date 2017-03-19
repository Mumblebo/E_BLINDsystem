#include <opencv\cv.h>
#include <iostream>
#include "EBLIND.h"
using namespace std;
using namespace cv;

void main(int arg,char *argc[])
{
	std::string temp;
	EBLIND s1 = EBLIND();
//	cout << "input the file path:" << endl;
//	cin >> temp;
//	EBLIND s1 = EBLIND();
//	s1.readAndShow(temp);
	s1.GenerateWaterMark(512);

}
