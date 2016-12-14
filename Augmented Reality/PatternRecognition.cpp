#include "PatternRecognition.h"
#include <iostream>
#include <windows.h>
#include <utility> 
std::map<unsigned long, unsigned> PatternRecognition::codesDictionary;
/*= {
	{ 0b0000011111111111111111111, 10 },
	{ 0b0111101111011110111101111, 11 },
	{ 0b1111111111111111111100000, 12 },
	{ 0b1111011110111101111011110, 13 },
	{ 0b0000011111101111011110111, 50 },
	{ 0b0111101111010000111101111, 51 },
	{ 0b1110111101111011111100000, 52 },
	{ 0b1111000010111101111011110, 53 }
};*/
unsigned long PatternRecognition::getPatternId(cv::Mat& frame, std::vector<cv::Point2f> imagePoints) {

	std::vector<cv::Point2f> destinyPoints = { {0, 0},{ (float)frameSize, 0},{ (float)frameSize, (float)frameSize },{0, (float)frameSize} };
	cv::Mat H = cv::findHomography(imagePoints, destinyPoints);
	cv::Mat outputImage;
	cv::warpPerspective(frame, outputImage, H, cv::Size(frameSize, frameSize));
	cv::cvtColor(outputImage, outputImage, CV_BGR2GRAY);
	cv::GaussianBlur(outputImage, outputImage, cv::Size(5, 5), 0);
	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	if(DEBUG_WINDOW)
		cv::imshow("Flat Marker", outputImage);
	int squareSize = frameSize / 7;
	unsigned long val = cvMatToVal(outputImage);
	std::cout << " val:" << val << "\n";
	auto id = codesDictionary.find(val);
	std::cout << "Marker nr: "<< id->second << "\n";
	if (id != codesDictionary.end()) {
		return id->second;
	}
	else
		return INT_MAX;
}
void PatternRecognition::init() {
	for (int i = 0; i < 10; i++)
		loadMarker(i);
}
void PatternRecognition::loadMarker(int id) {
	std::string path = "./data/markers/"+std::to_string(id)+".png";
	cv::Mat marker = cv::imread(path.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	marker = marker(cv::Rect(100, 100, 1000, 1000));
	for (int i = 0; i < 4; i++) {
		PatternRecognition::addMarker(id, marker, i);
		cv::Point2f src_center(marker.cols / 2.0F, marker.rows / 2.0F);
		cv::Mat rot_mat = getRotationMatrix2D(src_center, 90, 1.0);
		warpAffine(marker, marker, rot_mat, marker.size());
	}
}
void PatternRecognition::addMarker(int id, cv::Mat& image, int orientation) {

	unsigned long  val = cvMatToVal(image);
	std::cout << val <<"=" << id << "\n";
	codesDictionary.insert(std::make_pair(val, (unsigned long)id*10 + (unsigned long)orientation));
}

int PatternRecognition::cvMatToVal(cv::Mat & image)
{
	int imageWidth = image.size[0];
	int squareSize = image.size[0] / 7;
	int val=0;
	for (int i = squareSize; i<imageWidth - 2.f * squareSize; i += squareSize)
		for (int j = squareSize; j < imageWidth - 2.f * squareSize; j += squareSize) {
			if (image.at<char>(i + squareSize / 2, j + squareSize / 2) == 0) 
				val |= 1;
			val <<= 1;
		}
	val >>= 1;
	return val;
}
