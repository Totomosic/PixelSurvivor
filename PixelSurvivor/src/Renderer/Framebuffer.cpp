#include "Framebuffer.h"

namespace Pixel
{

    std::shared_ptr<Texture2D> CreateTexture2D(FramebufferTextureFormat format, uint32_t width, uint32_t height)
    {
        TextureFullProps props;
        props.Width = width;
        props.Height = height;
        switch (format)
        {
        case FramebufferTextureFormat::RGBA8:
            props.Format = GL_RGBA;
            break;
        case FramebufferTextureFormat::RED_INTEGER:
            props.Format = GL_RED_INTEGER;
            props.InternalFormat = GL_R32I;
            break;
        case FramebufferTextureFormat::RGBA16F:
            props.Format = GL_RGBA;
            break;
        case FramebufferTextureFormat::DEPTH32:
            props.Format = GL_DEPTH_COMPONENT;
            props.InternalFormat = GL_DEPTH_COMPONENT32;
            break;
        default:
            PIX_ASSERT(false, "Invalid texture format");
            break;
        }
        return std::make_shared<Texture2D>(Texture2D::FromData(nullptr, props));
    }

    Framebuffer::Framebuffer(const FramebufferProps& props) : m_Handle(), m_Props(props)
    {
        PIX_ASSERT(props.Width > 0 && props.Height > 0, "Invalid framebuffer dimensions");
        for (const FramebufferTextureSpecification& format : props.Attachments)
        {
            if (format.TextureFormat == FramebufferTextureFormat::Depth)
                m_DepthAttachmentSpecification = format;
            else
                m_ColorAttachmentSpecifications.push_back(format);
        }
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle.Id);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::SetSize(uint32_t width, uint32_t height)
    {
        m_Props.Width = width;
        m_Props.Height = height;
        if (m_Handle.Id != 0)
        {
            Invalidate();
        }
    }

    void Framebuffer::ClearAttachment(int index, int value)
    {
        PIX_ASSERT(index >= 0 && index < m_ColorAttachmentSpecifications.size(), "Invalid attachment index");
        FramebufferTextureSpecification& specification = m_ColorAttachmentSpecifications[index];
        glClearTexImage(m_ColorAttachments[index]->GetId(),
            0,
            FramebufferTextureFormatToOpenGL(specification.TextureFormat),
            GL_INT,
            &value);
    }

    int Framebuffer::ReadPixel(int index, int x, int y)
    {
        PIX_ASSERT(index >= 0 && index < m_ColorAttachmentSpecifications.size(), "Invalid attachment index");
        Bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    float Framebuffer::ReadDepthPixel(int x, int y)
    {
        PIX_ASSERT(m_DepthAttachment != nullptr &&
            m_DepthAttachmentSpecification.TextureType == FramebufferTextureType::Texture2D &&
            m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None,
            "Invalid depth attachment");
        Bind();
        float pixelData;
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixelData);
        return pixelData;
    }

    Framebuffer Framebuffer::Create(const FramebufferProps& props)
    {
        Framebuffer framebuffer(props);
        framebuffer.Invalidate();
        return framebuffer;
    }

    Framebuffer Framebuffer::CreateWindowFramebuffer(uint32_t width, uint32_t height)
    {
        FramebufferProps props;
        props.Width = width;
        props.Height = height;
        Framebuffer framebuffer(props);
        framebuffer.m_Handle.Id = 0;
        return framebuffer;
    }

    void Framebuffer::AttachColorTexture(const std::shared_ptr<Texture2D>& texture, int index)
    {
        PIX_ASSERT(m_Handle.Id != 0, "Invalid framebuffer");
        PIX_ASSERT(!IsMultisampled(), "Multisampled framebuffer cannot have texture attachments");
        texture->Bind();
        texture->SetWrapMode(GL_CLAMP_TO_EDGE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture->GetId(), 0);
    }

    void Framebuffer::AttachDepthTexture(const std::shared_ptr<Texture2D>& texture)
    {
        PIX_ASSERT(m_Handle.Id != 0, "Invalid framebuffer");
        PIX_ASSERT(!IsMultisampled(), "Multisampled framebuffer cannot have texture attachments");
        texture->Bind();
        texture->SetWrapMode(GL_CLAMP_TO_EDGE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->GetId(), 0);
    }

    void Framebuffer::Init()
    {
        glGenFramebuffers(1, &m_Handle.Id);
        Bind();
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    void Framebuffer::Invalidate()
    {
        if (m_Handle.Id != 0)
        {
            glDeleteFramebuffers(1, &m_Handle.Id);
            m_ColorAttachments = {};
            m_DepthAttachment = nullptr;
        }

        glCreateFramebuffers(1, &m_Handle.Id);
        Bind();

        bool multisample = IsMultisampled();

        if (m_ColorAttachmentSpecifications.size() > 0)
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                switch (m_ColorAttachmentSpecifications[i].TextureType)
                {
                case FramebufferTextureType::Texture2D:
                    m_ColorAttachments[i] =
                        CreateTexture2D(m_ColorAttachmentSpecifications[i].TextureFormat, GetWidth(), GetHeight());
                    break;
                }
                m_ColorAttachments[i]->Bind();
                AttachColorTexture(m_ColorAttachments[i], i);
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            switch (m_DepthAttachmentSpecification.TextureType)
            {
            case FramebufferTextureType::Texture2D:
                m_DepthAttachment =
                    CreateTexture2D(m_DepthAttachmentSpecification.TextureFormat, GetWidth(), GetHeight());
                break;
            }
            m_DepthAttachment->Bind();
            AttachDepthTexture(m_DepthAttachment);
        }

        if (m_ColorAttachments.size() > 1)
        {
            PIX_ASSERT(m_ColorAttachments.size() <= 4, "Too many color attachments");
            GLenum buffers[4] = {
              GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            glDrawBuffer(GL_NONE);
        }

        PIX_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
    }

}
