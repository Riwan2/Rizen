#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "../utils/log.h"

/*
	Texture
*/

const std::string TEXTURE_PATH = "../textures/";

class Texture {
public:
	Texture();
	~Texture();

	bool init_png(const std::string& filename);
    bool init_jpg(const std::string& filename);
	void init_framebuffer(const glm::vec2& size);

	void bind(GLenum sampler = GL_TEXTURE0);
	void resize(const glm::vec2& size);

	int width() { return m_width; }
	int height() { return m_height; }
	int nr_channels() { return m_nr_channels;}

	GLuint texture_id() { return m_texture_id; }

private:
	GLuint m_texture_id;
	unsigned char* m_data;
	
	int m_width;
	int m_height;
	int m_nr_channels;

	bool init(const std::string& filename, GLint internal_format);
	void setup_texture(GLint internal_format);
};

/*
	FrameBuffer
*/

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	bool init(const glm::vec2& size);
	void bind();
	void unbind();
	void resize(const glm::vec2& size);

	Texture* texture() const { return m_texture; }

private:
	GLuint m_rbo;
	GLuint m_fbo;
	Texture* m_texture;
};

#endif //TEXTURE_H