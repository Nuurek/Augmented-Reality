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
	static int getPatternId(cv::Mat& frame, std::vector<cv::Point2f> imagePoints);
	static std::map<unsigned, int> codesDictionary;
	static const int frameSize = 200.f;
	static const int DEBUG_WINDOW = false;
};

