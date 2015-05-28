#include "Terrain.h"

#include <iostream>

Terrain::Terrain()
{
	Center = sf::Vector3f(0, 0, 0);
	multiplier = 1;
}

Terrain::Terrain(std::string filename)
{
	Load(filename);
}

void Terrain::Load(std::string filename)
{
	Center = sf::Vector3f(0, 0, 0);
	multiplier = 1;

	if (!Image.LoadFromFile(filename))
        return;

    sf::Sprite Background(Image);

	Texture = 0;
	glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	pVerts = new GLfloat[(unsigned int) (Background.GetSize().x * Background.GetSize().y * 3 * 6)];
	pTexs = new GLfloat[(unsigned int) (Background.GetSize().x * Background.GetSize().y * 2 * 6)];

	unsigned long a = 0, b = 0, mode = 0;

	for (int t = 0; t < Background.GetSize().y - 2; t++)
	{
		if (!mode)
		{
			for (int i = 0; i < Background.GetSize().x; i++)
			{
				float h;
				sf::Color c;
				
				c = Background.GetPixel(i, t);
				//h = (255 * -255 * 255) / (c.r * 255 + c.g * 255 + c.b * 255 + 1) / 64;
				h = ((0.299f * c.r) + (0.587f * c.g) + (0.114f * c.b)) * multiplier;
				
				pVerts[a++] = (GLfloat) (Center.x / 2) + i; pVerts[a++] = (GLfloat) (Center.y / 2) + h; pVerts[a++] = (GLfloat) (Center.z / 2) + t;
				pTexs[b++] = (GLfloat) (i / Background.GetSize().x); pTexs[b++] = (GLfloat) (t / Background.GetSize().y);

				c = Background.GetPixel(i, t + 1);
				//h = (255 * -255 * 255) / (c.r * 255 + c.g * 255 + c.b * 255 + 1) / 64;
				h = ((0.299f * c.r) + (0.587f * c.g) + (0.114f * c.b)) * multiplier;
				
				pVerts[a++] = (GLfloat) (Center.x / 2) + i; pVerts[a++] = (GLfloat) (Center.y / 2) + h; pVerts[a++] = (GLfloat) (Center.z / 2) + (t + 1);
				pTexs[b++] = (GLfloat) (i / Background.GetSize().x); pTexs[b++] = (GLfloat) ((t + 1) / Background.GetSize().y);
			}

			mode = 1;
		} else
		{
			for (int i = (unsigned int) (Background.GetSize().x - 1); i >= 0; i--)
			{
				float h;
				sf::Color c;
				
				c = Background.GetPixel(i, t);
				//h = (255 * -255 * 255) / (c.r * 255 + c.g * 255 + c.b * 255 + 1) / 64;
				h = ((0.299f * c.r) + (0.587f * c.g) + (0.114f * c.b)) * multiplier;
				
				pVerts[a++] = (GLfloat) (Center.x / 2) + i; pVerts[a++] = (GLfloat) (Center.y / 2) + h; pVerts[a++] = (GLfloat) (Center.z / 2) + t;
				pTexs[b++] = (GLfloat) (i / Background.GetSize().x); pTexs[b++] = (GLfloat) (t / Background.GetSize().y);

				c = Background.GetPixel(i, t + 1);
				//h = (255 * -255 * 255) / (c.r * 255 + c.g * 255 + c.b * 255 + 1) / 64;
				h = ((0.299f * c.r) + (0.587f * c.g) + (0.114f * c.b)) * multiplier;
				
				pVerts[a++] = (GLfloat) (Center.x / 2) + i; pVerts[a++] = (GLfloat) (Center.y / 2) + h; pVerts[a++] = (GLfloat) (Center.z / 2) + (t + 1);
				pTexs[b++] = (GLfloat) (i / Background.GetSize().x); pTexs[b++] = (GLfloat) ((t + 1) / Background.GetSize().y);
			}

			mode = 0;
		}
	}

	for (int i = 0; i < 6; i++)
		pVerts[a++] = pVerts[a - 3];

	vertCnt = a;
}

Terrain::~Terrain()
{
	glDeleteTextures(1, &Texture);
}

sf::Vector3f Terrain::GetCenter()
{
	return Center;
}

void Terrain::SetCenter(sf::Vector3f v)
{
	Center = v;
}

float Terrain::GetHeight(float x, float z)
{
	if (x < 0 || x >= Image.GetWidth() || z < 0 || z >= Image.GetHeight())
	{
		return 0;
	} else
	{
		sf::Color c = Image.GetPixel((unsigned int) (Center.x / 2) + (unsigned int) x, (unsigned int) (Center.y / 2) + (unsigned int) z);
		
		return ((0.299f * c.r) + (0.587f * c.g) + (0.114f * c.b)) * multiplier;
	}
}

void Terrain::SetHeightMultiplier(float v)
{
	multiplier = v;

	if (pVerts)
	{
		for (unsigned long i = 1; i < vertCnt; i += 3)
			pVerts[i] *= multiplier;
	}
}

void Terrain::Render()
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glColor4f(1.f, 1.f, 1.f, 1.f);

	glPushMatrix();

		glTranslatef(Center.x / 2, Center.y / 2, Center.z / 2);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, pVerts);
		glTexCoordPointer(2, GL_FLOAT, 0, pTexs);
	
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertCnt);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}