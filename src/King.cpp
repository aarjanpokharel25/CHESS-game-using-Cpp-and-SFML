#include <iostream>
#include <vector>
#include "Square.h"
#include "Pieces.h"
#include "King.h"
using namespace std;

King::King(int color)
{
    isWhite = color;
    isAlive = true;
    blackKing.loadFromFile("Textures/b_king.png");
    whiteKing.loadFromFile("Textures/w_king.png");
    piece.setTexture((color == 0) ? blackKing : whiteKing);
    x = (color == 0) ? 0 : 7;
    y = 4;
    occupied_value = 3;
    piece.setPosition(y * 100.0f + 50.f, x * 100.f + 50.f);
    piece.setOrigin(sf::Vector2f(piece.getTexture()->getSize().x / 2, piece.getTexture()->getSize().y / 2));
    piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
}

vector<Square> King::getMoves(Square Cells[][8], int x, int y)
{
    possibleMoves.clear();
    int dx[] = {1, 1, 1, -1, -1, -1, 0, 0}; // all possible moves.
    int dy[] = {1, -1, 0, -1, 1, 0, -1, 1}; // all possible moves.
    for (int i = 0; i < 8; i++)
    {
        if (x + dx[i] > 7 || x + dx[i] < 0)
            continue;
        if (y + dy[i] > 7 || y + dy[i] < 0)
            continue;
        if (Cells[x + dx[i]][y + dy[i]].occupied_color == Cells[x][y].occupied_color)
            continue;
        possibleMoves.push_back(Cells[x + dx[i]][y + dy[i]]);
    }

    // --- Castling ---
    // Allowed only if the king hasn't moved, isn't currently in check, the relevant rook
    // hasn't moved, the squares between them are empty, and the king doesn't pass through
    // or land on an attacked square.
    int color = Cells[x][y].occupied_color;
    if (!Cells[x][y].hasMoved && !isSquareAttacked(Cells, x, y, -color))
    {
        // Kingside (short) castle: rook on column 7, king ends on column 6.
        if (Cells[x][7].occupied_value == 1 && Cells[x][7].occupied_color == color && !Cells[x][7].hasMoved)
        {
            if (Cells[x][5].occupied_value == 0 && Cells[x][6].occupied_value == 0)
            {
                if (!isSquareAttacked(Cells, x, 5, -color) && !isSquareAttacked(Cells, x, 6, -color))
                    possibleMoves.push_back(Cells[x][6]);
            }
        }
        // Queenside (long) castle: rook on column 0, king ends on column 2.
        if (Cells[x][0].occupied_value == 1 && Cells[x][0].occupied_color == color && !Cells[x][0].hasMoved)
        {
            if (Cells[x][1].occupied_value == 0 && Cells[x][2].occupied_value == 0 && Cells[x][3].occupied_value == 0)
            {
                if (!isSquareAttacked(Cells, x, 3, -color) && !isSquareAttacked(Cells, x, 2, -color))
                    possibleMoves.push_back(Cells[x][2]);
            }
        }
    }
    return possibleMoves;
}

// Returns true if square (x,y) is attacked by any piece belonging to 'byColor' (1 = white, -1 = black).
// Stateless board scan - can be called on any King instance regardless of whose king is being tested.
bool King::isSquareAttacked(Square Cells[][8], int x, int y, int byColor)
{
    // Rook / Queen - horizontal & vertical rays
    int rdx[] = {1, -1, 0, 0};
    int rdy[] = {0, 0, 1, -1};
    for (int d = 0; d < 4; d++)
    {
        int cx = x + rdx[d], cy = y + rdy[d];
        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8)
        {
            if (Cells[cx][cy].occupied_color != 0)
            {
                if (Cells[cx][cy].occupied_color == byColor &&
                    (Cells[cx][cy].occupied_value == 1 || Cells[cx][cy].occupied_value == 2))
                    return true;
                break;
            }
            cx += rdx[d];
            cy += rdy[d];
        }
    }
    // Bishop / Queen - diagonal rays
    int bdx[] = {1, 1, -1, -1};
    int bdy[] = {1, -1, 1, -1};
    for (int d = 0; d < 4; d++)
    {
        int cx = x + bdx[d], cy = y + bdy[d];
        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8)
        {
            if (Cells[cx][cy].occupied_color != 0)
            {
                if (Cells[cx][cy].occupied_color == byColor &&
                    (Cells[cx][cy].occupied_value == -2 || Cells[cx][cy].occupied_value == 2))
                    return true;
                break;
            }
            cx += bdx[d];
            cy += bdy[d];
        }
    }
    // Knight
    int ndx[] = {2, 2, -2, -2, 1, 1, -1, -1};
    int ndy[] = {1, -1, 1, -1, 2, -2, 2, -2};
    for (int i = 0; i < 8; i++)
    {
        int cx = x + ndx[i], cy = y + ndy[i];
        if (cx < 0 || cx > 7 || cy < 0 || cy > 7)
            continue;
        if (Cells[cx][cy].occupied_color == byColor && Cells[cx][cy].occupied_value == -1)
            return true;
    }
    // King (adjacency)
    int kdx[] = {1, 1, 1, -1, -1, -1, 0, 0};
    int kdy[] = {1, -1, 0, -1, 1, 0, -1, 1};
    for (int i = 0; i < 8; i++)
    {
        int cx = x + kdx[i], cy = y + kdy[i];
        if (cx < 0 || cx > 7 || cy < 0 || cy > 7)
            continue;
        if (Cells[cx][cy].occupied_color == byColor && Cells[cx][cy].occupied_value == 3)
            return true;
    }
    // Pawn - white pawns attack diagonally toward decreasing x, black toward increasing x
    if (byColor == 1 && x + 1 <= 7)
    {
        if (y - 1 >= 0 && Cells[x + 1][y - 1].occupied_color == 1 && Cells[x + 1][y - 1].occupied_value == -3)
            return true;
        if (y + 1 <= 7 && Cells[x + 1][y + 1].occupied_color == 1 && Cells[x + 1][y + 1].occupied_value == -3)
            return true;
    }
    else if (byColor == -1 && x - 1 >= 0)
    {
        if (y - 1 >= 0 && Cells[x - 1][y - 1].occupied_color == -1 && Cells[x - 1][y - 1].occupied_value == -3)
            return true;
        if (y + 1 <= 7 && Cells[x - 1][y + 1].occupied_color == -1 && Cells[x - 1][y + 1].occupied_value == -3)
            return true;
    }
    return false;
}

bool King::isCheck(Square Cells[][8], int x, int y)
{
    int color = Cells[x][y].occupied_color;
    if (color == 0)
        return false;
    return isSquareAttacked(Cells, x, y, -color);
}

// NOTE: these two only look at the king's own moves, so they're a quick heuristic,
// not authoritative (a check can also be escaped by blocking or capturing the checker
// with another piece). Game::updateGameStatus() does the real, whole-side legality
// check and is what actually drives checkmate/stalemate in the running game.
bool King::isCheckmate(Square Cells[][8], int x, int y)
{
    if (!possibleMoves.size() && isCheck(Cells, x, y))
        return true;
    return false;
}

bool King::isStaleMate(Square Cells[][8], int x, int y)
{
    if (!possibleMoves.size() && !isCheck(Cells, x, y))
        return true;
    return false;
}
