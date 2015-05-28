#ifndef __3DSMODEL__
#define __3DSMODEL__

#include <SFML/Graphics.hpp>

// The chunk's id numbers
#define MAIN3DS			0x4D4D
#define MAIN_VERS		0x0002
#define EDIT3DS			0x3D3D
#define MESH_VERS		0x3D3E
#define OBJECT			0x4000
#define TRIG_MESH		0x4100
#define VERT_LIST		0x4110
#define FACE_DESC		0x4120
#define FACE_MAT		0x4130
#define TEX_VERTS		0x4140
#define SMOOTH_GROUP	0x4150
#define LOCAL_COORDS	0x4160

#define MATERIAL		0xAFFF
#define MAT_NAME		0xA000
#define MAT_AMBIENT		0xA010
#define MAT_DIFFUSE		0xA020
#define MAT_SPECULAR	0xA030
#define SHINY_PERC		0xA040
#define SHINY_STR_PERC	0xA041
#define TRANS_PERC		0xA050
#define TRANS_FOFF_PERC	0xA052
#define REF_BLUR_PERC	0xA053
#define RENDER_TYPE		0xA100
#define SELF_ILLUM		0xA084
#define MAT_SELF_ILPCT	0xA08A
#define WIRE_THICKNESS	0xA087
#define MAT_TEXMAP		0xA200
#define MAT_MAPNAME		0xA300
#define ONE_UNIT		0x0100

#define KEYF3DS			0xB000
#define FRAMES			0xB008

#define MESH_INFO		0xB002

#define HIER_POS		0xB030
#define HIER_FATHER		0xB010
#define PIVOT_PT		0xB013

#define TRACK00			0xB020
#define TRACK01			0xB021
#define TRACK02			0xB022

#define	COLOR_RGB		0x0010
#define COLOR_TRU		0x0011
#define COLOR_TRUG		0x0012
#define COLOR_RGBG		0x0013
#define PERC_INT		0x0030
#define PERC_FLOAT		0x0031

#include "Utils.h"

#include <stdio.h>

class Model_3DS  
{
private:
	// A VERY simple vector struct
	// I could have included a complex class but I wanted the model class to stand alone
	struct vector_t {
		float x;
		float y;
		float z;
	};

	// vertex_t struct to make code easier to read in places
	struct vertex_t {
		float x;
		float y;
		float z;
	};

	// Color struct holds the diffuse color of the material
	struct color4i_t {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	// Holds the material info
	// TODO: add color support for non textured polys
	struct material_t {
		char name[80];	// The material's name
		GLuint tex;	// The texture (this is the only outside reference in this class)
		bool textured;	// whether or not it is textured
		color4i_t color;
	};

	// Every chunk in the 3ds file starts with this struct
	struct chunk_header_t {
		unsigned short id;	// The chunk's id
		unsigned long  len;	// The lenght of the chunk
	};

	// I sort the mesh by material so that I won't have to switch textures a great deal
	struct material_faces_t {
		unsigned short *subFaces;	// Index to our vertex array of all the faces that use this material
		int numSubFaces;			// The number of faces
		int MatIndex;				// An index to our materials
	};

	// The 3ds file can be made up of several objects
	struct object_t {
		char name[80];				// The object name
		float *vertex_tes;			// The array of vertices
		float *Normals;				// The array of the normals for the vertices
		float *TexCoords;			// The array of texture coordinates for the vertices
		unsigned short *Faces;		// The array of face indices
		int numFaces;				// The number of faces
		int numMatFaces;			// The number of differnet material faces
		int numVerts;				// The number of vertices
		int numTexCoords;			// The number of vertices
		bool textured;				// True: the object has textures
		material_faces_t *MatFaces;	// The faces are divided by materials
		vector_t pos;					// The position to move the object to
		vector_t rot;					// The angles to rotate the object
	};

	char *modelname;		// The name of the model
	char *path;				// The path of the model
	int numobjects;			// Total number of objects in the model
	int nummaterials;		// Total number of materials in the model
	int totalVerts;			// Total number of vertices in the model
	int totalFaces;			// Total number of faces in the model
	bool shownormals;		// True: show the normals
	material_t *materials;	// The array of materials
	object_t *objects;		// The array of objects in the model
	bool lit;				// True: the model is lit
	bool visible;			// True: the model gets rendered
	bool loaded;
	FILE *bin3ds;			// The binary 3ds file

	Vector pos;				// The position to move the model to
	Vector rot;				// The angles to rotate the model
	Vector size;
	float scale;			// The size you want the model scaled to

private:
	void IntColorChunkProcessor(long length, long findex, int matindex);
	void FloatColorChunkProcessor(long length, long findex, int matindex);
	// Processes the Main Chunk that all the other chunks exist is
	void MainChunkProcessor(long length, long findex);
		// Processes the model's info
		void EditChunkProcessor(long length, long findex);
			
			// Processes the model's materials
			void MaterialChunkProcessor(long length, long findex, int matindex);
				// Processes the names of the materials
				void material_tNameChunkProcessor(long length, long findex, int matindex);
				// Processes the material's diffuse color
				void DiffuseColorChunkProcessor(long length, long findex, int matindex);
				// Processes the material's texture maps
				void TextureMapChunkProcessor(long length, long findex, int matindex);
					// Processes the names of the textures and load the textures
					void MapNameChunkProcessor(long length, long findex, int matindex);
			
			// Processes the model's geometry
			void ObjectChunkProcessor(long length, long findex, int objindex);
				// Processes the triangles of the model
				void TriangularMeshChunkProcessor(long length, long findex, int objindex);
					// Processes the vertices of the model and loads them
					void VertexListChunkProcessor(long length, long findex, int objindex);
					// Processes the texture cordiantes of the vertices and loads them
					void TexCoordsChunkProcessor(long length, long findex, int objindex);
					// Processes the faces of the model and loads the faces
					void FacesDescriptionChunkProcessor(long length, long findex, int objindex);
						// Processes the materials of the faces and splits them up by material
						void FacesMaterialsListChunkProcessor(long length, long findex, int objindex, int subfacesindex);

	// Calculates the normals of the vertices by averaging
	// the normals of the faces that use that vertex
	void CalculateNormals();

public:
	Model_3DS();			// Constructor
	Model_3DS(char *name);			// Constructor

	void Load(char *name);	// Loads a model
	void Draw();			// Draws the model

	void SetPosition(Vector &v);
	void SetRotation(Vector &v);

	void SetPosition(double x, double y, double z);
	void SetRotation(double x, double y, double z);

	bool IsLoaded() const;

	Vector GetPosition() const;
	Vector GetRotation() const;
	Vector GetSize() const;

	virtual ~Model_3DS();	// Destructor
};

#endif