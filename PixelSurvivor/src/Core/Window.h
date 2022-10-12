#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer/Utils.h"

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

		inline Viewport GetViewport() const { return { 0, 0, m_Props.Width, m_Props.Height }; }

		bool ShouldClose() const;
		void PollEvents();
		void SwapBuffers();

	private:
		void Init();
	};

}
