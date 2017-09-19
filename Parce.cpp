#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "Parse.h"
void findX(IplImage* imgSrc, int* min, int* max)	//Ѱ��x�߽�
{
	int i;
	int minFound = 0;
	CvMat data;
	CvScalar maxVal = cvRealScalar(imgSrc->height * 255);//��ɫ����255
	CvScalar val = cvRealScalar(0);

	for (i = 0; i< imgSrc->width; i++)
	{
		cvGetCol(imgSrc, &data, i);
		val = cvSum(&data);
		if (val.val[0] < maxVal.val[0])
		{
			*max = i;
			if (!minFound)
			{
				*min = i;
				minFound = 1;
			}
		}
	}
}

void findY(IplImage* imgSrc, int* min, int* max)	//Ѱ��y�߽�
{
	int i;
	int minFound = 0;
	CvMat data;
	CvScalar maxVal = cvRealScalar(imgSrc->width * 255);
	CvScalar val = cvRealScalar(0);

	for (i = 0; i< imgSrc->height; i++)
	{
		cvGetRow(imgSrc, &data, i);
		val = cvSum(&data);
		if (val.val[0] < maxVal.val[0])
		{
			*max = i;
			if (!minFound)
			{
				*min = i;
				minFound = 1;
			}
		}
	}
}
CvRect findBB(IplImage* imgSrc)	//Ѱ��ͼƬ�߽�
{
	CvRect aux;
	int xmin, xmax, ymin, ymax;
	xmin = xmax = ymin = ymax = 0;

	findX(imgSrc, &xmin, &xmax);
	findY(imgSrc, &ymin, &ymax);

	aux = cvRect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
	return aux;
}

IplImage preprocessing(IplImage* imgSrc, int new_width, int new_height, int type)	//��ͼƬԤ����
{
	IplImage* result;
	IplImage* scaledResult;

	CvMat data;
	CvMat dataA;
	CvRect bb;

	//�ҵ��߽��
	bb = findBB(imgSrc);

	cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));

	int size = (bb.width>bb.height) ? bb.width : bb.height;
	result = cvCreateImage(cvSize(size, size), 8, type);
	cvSet(result, CV_RGB(255, 255, 255), NULL);
	//��ͼ��������м䣬��С��һ��
	int x = (int)floor((float)(size - bb.width) / 2.0f);
	int y = (int)floor((float)(size - bb.height) / 2.0f);
	cvGetSubRect(result, &dataA, cvRect(x, y, bb.width, bb.height));
	cvCopy(&data, &dataA, NULL);

	scaledResult = cvCreateImage(cvSize(new_width, new_height), 8, type);
	cvResize(result, scaledResult, CV_INTER_NN);

	return *scaledResult;

}