#include "CameraCalibrator.h"

CameraCalibrator::CameraCalibrator(size_t numberOfFrames, float delayBetweenFrames)
	: framesPerCalibration(numberOfFrames), delayInSeconds(delayBetweenFrames) {
	lastFrameTakenTime = std::chrono::steady_clock::now();
}

void CameraCalibrator::setFrameSize(int width, int height) {
	frameSize = cv::Size(width, height);
}

void CameraCalibrator::startSession() throw(CalibrationException) {
	imagePointsPatterns.clear();
	cameraCalibration = CameraCalibration();
	state = CalibrationState::RUNNING;
	std::cout << "Starting camera calibration.\n";
}

bool CameraCalibrator::isSessionRunning() const {
	return state == CalibrationState::RUNNING;
}

bool CameraCalibrator::addFrame(cv::Mat& frame) {
	if (state != CalibrationState::RUNNING) {
		throw CalibrationException("Not possible to add a frame while the session is not running.");
	}

	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> duration = now - lastFrameTakenTime;
	auto durationInSeconds = duration.count();

	bool found;

	if (durationInSeconds > delayInSeconds) {
		std::vector<cv::Point2f> pointsBuffer;
		found = findChessPattern(frame, pointsBuffer);

		if (found) {
			imagePointsPatterns.push_back(pointsBuffer);

			std::cout << "Added " << imagePointsPatterns.size() << " frame to camera calibration.\n";
		} else {
			std::cout << "Chess pattern not found.\n";
		}

		lastFrameTakenTime = std::chrono::steady_clock::now();
	}

	if (imagePointsPatterns.size() == framesPerCalibration) {
		std::cout << "Enough frames in camera calibration.\n";

		averageReprojectionError = calibrateCamera();
		state = CalibrationState::FINISHED;
	}

	return found;
}

bool CameraCalibrator::isSessionFinished() const {
	return state == CalibrationState::FINISHED;
}

CameraCalibration CameraCalibrator::getCameraCalibration() {
	if (state == CalibrationState::RUNNING) {
		throw CalibrationException("Calibration session is still running.");
	}
	return cameraCalibration;
}

void CameraCalibrator::saveToFile(CameraCalibration cameraCalibration, std::string filename) {
	cv::FileStorage fileStorage(filename, cv::FileStorage::WRITE);

	fileStorage << "cameraMatrix" << cameraCalibration.cameraMatrix;
	fileStorage << "distCoeffs" << cameraCalibration.distCoeffs;

	fileStorage.release();
}

CameraCalibration CameraCalibrator::loadFromFile(std::string filename) {
	cv::FileStorage fileStorage(filename, cv::FileStorage::READ);

	CameraCalibration cameraCalibration;
	fileStorage["cameraMatrix"] >> cameraCalibration.cameraMatrix;
	fileStorage["disoCoeffs"] >> cameraCalibration.distCoeffs;

	return cameraCalibration;
}

double CameraCalibrator::calibrateCamera() {
	std::vector<std::vector<cv::Point3f>> objectPointsPattern{ imagePointsPatterns.size(), getCorners3DPoints(SQUARE_SIZE) };

	ViewMatrix viewMatrix;

	double rms = cv::calibrateCamera(objectPointsPattern, imagePointsPatterns, frameSize, 
		cameraCalibration.cameraMatrix, cameraCalibration.distCoeffs, 
		viewMatrix.rotationVector, viewMatrix.translationVector);

	return rms;
}

bool CameraCalibrator::findChessPattern(const cv::Mat frame, std::vector<cv::Point2f>& pointsBuffer) {
	bool found = cv::findChessboardCorners(frame, CHESSBOARD_SIZE, pointsBuffer, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
	
	if (found) {
		improveChessPattern(frame, pointsBuffer);
	}

	return found;
}

std::vector<cv::Point2f> CameraCalibrator::improveChessPattern(const cv::Mat & frame, std::vector<cv::Point2f>& pointsBuffer) {
	cv::Mat grayFrame;
	cv::cvtColor(frame, grayFrame, CV_BGR2GRAY);
	cv::cornerSubPix(grayFrame, pointsBuffer, cv::Size(11, 11), cv::Size(-1, -1),
		cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

	return pointsBuffer;
}

std::vector<cv::Point3f> CameraCalibrator::getCorners3DPoints(float squareSize) {
	std::vector<cv::Point3f> corners;

	for (int i = 0; i < CHESSBOARD_SIZE.height; ++i) {
		for (int j = 0; j < CHESSBOARD_SIZE.width; ++j) {
			corners.push_back(cv::Point3f(j * squareSize, i * squareSize, 0.0f));
		}
	}

	return corners;
}
