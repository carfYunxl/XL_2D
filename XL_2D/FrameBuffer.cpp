#include "FrameBuffer.hpp"
#include <algorithm>

_NAMESPACE_BEGIN

GlFrameBuffer::GlFrameBuffer(const FramebufferSpecification& spec)
	: m_Specification(spec)
	, m_RendererID(0)
	, m_ColorAttachment(0)
	, m_DepthAttachment(0)
{
	Invalidate();
}

GlFrameBuffer::~GlFrameBuffer()
{
	if (m_RendererID)      glDeleteFramebuffers(1, &m_RendererID);
	if (m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment);
	if (m_DepthAttachment) glDeleteTextures(1, &m_DepthAttachment);
}

void GlFrameBuffer::Invalidate()
{
	// 清理旧资源
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		if (m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment);
		if (m_DepthAttachment) glDeleteTextures(1, &m_DepthAttachment);

		m_RendererID = 0;
		m_ColorAttachment = 0;
		m_DepthAttachment = 0;
	}

	// 创建新的 FBO
	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	const uint32_t width = m_Specification.Width;
	const uint32_t height = m_Specification.Height;
	const uint32_t samples = std::max<uint32_t>(1, m_Specification.Samples);

	if (samples > 1)
	{
		// 多重采样：使用 GL_TEXTURE_2D_MULTISAMPLE 附件（颜色 + 深度）
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(samples), GL_RGBA8, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(samples), GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment, 0);
	}
	else
	{
		// 单样本路径：原有实现
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
	}

	// 指定输出目标
	GLenum bufs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, bufs);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		//TRACE("Framebuffer incomplete: 0x%X\n", status);
	}

	// 恢复默认绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, static_cast<GLsizei>(m_Specification.Width), static_cast<GLsizei>(m_Specification.Height));
}

void GlFrameBuffer::Unbind()
{
	// 将 FBO 的颜色缓冲 blit 到默认帧缓冲（如果是多重采样 glBlit 会执行 resolve）
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glBlitFramebuffer(
		0, 0, static_cast<GLint>(m_Specification.Width), static_cast<GLint>(m_Specification.Height),
		0, 0, static_cast<GLint>(m_Specification.Width), static_cast<GLint>(m_Specification.Height),
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlFrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
		return;

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

_NAMESPACE_END