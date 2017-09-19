#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

class StandardKNN
{
public:
	float classify(IplImage* img, int showResult);
	StandardKNN();
	void test();
private:
	char file_path[255];
	int train_samples;
	int classes;
	CvMat* trainData;	//���ݼ�
	CvMat* trainClasses;	//��ǩ��
	int size;
	static const int K = 5;//����ھӸ���
	CvKNearest *knn;
	void getData();
	void train();
};
