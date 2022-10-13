#pragma once
#include "Keycodes.h"
#include "Events.h"
#include <glm/glm.hpp>

namespace Pixel
{

	class Window;

	struct MouseMove
	{
	public:
		glm::vec2 Movement;
	};

	class Input
	{
	private:
		static const Window* s_Window;
		static glm::vec2 s_LastMousePos;

		static glm::vec2 s_MouseDownPos;
		static MouseButton s_MouseDownButton;

	public:
		static const EventEmitter<KeyCode> KeyPressed;
		static const EventEmitter<KeyCode> KeyReleased;

		static const EventEmitter<MouseMove> MouseMoved;
		static const EventEmitter<MouseButton> MousePressed;
		static const EventEmitter<MouseButton> MouseReleased;
		static const EventEmitter<MouseButton> MouseClicked;

	public:
		static void SetWindow(const Window* window);
		static bool IsKeyDown(KeyCode key);

		static bool IsMouseButtonDown(MouseButton button);
		static glm::vec2 GetMousePosition();
		static glm::vec2 GetRelMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
