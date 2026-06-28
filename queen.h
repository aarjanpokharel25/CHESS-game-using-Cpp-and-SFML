#ifndef QUEEN_H
#define QUEEN_H

#include "Pieces.h"
#include "Square.h"
#include <vector>
using namespace std;

class Queen : public Pieces
{
public:
    Queen(bool isWhite);
    vector<Square> getMoves(Square cells[][8], int x, int y);

    sf::Texture blackQueen;
    sf::Texture whiteQueen;
};

#endif // QUEEN_H
