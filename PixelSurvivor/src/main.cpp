#include <iostream>
#include "Core/Window.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture2D.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Framebuffer.h"

int main()
{
    Pixel::Logger::Init();

    Pixel::WindowProps props;
    props.Width = 1280;
    props.Height = 720;
    props.Title = "Pixel Survivor";
    Pixel::Window window(props);

    Pixel::VertexArray vertexArray;

    float* vertices = new float[]{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    };
    uint32_t* indices = new uint32_t[]{ 1, 0, 3, 1, 3, 2 };

    Pixel::BufferLayout layout = {
        { GL_FLOAT, 3 },
        { GL_FLOAT, 2 },
    };

    vertexArray.SetVertexBuffer(std::make_unique<Pixel::VertexBuffer>(vertices, 4, layout));
    vertexArray.SetIndexBuffer(std::make_unique<Pixel::IndexBuffer>(indices, 6));

    delete[] vertices;
    delete[] indices;

    std::string vertexShader =
        "#version 450 core\n"
        "in layout(location = 0) vec3 v_Position;\n"
        "in layout(location = 1) vec2 v_TexCoord;\n"
        "uniform mat4 u_ModelMatrix;\n"
        "uniform mat4 u_ViewMatrix;\n"
        "uniform mat4 u_ProjectionMatrix;\n"
        "out vec2 f_TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);\n"
        "   f_TexCoord = v_TexCoord;\n"
        "}\n";

    std::string fragmentShader =
        "#version 450 core\n"
        "in vec2 f_TexCoord;\n"
        "out layout(location = 0) vec4 f_Color;\n"
        "uniform vec4 u_Color;\n"
        "uniform sampler2D u_Texture;\n"
        "void main()\n"
        "{\n"
        "   f_Color = texture(u_Texture, f_TexCoord);\n"
        "}\n";

    Pixel::Shader shader(vertexShader, fragmentShader);
    int colorLocation = shader.GetUniformLocation("u_Color");
    int textureLocation = shader.GetUniformLocation("u_Texture");

    int modelMatrixLocation = shader.GetUniformLocation("u_ModelMatrix");
    int viewMatrixLocation = shader.GetUniformLocation("u_ViewMatrix");
    int projMatrixLocation = shader.GetUniformLocation("u_ProjectionMatrix");

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3{ 500.0f, 200.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3{ 100.0f, 200.0f, 1.0f });
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)props.Width, 0.0f, (float)props.Height);

    Pixel::Texture2D texture = Pixel::Texture2D::FromFile("../Art/Character.png", {});

    int pixelSize = 3;
    Pixel::FramebufferProps fprops;
    fprops.Width = props.Width / pixelSize;
    fprops.Height = props.Height / pixelSize;
    fprops.Attachments = {
        { Pixel::FramebufferTextureFormat::RGBA16F },
        { Pixel::FramebufferTextureFormat::Depth },
    };
    Pixel::Framebuffer framebuffer = Pixel::Framebuffer::Create(fprops);
    framebuffer.Unbind();

    Pixel::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });

    while (!window.ShouldClose())
    {
        window.PollEvents();

        framebuffer.Bind();

        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3{ 500.0f, 200.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3{ 100.0f, 200.0f, 1.0f });

        shader.Bind();
        shader.SetUniform(colorLocation, glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
        shader.SetUniform(textureLocation, 0);

        shader.SetUniform(modelMatrixLocation, modelMatrix);
        shader.SetUniform(viewMatrixLocation, viewMatrix);
        shader.SetUniform(projMatrixLocation, projMatrix);

        texture.Bind(0);

        Pixel::RenderCommand::SetViewport(framebuffer.GetViewport());
        Pixel::RenderCommand::Clear();
        Pixel::RenderCommand::DrawIndexed(GL_TRIANGLES, vertexArray);

        framebuffer.Unbind();
        framebuffer.GetColorAttachment(0)->Bind(0);

        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3{ props.Width / 2.0f, props.Height / 2.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3{ props.Width, props.Height, 1.0f });
        shader.SetUniform(modelMatrixLocation, modelMatrix);

        Pixel::RenderCommand::SetViewport(window.GetViewport());
        Pixel::RenderCommand::Clear();
        Pixel::RenderCommand::DrawIndexed(GL_TRIANGLES, vertexArray);

        shader.Unbind();

        window.SwapBuffers();
    }
    return 0;
}
