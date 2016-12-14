#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <GLFW/glfw3.h>

class KeyManager {
	GLFWwindow* window;
	
public:
	static std::unordered_map<char, std::string> keyToOptionsMap;
	static std::unordered_map<std::string, bool> options;
	static std::string keyString;

	KeyManager(GLFWwindow* window);
	static void keyPressed(char keyCode);
	bool isActive(std::string option);
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	void handleEvents();
};

