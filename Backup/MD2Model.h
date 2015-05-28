#ifndef __MD2MODEL__
#define __MD2MODEL__

#include <SFML/Graphics.hpp>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Utils.h"

// animation
typedef struct
{
    int     first_frame;            // first frame of the animation
    int     last_frame;             // number of frames
    int     fps;                    // number of frames per second

} MD2_Animation_t;

// animation list
typedef enum {
    MD2ANIM_STAND,
    MD2ANIM_RUN,
    MD2ANIM_ATTACK,
    MD2ANIM_PAIN_A,
    MD2ANIM_PAIN_B,
    MD2ANIM_PAIN_C,
    MD2ANIM_JUMP,
    MD2ANIM_FLIP,
    MD2ANIM_SALUTE,
    MD2ANIM_FALLBACK,
    MD2ANIM_WAVE,
    MD2ANIM_POINT,
    MD2ANIM_CROUCH_STAND,
    MD2ANIM_CROUCH_WALK,
    MD2ANIM_CROUCH_ATTACK,
    MD2ANIM_CROUCH_PAIN,
    MD2ANIM_CROUCH_DEATH, 
    MD2ANIM_DEATH_FALLBACK,
    MD2ANIM_DEATH_FALLFORWARD,
    MD2ANIM_DEATH_FALLBACKSLOW,
    MD2ANIM_BOOM,

    MD2ANIM_MAX_ANIMATIONS

} MD2_AnimationType;

// ----------------------------------------------
// initialize the 21 MD2 model animations.
// ----------------------------------------------
const MD2_Animation_t animlist[ 21 ] = 
{
    // first, last, fps

    {   0,  39,  9 },   // STAND
    {  40,  45, 10 },   // RUN
    {  46,  53, 10 },   // ATTACK
    {  54,  57,  7 },   // PAIN_A
    {  58,  61,  7 },   // PAIN_B
    {  62,  65,  7 },   // PAIN_C
    {  66,  71,  7 },   // JUMP
    {  72,  83,  7 },   // FLIP
    {  84,  94,  7 },   // SALUTE
    {  95, 111, 10 },   // FALLBACK
    { 112, 122,  7 },   // WAVE
    { 123, 134,  6 },   // POINT
    { 135, 153, 10 },   // CROUCH_STAND
    { 154, 159,  7 },   // CROUCH_WALK
    { 160, 168, 10 },   // CROUCH_ATTACK
    { 196, 172,  7 },   // CROUCH_PAIN
    { 173, 177,  5 },   // CROUCH_DEATH
    { 178, 183,  7 },   // DEATH_FALLBACK
    { 184, 189,  7 },   // DEATH_FALLFORWARD
    { 190, 197,  7 },   // DEATH_FALLBACKSLOW
    { 198, 198,  5 },   // BOOM
};

// animation state
typedef struct
{
    int     startframe;              // first frame
    int     endframe;                // last frame
    int     fps;                     // frame per second for this animation

    float   curr_time;               // current time
    float   old_time;                // old time
    float   interpol;                // percent of interpolation

    int     type;                    // animation type

    int     curr_frame;              // current frame
    int     next_frame;              // next frame

	bool    looped;                  // looped animation or not? 
	bool	switchToDefault;         // if true, animation will switch to STAND when its finished 

} MD2_AnimationState_t;

class MD2Model
{
	private:
		static const unsigned int MD2_MAGIC = ('I'+('D'<<8)+('P'<<16)+('2'<<24));
		static const unsigned int MD2_VERSION = 8;
		static const unsigned int MD2_HEADER_SIZE = 68;
		static const unsigned int MD2_MAX_TRIANGLES = 4096;
		static const unsigned int MD2_MAX_VERTICES = 2048;
		static const unsigned int MD2_MAX_TEXCOORDS = 2048;
		static const unsigned int MD2_MAX_FRAMES = 512;
		static const unsigned int MD2_MAX_SKINS = 32;
		static const unsigned int MD2_MAX_FRAMESIZE = 8320;
		static const unsigned int NUMVERTEXNORMALS = 162;

		typedef struct
		{
			int magic;
			int version;
			int skinWidth;
			int skinHeight;
			int frameSize;
			int numSkins;
			int numVertices;
			int numTexCoords;
			int numTriangles;
			int numGlCommands;
			int numFrames;
			int offsetSkins;
			int offsetTexCoords;
			int offsetTriangles;
			int offsetFrames;
			int offsetGlCommands;
			int offsetEnd;
		} MD2_Header_t;

		typedef struct
		{
			unsigned char vertex[3];
			unsigned char lightNormalIndex;
		} MD2_Alias_TriangleVertex_t;

		typedef struct
		{
			float vertex[3];
			float normal[3];
		} MD2_TriangleVertex_t;

		typedef struct
		{
			short vertexIndices[3];
			short textureIndices[3];
		} MD2_Triangle_t;

		typedef struct
		{
			short s;
			short t;
		} MD2_TextureCoordinate_t;

		typedef struct
		{
			float scale[3];
			float translate[3];
			char name[16];
			MD2_Alias_TriangleVertex_t alias_vertices[1];
		} MD2_Alias_Frame_t;

		typedef struct
		{
			char name[16];
			MD2_TriangleVertex_t *vertices;
		} MD2_Frame_t;

		typedef char MD2_Skin_t[64];

		typedef struct
		{
			float s;
			float t;
			int vertexIndex;
		} MD2_GLCommandVertex_t;

		MD2_Header_t header;
		MD2_Skin_t *skins;
		MD2_TextureCoordinate_t *texcoords;
		MD2_Triangle_t *triangles;
		MD2_Frame_t *frames;
		MD2_AnimationState_t animationState;

		GLuint Texture;

		float sizeX, sizeY, sizeZ;
		double scale;
		double positionX, positionY, positionZ;
		double rotationX, rotationY, rotationZ;

		unsigned int currentFrame;

		int *glcommandbuffer;
		bool is_loaded;

	public:
		MD2Model();
		MD2Model(std::string filename);

		void Load(std::string filename);
		void DrawFrame(unsigned int frame);
		void DrawFrameWithOutline(unsigned int frame, sf::Color LineColor);

		bool IsLoaded();

		void SetTexture(std::string filename);
		void SetTexture(GLuint tex);

		void SetPosition(double x, double y, double z);
		void SetRotation(double x, double y, double z);

		void SetPosition(Vector v);
		void SetRotation(Vector v);

		void Move(double x, double y, double z);
		void Rotate(double x, double y, double z);

		void Move(Vector v);
		void Rotate(Vector v);

		Vector GetPosition();
		Vector GetRotation();

		Vector GetSize();
		void SetScale(double multiplier);

		void SetAnimation(int type, bool looped = true, bool switchToDef = true);

		int GetAnimation();
		bool GetAnimationLooped();

		void Draw(float time);
};

#endif