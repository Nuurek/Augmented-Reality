#pragma once
#include <vector>
#include <string>
#include <chrono>

#include "RegionBasedOperator.h"
#include "CameraCalibration.h"
#include "ViewMatrix.h"

const cv::Size CHESSBOARD_SIZE{ 9, 6 };
const float SQUARE_SIZE = 1.0;

using Frames = std::vector<cv::Mat>;

enum class CalibrationState { IDLE, RUNNING, FINISHED };

struct CalibrationException 
	: std::logic_error {

	CalibrationException(const std::string& what) 
		: std::logic_error(what) {}
};

class CameraCalibrator {

public:
	CameraCalibrator(size_t numberOfFrames, float delayBetweenFrames);
	void setFrameSize(int width, int height);
	void startSession() throw (CalibrationException);
	bool isSessionRunning() const;
	bool addFrame(cv::Mat& frame);
	bool isSessionFinished() const;
	CameraCalibration getCameraCalibration();
	static void saveToFile(CameraCalibration cameraCalibration, std::string filename);
	static CameraCalibration loadFromFile(std::string filename);

private:
	CalibrationState state{ CalibrationState::IDLE };
	CameraCalibration cameraCalibration;
	double averageReprojectionError;

	size_t framesPerCalibration;

	float delayInSeconds;
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTakenTime;

	cv::Size frameSize;

	std::vector<std::vector<cv::Point2f>> imagePointsPatterns;

	double calibrateCamera();
	bool findChessPattern(const cv::Mat frame, std::vector<cv::Point2f>& pointsBuffer);
	std::vector<cv::Point2f> improveChessPattern(const cv::Mat& frame, std::vector<cv::Point2f>& pointsBuffer);

	static std::vector<cv::Point3f> getCorners3DPoints(float squareSize);
};

