#include "MD2Model.h"

MD2Model::MD2Model()
{
	is_loaded = false;
	positionX = positionY = positionZ = 0;
	rotationX = rotationY = rotationZ = 0;
}

MD2Model::MD2Model(std::string filename)
{
	is_loaded = false;
	positionX = positionY = positionZ = 0;
	rotationX = rotationY = rotationZ = 0;

	Load(filename);
}

void MD2Model::Load(std::string filename)
{
	is_loaded = false;

	FILE *stream;

	unsigned char buffer[MD2_MAX_FRAMESIZE];
	float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f;
	float maxz = 0.0f, minz = 0.0f, w = 0.0f, h = 0.0f, d = 0.0f; //scale = 1.0f;

	if ((stream = fopen(filename.c_str(), "rb")) == NULL)
		return;

	fread(&header, sizeof(unsigned char), sizeof(MD2_Header_t), stream);

	if (header.magic != MD2_MAGIC || header.version != MD2_VERSION)
	{
		fclose(stream);

		return;
	}

	fseek(stream, header.offsetSkins, SEEK_SET);
	skins = (MD2_Skin_t*) malloc(sizeof(MD2_Skin_t)*header.numSkins);

	for(int i = 0; i < header.numSkins; i++)
		fread(&skins[i], sizeof(MD2_Skin_t), 1, stream);

	fseek(stream, header.offsetTexCoords, SEEK_SET);
	texcoords = (MD2_TextureCoordinate_t*) malloc(sizeof(MD2_TextureCoordinate_t) * header.numTexCoords);

	for(int i = 0; i < header.numTexCoords; i++)
		fread(&texcoords[i], sizeof(MD2_TextureCoordinate_t), 1, stream);

	fseek(stream, header.offsetTriangles, SEEK_SET);
	triangles = (MD2_Triangle_t*) malloc(sizeof(MD2_Triangle_t) * header.numTriangles);

	for (int i = 0; i < header.numTriangles; i++)
		fread(&triangles[i], sizeof(MD2_Triangle_t), 1, stream);

	fseek(stream, header.offsetFrames, SEEK_SET);
	frames = (MD2_Frame_t*) malloc(sizeof(MD2_Frame_t) * header.numFrames);

	for (int i = 0; i < header.numFrames; i++)
	{
		MD2_Alias_Frame_t *frame = (MD2_Alias_Frame_t *) buffer;

		frames[i].vertices = (MD2_TriangleVertex_t*) malloc(sizeof(MD2_TriangleVertex_t) * header.numVertices);

		fread(frame, 1, header.frameSize, stream);

		strcpy(frames[i].name, frame->name);

		for (int j = 0; j < header.numVertices; j++)
		{
			frames[i].vertices[j].vertex[0] = (float) frame->alias_vertices[j].vertex[0] * frame->scale[0] + frame->translate[0];
			frames[i].vertices[j].vertex[1] = (float) frame->alias_vertices[j].vertex[1] * frame->scale[1] + frame->translate[1];
			frames[i].vertices[j].vertex[2] = (float) frame->alias_vertices[j].vertex[2] * frame->scale[2] + frame->translate[2];
		}
	}

	fseek(stream, header.offsetGlCommands, SEEK_SET);
	glcommandbuffer = (int*) malloc(sizeof(int) * header.numGlCommands);
	fread(glcommandbuffer, sizeof(int), header.numGlCommands, stream);

	fclose(stream);

	for (int i = 0; i < header.numFrames; i++)
	{
		for (int j = 0; j < header.numTriangles; j++)
		{
			/*float facetnormal[3];
			float v1[3], v2[3];

			v1[0] = frames[i].vertices[triangles[j].vertexIndices[1]].vertex[0] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[0];
			v1[1] = frames[i].vertices[triangles[j].vertexIndices[1]].vertex[1] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[1];
			v1[2] = frames[i].vertices[triangles[j].vertexIndices[1]].vertex[2] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[2];
			v2[0] = frames[i].vertices[triangles[j].vertexIndices[2]].vertex[0] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[0];
			v2[1] = frames[i].vertices[triangles[j].vertexIndices[2]].vertex[1] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[1];
			v2[2] = frames[i].vertices[triangles[j].vertexIndices[2]].vertex[2] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[2];

			facetnormal[0]=(v1[1] * v2[2] - v1[2] * v2[1]);
			facetnormal[1]=(v1[2] * v2[0] - v1[0] * v2[2]);
			facetnormal[2]=(v1[0] * v2[1] - v1[1] * v2[0]);*/

			Vector v1(
					frames[i].vertices[triangles[j].vertexIndices[1]].vertex[0] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[0],
					frames[i].vertices[triangles[j].vertexIndices[1]].vertex[1] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[1],
					frames[i].vertices[triangles[j].vertexIndices[1]].vertex[2] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[2]);

			Vector v2(
					frames[i].vertices[triangles[j].vertexIndices[2]].vertex[0] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[0],
					frames[i].vertices[triangles[j].vertexIndices[2]].vertex[1] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[1],
					frames[i].vertices[triangles[j].vertexIndices[2]].vertex[2] - frames[i].vertices[triangles[j].vertexIndices[0]].vertex[2]);

			Vector v3 = v1.DotProduct(v2);

			for (int k = 0; k < 3; k++)
			{
				/*frames[i].vertices[triangles[j].vertexIndices[k]].normal[0] -= facetnormal[0];
				frames[i].vertices[triangles[j].vertexIndices[k]].normal[1] -= facetnormal[1];
				frames[i].vertices[triangles[j].vertexIndices[k]].normal[2] -= facetnormal[2];*/

				frames[i].vertices[triangles[j].vertexIndices[k]].normal[0] -= (float) v3[0];
				frames[i].vertices[triangles[j].vertexIndices[k]].normal[1] -= (float) v3[1];
				frames[i].vertices[triangles[j].vertexIndices[k]].normal[2] -= (float) v3[2];
			}
		}

		for (int j = 0; j < header.numVertices; j++)
		{
			Vector n(frames[i].vertices[j].normal[0], frames[i].vertices[j].normal[1], frames[i].vertices[j].normal[2]);
			
			n = n.Normalize();

			for(int k = 0; k < 3; k++)
				frames[i].vertices[j].normal[k] = (float) n[k];

			/*float *n = frames[i].vertices[j].normal;
			float l = (float) sqrt((n[0] * n[0]) + (n[1] * n[1]) + (n[2] * n[2]));

			if (l != 0.0f)
			{
				n[0] /= l;
				n[1] /= l;
				n[2] /= l;
			}*/
		}
	}

	for (int i = 0; i < header.numFrames; i++)
	{
		for(int j = 0; j < header.numVertices; j++)
		{
			if (maxx < frames[i].vertices[j].vertex[0])
				maxx = frames[i].vertices[j].vertex[0];

			if (minx > frames[i].vertices[j].vertex[0])
				minx = frames[i].vertices[j].vertex[0];

			if (maxy < frames[i].vertices[j].vertex[1])
				maxy = frames[i].vertices[j].vertex[1];

			if (miny > frames[i].vertices[j].vertex[1])
				miny = frames[i].vertices[j].vertex[1];

			if (maxz < frames[i].vertices[j].vertex[2])
				maxz = frames[i].vertices[j].vertex[2];

			if (minz > frames[i].vertices[j].vertex[2])
				minz = frames[i].vertices[j].vertex[2];

			float c = frames[i].vertices[j].vertex[2];
			frames[i].vertices[j].vertex[2] = frames[i].vertices[j].vertex[1];
			frames[i].vertices[j].vertex[1] = c;
		}
	}

	w = fabs(maxx) + fabs(minx);
	h = fabs(maxy) + fabs(miny);
	d = fabs(maxz) + fabs(minz);

	scale = 1.0f; ///Max(Max(w, h), d);

	sizeX = w;
	sizeY = h;
	sizeZ = d;

	currentFrame = 0;
	is_loaded = true;
}

void MD2Model::DrawFrame(unsigned int frame)
{
	if (!is_loaded)
		return;

	int i = 0, val = glcommandbuffer[i++];

	glPushMatrix();

		glTranslated(positionX, positionY, positionZ);
		glRotated(rotationX, 1, 0, 0);
		glRotated(rotationY, 0, 1, 0);
		glRotated(rotationZ, 0, 0, 1);
		glScaled(scale, scale, scale);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture);

			//glColor4f(1.f, 1.f, 1.f, 1.f);

			while (val != 0)
			{
				int count;

				if (val > 0)
				{
					glBegin(GL_TRIANGLE_STRIP);

					count = val;
				} else
				{
					glBegin(GL_TRIANGLE_FAN);

					count = -val;
				}

				while (count--)
				{
					float s = *(float *) &glcommandbuffer[i++];
					float t = *(float *) &glcommandbuffer[i++];
					int index = glcommandbuffer[i++];

					if (frame < 0 || frame > header.numFrames)
						continue;

					glTexCoord2f(s, t);
					glNormal3f(frames[frame].vertices[index].normal[0], frames[frame].vertices[index].normal[1], frames[frame].vertices[index].normal[2]);
					glVertex3f(frames[frame].vertices[index].vertex[0], frames[frame].vertices[index].vertex[1], frames[frame].vertices[index].vertex[2]);
				}

				glEnd();

				val = glcommandbuffer[i++];
			}

		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void MD2Model::DrawFrameWithOutline(unsigned int frame, sf::Color LineColor)
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glEnable( GL_LIGHTING );

		glClearStencil(0);
		glClear( GL_STENCIL_BUFFER_BIT );
		glEnable( GL_STENCIL_TEST );
		// Set the stencil buffer to write a 1 in every time

		// a pixel is written to the screen

		glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		// Render the object in black

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			
			DrawFrame(frame);
			
	glDisable( GL_LIGHTING );
		// Set the stencil buffer to only allow writing
		// to the screen when the value of the
		// stencil buffer is not 1

		glStencilFunc( GL_NOTEQUAL, 1, 0xFFFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		// Draw the object with thick lines

		glLineWidth( 5.0f );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glColor4f( LineColor.r, LineColor.g, LineColor.b, LineColor.a );
		
			DrawFrame(frame);

		// Pop the state changes off the attribute stack
	glEnable( GL_LIGHTING );
	glPopAttrib();
}

void MD2Model::SetTexture(std::string filename)
{
	Texture = TextureManager::Singleton().GetTexture(filename); //LoadTexture(filename);
}

void MD2Model::SetTexture(GLuint tex)
{
	Texture = tex;
}

bool MD2Model::IsLoaded()
{
	return is_loaded;
}

void MD2Model::SetPosition(double x, double y, double z)
{
	positionX = x; positionY = y; positionZ = z;
}

void MD2Model::SetRotation(double x, double y, double z)
{
	rotationX = x; rotationY = y; rotationZ = z;
}

void MD2Model::SetPosition(Vector v)
{
	positionX = v.GetX(); positionY = v.GetY(); positionZ = v.GetZ();
}

void MD2Model::SetRotation(Vector v)
{
	rotationX = v.GetX(); rotationY = v.GetY(); rotationZ = v.GetZ();
}

void MD2Model::Move(double x, double y, double z)
{
	positionX += x; positionY += y; positionZ += z;
}

void MD2Model::Rotate(double x, double y, double z)
{
	rotationX += x; rotationY += y; rotationZ += z;
}

void MD2Model::Move(Vector v)
{
	positionX += v.GetX(); positionY += v.GetY(); positionZ += v.GetZ();
}

void MD2Model::Rotate(Vector v)
{
	rotationX += v.GetX(); rotationY += v.GetY(); rotationZ += v.GetZ();
}

Vector MD2Model::GetPosition()
{
	return Vector(positionX, positionY, positionZ);
}

Vector MD2Model::GetRotation()
{
	return Vector(rotationX, rotationY, rotationZ);
}

Vector MD2Model::GetSize()
{
	return Vector(sizeX, sizeY, sizeZ) * scale;
}

void MD2Model::SetScale(double multiplier)
{
	scale = multiplier;
}

void MD2Model::SetAnimation(int type, bool looped, bool switchToDef)
{
	if (!is_loaded)
		return;

	if ((type < 0) || (type > MD2ANIM_MAX_ANIMATIONS))
        type = 0;

    animationState.startframe = animlist[type].first_frame;
    animationState.endframe = animlist[type].last_frame;
    animationState.next_frame = animlist[type].first_frame + 1;
    animationState.fps = animlist[type].fps;
    animationState.type = type;
	animationState.looped = looped;
	animationState.switchToDefault = switchToDef;
}

void MD2Model::Draw(float time)
{
	if (!is_loaded)
		return;

    // animate. calculate current frame and next frame
    if (time > 0.0)
	{
		animationState.curr_time = time;

		// calculate current and next frames
		if (animationState.curr_time - animationState.old_time > (1.0 / animationState.fps))
		{
			animationState.curr_frame = animationState.next_frame;
			animationState.next_frame++;

			if ((animationState.next_frame > animationState.endframe) && (animationState.looped))
				animationState.next_frame = animationState.startframe; else
			if ((animationState.next_frame > animationState.endframe) && (!animationState.looped))
				animationState.next_frame = animationState.endframe;

			animationState.old_time = animationState.curr_time;
		}

		// prevent having a current/next frame greater
		// than the total number of frames... if animation is looped...

		if (animationState.looped)
		{
			if (animationState.curr_frame > (header.numFrames - 1))
				animationState.curr_frame = 0;

			if (animationState.next_frame > (header.numFrames - 1))
				animationState.next_frame = 0;
		} else
		{
			if (animationState.curr_frame >= animationState.endframe && animationState.switchToDefault)
				SetAnimation(MD2ANIM_STAND, true);
		}

		animationState.interpol = animationState.fps * (animationState.curr_time - animationState.old_time);
	}

	DrawFrame(animationState.curr_frame);
}

int MD2Model::GetAnimation()
{
	return animationState.type;
}

bool MD2Model::GetAnimationLooped()
{
	return animationState.looped;
}