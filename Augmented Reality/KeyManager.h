#pragma once
#include <unordered_map>
#include <string>
#include <GLFW/glfw3.h>

class KeyManager {
public:
	static void init(GLFWwindow* window);
	static void keyPressed(char keyCode);
	bool isActive(std::string option);
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	void handleEvents();
	static std::unordered_map<std::string, bool> options;
	static std::unordered_map<char, std::string> keyToOptionsMap;
};

