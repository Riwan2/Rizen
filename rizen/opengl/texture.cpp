#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../extern/stb_image.h"

/*
    Texture
*/

Texture::Texture() {}
Texture::~Texture()
{
	glDeleteTextures(1, &m_texture_id);
}

void Texture::bind(GLenum sampler)
{
	glActiveTexture(sampler);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

bool Texture::init_jpg(const std::string& filename)
{
    return init(TEXTURE_PATH + filename, GL_RGB);
}

bool Texture::init_png(const std::string& filename)
{
	return init(TEXTURE_PATH + filename, GL_RGBA);
}

bool Texture::init(const std::string& filename, GLint internal_format)
{
    int nrChannels;
	m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_nr_channels, 0);
 	
	if (!m_data)
 		return rizen_error("texture file opening error: [" + filename + "]");
         
    setup_texture(internal_format);
    stbi_image_free(m_data);
    return true;
}

void Texture::setup_texture(GLint internal_format)
{
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, internal_format, GL_UNSIGNED_BYTE, m_data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::init_framebuffer(const glm::vec2& size)
{
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);
}

void Texture::resize(const glm::vec2& size)
{
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// void Texture::set_lighting(float ambient, float reflectivity, float shineDamper)
// {
// 	m_ambient = ambient;
// 	m_reflectivity = reflectivity;
// 	m_shineDamper = shineDamper;
// }

/*
	FrameBuffer
*/


FrameBuffer::FrameBuffer() {}
FrameBuffer::~FrameBuffer()
{
	delete m_texture;
	glDeleteRenderbuffers(GL_RENDERBUFFER, &m_rbo);
	glDeleteFramebuffers(GL_FRAMEBUFFER, &m_fbo);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::init(const glm::vec2& size)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	m_texture = new Texture();
	m_texture->init_framebuffer(size);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return rizen_error("frame buffer creation error");

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void FrameBuffer::resize(const glm::vec2& size)
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	m_texture->resize(size);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);	
}