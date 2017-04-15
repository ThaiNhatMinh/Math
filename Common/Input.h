#pragma once

#include "..\iostream.h"

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

class Input
{
private:
	bool keys[1024]; // hold key status
	bool checkKeys[1024]; // hold last keu status
public:
	Input();
	~Input();

	bool OnKey(int Key) {return keys[Key];	};
	bool Press(int key);
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
};