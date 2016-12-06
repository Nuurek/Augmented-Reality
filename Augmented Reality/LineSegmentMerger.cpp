#include "LineSegmentMerger.h"


LineSegmentMerger::LineSegmentMerger(const size_t borderSize, const size_t regionSize, const size_t stepSize) :
	EdgelDetector(borderSize, regionSize, stepSize) {}

std::vector<LineSegment> LineSegmentMerger::mergeLineSegments(std::vector<LineSegment> lineSegments) {
	std::vector<LineWithDistance> linesWithDistance;

	for (int i = 0; i < lineSegments.size(); i++) {
		auto lineSegment = lineSegments[i];

		linesWithDistance.clear();

		for (int j = 0; j < lineSegments.size(); j++) {
			auto otherLineSegment = lineSegments[j];
			
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

			if (isLineExtensible(startPoint, endPoint, slope, lineSegment.end.slope, length)) {
				lineSegment.end = closeLine.end;
				lineSegment.slope = (lineSegment.end.position - lineSegment.start.position).get_normalized();

				closeLine.isMerged = true;
			} else {
				break;
			}
		}

		size_t oldSize = lineSegments.size();

		lineSegments.erase(std::remove_if(lineSegments.begin(), lineSegments.end(), [](LineSegment& segment) {
			return segment.isMerged;
		}), lineSegments.end());

		if (lineSegments.size() < oldSize) {
			i = -1;
		}
	}

	return lineSegments;
}

bool LineSegmentMerger::orientationAndDirectionCompatible(LineSegment & first, LineSegment & second) {
	return dot(second.slope, first.slope) > COMMON_ORIENTATION_THRESHOLD &&
		dot((second.end.position - first.start.position).get_normalized(), first.slope) > COMMON_ORIENTATION_THRESHOLD;
}

bool LineSegmentMerger::isLineExtensible(Vector2f startPoint, Vector2f & endPoint, const Vector2f slope, const Vector2f gradient, const size_t maxLength) {
	const Vector2f normal = Vector2f(slope.y, -slope.x);
	bool isExtensible = true;

	for (size_t step = 0; step < maxLength; step++) {
		startPoint += slope;

		if (edgeKernelX(startPoint.x, startPoint.y) >= KERNEL_THRESHOLD / 2 ||
			edgeKernelY(startPoint.x, startPoint.y) >= KERNEL_THRESHOLD / 2 ||
			isGradientIntensive(startPoint, normal, gradient)) {
			continue;
		}

		isExtensible = false;
		break;
	}

	endPoint = startPoint - slope;
	return isExtensible;
}

bool LineSegmentMerger::isGradientIntensive(Vector2f startPoint, Vector2f normal, Vector2f gradient) const {
	return
		edgelGradientIntensity(startPoint.x, startPoint.y) * gradient > ORIENTATION_THRESHOLD ||
		edgelGradientIntensity(startPoint.x + normal.x, startPoint.y + normal.y) * gradient > ORIENTATION_THRESHOLD ||
		edgelGradientIntensity(startPoint.x - normal.x, startPoint.y - normal.y) * gradient > ORIENTATION_THRESHOLD;
}
