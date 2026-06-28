#include "Queen.h"

Queen::Queen(bool isWhite)
{
    this->isWhite = isWhite;
    this->isAlive = true;
    this->occupied_value = 2;

    if (isWhite) {
        whiteQueen.loadFromFile("images/wQ.png");
        piece.setTexture(whiteQueen);
        this->occupied_color = 1;
    } else {
        blackQueen.loadFromFile("images/bQ.png");
        piece.setTexture(blackQueen);
        this->occupied_color = 0;  // 0 for black since occupied_color is bool
    }
}

vector<Square> Queen::getMoves(Square cells[][8], int x, int y)
{
    vector<Square> moves;

    int dirs[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1,-1}, {-1, 1}, {1,-1}, {1, 1}
    };

    for (int d = 0; d < 8; d++)
    {
        int nx = x + dirs[d][0];
        int ny = y + dirs[d][1];

        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
        {
            if (cells[nx][ny].occupied_value == 0)
            {
                moves.push_back(cells[nx][ny]);
            }
            else
            {
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