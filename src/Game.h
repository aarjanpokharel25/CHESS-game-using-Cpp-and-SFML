#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Pieces.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"
#include "Rook.h"

class Game : public sf::Drawable
{
public:
    Game(sf::Color c1, sf::Color c2);
    void Start(sf::Color c1, sf::Color c2);
    void DrawPossibleMoves();
    bool SelectPiece(Square Cells[][8], int x, int y);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void moveSelected(Square Cells[][8], int x, int y);
    bool getSelected();
    void gameOver();
    void SetRightSideofWindow();
    // Filters a piece's pseudo-legal moves down to ones that don't leave that piece's
    // own king in check (simulates each move on a scratch copy of the board).
    vector<Square> getLegalMoves(Pieces *piece);
    // Called after every completed move: figures out whether the side now to move is
    // in check, checkmated, or stalemated, and updates isOver/situation accordingly.
    void updateGameStatus();
    // Converts board coordinates to algebraic notation, e.g. (6,4) -> "e2".
    std::string squareName(int x, int y);
    // Appends "+" or "#" to the last recorded move if the position is now check/mate.
    void AppendMoveSuffix();
    Square cells[8][8];
    bool isOver;
private:
    vector<Square> moves;
    vector<sf::RectangleShape> newmoves;
    vector<Pieces *> whitePieces;
    vector<Pieces *> blackPieces;
    // white pieces
    King *w_king;
    Queen *w_queen;
    Bishop *w_bishop[2];
    Rook *w_rook[2];
    Knight *w_knight[2];
    Pawn *w_pawn[8];
    // black pieces
    King *b_king;
    Queen *b_queen;
    Bishop *b_bishop[2];
    Rook *b_rook[2];
    Knight *b_knight[2];
    Pawn *b_pawn[8];

    bool gameStatus, selected, whiteTurn;
    Pieces *selected_piece;
    sf::Font font;
    sf::RectangleShape infoRestart;
    sf::Text turn, situation, textRestart, chance;
    int number_of_moves;

    // --- last move highlight (border only, shown on the two squares of the last move) ---
    bool hasLastMove;
    int lastMoveFromX, lastMoveFromY, lastMoveToX, lastMoveToY;

    // --- move history, shown as algebraic notation on the side panel ---
    vector<std::string> moveHistory;
};

#endif
