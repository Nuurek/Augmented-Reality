#pragma once
#include "RegionBasedOperator.h"
#include "Edgel.h"
#include "EdgelDetector.h"
#include "LineSegment.h"
#include "LineSegmentDetector.h"
#include "LineSegmentMerger.h"
#include "LineSegmentExtender.h"
#include "CornersFinder.h"


const size_t EDGELS_IN_REGION = 5;

class ARMarkerDetector : public RegionBasedOperator {
	EdgelDetector edgelDetector;
	std::vector<Edgel> edgels;
	LineSegmentDetector lineSegmentDetector;
	std::vector<LineSegment> lineSegments;
	LineSegmentMerger lineSegmentMerger;
	std::vector<LineSegment> mergedLineSegments;
	LineSegmentExtender lineSegmentExtender;
	std::vector<LineSegment> extendedLineSegments;
	CornersFinder cornersFinder;
	std::vector<LineSegment> lineSegmentsWithCorner;

	void clearStructures();

public:
	ARMarkerDetector(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	
	void setBuffer(Buffer* buffer);

	void findARMarkers();

	std::vector<Edgel> getEdgels();
	std::vector<LineSegment> getLineSegments();
	std::vector<LineSegment> getMergedLineSegments();
	std::vector<LineSegment> getExtendedLineSegments();
	std::vector<LineSegment> getLineSegmentsWithCorner();
};

