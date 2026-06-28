#ifndef ROOK_H
#define ROOK_H

#include "Pieces.h"
#include "Square.h"
#include <vector>
using namespace std;

class Rook : public Pieces
{
public:
    Rook(bool isWhite);
    vector<Square> getMoves(Square cells[][8], int x, int y);

    sf::Texture blackRook;
    sf::Texture whiteRook;
    bool hasMoved;
};

#endif // ROOK_H
