#include "PatternRecognition.h"
#include <iostream>
std::map<unsigned,int> PatternRecognition::codesDictionary = {
	{ 0b0000011111111111111111111, 10 },
	{ 0b0111101111011110111101111, 11 },
	{ 0b1111111111111111111100000, 12 },
	{ 0b1111011110111101111011110, 13 },
	{ 0b0000011111101111011110111, 50 },
	{ 0b0111101111010000111101111, 51 },
	{ 0b1110111101111011111100000, 52 },
	{ 0b1111000010111101111011110, 53 }
};
int PatternRecognition::getPatternId(cv::Mat& frame, std::vector<cv::Point2f> imagePoints) {

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
	unsigned int val = 0;
	for (int i = squareSize; i<frameSize-2*squareSize; i += squareSize)
		for (int j = squareSize; j < frameSize - 2*squareSize; j += squareSize) {
			if (outputImage.at<char>(i + squareSize / 2, j + squareSize / 2) == 0)
				val |= 1;
			val <<= 1;
		}
	val >>= 1;
	auto id = codesDictionary.find(val);
	if (id != codesDictionary.end()) {
		std::cout << "Marker nr: "<< id->second << "\n";
		return id->second;
	}
	else
		return -1;
}
