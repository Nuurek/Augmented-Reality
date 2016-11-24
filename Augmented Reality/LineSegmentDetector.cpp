#include "LineSegmentDetector.h"


std::vector<LineSegment> LineSegmentDetector::findLineSegmentsInRegion(std::vector<Edgel> edgels) {
	std::vector<LineSegment> lineSegments;
	LineSegment maximumLineSegment;

	size_t numberOfEdgels = edgels.size();
	std::uniform_int_distribution<size_t> randomIndex(0, numberOfEdgels - 1);

	do {
		maximumLineSegment.clearSupportEdgels();

		for (size_t i = 0; i < SEGMENT_SEARCH_ITERATIONS; i++) {
			Edgel start;
			Edgel end;

			size_t iteration = 0;

			do {
				size_t startIndex, endIndex;
				do {
					startIndex = randomIndex(randomGenerator);
					endIndex = randomIndex(randomGenerator);
				} while (startIndex == endIndex);

				start = edgels[startIndex];
				end = edgels[endIndex];
			} while (!(isEdgelsPairFound(start, end, iteration++)));
		}

	} while (false);

	return lineSegments;
}

bool LineSegmentDetector::isEdgelsPairFound(Edgel & start, Edgel & end, size_t iteration) {
	return start.isOrientationCompatible(end) || iteration > EDGELS_PAIRING_ITERATIONS;
}
