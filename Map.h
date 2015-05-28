#ifndef __MAP__
#define __MAP__

#include "Utils.h"
#include "Player.h"

#include <vector>

#define CellWidth 40
#define CellsPerPlayer 10

class Map
{
	private:
		int MaxPlayers;

		int HalfMapSpaceSize, HalfMapCellSize;
		float *GroundVertices, *GridVertices; 
		int GridVerticesNum;

	public:
		std::vector<Player> Players;

	public:
		Map();

		int AddPlayer(int Race);

		void Prepare();
		void Draw();
		void DrawPlayers(float time);

		void SpaceToCell(float x, float z, int &x_res, int &z_res, float &x_normal, float &z_normal);
		void SpaceToCell(float x, float z, int &x_res, int &z_res);
		void CellToSpace(int x, int z, float &x_normal, float &z_normal);

		void HighlightArea(int x_center_cell, int z_center_cell, int x_border, int z_border, sf::Color color, int y_shift = 1);
		void HighlightSquare(int x_center_cell, int z_center_cell, int border, sf::Color color, int y_shift = 1);

		void PlayerAttack(int x, int y, int PlayerIndex);
		void PlayerMove(int x, int y, int PlayerIndex);
};

#endif