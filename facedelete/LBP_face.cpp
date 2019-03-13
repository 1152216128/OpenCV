//LBP�����������
//������������У���������ʶ�������ʶ��

#include<opencv2/opencv.hpp>
#include <iostream>   
using namespace std;
using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
void detectAndDisplay(Mat frame);

int main(int argc, char** argv)
{
	Mat srcImage;
	srcImage = imread("1.jpg", 1);    
	imshow("ԭͼ", srcImage);
	//============���ط�����=========
  
	if (!face_cascade.load("D:\\Program Files\\OpenCV\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml"))//Ҳ����Haar������
	{
		printf("�������������ʧ��\n");
		return -1;
	}
	if (!eyes_cascade.load("D:\\Program Files\\OpenCV\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_eye.xml"))
	{
		printf("���ۼ��������ʧ��\n");
		return -1;
	};
	//============����������⺯��  =========
	detectAndDisplay(srcImage);
	waitKey(0); 
}


void detectAndDisplay(Mat dispFace)
{
	//�������
	std::vector<Rect> faces;
	std::vector<Rect>eyes;
	Mat srcFace, grayFace, eqlHistFace;
	int eye_number = 0;

	cvtColor(dispFace, grayFace, CV_BGR2GRAY);   
	equalizeHist(grayFace, eqlHistFace);   //ֱ��ͼ���⻯  
    //�������******************
	face_cascade.detectMultiScale(eqlHistFace, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));
	//������ĸ�����������߼�⾫��,��Ҳ���ܻ������©
	//�����ߴ�minSize��maxSize���ؼ������������趨����ͼƬ�ߴ��кܴ��ϵ��

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//����ɫ��Բ��Ǽ�⵽������
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(dispFace, center, Size(faces[i].width / 2, faces[i].height * 65 / 100), 0, 0, 360, Scalar(255, 0, 0), 2, 8, 0);
		//���ۼ��*****************
		Mat faceROI = eqlHistFace(faces[i]);
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.2, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(15, 15), Size(80, 80));
		eye_number += eyes.size();//���ۼ���

		 //����ɫԲ��Ǽ�⵽������*****************
		for (unsigned int j = 0; j <eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[i].height)*0.25);
			circle(dispFace, center, radius, Scalar(105, 50, 255), 2, 8, 0);
		}
	}
	//*****************3.0��������*****************
	cout << "�����\n����: " << faces.size() << " ��" << endl;
	cout << "����: " << eye_number << " ֻ" << endl;
	imshow("����ʶ����", dispFace);
}
