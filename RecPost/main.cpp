#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <algorithm>
#include <math.h>
#include <iostream>

#include <opencv2\opencv.hpp>
#include <vector>  


using namespace cv;
using namespace std;



Mat toBinary(Mat src);
bool ascendSort(vector<Point> a, vector<Point> b);
Mat getContour(Mat src, Mat binary);

Mat srcImage, binaryImage, contourImage;



Mat toBinary(Mat src)
{
	Mat temp = src.clone();
	int thresh = 110, maxValue = 254;
	cvtColor(temp, temp, CV_BGR2GRAY);//convert to gray image
	threshold(temp, temp, thresh, maxValue, THRESH_BINARY);//binary processing
	return temp;
}

bool ascendSort(vector<Point> a, vector<Point> b)
{
	return a.size() > b.size();
}

Mat getContour(Mat src, Mat binary)
{
	Mat temp = src.clone();
	vector< vector< Point> > contours;//save all contours data
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//find contours
	sort(contours.begin(), contours.end(), ascendSort);//ascending sort
	vector< vector<Point> >::iterator itc = contours.begin(); //iterator of contour vector
	int i = 0;
	while (itc != contours.end())
	{
		if (itc->size() > 150)//ignore the small object
		{
			if (i == 1)
			{
				Rect rect = boundingRect(*itc);//get the rectangle bounding
				rectangle(temp, rect, { 0, 0, 255 }, 2, 8);//draw the rectangle
				RotatedRect resultRect;
				resultRect = minAreaRect(*itc);//get the min area rectangle   
				Point2f pt[4];
				resultRect.points(pt);//get the coordinate of vertex
				//draw the min area rectangle
				line(temp, pt[0], pt[1], Scalar(255, 0, 0), 2, 8);
				line(temp, pt[1], pt[2], Scalar(255, 0, 0), 2, 8);
				line(temp, pt[2], pt[3], Scalar(255, 0, 0), 2, 8);
				line(temp, pt[3], pt[0], Scalar(255, 0, 0), 2, 8);
				cout << "**************形心*************" << endl;
				cout << "X坐标：" << resultRect.center.x << " Y坐标：" << resultRect.center.y << " 偏转角度：" << resultRect.angle << endl;
				cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << contourArea(contours[i], true) << endl;
				cout << "*******************************" << endl;
			}
			i++;
		}
		++itc;
	}
	return temp;
}

Mat findMaxContour()
{
	Mat srcImage = imread("1.png");
	imshow("【原图】", srcImage);

	//首先对图像进行空间的转换  
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	//对灰度图进行滤波  
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	//imshow("【滤波后的图像】", grayImage);

	//为了得到二值图像，对灰度图进行边缘检测  
	Mat cannyImage;
	//Canny(grayImage, cannyImage, 128, 255, 3);
	threshold(grayImage, cannyImage, 254, 255, CV_THRESH_BINARY);
	//在得到的二值图像中寻找轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(cannyImage, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//find contours
	sort(contours.begin(), contours.end(),ascendSort);//ascending sort ！有问题 要改进

	//求面积绝对值最大的轮廓
	/*double max = 0.0;
	int index = 0;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		if (max < abs(g_dConArea)) {
			max = abs(g_dConArea);
			index = i;
		}
	}*/

	//显示排序后的结果
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
	}


	Mat result(srcImage.size(), CV_8U, Scalar(128));
	drawContours(result, contours, 0, Scalar(255), 1); // 最后一个轮廓，即外轮廓  
	//imshow("result", result);
	return result;
}

int main()
{
	//srcImage = imread("1.png");//input image
	//imshow("srcImage", srcImage);

	//binaryImage = toBinary(srcImage);//convert to binary image
	//imshow("binaryImage", binaryImage);

	//contourImage = getContour(srcImage, binaryImage);
	//imshow("contourImage", contourImage);

	Mat srcImage = imread("1.png");
	imshow("【原图】", srcImage);

	//首先对图像进行空间的转换  
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	//对灰度图进行滤波  
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	//imshow("【滤波后的图像】", grayImage);

	//为了得到二值图像，对灰度图进行边缘检测  
	Mat cannyImage;
	//Canny(grayImage, cannyImage, 128, 255, 3);
	threshold(grayImage, cannyImage, 254, 255, CV_THRESH_BINARY);
	//在得到的二值图像中寻找轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(cannyImage, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);//find contours
	sort(contours.begin(), contours.end(), ascendSort);//ascending sort ！有问题 要改进

	//显示排序后的结果
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
	}

	//Mat result(srcImage.size(), CV_8U, Scalar(128));
	Mat result = srcImage.clone();
	//指定画哪一个轮廓
	int a = 0;
	drawContours(result, contours, a, Scalar(255), 2); // 若a=-1，则画出所有轮廓
	

	Rect rect = boundingRect(contours[a]);//get the rectangle bounding
	rectangle(result, rect, { 0, 0, 255 }, 2, 8);//draw the rectangle
	RotatedRect resultRect;
	resultRect = minAreaRect(contours[a]);//get the min area rectangle   
	Point2f pt[4];
	resultRect.points(pt);//get the coordinate of vertex
	//draw the min area rectangle
	line(result, pt[0], pt[1], Scalar(255, 0, 0), 2, 8);
	line(result, pt[1], pt[2], Scalar(255, 0, 0), 2, 8);
	line(result, pt[2], pt[3], Scalar(255, 0, 0), 2, 8);
	line(result, pt[3], pt[0], Scalar(255, 0, 0), 2, 8);
	cout << "**************形心*************" << endl;
	cout << "X坐标：" << resultRect.center.x << " Y坐标：" << resultRect.center.y << " 偏转角度：" << resultRect.angle << endl;
	cout << "*******************************" << endl;
	imshow("result", result);

	waitKey(0);
	return 0;
}