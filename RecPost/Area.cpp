#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/types_c.h>  
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat srcImage = imread("1.png");
	imshow("【原图】", srcImage);

	//首先对图像进行空间的转换  
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	//对灰度图进行滤波  
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	imshow("【滤波后的图像】", grayImage);

	//为了得到二值图像，对灰度图进行边缘检测  
	Mat cannyImage;
	//Canny(grayImage, cannyImage, 128, 255, 3);
	threshold(grayImage, cannyImage, 254, 255, CV_THRESH_BINARY);
	//在得到的二值图像中寻找轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));


	Mat result(srcImage.size(), CV_8U, Scalar(0));
	drawContours(result, contours, contours.size()-1, Scalar(255), 1); // -1 表示所有轮廓  
	namedWindow("result");
	imshow("result", result);
	waitKey(0);
	return 0;

}
