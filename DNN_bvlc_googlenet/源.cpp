
//use opencv_dnn module for image classification by using GoogLeNet trained network
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
using namespace cv;
using namespace cv::dnn;
using namespace std;
String modelTxt = "bvlc_googlenet.prototxt";
String modelBin = "bvlc_googlenet.caffemodel";
String labelFile = "synset_words.txt";
vector<String> readClasslabels();
int main(int argc, char** argv) {
	Mat testImage = imread("monky.jpg");
	if (testImage.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	// create googlenet with caffemodel text and bin
	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty())
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		return -1;
	}
	// ��ȡ��������
	vector<String> labels = readClasslabels();
	//GoogLeNet accepts only 224x224 RGB-images
	Mat inputBlob = blobFromImage(testImage, 1, Size(224, 224), Scalar(104, 117, 123));//mean: Scalar(104, 117, 123)
	// ֧��1000��ͼ�������
	Mat prob;
	// ѭ��10+
	for (int i = 0; i < 10; i++)
	{
		// ����
		net.setInput(inputBlob, "data");
		// ����Ԥ��
		prob = net.forward("prob");
	}
	// ��ȡ�����������������Сֵ
	Mat probMat = prob.reshape(1, 1); //reshape the blob to 1x1000 matrix // 1000������
	Point classNumber;
	double classProb;
	minMaxLoc(probMat, NULL, &classProb, NULL, &classNumber); // ����������һ��
	int classIdx = classNumber.x; // ����������
	printf("\n current image classification : %s, possible : %.2f \n", labels.at(classIdx).c_str(), classProb);
	putText(testImage, labels.at(classIdx), Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2, 8);
	imshow("Image Category", testImage);
	waitKey(0);
	return 0;
}
/* ��ȡͼ���1000���������ı����� */
vector<String> readClasslabels() {
	std::vector<String> classNames;
	std::ifstream fp(labelFile);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << labelFile << std::endl;
		exit(-1);
	}
	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}
	fp.close();
	return classNames;
}