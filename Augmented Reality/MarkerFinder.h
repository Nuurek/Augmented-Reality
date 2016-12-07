#pragma once

#include "LineSegment.h"
#include "Marker.h"

class MarkerFinder {

public:
	std::vector<Marker> findMarkers(std::vector<LineSegment>);
};

