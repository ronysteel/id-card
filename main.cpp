#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


#if 1

Mat rotateImage1(Mat img, int degree, CvPoint2D32f center)
{
	//degree = -degree;//warpAffine默认的旋转方向是逆时针，所以加负号表示转化为顺时针
	double angle = degree  * CV_PI / 180.; // 弧度  
	double a = sin(angle), b = cos(angle);
	int width = img.cols;
	int height = img.rows;
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));
	//旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float map[6];
	Mat map_matrix = Mat(2, 3, CV_32F, map);
	// 旋转中心
	CvMat map_matrix2 = map_matrix;
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);//计算二维旋转的仿射变换矩阵
	//map[2] += (width_rotate - width) / 2;
	//map[5] += (height_rotate - height) / 2;
	Mat img_rotate;
	//对图像做仿射变换
	//CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
	//如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
	//CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
	warpAffine(img, img_rotate, map_matrix, Size(width, height), 1, 0, 0);
	return img_rotate;
}

//第一个参数：输入图片名称；第二个参数：输出图片名称
void GetContoursPic(const char* pSrcFileName, const char* pDstFileName)
{
	Mat srcImg = imread(pSrcFileName);
	Mat gray, binImg;
	//灰度化
	cvtColor(srcImg, gray, COLOR_RGB2GRAY);
	imwrite("C:\\Users\\sun\\Desktop\\c1.jpg", gray);

	//二值化
	threshold(gray, binImg, 100, 200, CV_THRESH_BINARY);
	imwrite("C:\\Users\\sun\\Desktop\\c2.jpg", binImg);

	Mat dilateImg;
	//	Mat element = getStructuringElement(MORPH_RECT, Size(60, 60));
	int height = srcImg.size().height;
	int length = srcImg.size().width;
	int dil = (height > length) ? height / 100 * 2 : length / 100 * 2 ;
	cout << "dilate = " << dil << endl;
	Mat element = getStructuringElement(MORPH_RECT, Size(dil, dil));
	//Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));

	erode(binImg, dilateImg, element);

	imwrite("C:\\Users\\sun\\Desktop\\erode.jpg", dilateImg);

	vector<vector<Point> > contours;
	vector<Rect> boundRect(contours.size());
	//注意第5个参数为CV_RETR_EXTERNAL，只检索外框  
	findContours(dilateImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE); //找轮廓
	cout << contours.size() << endl;


	for (int i = 0; i < contours.size(); i++)
	{
		//需要获取的坐标  
		CvPoint2D32f rectpoint[4];
		CvBox2D rect = minAreaRect(Mat(contours[i]));

		cvBoxPoints(rect, rectpoint); //获取4个顶点坐标

		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));
		//rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
		//面积太小的直接pass
		//cout << "line length:" << line1 << '\t' << line2 << endl;
		if (line1 * line2 < 600)
		{
			continue;
		}
		if ((line1 >= line2 && line1 / line2 < 6) || line2 < 30)
		{
			continue;
		}
		if ((line2 >= line1 && line2 / line1 < 6) || line1 < 30)
		{
			continue;
		}
		int length, height;
		if (line1 > line2)
		{
			length = line1;
			height = line2;
		}
		else
		{
			length = line2;
			height = line1;
		}
		cout << "4line length:" << i <<'\t'<< line1 << '\t' << line2 << endl;

		cout << "de=" << line2 / line1 << endl;

		cout << "4degree:" << rect.angle<< endl;

		//drawContours(srcImg, contours, i, Scalar(0,0,255), 2, 8);
		//imwrite("C:\\Users\\sun\\Desktop\\cont.jpg", srcImg);

		float angle = rect.angle;
		if (abs(rect.angle) > 45){}
		//	angle = 0;
		Mat rotate_img = rotateImage1(srcImg, angle, rect.center);

		int xmax = 0, xmin = 0, ymax = 0, ymin = 0;

		xmax = rect.center.x + length / 2;
		xmin = rect.center.x - length / 2;
		ymax = rect.center.y + height / 2;
		ymin = rect.center.y - height / 2;


		//将身份证号区域截取出来
		Mat id(rotate_img(Range(ymin, ymax), Range(xmin, xmax)));

		imwrite("C:\\Users\\sun\\Desktop\\id.jpg", id);

	}
	imwrite("C:\\Users\\sun\\Desktop\\aa.jpg", srcImg); //将矫正后的图片保存下来
	return;



} 

void main()
{
	GetContoursPic("C:\\Users\\sun\\Desktop\\ocr-images\\sun.jpg", "C:\\Users\\sun\\Desktop\\result.jpg");
	 //GetContoursPic("C:\\Users\\sun\\Desktop\\wang2.jpg", "C:\\Users\\sun\\Desktop\\result.jpg");

	//GetContoursPic("C:\\Users\\sun\\Desktop\\ocr-images\\sun.jpg", "C:\\Users\\sun\\Desktop\\result.jpg");

	waitKey();
}

#endif
