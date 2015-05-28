#include "MAXModel.h"

// You need to uncomment this if you are using MFC
//#pragma warn( You need to uncomment this if you are using MFC )

#include "MAXModel.h"

#include <math.h>			// Header file for the math library


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Model_3DS::Model_3DS()
{
	// Initialization

	// Don't show the normals by default
	shownormals = false;

	loaded = false;

	// The model is lit by default
	lit = true;

	// The model is visible by default
	visible = true;

	// Set up the default position
	pos.Set(0, 0, 0);
	// Set up the default rotation
	rot.Set(0, 0, 0);

	// Set up the path
	path = new char[80];
	sprintf(path, "");

	// Zero out our counters for MFC
	numobjects = 0;
	nummaterials = 0;

	// Set the scale to one
	scale = 1.0f;
}

Model_3DS::Model_3DS(char *name)
{
	// Initialization

	// Don't show the normals by default
	shownormals = false;

	loaded = false;

	// The model is lit by default
	lit = true;

	// The model is visible by default
	visible = true;

	// Set up the default position
	pos.Set(0, 0, 0);
	// Set up the default rotation
	rot.Set(0, 0, 0);

	// Set up the path
	path = new char[80];
	sprintf(path, "");

	// Zero out our counters for MFC
	numobjects = 0;
	nummaterials = 0;

	// Set the scale to one
	scale = 1.0f;

	Load(name);
}

Model_3DS::~Model_3DS()
{
	for (int i = 0; i < nummaterials; i++)
		glDeleteTextures(1, &materials[i].tex);
}

void Model_3DS::Load(char *name)
{
	// holds the main chunk header
	chunk_header_t main;

	// strip "'s
	if (strstr(name, "\""))
		name = strtok(name, "\"");

	// Find the path
	if (strstr(name, "/") || strstr(name, "\\"))
	{
		// Holds the name of the model minus the path
		char *temp;

		// Find the name without the path
		if (strstr(name, "/"))
			temp = strrchr(name, '/');
		else
			temp = strrchr(name, '\\');

		// Allocate space for the path
		path = new char[strlen(name)-strlen(temp)+1];

		// Get a pointer to the end of the path and name
		char *src = name + strlen(name) - 1;

		// Back up until a \ or the start
		while (src != path && !((*(src-1)) == '\\' || (*(src-1)) == '/'))
			src--;

		// Copy the path into path
		memcpy (path, name, src-name);
		path[src-name] = 0;
	}

	// Load the file
	bin3ds = fopen(name,"rb");

	// Make sure we are at the beginning
	fseek(bin3ds, 0, SEEK_SET);

	// Load the Main Chunk's header
	fread(&main.id,sizeof(main.id),1,bin3ds);
    fread(&main.len,sizeof(main.len),1,bin3ds);

	// Start Processing
	MainChunkProcessor(main.len, ftell(bin3ds));

	// Don't need the file anymore so close it
	fclose(bin3ds);

	// Calculate the vertex normals
	CalculateNormals();

	// For future reference
	modelname = name;

	// Find the total number of faces and vertices
	totalFaces = 0;
	totalVerts = 0;

	Vector minA, maxA;

	for (int i = 0; i < numobjects; i++)
	{
		totalFaces += objects[i].numFaces/3;
		totalVerts += objects[i].numVerts;

		for (int t = 0; t < objects[i].numVerts; t += 3)
		{
			if (objects[i].vertex_tes[t] < minA.GetX())
				minA.SetX(objects[i].vertex_tes[t]);

			if (objects[i].vertex_tes[t + 1] < minA.GetY())
				minA.SetY(objects[i].vertex_tes[t + 1]);

			if (objects[i].vertex_tes[t + 2] < minA.GetZ())
				minA.SetZ(objects[i].vertex_tes[t + 2]);

			if (objects[i].vertex_tes[t] > maxA.GetX())
				maxA.SetX(objects[i].vertex_tes[t]);

			if (objects[i].vertex_tes[t + 1] > maxA.GetY())
				maxA.SetY(objects[i].vertex_tes[t + 1]);

			if (objects[i].vertex_tes[t + 2] > maxA.GetZ())
				maxA.SetZ(objects[i].vertex_tes[t + 2]);
		}
	}

	size = (maxA - minA) * scale * 100;

	// If the object doesn't have any texcoords generate some
	for (int k = 0; k < numobjects; k++)
	{
		if (objects[k].numTexCoords == 0)
		{
			// Set the number of texture coords
			objects[k].numTexCoords = objects[k].numVerts;

			// Allocate an array to hold the texture coordinates
			objects[k].TexCoords = new GLfloat[objects[k].numTexCoords * 2];

			// Make some texture coords
			for (int m = 0; m < objects[k].numTexCoords; m++)
			{
				objects[k].TexCoords[2*m] = objects[k].vertex_tes[3*m];
				objects[k].TexCoords[2*m+1] = objects[k].vertex_tes[3*m+1];
			}
		}
	}

	// Let's build simple colored textures for the materials w/o a texture
	/*for (int j = 0; j < nummaterials; j++)
	{
		if (materials[j].textured == false)
		{
			unsigned char r = materials[j].color.r;
			unsigned char g = materials[j].color.g;
			unsigned char b = materials[j].color.b;
			materials[j].tex.BuildColorTexture(r, g, b);
			materials[j].textured = true;
		}
	}*/

	loaded = true;
}

void Model_3DS::Draw()
{
	if (visible && loaded)
	{
		glPushMatrix();

			// Move the model
			glTranslatef((GLfloat) pos.GetX(), (GLfloat) pos.GetY(), (GLfloat) pos.GetZ());

			// Rotate the model
			glRotatef((GLfloat) rot.GetX(), 1.0f, 0.0f, 0.0f);
			glRotatef((GLfloat) rot.GetY(), 0.0f, 1.0f, 0.0f);
			glRotatef((GLfloat) rot.GetZ(), 0.0f, 0.0f, 1.0f);

			glScalef(scale, scale, scale);

			// Loop through the objects
			for (int i = 0; i < numobjects; i++)
			{
				// Enable texture coordiantes, normals, and vertices arrays
				if (objects[i].textured)
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				if (lit)
					glEnableClientState(GL_NORMAL_ARRAY);

				glEnableClientState(GL_VERTEX_ARRAY);

				// Point them to the objects arrays
				if (objects[i].textured)
					glTexCoordPointer(2, GL_FLOAT, 0, objects[i].TexCoords);

				if (lit)
					glNormalPointer(GL_FLOAT, 0, objects[i].Normals);

				glVertexPointer(3, GL_FLOAT, 0, objects[i].vertex_tes);

				// Loop through the faces as sorted by material and draw them
				for (int j = 0; j < objects[i].numMatFaces; j ++)
				{
					// Use the material's texture
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, materials[objects[i].MatFaces[j].MatIndex].tex);

					glPushMatrix();

						// Move the model
						glTranslatef(objects[i].pos.x, objects[i].pos.y, objects[i].pos.z);

						// Rotate the model
						//glRotatef(objects[i].rot.x, 1.0f, 0.0f, 0.0f);
						//glRotatef(objects[i].rot.y, 0.0f, 1.0f, 0.0f);
						//glRotatef(objects[i].rot.z, 0.0f, 0.0f, 1.0f);

						glRotatef(objects[i].rot.z, 0.0f, 0.0f, 1.0f);
						glRotatef(objects[i].rot.y, 0.0f, 1.0f, 0.0f);
						glRotatef(objects[i].rot.x, 1.0f, 0.0f, 0.0f);

						// Draw the faces using an index to the vertex array
						glDrawElements(GL_TRIANGLES, objects[i].MatFaces[j].numSubFaces, GL_UNSIGNED_SHORT, objects[i].MatFaces[j].subFaces);

					glPopMatrix();
					glDisable(GL_TEXTURE_2D);
				}

				// Show the normals?
				if (shownormals)
				{
					// Loop through the vertices and normals and draw the normal
					for (int k = 0; k < objects[i].numVerts * 3; k += 3)
					{
						// Disable texturing
						glDisable(GL_TEXTURE_2D);
						// Disbale lighting if the model is lit
						if (lit)
							glDisable(GL_LIGHTING);
						// Draw the normals blue
						glColor3f(0.0f, 0.0f, 1.0f);

						// Draw a line between the vertex and the end of the normal
						glBegin(GL_LINES);
							glVertex3f(objects[i].vertex_tes[k], objects[i].vertex_tes[k+1], objects[i].vertex_tes[k+2]);
							glVertex3f(objects[i].vertex_tes[k]+objects[i].Normals[k], objects[i].vertex_tes[k+1]+objects[i].Normals[k+1], objects[i].vertex_tes[k+2]+objects[i].Normals[k+2]);
						glEnd();

						// Reset the color to white
						glColor3f(1.0f, 1.0f, 1.0f);
						// If the model is lit then renable lighting
						if (lit)
							glEnable(GL_LIGHTING);
					}
				}

				if (objects[i].textured)
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				if (lit)
					glDisableClientState(GL_NORMAL_ARRAY);

				glDisableClientState(GL_VERTEX_ARRAY);
			}

		glPopMatrix();
	}
}

void Model_3DS::CalculateNormals()
{
	// Let's build some normals
	for (int i = 0; i < numobjects; i++)
	{
		for (int g = 0; g < objects[i].numVerts; g++)
		{
			// Reduce each vert's normal to unit
			float length;
			vector_t unit;

			unit.x = objects[i].Normals[g*3];
			unit.y = objects[i].Normals[g*3+1];
			unit.z = objects[i].Normals[g*3+2];

			length = (float)sqrt((unit.x*unit.x) + (unit.y*unit.y) + (unit.z*unit.z));

			if (length == 0.0f)
				length = 1.0f;

			unit.x /= length;
			unit.y /= length;
			unit.z /= length;

			objects[i].Normals[g*3]   = unit.x;
			objects[i].Normals[g*3+1] = unit.y;
			objects[i].Normals[g*3+2] = unit.z;
		}
	}
}

void Model_3DS::MainChunkProcessor(long length, long findex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			// This is the mesh information like vertices, faces, and materials
			case EDIT3DS	:
				EditChunkProcessor(h.len, ftell(bin3ds));
				break;
			// I left this in case anyone gets very ambitious
			case KEYF3DS	:
				//KeyFrameChunkProcessor(h.len, ftell(bin3ds));
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::EditChunkProcessor(long length, long findex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// First count the number of objects and materials
	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case OBJECT	:
				numobjects++;
				break;
			case MATERIAL	:
				nummaterials++;
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// Now load the materials
	if (nummaterials > 0)
	{
		materials = new material_t[nummaterials];

		// material_t is set to untextured until we find otherwise
		for (int d = 0; d < nummaterials; d++)
			materials[d].textured = false;

		fseek(bin3ds, findex, SEEK_SET);

		int i = 0;

		while (ftell(bin3ds) < (findex + length - 6))
		{
			fread(&h.id,sizeof(h.id),1,bin3ds);
			fread(&h.len,sizeof(h.len),1,bin3ds);

			switch (h.id)
			{
				case MATERIAL	:
					MaterialChunkProcessor(h.len, ftell(bin3ds), i);
					i++;
					break;
				default			:
					break;
			}

			fseek(bin3ds, (h.len - 6), SEEK_CUR);
		}
	}

	// Load the objects (individual meshes in the whole model)
	if (numobjects > 0)
	{
		objects = new object_t[numobjects];

		// Set the textured variable to false until we find a texture
		for (int k = 0; k < numobjects; k++)
			objects[k].textured = false;

		// Zero the objects position and rotation
		for (int m = 0; m < numobjects; m++)
		{
			objects[m].pos.x = 0.0f;
			objects[m].pos.y = 0.0f;
			objects[m].pos.z = 0.0f;

			objects[m].rot.x = 0.0f;
			objects[m].rot.y = 0.0f;
			objects[m].rot.z = 0.0f;
		}

		// Zero out the number of texture coords
		for (int n = 0; n < numobjects; n++)
			objects[n].numTexCoords = 0;

		fseek(bin3ds, findex, SEEK_SET);

		int j = 0;

		while (ftell(bin3ds) < (findex + length - 6))
		{
			fread(&h.id,sizeof(h.id),1,bin3ds);
			fread(&h.len,sizeof(h.len),1,bin3ds);

			switch (h.id)
			{
				case OBJECT	:
					ObjectChunkProcessor(h.len, ftell(bin3ds), j);
					j++;
					break;
				default			:
					break;
			}

			fseek(bin3ds, (h.len - 6), SEEK_CUR);
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::MaterialChunkProcessor(long length, long findex, int matindex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case MAT_NAME	:
				// Loads the material's names
				material_tNameChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			case MAT_AMBIENT	:
				//ColorChunkProcessor(h.len, ftell(bin3ds));
				break;
			case MAT_DIFFUSE	:
				DiffuseColorChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			case MAT_SPECULAR	:
				//ColorChunkProcessor(h.len, ftell(bin3ds));
			case MAT_TEXMAP	:
				// Finds the names of the textures of the material and loads them
				TextureMapChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::material_tNameChunkProcessor(long length, long findex, int matindex)
{
	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the material's name
	for (int i = 0; i < 80; i++)
	{
		materials[matindex].name[i] = fgetc(bin3ds);
		if (materials[matindex].name[i] == 0)
		{
			materials[matindex].name[i] = NULL;
			break;
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::DiffuseColorChunkProcessor(long length, long findex, int matindex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		// Determine the format of the color and load it
		switch (h.id)
		{
			case COLOR_RGB	:
				// A rgb float color chunk
				FloatColorChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			case COLOR_TRU	:
				// A rgb int color chunk
				IntColorChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			case COLOR_RGBG	:
				// A rgb gamma corrected float color chunk
				FloatColorChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			case COLOR_TRUG	:
				// A rgb gamma corrected int color chunk
				IntColorChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::FloatColorChunkProcessor(long length, long findex, int matindex)
{
	float r;
	float g;
	float b;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	fread(&r,sizeof(r),1,bin3ds);
	fread(&g,sizeof(g),1,bin3ds);
	fread(&b,sizeof(b),1,bin3ds);

	materials[matindex].color.r = (unsigned char)(r*255.0f);
	materials[matindex].color.g = (unsigned char)(r*255.0f);
	materials[matindex].color.b = (unsigned char)(r*255.0f);
	materials[matindex].color.a = 255;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::IntColorChunkProcessor(long length, long findex, int matindex)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	fread(&r,sizeof(r),1,bin3ds);
	fread(&g,sizeof(g),1,bin3ds);
	fread(&b,sizeof(b),1,bin3ds);

	materials[matindex].color.r = r;
	materials[matindex].color.g = g;
	materials[matindex].color.b = b;
	materials[matindex].color.a = 255;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::TextureMapChunkProcessor(long length, long findex, int matindex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case MAT_MAPNAME:
				// Read the name of texture in the Diffuse Color map
				MapNameChunkProcessor(h.len, ftell(bin3ds), matindex);
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::MapNameChunkProcessor(long length, long findex, int matindex)
{
	char name[80];

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the name of the texture
	for (int i = 0; i < 80; i++)
	{
		name[i] = fgetc(bin3ds);
		if (name[i] == 0)
		{
			name[i] = NULL;
			break;
		}
	}

	// Load the name and indicate that the material has a texture
	char fullname[80];
	sprintf(fullname, "%s%s", path, name);
	materials[matindex].tex = TextureManager::Singleton().GetTexture(fullname); //LoadTexture(fullname);
	materials[matindex].textured = true;

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::ObjectChunkProcessor(long length, long findex, int objindex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Load the object's name
	for (int i = 0; i < 80; i++)
	{
		objects[objindex].name[i] = fgetc(bin3ds);
		if (objects[objindex].name[i] == 0)
		{
			objects[objindex].name[i] = NULL;
			break;
		}
	}

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case TRIG_MESH	:
				// Process the triangles of the object
				TriangularMeshChunkProcessor(h.len, ftell(bin3ds), objindex);
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::TriangularMeshChunkProcessor(long length, long findex, int objindex)
{
	chunk_header_t h;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case VERT_LIST	:
				// Load the vertices of the onject
				VertexListChunkProcessor(h.len, ftell(bin3ds), objindex);
				break;
			case LOCAL_COORDS	:
				//LocalCoordinatesChunkProcessor(h.len, ftell(bin3ds));
				break;
			case TEX_VERTS	:
				// Load the texture coordinates for the vertices
				TexCoordsChunkProcessor(h.len, ftell(bin3ds), objindex);
				objects[objindex].textured = true;
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// After we have loaded the vertices we can load the faces
	fseek(bin3ds, findex, SEEK_SET);

	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case FACE_DESC	:
				// Load the faces of the object
				FacesDescriptionChunkProcessor(h.len, ftell(bin3ds), objindex);
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::VertexListChunkProcessor(long length, long findex, int objindex)
{
	unsigned short numVerts;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the number of vertices of the object
	fread(&numVerts,sizeof(numVerts),1,bin3ds);

	// Allocate arrays for the vertices and normals
	objects[objindex].vertex_tes = new GLfloat[numVerts * 3];
	objects[objindex].Normals = new GLfloat[numVerts * 3];

	// Assign the number of vertices for future use
	objects[objindex].numVerts = numVerts;

	// Zero out the normals array
	for (int j = 0; j < numVerts * 3; j++)
		objects[objindex].Normals[j] = 0.0f;

	// Read the vertices, switching the y and z coordinates and changing the sign of the z coordinate
	for (int i = 0; i < numVerts * 3; i+=3)
	{
		fread(&objects[objindex].vertex_tes[i],sizeof(GLfloat),1,bin3ds);
		fread(&objects[objindex].vertex_tes[i+2],sizeof(GLfloat),1,bin3ds);
		fread(&objects[objindex].vertex_tes[i+1],sizeof(GLfloat),1,bin3ds);

		// Change the sign of the z coordinate
		objects[objindex].vertex_tes[i+2] = -objects[objindex].vertex_tes[i+2];
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::TexCoordsChunkProcessor(long length, long findex, int objindex)
{
	// The number of texture coordinates
	unsigned short numCoords;

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the number of coordinates
	fread(&numCoords,sizeof(numCoords),1,bin3ds);

	// Allocate an array to hold the texture coordinates
	objects[objindex].TexCoords = new GLfloat[numCoords * 2];

	// Set the number of texture coords
	objects[objindex].numTexCoords = numCoords;

	// Read teh texture coordiantes into the array
	for (int i = 0; i < numCoords * 2; i+=2)
	{
		fread(&objects[objindex].TexCoords[i],sizeof(GLfloat),1,bin3ds);
		fread(&objects[objindex].TexCoords[i+1],sizeof(GLfloat),1,bin3ds);
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::FacesDescriptionChunkProcessor(long length, long findex, int objindex)
{
	chunk_header_t h;
	unsigned short numFaces;	// The number of faces in the object
	unsigned short vertA;		// The first vertex of the face
	unsigned short vertB;		// The second vertex of the face
	unsigned short vertC;		// The third vertex of the face
	unsigned short flags;		// The winding order flags
	long subs;					// Holds our place in the file
	int numMatFaces = 0;		// The number of different materials

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the number of faces
	fread(&numFaces,sizeof(numFaces),1,bin3ds);

	// Allocate an array to hold the faces
	objects[objindex].Faces = new GLushort[numFaces * 3];
	// Store the number of faces
	objects[objindex].numFaces = numFaces * 3;

	// Read the faces into the array
	for (int i = 0; i < numFaces * 3; i+=3)
	{
		// Read the vertices of the face
		fread(&vertA,sizeof(vertA),1,bin3ds);
		fread(&vertB,sizeof(vertB),1,bin3ds);
		fread(&vertC,sizeof(vertC),1,bin3ds);
		fread(&flags,sizeof(flags),1,bin3ds);

		// Place them in the array
		objects[objindex].Faces[i]   = vertA;
		objects[objindex].Faces[i+1] = vertB;
		objects[objindex].Faces[i+2] = vertC;

		// Calculate the face's normal
		vector_t n;
		vertex_t v1;
		vertex_t v2;
		vertex_t v3;

		v1.x = objects[objindex].vertex_tes[vertA*3];
		v1.y = objects[objindex].vertex_tes[vertA*3+1];
		v1.z = objects[objindex].vertex_tes[vertA*3+2];
		v2.x = objects[objindex].vertex_tes[vertB*3];
		v2.y = objects[objindex].vertex_tes[vertB*3+1];
		v2.z = objects[objindex].vertex_tes[vertB*3+2];
		v3.x = objects[objindex].vertex_tes[vertC*3];
		v3.y = objects[objindex].vertex_tes[vertC*3+1];
		v3.z = objects[objindex].vertex_tes[vertC*3+2];

		// calculate the normal
		float u[3], v[3];

		// V2 - V3;
		u[0] = v2.x - v3.x;
		u[1] = v2.y - v3.y;
		u[2] = v2.z - v3.z;

		// V2 - V1;
		v[0] = v2.x - v1.x;
		v[1] = v2.y - v1.y;
		v[2] = v2.z - v1.z;

		n.x = (u[1]*v[2] - u[2]*v[1]);
		n.y = (u[2]*v[0] - u[0]*v[2]);
		n.z = (u[0]*v[1] - u[1]*v[0]);

		// Add this normal to its verts' normals
		objects[objindex].Normals[vertA*3]   += n.x;
		objects[objindex].Normals[vertA*3+1] += n.y;
		objects[objindex].Normals[vertA*3+2] += n.z;
		objects[objindex].Normals[vertB*3]   += n.x;
		objects[objindex].Normals[vertB*3+1] += n.y;
		objects[objindex].Normals[vertB*3+2] += n.z;
		objects[objindex].Normals[vertC*3]   += n.x;
		objects[objindex].Normals[vertC*3+1] += n.y;
		objects[objindex].Normals[vertC*3+2] += n.z;
	}

	// Store our current file position
	subs = ftell(bin3ds);

	// Check to see how many materials the faces are split into
	while (ftell(bin3ds) < (findex + length - 6))
	{
		fread(&h.id,sizeof(h.id),1,bin3ds);
		fread(&h.len,sizeof(h.len),1,bin3ds);

		switch (h.id)
		{
			case FACE_MAT	:
				//FacesMaterialsListChunkProcessor(h.len, ftell(bin3ds), objindex);
				numMatFaces++;
				break;
			default			:
				break;
		}

		fseek(bin3ds, (h.len - 6), SEEK_CUR);
	}

	// Split the faces up according to their materials
	if (numMatFaces > 0)
	{
		// Allocate an array to hold the lists of faces divided by material
		objects[objindex].MatFaces = new material_faces_t[numMatFaces];
		// Store the number of material faces
		objects[objindex].numMatFaces = numMatFaces;

		fseek(bin3ds, subs, SEEK_SET);

		int j = 0;

		// Split the faces up
		while (ftell(bin3ds) < (findex + length - 6))
		{
			fread(&h.id,sizeof(h.id),1,bin3ds);
			fread(&h.len,sizeof(h.len),1,bin3ds);

			switch (h.id)
			{
				case FACE_MAT	:
					// Process the faces and split them up
					FacesMaterialsListChunkProcessor(h.len, ftell(bin3ds), objindex, j);
					j++;
					break;
				default			:
					break;
			}

			fseek(bin3ds, (h.len - 6), SEEK_CUR);
		}
	}

	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::FacesMaterialsListChunkProcessor(long length, long findex, int objindex, int subfacesindex)
{
	char name[80];				// The material's name
	unsigned short numEntries;	// The number of faces associated with this material
	unsigned short Face;		// Holds the faces as they are read
	int material;				// An index to the materials array for this material

	// move the file pointer to the beginning of the main
	// chunk's data findex + the size of the header
	fseek(bin3ds, findex, SEEK_SET);

	// Read the material's name
	for (int i = 0; i < 80; i++)
	{
		name[i] = fgetc(bin3ds);
		if (name[i] == 0)
		{
			name[i] = NULL;
			break;
		}
	}

	// Faind the material's index in the materials array
	for (material = 0; material < nummaterials; material++)
	{
		if (strcmp(name, materials[material].name) == 0)
			break;
	}

	// Store this value for later so that we can find the material
	objects[objindex].MatFaces[subfacesindex].MatIndex = material;

	// Read the number of faces associated with this material
	fread(&numEntries,sizeof(numEntries),1,bin3ds);

	// Allocate an array to hold the list of faces associated with this material
	objects[objindex].MatFaces[subfacesindex].subFaces = new GLushort[numEntries * 3];
	// Store this number for later use
	objects[objindex].MatFaces[subfacesindex].numSubFaces = numEntries * 3;

	// Read the faces into the array
	for (int i = 0; i < numEntries * 3; i+=3)
	{
		// read the face
		fread(&Face,sizeof(Face),1,bin3ds);
		// Add the face's vertices to the list
		objects[objindex].MatFaces[subfacesindex].subFaces[i] = objects[objindex].Faces[Face * 3];
		objects[objindex].MatFaces[subfacesindex].subFaces[i+1] = objects[objindex].Faces[Face * 3 + 1];
		objects[objindex].MatFaces[subfacesindex].subFaces[i+2] = objects[objindex].Faces[Face * 3 + 2];
	}
	
	// move the file pointer back to where we got it so
	// that the ProcessChunk() which we interrupted will read
	// from the right place
	fseek(bin3ds, findex, SEEK_SET);
}

void Model_3DS::SetPosition(Vector &v)
{
	pos = v;
}

void Model_3DS::SetRotation(Vector &v)
{
	rot = v;
}

void Model_3DS::SetPosition(double x, double y, double z)
{
	pos = Vector(x, y, z);
}

void Model_3DS::SetRotation(double x, double y, double z)
{
	rot = Vector(x, y, z);
}

bool Model_3DS::IsLoaded() const
{
	return loaded;
}

Vector Model_3DS::GetPosition() const
{
	return pos;
}

Vector Model_3DS::GetRotation() const
{
	return rot;
}

Vector Model_3DS::GetSize() const
{
	return size;
}