#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Events.h"
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

	struct WindowResize
	{
	public:
		int Width;
		int Height;
		int OldWidth;
		int OldHeight;
	};

	namespace Internal
	{
		void HandleWindowResize(GLFWwindow* window, int width, int height);
	}

	class Window
	{
	private:
		WindowProps m_Props;
		GLFWwindow* m_WindowHandle;

	public:
		const EventEmitter<WindowResize> WindowResized = {};

	public:
		Window(const WindowProps& props);
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(Window&& other) = delete;
		~Window();

		inline GLFWwindow* GetNativeHandle() const { return m_WindowHandle; }
		inline const WindowProps& GetProps() const { return m_Props; }

		inline Viewport GetViewport() const { return { 0, 0, m_Props.Width, m_Props.Height }; }

		bool ShouldClose() const;
		void PollEvents();
		void SwapBuffers();

		friend void Internal::HandleWindowResize(GLFWwindow* window, int width, int height);

	private:
		void Init();
	};

}
