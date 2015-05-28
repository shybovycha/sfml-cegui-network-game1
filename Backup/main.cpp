#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <string>
#include <vector>

#define DEBUG_MODE

#ifdef DEBUG_MODE
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-window-s-d.lib")
#else
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-window-s.lib")
#endif

#pragma comment(lib, "glu32.lib")

#include "Utils.h"
#include "Player.h"

std::vector<Player> Players;

/*float *skyboxVertices, **skyboxTextCoords;
GLuint *skyboxTextures;*/

int HalfMapSpaceSize = 0, HalfMapCellSize = 0;
float *map, *grid; 
int gridVertices;

void PreapareGrid()
{
	map = new float[4 * 3];
	grid = new float[HalfMapCellSize * 2 * HalfMapCellSize * 2 * 8 * 3];

	/*skyboxVertices = new float[4 * 6 * 3];
	skyboxTextCoords = (float**) new float[6];
	skyboxTextures = new GLuint[6];

	for (int i = 0; i < 6; i++)
		skyboxTextCoords[i] = new float[4 * 2];*/

	int cnt = 0;

	for (int i = 0; i < HalfMapCellSize * 2; i++)
	{
		for (int t = 0; t < HalfMapCellSize * 2; t++)
		{
			// GRID
			grid[cnt++] = (i + 0) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 0) * CellWidth * 1.f;

			grid[cnt++] = (i + 1) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 0) * CellWidth * 1.f;



			grid[cnt++] = (i + 1) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 0) * CellWidth * 1.f;

			grid[cnt++] = (i + 1) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 1) * CellWidth * 1.f;



			grid[cnt++] = (i + 1) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 1) * CellWidth * 1.f;

			grid[cnt++] = (i + 0) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 1) * CellWidth * 1.f;



			grid[cnt++] = (i + 0) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 1) * CellWidth * 1.f;

			grid[cnt++] = (i + 0) * CellWidth * 1.f; 
			grid[cnt++] = 0.5f; 
			grid[cnt++] = (t + 0) * CellWidth * 1.f;
		}
	}

	gridVertices = cnt / 3;

	// Map vertices
	map[0] = 0.f; 
	map[1] = 0.f; 
	map[2] = 0.f;

	map[3] = CellWidth * HalfMapCellSize * 2.f;
	map[4] = 0.f; 
	map[5] = 0.f;

	map[6] = CellWidth * HalfMapCellSize * 2.f; 
	map[7] = 0.f; 
	map[8] = CellWidth * HalfMapCellSize * 2.f;

	map[9] = 0.f; 
	map[10] = 0.f; 
	map[11] = CellWidth * HalfMapCellSize * 2.f;

	/*// Skybox Textures
	skyboxTextures[0] = TextureManager::Singleton().GetTexture("Bottom.bmp");
	skyboxTextures[1] = TextureManager::Singleton().GetTexture("Top.bmp");
	skyboxTextures[2] = TextureManager::Singleton().GetTexture("Back.bmp");
	skyboxTextures[3] = TextureManager::Singleton().GetTexture("Front.bmp");
	skyboxTextures[4] = TextureManager::Singleton().GetTexture("Left.bmp");
	skyboxTextures[5] = TextureManager::Singleton().GetTexture("Right.bmp");

	// Skybox Vetices

	// Bottom
	skyboxVertices[0] = 0;
	skyboxVertices[1] = 0;
	skyboxVertices[2] = 0;

	skyboxVertices[3] = 1;
	skyboxVertices[4] = 0;
	skyboxVertices[5] = 0;

	skyboxVertices[6] = 1;
	skyboxVertices[7] = 0;
	skyboxVertices[8] = 1;

	skyboxVertices[9] = 0;
	skyboxVertices[10] = 0;
	skyboxVertices[11] = 1;

	skyboxTextCoords[0][0] = 0;
	skyboxTextCoords[0][1] = 0;

	skyboxTextCoords[0][2] = 1;
	skyboxTextCoords[0][3] = 0;

	skyboxTextCoords[0][4] = 1;
	skyboxTextCoords[0][5] = 1;

	skyboxTextCoords[0][6] = 0;
	skyboxTextCoords[0][7] = 1;

	// Top
	skyboxVertices[12] = 0;
	skyboxVertices[13] = 1;
	skyboxVertices[14] = 0;

	skyboxVertices[15] = 1;
	skyboxVertices[16] = 1;
	skyboxVertices[17] = 0;

	skyboxVertices[18] = 1;
	skyboxVertices[19] = 1;
	skyboxVertices[20] = 1;

	skyboxVertices[21] = 0;
	skyboxVertices[22] = 1;
	skyboxVertices[23] = 1;

	skyboxTextCoords[1][0] = 0;
	skyboxTextCoords[1][1] = 0;

	skyboxTextCoords[1][2] = 1;
	skyboxTextCoords[1][3] = 0;

	skyboxTextCoords[1][4] = 1;
	skyboxTextCoords[1][5] = 1;

	skyboxTextCoords[1][6] = 0;
	skyboxTextCoords[1][7] = 1;

	// Back
	skyboxVertices[24] = 0;
	skyboxVertices[25] = 0;
	skyboxVertices[26] = 0;

	skyboxVertices[27] = 1;
	skyboxVertices[28] = 0;
	skyboxVertices[29] = 0;

	skyboxVertices[30] = 1;
	skyboxVertices[31] = 1;
	skyboxVertices[32] = 0;

	skyboxVertices[33] = 0;
	skyboxVertices[34] = 1;
	skyboxVertices[35] = 0;

	skyboxTextCoords[2][0] = 0;
	skyboxTextCoords[2][1] = 0;

	skyboxTextCoords[2][2] = 1;
	skyboxTextCoords[2][3] = 0;

	skyboxTextCoords[2][4] = 1;
	skyboxTextCoords[2][5] = 1;

	skyboxTextCoords[2][6] = 0;
	skyboxTextCoords[2][7] = 1;

	// Front
	skyboxVertices[36] = 0;
	skyboxVertices[37] = 0;
	skyboxVertices[38] = 1;

	skyboxVertices[39] = 1;
	skyboxVertices[40] = 0;
	skyboxVertices[41] = 1;

	skyboxVertices[42] = 1;
	skyboxVertices[43] = 1;
	skyboxVertices[44] = 1;

	skyboxVertices[45] = 0;
	skyboxVertices[46] = 1;
	skyboxVertices[47] = 1;

	skyboxTextCoords[3][0] = 0;
	skyboxTextCoords[3][1] = 0;

	skyboxTextCoords[3][2] = 1;
	skyboxTextCoords[3][3] = 0;

	skyboxTextCoords[3][4] = 1;
	skyboxTextCoords[3][5] = 1;

	skyboxTextCoords[3][6] = 0;
	skyboxTextCoords[3][7] = 1;

	// Left
	skyboxVertices[48] = 0;
	skyboxVertices[49] = 0;
	skyboxVertices[50] = 0;

	skyboxVertices[51] = 0;
	skyboxVertices[52] = 0;
	skyboxVertices[53] = 1;

	skyboxVertices[54] = 0;
	skyboxVertices[55] = 1;
	skyboxVertices[56] = 1;

	skyboxVertices[57] = 0;
	skyboxVertices[58] = 1;
	skyboxVertices[59] = 0;

	skyboxTextCoords[4][0] = 0;
	skyboxTextCoords[4][1] = 0;

	skyboxTextCoords[4][2] = 1;
	skyboxTextCoords[4][3] = 0;

	skyboxTextCoords[4][4] = 1;
	skyboxTextCoords[4][5] = 1;

	skyboxTextCoords[4][6] = 0;
	skyboxTextCoords[4][7] = 1;

	// Right
	skyboxVertices[60] = 1;
	skyboxVertices[61] = 0;
	skyboxVertices[62] = 0;

	skyboxVertices[63] = 1;
	skyboxVertices[64] = 0;
	skyboxVertices[65] = 1;

	skyboxVertices[66] = 1;
	skyboxVertices[67] = 1;
	skyboxVertices[68] = 1;

	skyboxVertices[69] = 1;
	skyboxVertices[70] = 1;
	skyboxVertices[71] = 0;

	skyboxTextCoords[5][0] = 0;
	skyboxTextCoords[5][1] = 0;

	skyboxTextCoords[5][2] = 1;
	skyboxTextCoords[5][3] = 0;

	skyboxTextCoords[5][4] = 1;
	skyboxTextCoords[5][5] = 1;

	skyboxTextCoords[5][6] = 0;
	skyboxTextCoords[5][7] = 1;*/
}

void DrawGrid()
{
	glPushMatrix();
		int k = -1 * HalfMapSpaceSize;
		glTranslated(k, 0, k);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, map);

		glColor3f(0.0f, 0.25f, 0.5f);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, grid);

		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINES, 0, gridVertices);

		glDisableClientState(GL_VERTEX_ARRAY);

		glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	/*glPushMatrix();
		glTranslated(k * 1.5f, -1, k * 1.5f);
		glScaled(k * k, k * k, k * k);

		for (int i = 0; i < 6; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, skyboxTextures[i]);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glVertexPointer(3, GL_FLOAT, 0, skyboxVertices);
				glTexCoordPointer(3, GL_FLOAT, 0, skyboxTextCoords[i]);

				glDrawArrays(GL_QUADS, i * 4, 4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}
	glPopMatrix();*/
}

void SpaceToCell(float x, float z, int &x_res, int &z_res, float &x_normal, float &z_normal)
{
	x_res = int(x / CellWidth), z_res = int(z / CellWidth);

	x_res += sign(x); z_res += sign(z);

	x_normal = float(x_res * CellWidth), z_normal = float(z_res * CellWidth);

	x_res += HalfMapCellSize; z_res += HalfMapCellSize; 
	x_res -= (x < 0) ? 0 : 1; z_res -= (z < 0) ? 0 : 1;

	if (x_res < 0)
		x_res = 0; else
	if (x_res > HalfMapCellSize * 2)
		x_res = HalfMapCellSize * 2;

	if (z_res < 0)
		z_res = 0; else
	if (z_res > HalfMapCellSize * 2)
		z_res = HalfMapCellSize * 2;

	if (x > 0)
		x_normal -= (CellWidth / 2); else
	if (x < 0)
		x_normal += (CellWidth / 2);

	if (z > 0)
		z_normal -= (CellWidth / 2); else
	if (z < 0)
		z_normal += (CellWidth / 2);
}

void SpaceToCell(float x, float z, int &x_res, int &z_res)
{
	x_res = int(x / CellWidth), z_res = int(z / CellWidth);

	x_res += sign(x); z_res += sign(z);

	x_res += HalfMapCellSize; z_res += HalfMapCellSize; 
	x_res -= (x < 0) ? 0 : 1; z_res -= (z < 0) ? 0 : 1;

	if (x_res < 0)
		x_res = 0; else
	if (x_res > HalfMapCellSize * 2)
		x_res = HalfMapCellSize * 2;

	if (z_res < 0)
		z_res = 0; else
	if (z_res > HalfMapCellSize * 2)
		z_res = HalfMapCellSize * 2;
}

void CellToSpace(int x, int z, float &x_normal, float &z_normal)
{
	x_normal = float(x * CellWidth) - HalfMapSpaceSize + (CellWidth / 2);
	z_normal = float(z * CellWidth) - HalfMapSpaceSize + (CellWidth / 2);

	if (x_normal < -HalfMapSpaceSize)
		x_normal = (float) -HalfMapSpaceSize + (CellWidth / 2); else
	if (x_normal > HalfMapSpaceSize)
		x_normal = (float) HalfMapSpaceSize - (CellWidth / 2);

	if (z_normal < -HalfMapSpaceSize)
		z_normal = (float) -HalfMapSpaceSize + (CellWidth / 2); else
	if (z_normal > HalfMapSpaceSize)
		z_normal = (float) HalfMapSpaceSize - (CellWidth / 2);
}

void HighlightArea(int x_center_cell, int z_center_cell, int x_border, int z_border, sf::Color color, int y_shift = 1)
{
	int x1 = x_center_cell - x_border;
	int z1 = z_center_cell - z_border;

	int x2 = x_center_cell + x_border + 1;
	int z2 = z_center_cell + z_border + 1;

	if (x1 < 0)
		x1 = 0;

	if (x2 > HalfMapCellSize * 2)
		x2 = HalfMapCellSize * 2;

	if (z1 < 0)
		z1 = 0;

	if (z2 > HalfMapCellSize * 2)
		z2 = HalfMapCellSize * 2;

	glPushMatrix();

		for (int i = x1; i < x2; i++)
		{
			for (int t = z1; t < z2; t++)
			{
				glColor3f(color.r, color.g, color.b);

				glBegin(GL_QUADS);
					glVertex3d(((i + 0) * CellWidth) - HalfMapSpaceSize + 2, y_shift, ((t + 0) * CellWidth) - HalfMapSpaceSize + 2);
					glVertex3d(((i + 1) * CellWidth) - HalfMapSpaceSize - 2, y_shift, ((t + 0) * CellWidth) - HalfMapSpaceSize + 2);
					glVertex3d(((i + 1) * CellWidth) - HalfMapSpaceSize - 2, y_shift, ((t + 1) * CellWidth) - HalfMapSpaceSize - 2);
					glVertex3d(((i + 0) * CellWidth) - HalfMapSpaceSize + 2, y_shift, ((t + 1) * CellWidth) - HalfMapSpaceSize - 2);
				glEnd();
			}
		}

		glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void HighlightSquare(int x_center_cell, int z_center_cell, int border, sf::Color color, int y_shift = 1)
{
	HighlightArea(x_center_cell, z_center_cell, border, border, color, y_shift);
}

void PlayerAttack(int x, int y, Player &player)
{
	int target = 0;

	for (unsigned int i = 1; i < Players.size(); i++)
	{
		if (x == Players[i].GetXCell() && y == Players[i].GetYCell())
		{
			if (player.GetID() == Players[i].GetID())
			{
				return;
			}else
			{
				target = i;

				break;
			}
		} 
	}

	if ((x >= player.GetXCell() - player.GetARadius() && x <= player.GetXCell() + player.GetARadius() + 1) &&
		(y >= player.GetYCell() - player.GetARadius() && y <= player.GetYCell() + player.GetARadius() + 1) &&
		(x >= 0 && x <= HalfMapCellSize * 2 && y >= 0 && y <= HalfMapCellSize * 2) &&
		(target != 0 && Players[target].GetHP() > 0))
	{
		int oldHP = Players[target].GetHP();

		Players[target].SetHP(Players[target].GetHP() - player.GetAttack());
		player.GetModel().SetAnimation(MD2ANIM_ATTACK, false);

		if (Players[target].GetHP() <= 0)
		{
			if (oldHP > 0)
			{
				player.SetXP(player.GetXP() + oldHP);
			}

			Players[target].GetModel().SetAnimation(MD2ANIM_DEATH_FALLBACK, false, false);
		}
	}
}

void PlayerMove(int x, int y, Player &player)
{
	PlayerAttack(x, y, player);

	for (unsigned int i = 0; i < Players.size(); i++)
		if (player.GetID() != Players[i].GetID() && x == Players[i].GetXCell() && y == Players[i].GetYCell())
			return;

	if ((x >= player.GetXCell() - player.GetWRadius() && x <= player.GetXCell() + player.GetWRadius() + 1) &&
		(y >= player.GetYCell() - player.GetWRadius() && y <= player.GetYCell() + player.GetWRadius() + 1) &&
		(x >= 0 && x <= HalfMapCellSize * 2 && y >= 0 && y <= HalfMapCellSize * 2))
	{
		player.SetCellPosition(x, y);
	}
}

int main()
{
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MyDemo");
    App.PreserveOpenGLStates(true);

	sf::Clock Clock;
	sf::Vector3f CamPos;

	// ======= Create some players =======
	Player p1;

	p1.SetRace('b');
	p1.SetID((int) Players.size());
	p1.SetCellPosition(0, 10);

	Players.push_back(p1);
	
	Player p2;

	p2.SetRace('a');
	p2.SetID((int) Players.size());
	p2.SetCellPosition(10, 10);
	
	Players.push_back(p2);

	// ======== Calculate 1/2 of map size ==========
	HalfMapSpaceSize = (int) Players.size() * CellsPerPlayer * (CellWidth / 2);
	HalfMapCellSize = (int) Players.size() * (CellsPerPlayer / 2);

	PreapareGrid();

	// ======== Place players in random corners of the map ========
	for (unsigned int i = 0; i < Players.size(); i++)
    {
		int a = i % 3, b = i / 3;
        Players[i].SetCellPosition(((HalfMapCellSize * 2) - 1) * ((a > 1) ? 1 : a), ((HalfMapCellSize * 2) - 1) * ((b > 1) ? 1 : b));
    }

	// ======= OpenGL settings =======
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 500.f);

	glClearColor(0.0f, 0.5f, 1.0f, 0.5f);

	// ======= Set camera position =======
	CamPos = sf::Vector3f(0, 100, 75);

	App.ShowMouseCursor(false);

	while (App.IsOpened())
    {
        sf::Event Event;

		while (App.GetEvent(Event))
        {
	        if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);

			if (Event.Type == sf::Event::Closed || App.GetInput().IsKeyDown(sf::Key::Escape))
				App.Close();

			if (App.GetInput().IsKeyDown(sf::Key::W))
				CamPos.z -= 3;

			if (App.GetInput().IsKeyDown(sf::Key::S))
				CamPos.z += 3;

			if (App.GetInput().IsKeyDown(sf::Key::A))
				CamPos.x -= 3;

			if (App.GetInput().IsKeyDown(sf::Key::D))
				CamPos.x += 3;

			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				if (Event.MouseButton.Button == sf::Mouse::Left)
				{
					sf::Vector3f v = ScreenToSpace(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());

					int x1, z1; // cell indexes
					float x2, z2; // cell normal basis

					SpaceToCell(v.x, v.z, x1, z1, x2, z2);

					PlayerMove(x1, z1, Players[0]);
				}
			}
		}
		
		//  ======= Clear OpenGL state ======= 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (true)
		{
			//  ======= Update camera view ======= 
			/*gluLookAt(Players[0].Model.GetPosition().GetX(), Players[0].Model.GetPosition().GetY() + 100, Players[0].Model.GetPosition().GetZ() + 75, 
					  Players[0].Model.GetPosition().GetX(), Players[0].Model.GetPosition().GetY(), Players[0].Model.GetPosition().GetZ(), 
					  0, 1, 0);*/

			gluLookAt(CamPos.x, CamPos.y, CamPos.z,
					  CamPos.x, CamPos.y - 100, CamPos.z - 75, 
					  0, 1, 0);

			DrawGrid();

			//  ======= Pick terrain point at mouse cursor position ======= 
			{
				sf::Vector3f v = ScreenToSpace(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());

				int x1, z1; // cell indexes
				float x2, z2; // cell normal basis

				SpaceToCell(v.x, v.z, x1, z1);
				CellToSpace(x1, z1, x2, z2);

				//  ======= Draw normal ======= 
				glBegin(GL_LINES);
					glVertex3f(x2, 0, z2);
					glVertex3f(x2, 150, z2);
				glEnd();
			}

			//  ======= Render models ======= 
			for (unsigned int i = 0; i < Players.size(); i++)
			{
				float x1, x2;
				float time = Clock.GetElapsedTime();

				CellToSpace(Players[i].GetXCell(), Players[i].GetYCell(), x1, x2);

				Players[i].SetSpacePosition(x1,
					(float) (Players[i].GetModel().GetSize().GetY() / 4.f) + 6.5f, 
					x2);

				HighlightSquare(Players[i].GetXCell(), Players[i].GetYCell(), Players[i].GetWRadius(), sf::Color(1, 1, 0), 1);
				HighlightSquare(Players[i].GetXCell(), Players[i].GetYCell(), Players[i].GetARadius(), sf::Color(1, 0, 0), 2);

				Players[i].Draw(time);
			}
		}

		App.Display();
    }

	return EXIT_SUCCESS;
}