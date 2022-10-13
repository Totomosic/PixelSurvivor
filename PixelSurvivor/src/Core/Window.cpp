#include "Window.h"
#include "Renderer/RenderCommand.h"

namespace Pixel
{

	namespace Internal
	{

		void HandleWindowResize(GLFWwindow* window, int width, int height)
		{
			Window* w = (Window*)glfwGetWindowUserPointer(window);
			WindowResize evt;
			evt.Width = width;
			evt.Height = height;
			evt.OldWidth = w->GetProps().Width;
			evt.OldHeight = w->GetProps().Height;

			w->m_Props.Width = width;
			w->m_Props.Height = height;

			w->WindowResized.Trigger(std::move(evt));
		}

	}

	Window::Window(const WindowProps& props)
		: m_Props(props), m_WindowHandle(nullptr)
	{
		Init();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowHandle);
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
		glfwSetWindowUserPointer(m_WindowHandle, this);
		glfwMakeContextCurrent(m_WindowHandle);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSwapInterval(1);

		glfwSetFramebufferSizeCallback(m_WindowHandle, Internal::HandleWindowResize);

		RenderCommand::Init();
	}

}
