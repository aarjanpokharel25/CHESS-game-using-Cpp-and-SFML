#ifndef KING_H
#define KING_H

#include <vector>
#include "Square.h"
#include "Pieces.h"

class King : public Pieces
{
public:
    King(int);
    bool isStaleMate(Square Cells[][8], int x, int y);
    bool isCheck(Square Cells[][8], int x, int y);
    bool isCheckmate(Square Cells[][8], int x, int y);
    // Returns true if the square (x,y) is attacked by any piece of color 'byColor' (1 = white, -1 = black).
    // Stateless board scan - safe to call on any King instance.
    bool isSquareAttacked(Square Cells[][8], int x, int y, int byColor);
    vector<Square> getMoves(Square cells[][8], int x, int y);
    sf::Texture blackKing;
    sf::Texture whiteKing;

protected:
private:
};

#endif // KING_H
