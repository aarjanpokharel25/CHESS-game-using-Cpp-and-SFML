#include "Rook.h"

Rook::Rook(bool isWhite)
{
    this->isWhite = isWhite;
    this->isAlive = true;
    this->hasMoved = false;
    this->occupied_value = 1;

    if (isWhite) {
        whiteRook.loadFromFile("images/wR.png");
        piece.setTexture(whiteRook);
        this->occupied_color = 1;
    } else {
        blackRook.loadFromFile("images/bR.png");
        piece.setTexture(blackRook);
        this->occupied_color = 0;  // using 0 for black since occupied_color is bool
    }
}

vector<Square> Rook::getMoves(Square cells[][8], int x, int y)
{
    vector<Square> moves;

    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    for (int d = 0; d < 4; d++)
    {
        int nx = x + dirs[d][0];
        int ny = y + dirs[d][1];

        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
        {
            if (cells[nx][ny].occupied_value == 0)
            {
                // Empty square
                moves.push_back(cells[nx][ny]);
            }
            else
            {
                // Occupied — capture if enemy
                if (cells[nx][ny].occupied_color != this->occupied_color)
                {
                    moves.push_back(cells[nx][ny]);
                }
                break;
            }

            nx += dirs[d][0];
            ny += dirs[d][1];
        }
    }

    return moves;
}