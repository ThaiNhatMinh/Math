#pragma once
#include "iostream.h"
extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void scroll_callback(GLFWwindow*, double, double);


class Windows
{
private:
	GLuint m_iWidth;
	GLuint m_iHeight;
	GLFWmonitor* m_pMonitor;
	GLFWwindow* m_pWindow;
	string m_WindowTitle;
	GLboolean m_bIsFullscreen;
	GLuint m_iScreenWidth;
	GLuint m_iScreenHeight;
public:
	Windows(string title,int W,int H);
	~Windows();
	bool InitWindow();
	void SetSize(int W, int H);
	void EnableFullScreen(bool enable);
	void SwitchMode();
	Vec2 GetWindowSize(){ return Vec2(m_iWidth, m_iHeight); };
	GLFWwindow* Window() {		return m_pWindow;	};
};

