#pragma once
#include "stdafx.h"
#include "Converter.h"


int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage) {
	if (sourceImage.type() != CV_8UC3) {
		return 1; // Thất bại
	}
	if (sourceImage.type() == CV_8UC1) {
		destinationImage = sourceImage;
		return 0;
	}
	int rows = sourceImage.rows, cols = sourceImage.cols;
	destinationImage.create(sourceImage.size(), CV_8UC1);

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
			dst_ptr[col] = (uchar)(src_ptr[0] * 0.114f + src_ptr[1] * 0.587f + src_ptr[2] * 0.299f);
			src_ptr += 3;
		}
	}
	return 0;


}

int Converter::GrayScale2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.type() != CV_8UC1) {
		return 1; // Thất bại
	}
	if (sourceImage.type() == CV_8UC3) {
		destinationImage = sourceImage;
		return 0;
	}
	int rows = sourceImage.rows, cols = sourceImage.cols;
	destinationImage.create(sourceImage.size(), CV_8UC3);

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
			dst_ptr[0] = (uchar)(src_ptr[col]);
			dst_ptr[1] = (uchar)(src_ptr[col]);
			dst_ptr[2] = (uchar)(src_ptr[col]);
			dst_ptr += 3;
		}
	}
	return 0;
}

int Converter::RGB2HSV(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.type() != CV_8UC3) {
		return 1; // Thất bại
	}

	int rows = sourceImage.rows, cols = sourceImage.cols;
	destinationImage.create(sourceImage.size(), CV_8UC3);

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
			float r, g, b;
			b = src_ptr[0];
			g = src_ptr[1];
			r = src_ptr[2];

			float h, s, v;
			rgb2hsv(r, g, b, h, s, v);
			h = h * 100;
			s = s * 100;
			v = v * 100;

			dst_ptr[0] = (uchar)h;
			dst_ptr[1] = (uchar)s;
			dst_ptr[2] = (uchar)v;

			src_ptr += 3;
			dst_ptr += 3;
		}
	}
	imwrite("HSV.png", destinationImage);
	return 0;
}

int Converter::HSV2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.type() != CV_8UC3) {
		return 1; // Thất bại
	}

	int rows = sourceImage.rows, cols = sourceImage.cols;
	destinationImage.create(sourceImage.size(), CV_8UC3);

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
			float h, s, v, r, g, b;
			h = (float)src_ptr[0] / 100;
			s = (float)src_ptr[1] / 100;
			v = (float)src_ptr[2] / 100;

			hsv2rgb(h, s, v, r, g, b);

			dst_ptr[0] = (uchar)(b *255);
			dst_ptr[1] = (uchar)(g * 255);
			dst_ptr[2] = (uchar)(r * 255);

			src_ptr += 3;
			dst_ptr += 3;
		}
	}
	return 0;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type) {
	if (type == 0) {
		// RGB-GrayScale
		if (RGB2GrayScale(sourceImage, destinationImage) == 0) {
			return 0;
		}
	}
	else if (type == 1) {
		// RGB-GrayScale
		bool a = GrayScale2RGB(sourceImage, destinationImage);
		if (a==0) {
			return 0;
		}
	}
	else if (type == 2) {
		// RBG - HSV
		if (RGB2HSV(sourceImage, destinationImage) == 0) {
			return 0;
		}
	}
	else if(type == 3){
		// HSV - RBG
		if (HSV2RGB(sourceImage, destinationImage) == 0) {
			return 0;
		}
	}
	return 1;
}

int Converter::Convert(IplImage * sourceImage, IplImage * destinationImage, int type)
{
	return 0;
}

void Converter::rgb2hsv(float r, float g, float b, float & h, float & s, float & v)
{
	r /= 255;
	g /= 255;
	b /= 255;
	float Cmax = max(r, max(g, b));
	float Cmin = min(r, min(g, b));
	v = Cmax;
	float delta = Cmax - Cmin;
	s = Cmax == 0 ? 0 : delta / Cmax;
	if (Cmax == Cmin) {
		h = 0;
	}
	else {
		if (Cmax == r) {
			h = (g - b) / delta + (g < b ? 6 : 0);
		}
		else if (Cmax == g) {
			h = (b - r) / delta + 2;
		}
		else if (Cmax == b) {
			h = (r - g) / delta + 4;
		}
		h /= 6;
	}
}

void Converter::hsv2rgb(float h, float s, float v, float & r, float & g, float & b)
{
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	switch (i % 6)
	{
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}
}



Converter::Converter()
{
}

Converter::~Converter()
{
}



