#include "Window.h"

namespace Pixel
{

	Window::Window(const WindowProps& props)
		: m_Props(props), m_WindowHandle(nullptr)
	{
		Init();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_WindowHandle);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void Window::Init()
	{
		auto result = glfwInit();
		m_WindowHandle = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_WindowHandle);
		gladLoadGL();
	}

}
