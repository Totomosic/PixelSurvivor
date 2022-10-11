#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Pixel
{

	struct WindowProps
	{
	public:
		int Width = 1280;
		int Height = 720;
		std::string Title = "PixelSurvivor";
	};

	class Window
	{
	private:
		WindowProps m_Props;
		GLFWwindow* m_WindowHandle;

	public:
		Window(const WindowProps& props);

		inline GLFWwindow* GetHandle() const { return m_WindowHandle; }
		inline const WindowProps& GetProps() const { return m_Props; }

		bool ShouldClose() const;
		void PollEvents();
		void SwapBuffers();

	private:
		void Init();
	};

}
