#include "ARMarkersBuffer.h"

void ARMarkersBuffer::saveToBuffer(std::vector<ARMarker> newMarkers) {
	markersBuffer = newMarkers;
	lastSaveTime = std::chrono::steady_clock::now();
}

bool ARMarkersBuffer::isBufferUpToDate() {
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSaveTime).count();
	return duration < bufferExpirationTimeInMs;
}

size_t ARMarkersBuffer::getBufferedMarkersSize() {
	return markersBuffer.size();
}

std::vector<ARMarker> ARMarkersBuffer::loadFromBuffer() {
	return markersBuffer;
}
