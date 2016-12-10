#include "MarkerFinder.h"

/*
std::vector<ARMarker> MarkerFinder::findMarkers(std::vector<LineSegment> linesWithCorners) {
	std::vector<ARMarker> markers;
	std::vector<std::vector<LineSegment>> chains;
	std::list<LineSegment> listOfLines{ linesWithCorners.begin(), linesWithCorners.end() };

	while (listOfLines.size() >= 4) {
		auto chain = findMarker(listOfLines);
		if (chain.size() == 4) {
			markers.push_back(convertChainToARMarker(chain));
		}
	}

	return markers;
}

std::vector<LineSegment> MarkerFinder::findMarker(std::list<LineSegment>& listOfSegments) {
	size_t lengthOfChain = 0;
	std::vector<LineSegment> chain;

	auto startOfChain = listOfSegments.front();
	listOfSegments.pop_front();
	chain.push_back(LineSegment(startOfChain));

	bool nextFound = false;
	bool isStart = true;
	EndPoint endPoint = EndPoint::NONE;

	do {
		nextFound = false;
		isStart = !(chain.size() > 1);

		for (auto& segment : listOfSegments) {
			auto isChain = areSegmentsInChain(startOfChain, segment, isStart, endPoint);
			if (isChain.first) {
				chain.push_back(LineSegment(segment));
				startOfChain = chain[chain.size() - 1];
				listOfSegments.remove(segment);
				nextFound = true;
				endPoint = isChain.second;
				break;
			}
		}
	} while (nextFound && chain.size() < 4);

	return chain;
}
*/

ARMarker MarkerFinder::convertChainToARMarker(std::vector<LineSegment>& chain) {
	size_t size = chain.size();

	std::array<Vector2f, 4> vertices;

	for (size_t i = 0; i < size; i++) {
		auto& predecessor = chain[i % size];
		auto& successor = chain[(i + 1) % size];

		auto intersectionPoint = predecessor.getIntersection(successor);
		vertices[i] = intersectionPoint;
	}

	return ARMarker(vertices);
}


std::vector<ARMarker> MarkerFinder::findMarkers(std::vector<LineSegment> linesWithCorners) {
	std::vector<ARMarker> markers;

	if (linesWithCorners.size() >= 4) {
		do {

			// pak een willekeurig segment, en probeer hier een chain mee te maken..
			LineSegment chainSegment = linesWithCorners[0];
			linesWithCorners[0] = linesWithCorners[linesWithCorners.size() - 1];
			linesWithCorners.resize(linesWithCorners.size() - 1);

			std::vector<LineSegment> chain;
			int length = 1;

			// kijk eerst of er schakels voor dit element moeten...
			findChainOfLines(chainSegment, linesWithCorners, chain, length);

			chain.push_back(chainSegment);

			if (length > 3) {
				markers.push_back(convertChainToARMarker(chain));
			}
		} while (linesWithCorners.size());
	}

	return markers;
}

void MarkerFinder::findChainOfLines(LineSegment &startSegment, std::vector<LineSegment> &linesegments, std::vector<LineSegment> &chain, int& length) {
	for (int i = 0; i<linesegments.size(); i++) {
		if (!areSegmentsInChain(startSegment, linesegments[i])) {
			continue;
		}

		length++;

		LineSegment chainSegment = linesegments[i];
		linesegments[i] = linesegments[linesegments.size() - 1];
		linesegments.resize(linesegments.size() - 1);

		if (length == 4) {
			chain.push_back(chainSegment);
			return;
		}
		// recursie!
		findChainOfLines(chainSegment, linesegments, chain, length);
		chain.push_back(chainSegment);
		return;
	}
}

bool MarkerFinder::areSegmentsInChain(const LineSegment& first, const LineSegment& second) const {
	if (first.isOrientationCompatible(second)) {
		return false;
	}

	const auto& startPoint = first.start.position;
	const auto& endPoint = second.end.position;

	if ((endPoint - startPoint).get_squared_length() > CHAIN_MAX_GAP) {
		return false;
	}

	const float orientation = first.slope.x * second.slope.y - first.slope.y * second.slope.x;

	if (orientation <= 0) {
		return false;
	}

	return true;
}
