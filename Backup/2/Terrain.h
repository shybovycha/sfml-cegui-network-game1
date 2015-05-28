#ifndef __TERRAIN__
#define __TERRAIN__

#include <SFML/Graphics.hpp>
#include <string.h>

class Terrain
{
	private:
		sf::Vector3f Center;
		sf::Image Image;

		GLuint Texture;
		GLfloat *pVerts, *pTexs;

		unsigned long vertCnt;
		float multiplier;

	public:
		Terrain();
		Terrain(std::string filename);

		~Terrain();

		void Load(std::string filename);

		sf::Vector3f GetCenter();
		void SetCenter(sf::Vector3f v);

		float GetHeight(float x, float z);
		void SetHeightMultiplier(float v = 1);

		void Render();
};

#endif