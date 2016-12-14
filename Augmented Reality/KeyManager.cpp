#include "KeyManager.h"
#include <iostream>

std::unordered_map<char, std::string> KeyManager::keyToOptionsMap {
	{ GLFW_KEY_1, "regions"},
	{ GLFW_KEY_2, "subRegions" },
	{ GLFW_KEY_3, "edgels" },
	{ GLFW_KEY_4, "lineSegments" },
	{ GLFW_KEY_5, "mergedLineSegments" },
	{ GLFW_KEY_6, "extendedLineSegments" },
	{ GLFW_KEY_7, "lineSegmentsWithCorner" },
	{ GLFW_KEY_8, "markers" },
	{ GLFW_KEY_ESCAPE, "escape" },
	{ GLFW_KEY_W, "W" },
	{ GLFW_KEY_A, "A" },
	{ GLFW_KEY_D, "D" },
	{ GLFW_KEY_S, "S" },
	{ GLFW_KEY_E, "E" },
	{ GLFW_KEY_Q, "Q" },
	{ GLFW_KEY_C, "calibrating" },
	{ GLFW_KEY_V, "markerFinding" },
	{ GLFW_KEY_B, "augmentedReality" },
	{ GLFW_KEY_P, "cornersDebug" },
};

std::unordered_map<std::string, bool> KeyManager::options = {};
std::string KeyManager::keyString = "";

KeyManager::KeyManager(GLFWwindow * window) 
	: window(window) {
	glfwSetKeyCallback(window, keyCallback);
}

void KeyManager::keyPressed(char keyCode) {
	std::cout << "Key pressed: " << keyCode << "\n";
	auto option = keyToOptionsMap[keyCode];
	if (option != "") {
		bool previousState = options[option];
		options[option] = !previousState;

		keyString = option;
	}
}

bool KeyManager::isActive(std::string option) {
	return options[option];
}

void KeyManager::keyCallback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		keyPressed(key);
	}
}

void KeyManager::handleEvents() {
	glfwPollEvents();
}
