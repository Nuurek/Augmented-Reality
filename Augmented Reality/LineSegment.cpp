#include "LineSegment.h"


LineSegment::LineSegment(Edgel start, Edgel end)
	: start(start), end(end) {
	slope = start.slope;
}

bool LineSegment::coincide(Edgel & edgel) {
	if (!start.isOrientationCompatible(edgel)) {
		return false;
	}

	return distanceFromPointToLine(start, end, edgel) < LINE_COINCIDENCE_DISTANCE;
}

Vector2f LineSegment::getStartEndSlope() {
	return end.position - start.position;
}

Vector2f LineSegment::getOrienation() {
	return Vector2f(-start.slope.y, start.slope.x);
}

void LineSegment::swapEndPoints() {
	std::swap(start, end);
}


float LineSegment::distanceFromPointToLine(Edgel & start, Edgel & end, Edgel & point) const {
	float crossProduct = 0.0f;
	crossProduct += (end.position.x - start.position.x) * (point.position.y - start.position.y);
	crossProduct -= (end.position.y - start.position.y) * (point.position.x - start.position.x);

	float lineLength = Vector2f(end.position.x - start.position.x, end.position.y - start.position.y).get_length();

	return fabs(crossProduct / lineLength);
}