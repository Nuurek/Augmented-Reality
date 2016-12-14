#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <map>
class PatternRecognition
{
public:
	static void init();
	static unsigned long getPatternId(cv::Mat& frame, std::vector<cv::Point2f> imagePoints);
	static std::map<unsigned long, unsigned> codesDictionary;
	static const int frameSize = 200.f;
	static const int DEBUG_WINDOW = false;
private:
	static void loadMarker(int id);
	static void addMarker(int id, cv::Mat & image, int orientation);
	static int cvMatToVal(cv::Mat & image);
};

