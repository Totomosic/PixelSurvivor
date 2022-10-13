#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Pixel
{

    const Window* Input::s_Window = nullptr;
    glm::vec2 Input::s_LastMousePos = { 0, 0 };
    glm::vec2 Input::s_MouseDownPos = { 0, 0 };
    MouseButton Input::s_MouseDownButton = MouseButton::Last;

    const EventEmitter<KeyCode> Input::KeyPressed;
    const EventEmitter<KeyCode> Input::KeyReleased;

    const EventEmitter<MouseMove> Input::MouseMoved;
    const EventEmitter<MouseButton> Input::MousePressed;
    const EventEmitter<MouseButton> Input::MouseReleased;
    const EventEmitter<MouseButton> Input::MouseClicked;

    void Input::SetWindow(const Window* window)
    {
        s_Window = window;
        GLFWwindow* handle = (GLFWwindow*)window->GetNativeHandle();

        glfwSetKeyCallback(handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                switch (action)
                {
                case GLFW_PRESS:
                    KeyPressed.Trigger(KeyCode(key));
                    break;
                case GLFW_RELEASE:
                    KeyReleased.Trigger(KeyCode(key));
                    break;
                default:
                    break;
                }
            });

        glfwSetMouseButtonCallback(handle, [](GLFWwindow* window, int button, int action, int mods)
            {
                switch (action)
                {
                case GLFW_PRESS:
                    s_MouseDownPos = GetMousePosition();
                    s_MouseDownButton = MouseButton(button);
                    MousePressed.Trigger(MouseButton(button));
                    break;
                case GLFW_RELEASE:
                    MouseReleased.Trigger(MouseButton(button));
                    if (MouseButton(button) == s_MouseDownButton && glm::length(GetMousePosition() - s_MouseDownPos) < 3.0f)
                        MouseClicked.Trigger(MouseButton(button));
                    break;
                default:
                    break;
                }
            });

        glfwSetCursorPosCallback(handle, [](GLFWwindow* window, double xPos, double yPos)
            {
                glm::vec2 position = { float(xPos), float(yPos) };
                MouseMove evt;
                evt.Movement = position - s_LastMousePos;
                evt.Movement.y *= -1;
                s_LastMousePos = position;
                MouseMoved.Trigger(std::move(evt));
            });
    }

    bool Input::IsKeyDown(KeyCode key)
    {
        int state = glfwGetKey((GLFWwindow*)s_Window->GetNativeHandle(), int(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        int state = glfwGetMouseButton((GLFWwindow*)s_Window->GetNativeHandle(), int(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        double x;
        double y;
        glfwGetCursorPos((GLFWwindow*)s_Window->GetNativeHandle(), &x, &y);
        return glm::vec2{ float(x), s_Window->GetProps().Height - float(y) };
    }

    glm::vec2 Input::GetRelMousePosition()
    {
        glm::vec2 mousePos = GetMousePosition();
        return { mousePos.x - s_LastMousePos.x, s_LastMousePos.y - (s_Window->GetProps().Height - mousePos.y) };
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }

}
