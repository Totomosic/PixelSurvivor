#pragma once
#include "Utils.h"
#include "Texture2D.h"
#include "Core/Logging.h"
#include <memory>
#include <vector>

namespace Pixel
{

    enum class FramebufferTextureFormat
    {
        None = 0,

        RGBA8,
        RGBA16F,
        RED_INTEGER,

        DEPTH32,
        Depth = DEPTH32,
    };

    enum class FramebufferTextureType
    {
        Texture2D,
    };

    inline constexpr GLenum FramebufferTextureFormatToOpenGL(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::RGBA8:
            return GL_RGBA8;
        case FramebufferTextureFormat::RGBA16F:
            return GL_RGBA16F;
        case FramebufferTextureFormat::RED_INTEGER:
            return GL_RED_INTEGER;
        }
        PIX_ASSERT(false, "Invalid texture format");
        return 0;
    }

    struct FramebufferTextureSpecification
    {
    public:
        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        FramebufferTextureType TextureType = FramebufferTextureType::Texture2D;
    };

    struct FramebufferProps
    {
    public:
        uint32_t Width = 1280;
        uint32_t Height = 720;
        int Samples = 0;
        std::vector<FramebufferTextureSpecification> Attachments;
    };

    class Framebuffer
    {
    private:
        Internal::ScopedHandle<Internal::DeleteFramebuffer> m_Handle;
        FramebufferProps m_Props;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<std::shared_ptr<Texture2D>> m_ColorAttachments;
        std::shared_ptr<Texture2D> m_DepthAttachment;

    public:
        Framebuffer(const FramebufferProps& props);

        inline bool SupportsDepth() const
        {
            return m_DepthAttachment != nullptr;
        }

        inline uint32_t GetWidth() const
        {
            return m_Props.Width;
        }

        inline uint32_t GetHeight() const
        {
            return m_Props.Height;
        }

        inline int GetSamples() const
        {
            return m_Props.Samples;
        }

        inline float GetAspect() const
        {
            return float(GetWidth()) / float(GetHeight());
        }

        inline bool IsMultisampled() const
        {
            return GetSamples() > 0;
        }

        inline Viewport GetViewport() const
        {
            return { 0, 0, (int)GetWidth(), (int)GetHeight() };
        }

        inline std::shared_ptr<Texture2D> GetColorAttachment(int index) const
        {
            PIX_ASSERT(index >= 0 && index < m_ColorAttachments.size(), "Invalid index");
            return m_ColorAttachments[index];
        }
        inline std::shared_ptr<Texture2D> GetDepthAttachment() const
        {
            return m_DepthAttachment;
        }

        void Bind() const;
        void Unbind() const;
        void SetSize(uint32_t width, uint32_t height);
        void ClearAttachment(int index, int value);
        int ReadPixel(int index, int x, int y);
        float ReadDepthPixel(int x, int y);

        friend class RenderTexture;

    public:
        static Framebuffer Create(const FramebufferProps& props);
        static Framebuffer CreateWindowFramebuffer(uint32_t width, uint32_t height);

    private:
        void AttachColorTexture(const std::shared_ptr<Texture2D>& texture, int index);
        void AttachDepthTexture(const std::shared_ptr<Texture2D>& texture);
        void Init();
        void Invalidate();
    };

}
