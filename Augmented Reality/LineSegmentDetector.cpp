#include "LineSegmentDetector.h"


std::vector<LineSegment> LineSegmentDetector::findLineSegmentsInRegion(std::vector<Edgel> edgels) {
	std::vector<LineSegment> lineSegments;
	LineSegment maximumLineSegment;

	size_t numberOfEdgels = edgels.size();
	std::uniform_int_distribution<size_t> randomIndex;

	do {
		maximumLineSegment.supportEdgels.clear();

		numberOfEdgels = edgels.size();
		randomIndex = std::uniform_int_distribution<size_t>(0, numberOfEdgels - 1);

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

			if (iteration < EDGELS_PAIRING_ITERATIONS) {
				LineSegment newLineSegment(start, end);

				for (auto& edgel : edgels) {
					++counter;
					if (newLineSegment.coincide(edgel)) {
						newLineSegment.supportEdgels.push_back(edgel);
					}
				}

				if (newLineSegment.supportEdgels.size() > maximumLineSegment.supportEdgels.size()) {
					maximumLineSegment = newLineSegment;
				}

			} else {
				continue; //TODO try to break searching if nothing has been found in first iteration
			}
		}

		if (maximumLineSegment.supportEdgels.size() >= MIN_EDGELS_PER_LINE) {
			float startPointCoord = std::numeric_limits<float>::max();
			float endPointCoord = 0;

			const Vector2f slope = maximumLineSegment.start.position - maximumLineSegment.end.position;
			const Vector2f orientation = maximumLineSegment.getOrienation();

			if (fabs(slope.x) <= abs(slope.y)) {
				for (auto edgel : edgels) {
					if (edgel.position.y > endPointCoord) {
						endPointCoord = edgel.position.y;
						maximumLineSegment.start = edgel;
					}

					if (edgel.position.y < startPointCoord) {
						startPointCoord = edgel.position.y;
						maximumLineSegment.end = edgel;
					}
				}
			} else {
				for (auto edgel : edgels) {
					if (edgel.position.x > endPointCoord) {
						endPointCoord = edgel.position.x;
						maximumLineSegment.start = edgel;
					}

					if (edgel.position.x < startPointCoord) {
						startPointCoord = edgel.position.x;
						maximumLineSegment.end = edgel;
					}
				}
			}
		}

		if (dot(maximumLineSegment.getStartEndSlope(), maximumLineSegment.getOrienation()) < 0.0f) {
			maximumLineSegment.swapEndPoints();
		}

		maximumLineSegment.slope = maximumLineSegment.getStartEndSlope().get_normalized();

		lineSegments.push_back(maximumLineSegment);

		//TODO: optimize erasing edgels.
		for (auto& lineSegmentEdgel : maximumLineSegment.supportEdgels) {
			for (std::vector<Edgel>::iterator it = edgels.begin(); it != edgels.end(); ++it) {
				if (lineSegmentEdgel.isTheSamePoint((*it))) {
					edgels.erase(it);
					break;
				}
			}
		}

	} while (maximumLineSegment.supportEdgels.size() >= MIN_EDGELS_PER_LINE && edgels.size() >= MIN_EDGELS_PER_LINE);

	return lineSegments;
}

bool LineSegmentDetector::isEdgelsPairFound(Edgel & start, Edgel & end, size_t iteration) {
	return !(!start.isOrientationCompatible(end) && iteration < EDGELS_PAIRING_ITERATIONS);
}
