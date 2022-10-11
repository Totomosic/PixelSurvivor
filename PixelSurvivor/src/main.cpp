#include <iostream>
#include "Core/Window.h"
#include "Renderer/VertexArray.h"

int main()
{
    Pixel::WindowProps props;
    props.Width = 1280;
    props.Height = 720;
    props.Title = "Pixel Survivor";
    Pixel::Window window(props);

    Pixel::VertexArray vertexArray;

    float* vertices = new float[]{ -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
    uint32_t* indices = new uint32_t[]{ 0, 1, 2 };

    Pixel::BufferLayout layout = {
        { GL_FLOAT, 3 },
    };

    vertexArray.SetVertexBuffer(std::make_unique<Pixel::VertexBuffer>(vertices, 3, layout));
    vertexArray.SetIndexBuffer(std::make_unique<Pixel::IndexBuffer>(indices, 3));

    delete[] vertices;
    delete[] indices;

    while (!window.ShouldClose())
    {
        window.PollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
        vertexArray.Unbind();

        window.SwapBuffers();
    }
    return 0;
}
