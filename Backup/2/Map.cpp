#include "Map.h"
#include <iostream>

Map::Map()
{
	GroundVertices = 0;
	GridVertices = 0;
	GridVerticesNum = 0;
}

int Map::AddPlayer(int Race) 
{
	Player p;
	
	p.SetRace(Race);
	p.SetID((int) Players.size());

	Players.push_back(p);

	return (int) Players.size() - 1;
}

void Map::Prepare()
{
	// ======== Calculate 1/2 of map size ==========
	HalfMapSpaceSize = (int) Players.size() * CellsPerPlayer * (CellWidth / 2);
	HalfMapCellSize = (int) Players.size() * (CellsPerPlayer / 2);

	// ======== Calculate grid and ground vertices ========
	GroundVertices = new float[4 * 3];
	GridVertices = new float[HalfMapCellSize * 2 * HalfMapCellSize * 2 * 8 * 3];

	int cnt = 0;

	for (int i = 0; i < HalfMapCellSize * 2; i++)
	{
		for (int t = 0; t < HalfMapCellSize * 2; t++)
		{
			// GRID
			GridVertices[cnt++] = (i + 0) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 0) * CellWidth * 1.f;

			GridVertices[cnt++] = (i + 1) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 0) * CellWidth * 1.f;



			GridVertices[cnt++] = (i + 1) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 0) * CellWidth * 1.f;

			GridVertices[cnt++] = (i + 1) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 1) * CellWidth * 1.f;



			GridVertices[cnt++] = (i + 1) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 1) * CellWidth * 1.f;

			GridVertices[cnt++] = (i + 0) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 1) * CellWidth * 1.f;



			GridVertices[cnt++] = (i + 0) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 1) * CellWidth * 1.f;

			GridVertices[cnt++] = (i + 0) * CellWidth * 1.f; 
			GridVertices[cnt++] = 0.5f; 
			GridVertices[cnt++] = (t + 0) * CellWidth * 1.f;
		}
	}

	GridVerticesNum = cnt / 3;

	// Map vertices
	GroundVertices[0] = 0.f; 
	GroundVertices[1] = 0.f; 
	GroundVertices[2] = 0.f;

	GroundVertices[3] = CellWidth * HalfMapCellSize * 2.f;
	GroundVertices[4] = 0.f; 
	GroundVertices[5] = 0.f;

	GroundVertices[6] = CellWidth * HalfMapCellSize * 2.f; 
	GroundVertices[7] = 0.f; 
	GroundVertices[8] = CellWidth * HalfMapCellSize * 2.f;

	GroundVertices[9] = 0.f; 
	GroundVertices[10] = 0.f; 
	GroundVertices[11] = CellWidth * HalfMapCellSize * 2.f;

	// ======== Place players in random corners of the map ========
	for (int i = 0; i < Players.size(); i++)
    {
		int a = i % 3, b = i / 3;
        Players[i].SetCellPosition(((HalfMapCellSize * 2) - 1) * ((a > 1) ? 1 : a), ((HalfMapCellSize * 2) - 1) * ((b > 1) ? 1 : b));
    }
}

// ====== Simply renders floor and grid =======
void Map::Draw()
{
	glPushMatrix();
		int k = -1 * HalfMapSpaceSize;
		glTranslated(k, 0, k);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, GroundVertices);

		glColor3f(0.0f, 0.25f, 0.5f);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, GridVertices);

		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINES, 0, GridVerticesNum);

		glDisableClientState(GL_VERTEX_ARRAY);

		glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void Map::SpaceToCell(float x, float z, int &x_res, int &z_res, float &x_normal, float &z_normal)
{
	x_res = int(x / CellWidth), z_res = int(z / CellWidth);

	x_res += sign(x); z_res += sign(z);

	x_normal = float(x_res * CellWidth), z_normal = float(z_res * CellWidth);

	x_res += HalfMapCellSize; z_res += HalfMapCellSize; 
	x_res -= (x < 0) ? 0 : 1; z_res -= (z < 0) ? 0 : 1;

	if (x_res < 0)
		x_res = 0; else
	if (x_res >= HalfMapCellSize * 2)
		x_res = (HalfMapCellSize * 2) - 1;

	if (z_res < 0)
		z_res = 0; else
	if (z_res >= HalfMapCellSize * 2)
		z_res = (HalfMapCellSize * 2) - 1;

	if (x > 0)
		x_normal -= (CellWidth / 2); else
	if (x < 0)
		x_normal += (CellWidth / 2);

	if (z > 0)
		z_normal -= (CellWidth / 2); else
	if (z < 0)
		z_normal += (CellWidth / 2);
}

void Map::SpaceToCell(float x, float z, int &x_res, int &z_res)
{
	x_res = int(x / CellWidth), z_res = int(z / CellWidth);

	x_res += sign(x); z_res += sign(z);

	x_res += HalfMapCellSize; z_res += HalfMapCellSize; 
	x_res -= (x < 0) ? 0 : 1; z_res -= (z < 0) ? 0 : 1;

	if (x_res < 0)
		x_res = 0; else
	if (x_res >= HalfMapCellSize * 2)
		x_res = (HalfMapCellSize * 2) - 1;

	if (z_res < 0)
		z_res = 0; else
	if (z_res >= (HalfMapCellSize * 2))
		z_res = (HalfMapCellSize * 2) - 1;
}

void Map::CellToSpace(int x, int z, float &x_normal, float &z_normal)
{
	x_normal = float(x * CellWidth) - HalfMapSpaceSize + (CellWidth / 2);
	z_normal = float(z * CellWidth) - HalfMapSpaceSize + (CellWidth / 2);

	if (x_normal < -1 * HalfMapSpaceSize)
		x_normal = (float) -1.f * HalfMapSpaceSize + (CellWidth / 2); else
	if (x_normal > HalfMapSpaceSize)
		x_normal = (float) HalfMapSpaceSize - (CellWidth / 2);

	if (z_normal < -1 * HalfMapSpaceSize)
		z_normal = (float) -1.f * HalfMapSpaceSize + (CellWidth / 2); else
	if (z_normal > HalfMapSpaceSize)
		z_normal = (float) HalfMapSpaceSize - (CellWidth / 2);
}

void Map::HighlightArea(int x_center_cell, int z_center_cell, int x_border, int z_border, sf::Color color, int y_shift)
{
	int x1 = x_center_cell - x_border;
	int z1 = z_center_cell - z_border;

	int x2 = x_center_cell + x_border + 1;
	int z2 = z_center_cell + z_border + 1;

	if (x1 < 0)
		x1 = 0;

	if (x2 >= HalfMapCellSize * 2)
		x2 = HalfMapCellSize * 2;

	if (z1 < 0)
		z1 = 0;

	if (z2 >= HalfMapCellSize * 2)
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

void Map::HighlightSquare(int x_center_cell, int z_center_cell, int border, sf::Color color, int y_shift)
{
	HighlightArea(x_center_cell, z_center_cell, border, border, color, y_shift);
}

void Map::PlayerAttack(int x, int y, int PlayerIndex)
{
	int target = 0;

	for (int i = 1; i < Players.size(); i++)
	{
		if (x == Players[i].GetXCell() && y == Players[i].GetYCell())
		{
			if (Players[PlayerIndex].GetID() == Players[i].GetID())
			{
				return;
			}else
			{
				target = i;

				break;
			}
		} 
	}

	if (target == 0)
		return;

	if ((x >= Players[PlayerIndex].GetXCell() - Players[PlayerIndex].GetARadius() && x <= Players[PlayerIndex].GetXCell() + Players[PlayerIndex].GetARadius() + 1) &&
		(y >= Players[PlayerIndex].GetYCell() - Players[PlayerIndex].GetARadius() && y <= Players[PlayerIndex].GetYCell() + Players[PlayerIndex].GetARadius() + 1) &&
		(x >= 0 && x < HalfMapCellSize * 2 && y >= 0 && y < HalfMapCellSize * 2) &&
		(target != 0 && Players[target].GetHP() > 0))
	{
		int oldHP = Players[target].GetHP();

		Players[target].SetHP(Players[target].GetHP() - Players[PlayerIndex].GetAttack());
		Players[PlayerIndex].Model.SetAnimation(MD2ANIM_ATTACK, false);

		if (Players[target].GetHP() <= 0)
		{
			if (oldHP > 0)
			{
				int deltaLvl = Players[target].GetLevel() - Players[PlayerIndex].GetLevel();
				int gotXP = (pow(4.f, deltaLvl) * 10) + (oldHP - Players[target].GetHP());

				Players[PlayerIndex].SetXP(Players[PlayerIndex].GetXP() + gotXP);
			}

			Players[target].Model.SetAnimation(MD2ANIM_DEATH_FALLBACK, false, false);
		}
	}
}

void Map::PlayerMove(int x, int y, int PlayerIndex)
{
	PlayerAttack(x, y, PlayerIndex);

	for (int i = 0; i < Players.size(); i++)
	{
		if (x == Players[i].GetXCell() && y == Players[i].GetYCell())
			return;
	}

	if ((x >= Players[PlayerIndex].GetXCell() - Players[PlayerIndex].GetWRadius() && x <= Players[PlayerIndex].GetXCell() + Players[PlayerIndex].GetWRadius() + 1) &&
		(y >= Players[PlayerIndex].GetYCell() - Players[PlayerIndex].GetWRadius() && y <= Players[PlayerIndex].GetYCell() + Players[PlayerIndex].GetWRadius() + 1) &&
		(x >= 0 && x < HalfMapCellSize * 2 && y >= 0 && y < HalfMapCellSize * 2))
	{
		Players[PlayerIndex].SetCellPosition(x, y);
	}
}

void Map::DrawPlayers(float time)
{
	// ======= Render models ======= 
	for (int i = 0; i < Players.size(); i++)
	{
		float x1, x2;

		CellToSpace(Players[i].GetXCell(), Players[i].GetYCell(), x1, x2);

		Players[i].SetSpacePosition(x1,
			(float) (Players[i].Model.GetSize().GetY() / 4.f) + 6.5f, 
			x2);

		HighlightSquare(Players[i].GetXCell(), Players[i].GetYCell(), Players[i].GetWRadius(), sf::Color(1, 1, 0), 1);
		HighlightSquare(Players[i].GetXCell(), Players[i].GetYCell(), Players[i].GetARadius(), sf::Color(1, 0, 0), 2);

		Players[i].Draw(time);
	}
}