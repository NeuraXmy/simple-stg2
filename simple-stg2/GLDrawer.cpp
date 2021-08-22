#include "GLDrawer.h"
#include <SFML/OpenGL.hpp>

namespace GLDrawer
{
	void Quad3d::set_vert(int index, const sf::Vector3f& vert)
	{
		this->vert[index] = vert;
	}

	void Quad3d::set_coords(int index, const sf::Vector2f& coords)
	{
		this->coords[index] = coords;
	}

	void Quad3d::set_standard_coords()
	{
		coords[0] = { 0.0, 0.0 };
		coords[1] = { 1.0, 0.0 };
		coords[2] = { 1.0, 1.0 };
		coords[3] = { 0.0, 1.0 };
	}

	void Quad3d::scale_coords(float w, float h)
	{
		for (int i = 0; i < 4; i++)
			coords[i].x *= w, coords[i].y *= h;
	}

	void Quad3d::translate_coords(float x, float y)
	{
		for (int i = 0; i < 4; i++)
			coords[i].x += x, coords[i].y += y;
	}

	void Quad3d::set_color(int index, const Color& color)
	{
		this->color[index] = color;
	}

	void Quad3d::set_all_color(const Color& color)
	{
		for (int i = 0; i < 4; i++)
			this->color[i] = color;
	}

	void Quad3d::set_all_alpha(double alpha)
	{
		for (int i = 0; i < 4; i++)
			this->color[i].a = alpha;
	}


	void before_draw()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void bind_texture(Texture::Texture* tex, bool repeat, bool smooth)
	{
		sf::Texture::bind(tex->sf_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
	}

	void draw_quad(const Quad3d& q)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++)
		{
			glTexCoord2f(q.coords[i].x, q.coords[i].y);
			glColor4f(q.color[i].r, q.color[i].g, q.color[i].b, q.color[i].a);
			glVertex3f(q.vert[i].x, q.vert[i].y, q.vert[i].z);
		}
		glEnd();
	}
		
}


void GLDrawer::Camera::push()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
}

void GLDrawer::Camera::pop()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GLDrawer::Camera::set_identity()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void GLDrawer::set_viewport(float left, float top, float w, float h)
{
	glMatrixMode(GL_PROJECTION);
	glViewport(left, top, w, h);
}

void GLDrawer::Camera::set_frustum(float l, float r, float t, float b, float z_near, float z_far)
{
	glMatrixMode(GL_PROJECTION);
	glFrustum(l, r, t, b, z_near, z_far);
}

void GLDrawer::Camera::translate(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(-x, -y, -z);
}

void GLDrawer::Camera::rotate(float angle, const sf::Vector3f& axis)
{
	glMatrixMode(GL_PROJECTION);
	glRotatef(-angle, axis.x, axis.y, axis.z);
}

void GLDrawer::Camera::scale(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glScalef(x, y, z);
}


void GLDrawer::Model::push()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void GLDrawer::Model::pop()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLDrawer::Model::set_identity()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::Model::translate(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(x, y, z);
}

void GLDrawer::Model::rotate(float angle, const sf::Vector3f& axis)
{
	glMatrixMode(GL_MODELVIEW);
	glRotatef(angle, axis.x, axis.y, axis.z);
}

void GLDrawer::Model::scale(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glScalef(x, y, z);
}
