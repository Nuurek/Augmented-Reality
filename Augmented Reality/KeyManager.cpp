#include "KeyManager.h"
#include <iostream>

std::unordered_map<char, std::string> KeyManager::keyToOptionsMap={
	{ GLFW_KEY_1, "regions"},
	{ GLFW_KEY_2, "subRegions" },
	{ GLFW_KEY_3, "edgels" },
	{ GLFW_KEY_4, "lineSegments" },
	{ GLFW_KEY_ESCAPE, "escape" }
};
std::unordered_map<std::string, bool> KeyManager::options;
void KeyManager::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyManager::key_callback);
}
void KeyManager::keyPressed(char keyCode) {
	std::string option = keyToOptionsMap[keyCode];
	if (option == "") {
		return;
	}
	bool previousState = options[option];
	options[option] = !previousState;
}

bool KeyManager::isActive(std::string option) {
	return options[option];
}
void KeyManager::key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		keyPressed(key);
	}


}

void KeyManager::handleEvents() {
	glfwPollEvents();
}
