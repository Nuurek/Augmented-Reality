#pragma once
#include <chrono>
#include <vector>

#include "ARMarker.h"


class ARMarkersBuffer {
	std::chrono::time_point<std::chrono::steady_clock> lastSaveTime{ std::chrono::nanoseconds(0) };
	std::vector<ARMarker> markersBuffer;

	const long bufferExpirationTimeInMs;

public:
	ARMarkersBuffer(long expirationTimeInMs);

	void saveToBuffer(std::vector<ARMarker> newMarkers);
	bool isBufferUpToDate();
	size_t getBufferedMarkersSize();
	std::vector<ARMarker> loadFromBuffer();
};

