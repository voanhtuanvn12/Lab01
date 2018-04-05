#pragma once
#include "stdafx.h"
#include "ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat & sourceImage, Mat & destinationImage, uchar b)
{
	if (!sourceImage.data) {
		return 0;
	}
	destinationImage = sourceImage.clone();
	int rows = sourceImage.rows, cols = sourceImage.cols;
	if (sourceImage.isContinuous() && destinationImage.isContinuous()) // kiểm tra xem cả 2 ảnh có lưu data dưới dạng liên tục hay không
	{
		cols = rows * cols;
		rows = 1;
	}
	for (int row = 0; row < rows; row++)
	{
		const uchar* src_ptr = sourceImage.ptr<uchar>(row); // trả về con trỏ mảng
		uchar* dst_ptr = destinationImage.ptr<uchar>(row);

		for (int col = 0; col < cols; col++)
		{
			float bb = src_ptr[0] + b;
			float g = src_ptr[1] + b;
			float r = src_ptr[2] + b;

			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (bb > 255) bb = 255;

			dst_ptr[0] = (uchar)bb;
			dst_ptr[1] = (uchar)g;
			dst_ptr[2] = (uchar)r;

			dst_ptr += 3;
			src_ptr += 3;
		}
	}
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat & sourceImage, Mat & destinationImage, float c)
{
	if (!sourceImage.data) {
		return 0;
	}
	destinationImage = sourceImage.clone();
	int rows = sourceImage.rows, cols = sourceImage.cols;
	if (sourceImage.isContinuous() && destinationImage.isContinuous()) // kiểm tra xem cả 2 ảnh có lưu data dưới dạng liên tục hay không
	{
		cols = rows * cols;
		rows = 1;
	}
	for (int row = 0; row < rows; row++)
	{
		const uchar* src_ptr = sourceImage.ptr<uchar>(row); // trả về con trỏ mảng
		uchar* dst_ptr = destinationImage.ptr<uchar>(row);

		for (int col = 0; col < cols; col++)
		{
			float b = src_ptr[0] * c;
			float g = src_ptr[1] * c;
			float r = src_ptr[2] * c;
			
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			dst_ptr[0] = (uchar)b;
			dst_ptr[1] = (uchar)g;
			dst_ptr[2] = (uchar)r;

			dst_ptr += 3;
			src_ptr += 3;
		}
	}
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat & sourceImage, Mat & destinationImage)
{
	if (!sourceImage.data) {
		return 0;
	}


	
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	destinationImage.create(height, width, sourceImage.type());
	int nChannels = sourceImage.channels();
	double NumOfPixel = width * height;

	if (nChannels == 1) {
		Mat histogram;
		CalcHistogram(sourceImage, histogram);
		float T[256] = { 0 };
		T[0] = (float)(histogram.at<float>(0, 0) / NumOfPixel);
		for (int x = 1; x < 256; ++x) {
			T[x] = (float)((histogram).at<float>(0, x) / NumOfPixel) + T[x - 1];
		}

		// tinh gia trị pixel cho ảnh output
		for (int y = 0; y < height; ++y) {
			const unsigned char *src_data = sourceImage.ptr<uchar>(y);
			unsigned char *dst_data = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < width; ++x) {
				*dst_data = round(255 * T[(int)(*src_data)]); dst_data++; src_data++;
			}
		}
	}
	else if (nChannels == 3) {
		Mat histogram;
		CalcHistogram(sourceImage, histogram);
		float T[3][256] = { { 0 } };
		T[0][0] = (float)histogram.at<float>(0, 0) / NumOfPixel;
		T[1][0] = (float)histogram.at<float>(1, 0) / NumOfPixel;
		T[2][0] = (float)histogram.at<float>(2, 0) / NumOfPixel;

		// tính lược đồ xám tích lũy
		for (int i = 1; i < 256; ++i) {
			T[0][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[0][i - 1];
			T[1][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[1][i - 1];
			T[2][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[2][i - 1];
		}


		// chuẩn hóa lược đồ xám tích lũy
		for (int y = 0; y < height; ++y) {
			const unsigned char * src_data = sourceImage.ptr<uchar>(y);
			unsigned char *dst_data = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < width; ++x) {
				int b = round((float)(255 * T[0][(int)*src_data])); src_data++;
				int g = round((float)(255 * T[1][(int)*src_data])); src_data++;
				int r = round((float)(255 * T[2][(int)*src_data])); src_data++;

				*dst_data = b; dst_data++;
				*dst_data = g; dst_data++;
				*dst_data = r; dst_data++;
			}
		}
	}
	imwrite("equalization_Image.jpg", destinationImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}

int ColorTransformer::CalcHistogram(const Mat & sourceImage, Mat & histogram)
{
	if (!sourceImage.data) {
		return 0;
	}
	int bins = 256; // number of bins
	int nChannels = sourceImage.channels(); // number of channels
	int height = sourceImage.cols;
	int width = sourceImage.rows;
	
	if (nChannels == 1) {
		histogram = Mat(1, 256, CV_32FC3, Scalar::all(0));
		for (int y = 0; y < height; ++y) {

			const unsigned char* data = sourceImage.ptr<uchar>(y);
			float *data2 = histogram.ptr<float>(0);
			for (int x = 0; x < width; ++x) {
				*(data2 + (int)*data) += 1; data++;
			}
		}
	}
	else if (nChannels == 3) {
		histogram = Mat(3, 256, CV_32FC1, Scalar::all(0));
		for (int y = 0; y < height; ++y) {
			const unsigned char* data = sourceImage.ptr<uchar>(y);
			float *b = histogram.ptr<float>(0);
			float *g = histogram.ptr<float>(1);
			float *r = histogram.ptr<float>(2);
			for (int x = 0; x < width; ++x) {
				*(b + (int)(*data)) += 1; data++;
				*(g + (int)(*data)) += 1; data++;
				*(r + (int)(*data)) += 1; data++;
			}
		}
	}
	return 0;
}

int ColorTransformer::DrawHistogram(const Mat & sourceImage, Mat & histImage)
{
	int bins = 256;
	if (!sourceImage.data) return 0;
	int nChannels = sourceImage.channels();
	Mat histogram;
	CalcHistogram(sourceImage, histogram);

	int hmax[3] = { 0,0,0 };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };
	Mat canvas(200*3 + 10,256,CV_8UC3,Scalar::all(255));// = Mat::ones(200 * 3 + 3, 256, CV_8UC3);
	namedWindow("show", WINDOW_AUTOSIZE);

	float Max = findMaxValue(histogram);

	int rows = 200;
	for (int i = 0; i < nChannels; i++) {
		
		for (int j = 0; j < bins ; j++)
		{
			float x = histogram.at<float>(i, j);
			line(
				canvas,
				Point(j, rows),
				Point(j, rows - cvRound(histogram.at<float>(i,j) * 200 / Max)),
				nChannels == 1 ? Scalar(200, 200, 200) : colors[i],
				1, 8, 0
			);
		}
		rows += 200 + 2;
	}
	imwrite("histogram.png", canvas);
	imshow("show", canvas);
	waitKey(0);
	return 1;
}

int ColorTransformer::normalize(int b)
{
	if (b > 255) b = 255;
	else if (b < 0) b = 0;
	return b;
}

float ColorTransformer::findMaxValue(const Mat & arr)
{
	int width = arr.cols;
	int height = arr.rows;

	float m = 0;
	for (int y = 0; y < height; ++y) {
		const float *data = arr.ptr<float>(y);
		for (int x = 0; x < width; ++x) {
			m = max((float)(*data), m); data++;
		}
	}
	return m;
}


ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
