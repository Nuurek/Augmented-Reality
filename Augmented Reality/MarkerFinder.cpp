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

std::pair<bool, EndPoint> MarkerFinder::areSegmentsInChain(LineSegment & predecessor, LineSegment & successor, bool isStart, EndPoint endPoint) {
	// check if not parallel
	if (predecessor.isOrientationCompatible(successor)) {
		return std::make_pair<bool, EndPoint>(false, EndPoint::NONE);
	}

	// check if not to far away
	auto predecessorStartPoint = predecessor.start.position;
	auto predecessorEndPoint = predecessor.end.position;
	auto successorStartPoint = successor.start.position;
	auto successorEndPoint = successor.end.position;

	bool isCloseEnough = false;
	EndPoint newEndPoint = EndPoint::NONE;

	if (isStart) {
		if (((predecessorStartPoint - successorStartPoint).get_squared_length() < CHAIN_MAX_GAP) ||
			((predecessorEndPoint - successorStartPoint).get_squared_length() < CHAIN_MAX_GAP)) {
			isCloseEnough = true;
			newEndPoint = EndPoint::END;
		} else if (((predecessorStartPoint - successorEndPoint).get_squared_length() < CHAIN_MAX_GAP) ||
			((predecessorEndPoint - successorEndPoint).get_squared_length() < CHAIN_MAX_GAP)) {
			isCloseEnough = true;
			newEndPoint = EndPoint::START;
		}
	} else {
		if (endPoint == EndPoint::START) {
			if ((predecessorStartPoint - successorStartPoint).get_squared_length() < CHAIN_MAX_GAP) {
				isCloseEnough = true;
				newEndPoint = EndPoint::END;
			} else if ((predecessorStartPoint - successorEndPoint).get_squared_length() < CHAIN_MAX_GAP) {
				isCloseEnough = true;
				newEndPoint = EndPoint::START;
			}
		} else if (endPoint == EndPoint::END) {
			if ((predecessorEndPoint - successorStartPoint).get_squared_length() < CHAIN_MAX_GAP) {
				isCloseEnough = true;
				newEndPoint = EndPoint::END;
			} else if ((predecessorEndPoint - successorEndPoint).get_squared_length() < CHAIN_MAX_GAP) {
				isCloseEnough = true;
				newEndPoint = EndPoint::START;
			}
		}
	}
	
	return std::pair<bool, EndPoint>(isCloseEnough, newEndPoint);
}

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

	do {

		// pak een willekeurig segment, en probeer hier een chain mee te maken..
		LineSegment chainSegment = linesWithCorners[0];
		linesWithCorners[0] = linesWithCorners[linesWithCorners.size() - 1];
		linesWithCorners.resize(linesWithCorners.size() - 1);

		std::vector<LineSegment> chain;
		int length = 1;

		// kijk eerst of er schakels voor dit element moeten...
		findChainOfLines(chainSegment, true, linesWithCorners, chain, length);

		chain.push_back(chainSegment);

		// en misschien ook nog wel erna..
		if (length < 4) {
			findChainOfLines(chainSegment, false, linesWithCorners, chain, length);
		}

		if (length > 3) {
			markers.push_back(convertChainToARMarker(chain));
		}
	} while (linesWithCorners.size());

	return markers;
}

void MarkerFinder::findChainOfLines(LineSegment &startSegment, bool atStartPoint, std::vector<LineSegment> &linesegments, std::vector<LineSegment> &chain, int& length) {
	const Vector2f startPoint = atStartPoint ? startSegment.start.position : startSegment.end.position;

	for (int i = 0; i<linesegments.size(); i++) {
		// lijnen mogen niet parallel liggen
		if (startSegment.isOrientationCompatible(linesegments[i])) {
			continue;
		}
		// eind en startpunt moeten dicht bij elkaar liggen...
		if ((startPoint - (atStartPoint ? linesegments[i].end.position : linesegments[i].start.position)).get_squared_length() > 16.0f) {
			continue;
		}
		// en de orientatie moet natuurlijk goed zijn, dus tegen de klok mee rond een zwart vierkantje
		if ((atStartPoint &&
			(startSegment.slope.x * linesegments[i].slope.y - startSegment.slope.y * linesegments[i].slope.x <= 0)) ||
			(!atStartPoint &&
			(startSegment.slope.x * linesegments[i].slope.y - startSegment.slope.y * linesegments[i].slope.x >= 0))) {
			continue;
		}

		// het lijkt te mooi om waar te zijn, maar we hebben er 1 gevonden :)
		// haal dus dit segment er uit en kijk of de ketting langer te maken is...

		length++;

		LineSegment chainSegment = linesegments[i];
		linesegments[i] = linesegments[linesegments.size() - 1];
		linesegments.resize(linesegments.size() - 1);

		if (length == 4) {
			chain.push_back(chainSegment);
			return;
		}

		if (!atStartPoint) {
			chain.push_back(chainSegment);
		}
		// recursie!
		findChainOfLines(chainSegment, atStartPoint, linesegments, chain, length);
		if (atStartPoint) {
			chain.push_back(chainSegment);
		}
		return;
	}
}