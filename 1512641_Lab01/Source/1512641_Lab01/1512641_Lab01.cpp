// 1512641_Lab01.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MyLib.h"

#include "ColorTransformer.h"
#include "Converter.h"
bool readIMG(Mat &img, String path, int option);

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "loi tham so" << endl;
		return -1;
	}

	int ch = 0, b = 0, status = 0;
	float c = 1;
	Converter *converter = new Converter();
	ColorTransformer *colorTransformer = new ColorTransformer();
	Mat dest;
	Mat image = imread(argv[2], CV_LOAD_IMAGE_ANYCOLOR);

	if (!image.data) {
		cout << "Cannot Load Image" << endl;
		return -1;
	}
	if (strcmp(argv[1], "--rgb2gray") == 0) {
		ch = 0;
	}
	else if (strcmp(argv[1], "--gray2rgb") == 0) {
		image.release();
		Mat image2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
		image2.convertTo(image, CV_8UC1);
		ch = 1;
	}
	else if (strcmp(argv[1], "--rgb2hsv") == 0) {
		ch = 2;
	}
	else if (strcmp(argv[1], "--hsv2rgb") == 0) {
		ch = 3;
	}
	else if (strcmp(argv[1], "--bright") == 0) {
		ch = 4;
		stringstream ss(argv[3]); ss >> b;
		status = colorTransformer->ChangeBrighness(image, dest, b);
	}
	else if (strcmp(argv[1], "--contrast") == 0) {
		ch = 5;
		stringstream ss(argv[3]); ss >> c;
		status = colorTransformer->ChangeContrast(image, dest, c);
	}
	else if (strcmp(argv[1], "--hist") == 0) {
		ch = 6;
		status = colorTransformer->DrawHistogram(image, dest);
	}
	else if (strcmp(argv[1], "--equalhist") == 0) {
		ch = 7;
		status = colorTransformer->HistogramEqualization(image, dest);
	}
	else {
		cout << "error : " << argv[1] << endl;
		return -1;
	}

	if (ch < 4) {
		status = converter->Convert(image, dest, ch);
	}
	namedWindow("Display");
	if (dest.data) {
		imshow("Display", dest);
		waitKey(0);
	}
	

	if (ch < 4) {
		if (status == 1) {
			cout << "Process Fail" << endl;
		}
		else {
			cout << "Process success" << endl;
		}
	}
	else {
		if (status == 0) {
			cout << "Process Fail" << endl;
		}
		else {
			cout << "Process success" << endl;
		}
	}
	


	image.release();
	dest.release();


	return 0;
}

bool readIMG(Mat &img, String path, int option) {
	if (option == 1) { // Load ảnh màu
		img = imread(path, CV_LOAD_IMAGE_COLOR);
	}
	else if (option == 2) { // Load ảnh grayscale
		img = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	}
	else if (option == 3) {
		img = imread(path, IMREAD_ANYCOLOR | IMREAD_ANYDEPTH);
	}
	if (!img.data) { // Load ảnh thất bại
		return false;
	}
	return true;
}
