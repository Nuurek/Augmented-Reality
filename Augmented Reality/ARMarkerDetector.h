#pragma once
#include "RegionBasedOperator.h"
#include "Edgel.h"
#include "EdgelDetector.h"


class ARMarkerDetector : public RegionBasedOperator {
	EdgelDetector edgelDetector;
	std::vector<Edgel> edgels;

	void clearStructures();

public:
	ARMarkerDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	void setBuffer(Buffer* buffer);

	void findARMarkers();

	std::vector<Edgel> getEdgels();
};

