#include <iostream>
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Clock.h"
#include "Renderer/Renderer.h"

struct Animation
{
public:
    int FrameCount;
    int FrameRate;
    Pixel::Texture2D** Frames;
};

int main()
{
    Pixel::Logger::Init();

    Pixel::WindowProps props;
    props.Width = 1280;
    props.Height = 720;
    props.Title = "Pixel Survivor";
    Pixel::Window window(props);
    Pixel::Input::SetWindow(&window);

    Pixel::Renderer renderer(window.GetViewport());

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)props.Width, 0.0f, (float)props.Height, -1.0f, 1.0f);

    window.WindowResized.AddEventListener([&renderer, &window, &projMatrix](Pixel::Event<Pixel::WindowResize>& evt)
        {
            renderer.SetViewport(window.GetViewport());
            projMatrix = glm::ortho(0.0f, (float)evt.Data.Width, 0.0f, (float)evt.Data.Height, -1.0f, 1.0f);
        });

    Pixel::Texture2D run1 = Pixel::Texture2D::FromFile("../Art/Character/ScytheRun/Character_Pixel1.png", {});
    Pixel::Texture2D run2 = Pixel::Texture2D::FromFile("../Art/Character/ScytheRun/Character_Pixel2.png", {});
    Pixel::Texture2D run3 = Pixel::Texture2D::FromFile("../Art/Character/ScytheRun/Character_Pixel3.png", {});
    Pixel::Texture2D run4 = Pixel::Texture2D::FromFile("../Art/Character/ScytheRun/Character_Pixel4.png", {});
    Pixel::Texture2D run5 = Pixel::Texture2D::FromFile("../Art/Character/ScytheRun/Character_Pixel5.png", {});

    Pixel::Texture2D idle1 = Pixel::Texture2D::FromFile("../Art/Character/Idle/Character_Pixel1.png", {});
    Pixel::Texture2D idle2 = Pixel::Texture2D::FromFile("../Art/Character/Idle/Character_Pixel2.png", {});

    Pixel::Texture2D* runFrames[] = { &run1, &run2, &run3, &run2, &run1, &run4, &run5, &run4 };
    Pixel::Texture2D* idleFrames[] = { &idle1, &idle2 };

    Animation runAnimation;
    runAnimation.FrameCount = 8;
    runAnimation.FrameRate = 8;
    runAnimation.Frames = runFrames;

    Animation idleAnimation;
    idleAnimation.FrameCount = 2;
    idleAnimation.FrameRate = 2;
    idleAnimation.Frames = idleFrames;

    Animation animations[] = { idleAnimation, runAnimation };

    int pixelSize = 5;

    float position = 500.0f;
    float velocity = 0.0f;
    int direction = 1;
    float speed = 24 * pixelSize * 4.0f;

    int animationIndex = 0;
    float timeRemaining = 1.0f / animations[animationIndex].FrameRate;
    int frameIndex = 0;

    Pixel::Clock clock;
    while (!window.ShouldClose())
    {
        clock.StartFrame();
        window.PollEvents();

        float elapsed = clock.GetElapsedSeconds();

        timeRemaining -= elapsed;
        if (Pixel::Input::IsKeyDown(Pixel::KeyCode::A))
        {
            velocity = -speed;
            direction = -1;
            if (animationIndex != 1)
            {
                animationIndex = 1;
                frameIndex = 0;
                timeRemaining = 1.0f / animations[animationIndex].FrameRate;
            }
        }
        else if (Pixel::Input::IsKeyDown(Pixel::KeyCode::D))
        {
            velocity = speed;
            direction = 1;
            if (animationIndex != 1)
            {
                animationIndex = 1;
                frameIndex = 0;
                timeRemaining = 1.0f / animations[animationIndex].FrameRate;
            }
        }
        else
        {
            velocity = 0;
            if (animationIndex != 0)
            {
                animationIndex = 0;
                frameIndex = 0;
                timeRemaining = 1.0f / animations[animationIndex].FrameRate;
            }
        }

        if (timeRemaining < 0.0f)
        {
            frameIndex = (frameIndex + 1) % animations[animationIndex].FrameCount;
            timeRemaining = 1.0f / animations[animationIndex].FrameRate;
        }

        position += velocity * elapsed;

        renderer.BeginLayer({ pixelSize }, { viewMatrix, projMatrix, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f } });
        renderer.DrawSprite({ position, 200.0f, 0.0f}, {24.0f * pixelSize * direction, 24.0f * pixelSize}, animations[animationIndex].Frames[frameIndex]);
        renderer.EndLayer();

        window.SwapBuffers();
        clock.EndFrame();
    }
    return 0;
}
