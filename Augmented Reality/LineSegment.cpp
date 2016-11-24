#include "LineSegment.h"

LineSegment::LineSegment(Edgel start, Edgel end) 
	: start(start), end(end) {
	slope = start.slope;
}

void LineSegment::clearSupportEdgels() {
	supportEdgels.clear();
}

void LineSegment::addSupportEdgel(Edgel & edgel) {
	supportEdgels.emplace_back(edgel);
}
