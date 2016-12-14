#pragma once
#include <chrono>
#include <vector>

#include "ARMarker.h"

const long bufferExpirationTimeInMs = 200;

class ARMarkersBuffer {
	std::chrono::time_point<std::chrono::steady_clock> lastSaveTime{ std::chrono::nanoseconds(0) };
	std::vector<ARMarker> markersBuffer;
public:
	void saveToBuffer(std::vector<ARMarker> newMarkers);
	bool isBufferUpToDate();
	size_t getBufferedMarkersSize();
	std::vector<ARMarker> loadFromBuffer();
};

