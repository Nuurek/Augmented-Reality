#pragma once
#include <vector>
#include "Buffer.h"
#include "Edgel.h"


class EdgelDetector {
	Buffer* buffer;

public:
	void setBuffer(Buffer* buffer);

	std::vector<Edgel> findEdgels();
};

