#include "MagicEdgels.h"

MagicEdgels::MagicEdgels(int liveInterval) :liveInterval(liveInterval) {
}

void MagicEdgels::updateEdgels(std::vector<Edgel>& newEdgels) {
	std::vector<Edgel> edgelsToDelete;

	for (auto& liveEdgel : liveEdgels) {
		if (--liveEdgel.second <= 0) {
			edgelsToDelete.push_back(liveEdgel.first);
		}
	}

	for (auto& edgelToDelete : edgelsToDelete) {
		liveEdgels.erase(edgelToDelete);
	}

	for (auto& edgel : newEdgels) {
		liveEdgels[edgel] = liveInterval;
	}
}

std::vector<Edgel> MagicEdgels::getLiveEdgels() {
	std::vector<Edgel> edgels;
	for (auto edgel : liveEdgels) {
		edgels.push_back(edgel.first);
	}

	return edgels;
}

std::vector<std::pair<Edgel, double>> MagicEdgels::getMagicEdgels() {
	std::vector<std::pair<Edgel, double>> magicEdgels;

	for (auto edgel : liveEdgels) {
		magicEdgels.push_back(std::make_pair(Edgel(edgel.first), edgel.second / liveInterval));
	}

	return magicEdgels;
}
