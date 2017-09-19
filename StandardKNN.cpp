#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "StandardKNN.h"
#include "Parse.h"
#include <iostream>
using namespace std;
StandardKNN::StandardKNN()//���캯��
{
	sprintf(file_path, "E:/1work/6.code/Number-recognition-master/Number-recognition-master/NumberParser/NumberParser/OCR/");//�洢ͼƬ�ĵط�
	train_samples = 80;
	classes = 10;//ʶ��ʮ������
	size = 128;//ͼƬ��/��	
	printf("Start Training...\n");
	trainData = cvCreateMat(train_samples*classes, size*size, CV_32FC1);//ѵ�����ݵľ���
	trainClasses = cvCreateMat(train_samples*classes, 1, CV_32FC1);//��ǩ����
	getData();//������ͼƬ����װ�ص�ѵ��������
	//����ѵ��
	train();
	//����Ĭ�����ݼ���ȷ��
	time_t st, ed;
	st = time(NULL);
	///test();
	ed = time(NULL);
	printf("time:%.2f\n", difftime(ed, st));
	printf("Finish Training...\n");
	printf(" -----------------------------------------------\n");
	printf("|\tʶ����\t|\t  ׼ȷ��\t|\n");
	printf(" -----------------------------------------------\n");
}

void StandardKNN::getData()
{
	IplImage* src_image;
	IplImage prs_image;
	CvMat row, data;
	char file[255], outname[255];
	int i, j;
	for (i = 0; i<classes; i++)
	{
		for (j = 0; j< train_samples; j++)
		{
			//����pbm��ʽͼ����Ϊѵ��
			if (j<10)
				sprintf(file, "%s%d/%d0%d.pbm", file_path, i, i, j);
			else
				sprintf(file, "%s%d/%d%d.pbm", file_path, i, i, j);


			src_image = cvLoadImage(file, 0);
			if (!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}
			//��ͼƬ����Ԥ����
			prs_image = preprocessing(src_image, size, size, 1);
			//���ñ�ǩ��0-9��
			//cvGetRow��������row��trainClasses��ϵ��������һ����ͬʱ����һ��������ͬ
			cvGetRow(trainClasses, &row, i*train_samples + j);
			cvSet(&row, cvRealScalar(i));

			cvGetRow(trainData, &row, i*train_samples + j);

			IplImage* img = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);
			//��ͼƬprs_image���ź���img
			//0.0039215=1/255
			cvConvertScale(&prs_image, img, 0.0039215, 0);
			//�������С��ͼƬ�Ž�data��
			cvGetSubRect(img, &data, cvRect(0, 0, size, size));

			CvMat row_header, *row1;
			//ת����1ά����
			row1 = cvReshape(&data, &row_header, 0, 1);
			//�������ݾ�����
			cvCopy(row1, &row, NULL);
		}
	}
}
void StandardKNN::train()
{
	knn = new CvKNearest(trainData, trainClasses, 0, false, K);
}

float StandardKNN::classify(IplImage* img, int showResult)
{
	IplImage prs_image;
	CvMat data;
	CvMat* nearest = cvCreateMat(1, K, CV_32FC1);
	float result;
	//���������ͼ��
	prs_image = preprocessing(img, size, size, 1);
	IplImage* img32 = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);
	cvConvertScale(&prs_image, img32, 0.0039215, 0);
	cvGetSubRect(img32, &data, cvRect(0, 0, size, size));
	CvMat row_header, *row1;
	row1 = cvReshape(&data, &row_header, 0, 1);
	result = knn->find_nearest(row1, K, 0, 0, nearest, 0);
	int accuracy = 0;
	for (int i = 0; i<K; i++)
	{
		if ((nearest->data.fl[i]) == result)
			accuracy++;
	}
	if (showResult == 1)
	{
		printf("|\t    %.0f    \t| \t %d of %d \t| \n", result, accuracy, K);
		printf(" -----------------------------------------------\n");
	}

	return result;

}

void StandardKNN::test()
{
	IplImage* src_image;
	IplImage prs_image;
	char file[255];
	int i, j;
	int error = 0;
	int testCount = 0;
	for (i = 0; i<classes; i++)
	{
		for (j = train_samples; j< 100; j++)//��ʮ����������������һ�´�����
		{
			sprintf(file, "%s%d/%d%d.pbm", file_path, i, i, j);
			src_image = cvLoadImage(file, 0);
			if (!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}

			//Ԥ����ͼƬ
			prs_image = preprocessing(src_image, size, size, 1);
			float r = classify(&prs_image, 0);
			if ((int)r != i)
				error++;
			testCount++;
		}
	}
	float totalerror = 100 * (float)error / (float)testCount;
	printf("������: %.2f%%\n", totalerror);

}
