#pragma once
#include <unordered_map>
#include <string>
#include <GLFW/glfw3.h>

class KeyManager {
	std::unordered_map<std::string, bool> options;
	std::unordered_map<char, std::string> keyToOptionsMap{
		{'1', "regions"},
		{'2', "subRegions"},
		{'3', "edgels"},
		{'4', "lineSegments"},
		{'5', "mergedLineSegments"},
		{'6', "extendedLineSegments"},
		{'7', "lineSegmentsWithCorner"},
		{'8', "markers"}
	};

public:
	static void init(GLFWwindow* window);
	static void keyPressed(char keyCode);
	bool isActive(std::string option);
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	void handleEvents();
};

