#include "..\iostream.h"

Input::Input()
{
}

Input::~Input()
{
}

bool Input::Press(int key)
{
	if (keys[key] && checkKeys[key])
	{
		checkKeys[key] = false;
		return true;
	}

	return false;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key < 0) return;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//if (key == GLFW_KEY_F && action == GLFW_PRESS)
	//	gWindow.SwitchMode();
	//if (key == GLFW_KEY_T && action == GLFW_PRESS)
	//	bUpdateF = !bUpdateF;
	if (action == GLFW_PRESS)
	{
		gInput.checkKeys[key] = true;
		gInput.keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		gInput.checkKeys[key] = false;
		gInput.keys[key] = false;
	}

	
}