#include "Utils.h"

Vector::Vector()
{
	x = y = z = 0;
}

Vector::Vector(const Vector &v)
{
	x = NumBounds(v.x); y = NumBounds(v.y); z = NumBounds(v.z);
};

Vector::Vector(double _x, double _y, double _z)
{
	x = NumBounds(_x); y = NumBounds(_y); z = NumBounds(_z);
};

double Vector::GetX() const
{
	return NumBounds(x);
};

double Vector::GetY() const
{
	return NumBounds(y);
};

double Vector::GetZ() const
{
	return NumBounds(z);
};

void Vector::SetX(double s)
{
	x = NumBounds(s);
};

void Vector::SetY(double s)
{
	y = NumBounds(s);
};

void Vector::SetZ(double s)
{
	z = NumBounds(s);
};

void Vector::Set(double _x, double _y, double _z)
{
	x = NumBounds(_x); y = NumBounds(_y); z = NumBounds(_z);
};

double Vector::Length() const
{
	return NumBounds(sqrt((x * x) + (y * y) + (z * z)));
};

Vector Vector::Normalize()
{
	if (Length() != 0)
		return Vector(x / Length(), y / Length(), z / Length()); else
			return *this;
};

double Vector::operator[](unsigned int index) const
{
	if (index == 0)
		return NumBounds(x); else
	if (index == 1)
		return NumBounds(y); else
	if (index == 2)
		return NumBounds(z); else
			return 0;
};

bool Vector::operator==(const Vector &v)
{
	if (NumBounds(x) == NumBounds(v.x) && NumBounds(y) == NumBounds(v.y) && NumBounds(z) == NumBounds(v.z))
		return true; else
			return false;
};

void Vector::operator=(const Vector& v)
{
	x = NumBounds(v.x); y = NumBounds(v.y); z = NumBounds(v.z);
};

void Vector::operator+=(const Vector &v)
{
	x += NumBounds(v.x); y += NumBounds(v.y); z += NumBounds(v.z);
};

void Vector::operator+=(double s)
{
	x += NumBounds(s); y += NumBounds(s); z += NumBounds(s);
};

void Vector::operator-=(const Vector &v)
{
	x -= NumBounds(v.x); y -= NumBounds(v.y); z -= NumBounds(v.z);
};

void Vector::operator-=(double s)
{
	x += NumBounds(s); y += NumBounds(s); z += NumBounds(s);
};

void Vector::operator*=(double s)
{
	x *= NumBounds(s); y *= NumBounds(s); z *= NumBounds(s);
};

void Vector::operator/=(double s)
{
	x /= NumBounds(s); y /= NumBounds(s); z /= NumBounds(s);
};

Vector Vector::operator+(const Vector &v)
{
	return Vector(x + v.x, y + v.y, z + v.z);
};

Vector Vector::operator-(const Vector &v)
{
	return Vector(x - v.x, y - v.y, z - v.z);
};

double Vector::operator*(const Vector &v)
{
	return NumBounds((x * v.x) + (y * v.y) + (z * v.z));
};

Vector Vector::DotProduct(const Vector &v)
{
	double k1 = (y * v.z) - (z * v.y);
	double k2 = (z * v.x) - (x * v.z);
	double k3 = (x * v.y) - (y * v.x);

	return Vector(NumBounds(k1), NumBounds(k2), NumBounds(k3));
}

Vector Vector::operator*(double s)
{
	return Vector(x * s, y * s, z * s);
};

Vector Vector::operator/(double s)
{
	s = NumBounds(s);
	return Vector(x / s, y / s, z / s);
};

Vector Vector::Rotate(Vector &axis, double angle)
{
	Vector v = *this;
	
	return ((v - axis * (axis * v)) * cos(angle)) + (axis.DotProduct(v) * sin(angle)) + (axis * (axis * v));
}

// ============================================================================================================

template <typename T>
Parameter<T>::Parameter()
{
}

template <typename T>
Parameter<T>::Parameter(std::string new_name, T new_value)
{
	name = new_name;
	value = new_value;
}

template <typename T>
Parameter<T>::Parameter(std::string new_name, std::string new_description, T new_value)
{
	name = new_name;
	value = new_value;
	description = new_description;
}

template <typename T>
Parameter<T>::~Parameter()
{
}

template <typename T>
std::string Parameter<T>::GetName() const
{
	return name;
}

template <typename T>
void Parameter<T>::SetName(const std::string& new_name)
{
	name = new_name;
}

template <typename T>
std::string Parameter<T>::GetDescription() const
{
	return description;
}

template <typename T>
void Parameter<T>::SetDescription(const std::string& new_description)
{
	description = new_description;
}

template <typename T>
void Parameter<T>::SetValue(const T& new_value)
{
	value = new_value;
}

template <typename T>
T Parameter<T>::GetValue() const
{
	return value;
}

template <typename T>
void Parameter<T>::SetValueBounds(const T& new_max_value, const T& new_min_value)
{
	max_value = new_max_value;
	min_value = new_min_value;
}

template <typename T>
T Parameter<T>::GetMaxValueBound() const
{
	return max_value;
}

template <typename T>
T Parameter<T>::GetMinValueBound() const
{
	return min_value;
}

// ============================================================================================================

TextureManager::TextureManager()
{
}

TextureManager::TextureManager(const TextureManager &mgr)
{
	Textures = mgr.Textures;
}

TextureManager& TextureManager::operator=(const TextureManager &mgr)
{
	Textures = mgr.Textures;

	return *this;
}

TextureManager::~TextureManager()
{
}

TextureManager& TextureManager::Singleton()
{
	static TextureManager _instance;
	return _instance;
}

GLuint TextureManager::GetTexture(std::string fname)
{
	for (unsigned int i = 0; i < Textures.size(); i++)
		if (Textures[i].filename.compare(fname) == 0)
			return Textures[i].id;

	Texture tex;

	sf::Image Image;

	if (!Image.LoadFromFile(fname))
		return 0;

	tex.filename = fname;
	tex.id = Textures.size();

	glGenTextures(1, &(tex.id));
	glBindTexture(GL_TEXTURE_2D, tex.id);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	Textures.push_back(tex);

	return tex.id;
}

// ============================================================================================================

double Degree(double radians)
{
	return radians * 180 / PI;
}

double Radians(double degrees)
{
	return degrees * PI/ 180;
}

double NumBounds(double value)
{
	if (fabs(value) < (1 / 1000000.0f))
		return 0; else
			return value;
}

std::string IntToStr(int val)
{
	char *s = new char[255];

	sprintf(s, "%d", val);

	return std::string(s);
}

std::string FloatToStr(float val)
{
	char *s = new char[255];

	sprintf(s, "%f", val);

	return std::string(s);
}

sf::Vector3f ScreenToSpace(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float) x;
	winY = (float) viewport[3] - (float) y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return sf::Vector3f((float) posX, (float) posY, (float) posZ);
}

sf::Vector3f ScreenToSpace(sf::Vector2i pos)
{
	return ScreenToSpace(pos.x, pos.y);
}