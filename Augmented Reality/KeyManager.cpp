#include "KeyManager.h"

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
