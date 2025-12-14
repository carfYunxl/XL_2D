#ifndef OPENGL_FRAME_BUFFER_H_
#define OPENGL_FRAME_BUFFER_H_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

    struct FramebufferSpecification
	{
		uint32_t Width, Height;
		// FramebufferFormat Format = 
		uint32_t Samples = 8;

		bool SwapChainTarget = false;
	};

    class GlFrameBuffer
	{
	public:
		GlFrameBuffer(const FramebufferSpecification& spec);
		virtual ~GlFrameBuffer();

		void Invalidate();

		virtual void Bind();
		virtual void Unbind();
		virtual void Resize(uint32_t width, uint32_t height);

		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};

_NAMESPACE_END

#endif //OPENGL_FRAME_BUFFER_H_