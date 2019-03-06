#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = imread("D:\\Program Files\\OpenCV\\opencv\\sources\\samples\\data\\imageTextR.png");

	if (srcImage.cols>1000 || srcImage.rows>800) {//ͼƬ���󣬽��н�����
		pyrDown(srcImage, srcImage);
		pyrDown(srcImage, srcImage);
		pyrDown(srcImage, srcImage);
	}

	Mat grayImage, binaryImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);//ת���Ҷ�ͼ
	adaptiveThreshold(grayImage, binaryImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);//����Ӧ�˲�

	vector<vector<Point> > contours;
	//RETR_EXTERNAL:��ʾֻ������������
	//CHAIN_APPROX_NONE����ȡÿ��������ÿ�����أ����ڵ������������λ�ò����1 
	findContours(binaryImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//��þ��ΰ�Χ��,֮��������boundRect����Ϊ��ʹ������area�������������RotatedRect�಻�߱��÷���
	float area = boundingRect(contours[0]).area(); 
	int index = 0;
	for (int i = 1; i<contours.size(); i++)
	{

		if (boundingRect(contours[i]).area()>area)
		{
			area = boundingRect(contours[i]).area();
			index = i;
		}
	}
	Rect maxRect = boundingRect(contours[index]);//�ҳ������Ǹ����ο򣨼����������
	Mat ROI = binaryImage(maxRect);
	imshow("maxROI", ROI);

	RotatedRect rect = minAreaRect(contours[index]);//��ȡ��Ӧ����С���ο��������������б��
	Point2f rectPoint[4];
	rect.points(rectPoint);//��ȡ�ĸ��������꣬����RotatedRect�ඨ��ķ���
	double angle = rect.angle;
	//angle += 90;
	Point2f center = rect.center;


	drawContours(binaryImage, contours, -1, Scalar(255), CV_FILLED);
	// srcImage.copyTo(RoiSrcImg,binaryImage);
	Mat RoiSrcImg = Mat::zeros(srcImage.size(), srcImage.type());
	srcImage.copyTo(RoiSrcImg);

	Mat Matrix = getRotationMatrix2D(center, angle, 0.8);//�õ���ת�������ӣ�0.8��������
	warpAffine(RoiSrcImg, RoiSrcImg, Matrix, RoiSrcImg.size(), 1, 0, Scalar(0, 0, 0));

	imshow("src Image", srcImage);
	imshow("contours", binaryImage);
	imshow("recorrected", RoiSrcImg);

	while (waitKey() != 'q') {}
	return 0;
}