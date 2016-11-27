#include "LineSegmentMerger.h"


LineSegmentMerger::LineSegmentMerger(const size_t borderSize, const size_t regionSize, const size_t stepSize) :
	EdgelDetector(borderSize, regionSize, stepSize) {}

std::vector<LineSegment> LineSegmentMerger::mergeLineSegments(std::vector<LineSegment> lineSegmentsInRegion) {
	std::vector<LineWithDistance> linesWithDistance;

	for (int i = 0; i < lineSegmentsInRegion.size(); i++) {
		auto lineSegment = lineSegmentsInRegion[i];

		linesWithDistance.clear();

		for (int j = 0; j < lineSegmentsInRegion.size(); j++) {
			auto otherLineSegment = lineSegmentsInRegion[j];
			
			if (i == j) {
				continue;
			}

			if (orientationAndDirectionCompatible(lineSegment, otherLineSegment)) {
				const float squaredLength = (otherLineSegment.start.position - lineSegment.end.position).get_squared_length();

				if (squaredLength < SQUARED_LENGTH_THRESHOLD) {
					linesWithDistance.emplace_back(LineWithDistance(otherLineSegment, squaredLength));
				}
			}
		}

		if (linesWithDistance.size() == 0) {
			continue;
		}

		std::sort(linesWithDistance.begin(), linesWithDistance.end());

		for (auto& lineWithDistance : linesWithDistance) {
			auto& closeLine = lineWithDistance.lineSegment;

			const Vector2f startPoint = lineSegment.end.position;
			Vector2f endPoint = closeLine.start.position;

			const size_t length = static_cast<size_t>((endPoint - startPoint).get_length());
			const Vector2f slope = (endPoint - startPoint).get_normalized();
		}
	}

	return std::vector<LineSegment>();
}

bool LineSegmentMerger::orientationAndDirectionCompatible(LineSegment & first, LineSegment & second) {
	return dot(first.slope, second.slope) > COMMON_ORIENTATION_THRESHOLD &&
		dot((second.end.position - first.start.position).get_normalized(), first.slope) > COMMON_ORIENTATION_THRESHOLD;
}