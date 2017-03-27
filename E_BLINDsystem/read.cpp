#include <opencv\cv.h>
#include <iostream>
#include "EBLIND.h"
#include <memory>
#include<random>
#include <functional>
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
//	s1.GenerateWaterMark(256);
	s1.setSave(true);
	s1.SetParamters(1, 1, 0.7);
	s1.embedWM("./input/bank.BMP","./waterMark/256-0.bmp");
	std::default_random_engine generator(time(NULL));
	std::normal_distribution<double> dis(15.0, 46.67);
	auto dice = std::bind(dis, generator);
	int tep;
	cout << "----------------------\n";
	for (int i = 0; i<2550; i++)
	{
		tep = dice();
		if (tep==0||tep == 255)
			std::cout << tep << std::endl;
	}
	system("pause");
}
