#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
class IDCard
{
public:
	string srcFilePath;
	Mat idNumberImg;
	vector<Mat> idNumbers;
	void getIDNumberImg(const char* pSrcFileName, const char* pDstFileName);
	Mat rotateImage1(Mat img, int degree, CvPoint2D32f center);
	int splitNumbers(string file);

	//void GetContoursPic(const char* pSrcFileName, const char* pDstFileName)


};