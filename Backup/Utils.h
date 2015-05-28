#ifndef __UTILS__
#define __UTILS__

#ifndef PI
	#define PI 3.14159265358979323846
#endif

#ifndef sign
	#define sign(X) X < 0 ? -1 : 1
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <SFML/Graphics.hpp>

class Vector
{
	private:
		double x, y, z;

	public:
		Vector();
		Vector(const Vector &v);
		Vector(double _x, double _y, double _z);

		double GetX() const;
		double GetY() const;
		double GetZ() const;

		void SetX(double s);
		void SetY(double s);
		void SetZ(double s);

		void Set(double _x, double _y, double _z);

		double Length() const;
		Vector Normalize();

		double operator[](unsigned int index) const;

		bool operator==(const Vector &v);

		void operator=(const Vector &v);

		void operator+=(const Vector &v);
		void operator+=(double s);

		void operator-=(const Vector &v);
		void operator-=(double s);

		void operator*=(double s);

		void operator/=(double s);

		Vector operator+(const Vector &v);

		Vector operator-(const Vector &v);

		double operator*(const Vector &v);
		Vector operator*(double s);

		Vector operator/(double s);

		Vector DotProduct(const Vector &v);
		Vector Rotate(Vector &axis, double Angle);
};

template<typename T>
class Parameter
{
	private:
		std::string name, description;
		T value, max_value, min_value;
	
	public:
		Parameter();
		Parameter(std::string new_name, T new_value);
		Parameter(std::string new_name, std::string new_description, T new_value);
		
		~Parameter();
		
		std::string GetName() const;
		void SetName(const std::string& new_name);
		
		std::string GetDescription() const;
		void SetDescription(const std::string& new_description);
		
		void SetValue(const T& new_value);
		T GetValue() const;

		void SetValueBounds(const T& new_max_value, const T& new_min_value);
		T GetMaxValueBound() const;
		T GetMinValueBound() const;
};

class TextureManager
{
	private:
		struct Texture
		{
			GLuint id;
			std::string filename;
		};

		std::vector<Texture> Textures;

	public:
		TextureManager();
		TextureManager(const TextureManager &mgr);

		TextureManager & operator=(const TextureManager &mgr);

		~TextureManager();

		static TextureManager & Singleton();
		GLuint GetTexture(std::string fname);
};

double Degree(double radians);
double Radians(double degrees);
double NumBounds(double value);

std::string IntToStr(int val);
std::string FloatToStr(float val);

sf::Vector3f ScreenToSpace(sf::Vector2i pos);
sf::Vector3f ScreenToSpace(int x, int y);

#endif