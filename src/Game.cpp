#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Pieces.h"
#include "Square.h"
//fixed logic
Game::Game(sf::Color c1, sf::Color c2)
{
    font.loadFromFile("Textures/roboto.ttf");
    infoRestart.setFillColor(sf::Color::White);
    infoRestart.setOutlineThickness(-5.f);
    infoRestart.setOutlineColor(sf::Color::Black);
    infoRestart.setPosition(sf::Vector2f(850.f, 0.f));
    infoRestart.setSize(sf::Vector2f(180.f, 60.f));
    textRestart.setFont(font);
    textRestart.setString("Play Again");
    textRestart.setCharacterSize(28);
    textRestart.setFillColor(sf::Color::Black);
    textRestart.setPosition(infoRestart.getPosition().x + 35.f, infoRestart.getPosition().y + 13.f);
    turn.setFont(font);
    turn.setCharacterSize(30);
    turn.setStyle(sf::Text::Regular);
    turn.setFillColor(sf::Color::White);
    turn.setPosition(880.f, 70.f);
    chance.setFont(font);
    chance.setCharacterSize(30);
    chance.setStyle(sf::Text::Regular);
    chance.setFillColor(sf::Color::White);
    chance.setPosition(turn.getPosition().x, turn.getPosition().y + 50.f);
    situation.setFont(font);
    situation.setCharacterSize(30);
    situation.setStyle(sf::Text::Regular);
    situation.setFillColor(sf::Color::White);
    situation.setPosition(chance.getPosition().x, chance.getPosition().y + 50.f);
    w_king = new King(1);
    whitePieces.push_back(w_king);
    w_queen = new Queen(1);
    whitePieces.push_back(w_queen);
    b_king = new King(0);
    blackPieces.push_back(b_king);
    b_queen = new Queen(0);
    blackPieces.push_back(b_queen);
    for (int i = 0; i < 8; i++)
    {
        b_pawn[i] = new Pawn(0);
        blackPieces.push_back(b_pawn[i]);
        w_pawn[i] = new Pawn(1);
        whitePieces.push_back(w_pawn[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        w_bishop[i] = new Bishop(1);
        whitePieces.push_back(w_bishop[i]);
        w_rook[i] = new Rook(1);
        whitePieces.push_back(w_rook[i]);
        w_knight[i] = new Knight(1);
        whitePieces.push_back(w_knight[i]);
        b_bishop[i] = new Bishop(0);
        blackPieces.push_back(b_bishop[i]);
        b_rook[i] = new Rook(0);
        blackPieces.push_back(b_rook[i]);
        b_knight[i] = new Knight(0);
        blackPieces.push_back(b_knight[i]);
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cells[i][j].x = i;
            cells[i][j].y = j;
        }
    }
    for (int i = 0; i < 4; i++)
        promoChoices[i] = NULL;
    hasLastMove = false;
    awaitingPromotion = false;
    Start(c1, c2);
}

void Game::Start(sf::Color c1, sf::Color c2)
{
    number_of_moves = 0;
    isOver = false;
    gameStatus = true, whiteTurn = 1, selected = false;
    selected_piece = NULL;
    moveHistory.clear();
    hasLastMove = false;
    awaitingPromotion = false;
    for (int i = 0; i < 8; i++)
    {
        b_pawn[i]->y = i;
        b_pawn[i]->piece.setPosition(b_pawn[i]->y * 100.f + 50.f, b_pawn[i]->x * 100.f + 50.f);
        b_pawn[i]->piece.setOrigin(sf::Vector2f(b_pawn[i]->piece.getTexture()->getSize().x / 2, b_pawn[i]->piece.getTexture()->getSize().y / 2));
        b_pawn[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        w_pawn[i]->y = i;
        w_pawn[i]->piece.setPosition(w_pawn[i]->y * 100.f + 50.f, w_pawn[i]->x * 100.f + 50.f);
        w_pawn[i]->piece.setOrigin(sf::Vector2f(w_pawn[i]->piece.getTexture()->getSize().x / 2, w_pawn[i]->piece.getTexture()->getSize().y / 2));
        w_pawn[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
    }
    b_bishop[0]->y = 2;
    b_bishop[1]->y = 5;
    b_rook[0]->y = 0;
    b_rook[1]->y = 7;
    b_knight[0]->y = 1;
    b_knight[1]->y = 6;
    w_bishop[0]->y = 2;
    w_bishop[1]->y = 5;
    w_rook[0]->y = 0;
    w_rook[1]->y = 7;
    w_knight[0]->y = 1;
    w_knight[1]->y = 6;
    for (int i = 0; i < 2; i++)
    {
        b_bishop[i]->piece.setPosition(100.0f * b_bishop[i]->y + 50.f, 100.0f * b_bishop[i]->x + 50.f);
        b_knight[i]->piece.setPosition(100.0f * b_knight[i]->y + 50.f, 100.0f * b_knight[i]->x + 50.f);
        b_rook[i]->piece.setPosition(100.0f * b_rook[i]->y + 50.f, 100.0f * b_rook[i]->x + 50.f);
        b_bishop[i]->piece.setOrigin(100.0f * b_bishop[i]->x, 100.0f * b_bishop[i]->y);
        b_knight[i]->piece.setOrigin(100.0f * b_knight[i]->x, 100.0f * b_knight[i]->y);
        b_rook[i]->piece.setOrigin(100.0f * b_rook[i]->x, 100.0f * b_rook[i]->y);
        b_bishop[i]->piece.setOrigin(sf::Vector2f(b_bishop[i]->piece.getTexture()->getSize().x / 2, b_bishop[i]->piece.getTexture()->getSize().y / 2));
        b_bishop[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        b_rook[i]->piece.setOrigin(sf::Vector2f(b_rook[i]->piece.getTexture()->getSize().x / 2, b_rook[i]->piece.getTexture()->getSize().y / 2));
        b_rook[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        b_knight[i]->piece.setOrigin(sf::Vector2f(b_knight[i]->piece.getTexture()->getSize().x / 2, b_knight[i]->piece.getTexture()->getSize().y / 2));
        b_knight[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        w_bishop[i]->piece.setPosition(100.0f * w_bishop[i]->y + 50.f, 100.0f * w_bishop[i]->x + 50.f);
        w_knight[i]->piece.setPosition(100.0f * w_knight[i]->y + 50.f, 100.0f * w_knight[i]->x + 50.f);
        w_rook[i]->piece.setPosition(100.0f * w_rook[i]->y + 50.f, 100.0f * w_rook[i]->x + 50.f);
        w_bishop[i]->piece.setOrigin(100.0f * w_bishop[i]->x, 100.0f * w_bishop[i]->y);
        w_knight[i]->piece.setOrigin(100.0f * w_knight[i]->x, 100.0f * w_knight[i]->y);
        w_rook[i]->piece.setOrigin(100.0f * w_rook[i]->x, 100.0f * w_rook[i]->y);
        w_bishop[i]->piece.setOrigin(sf::Vector2f(w_bishop[i]->piece.getTexture()->getSize().x / 2, w_bishop[i]->piece.getTexture()->getSize().y / 2));
        w_bishop[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        w_rook[i]->piece.setOrigin(sf::Vector2f(w_rook[i]->piece.getTexture()->getSize().x / 2, w_rook[i]->piece.getTexture()->getSize().y / 2));
        w_rook[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
        w_knight[i]->piece.setOrigin(sf::Vector2f(w_knight[i]->piece.getTexture()->getSize().x / 2, w_knight[i]->piece.getTexture()->getSize().y / 2));
        w_knight[i]->piece.setScale(sf::Vector2f(0.46875f, 0.46875f));
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cells[i][j].square.setPosition(sf::Vector2f(j * 100.0f, i * 100.0f));
            cells[i][j].square.setSize(sf::Vector2f(100.f, 100.f));
            cells[i][j].square.setFillColor((i + j) % 2 ? c2 : c1);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cells[7 - i][j].occupied_color = 1;
            cells[i][j].occupied_color = -1;
        }
    }
    // loop to set up pawns on board
    for (int j = 0; j < 8; j++)
    {
        cells[1][j].occupied_value = -3;
        cells[6][j].occupied_value = -3;
    }
    cells[0][0].occupied_value = 1;
    cells[7][7].occupied_value = 1;
    cells[7][0].occupied_value = 1;
    cells[0][7].occupied_value = 1; // setting up rook
    cells[0][1].occupied_value = -1;
    cells[7][6].occupied_value = -1;
    cells[0][6].occupied_value = -1;
    cells[7][1].occupied_value = -1; // setting up knight
    cells[0][2].occupied_value = -2;
    cells[7][5].occupied_value = -2;
    cells[0][5].occupied_value = -2;
    cells[7][2].occupied_value = -2; // setting up bishop
    cells[7][3].occupied_value = 2;
    cells[0][3].occupied_value = 2; // setting up queen
    cells[7][4].occupied_value = 3;
    cells[0][4].occupied_value = 3; // setting up king
    SetRightSideofWindow();
}

void Game::SetRightSideofWindow()
{
    turn.setString("Moves: " + std::to_string(number_of_moves/2 + number_of_moves%2));
    number_of_moves++;
    if(whiteTurn == 0 && !isOver)
        chance.setString("Black's Turn");
    else if(whiteTurn == 1 && !isOver)
        chance.setString("White's Turn");   
}

std::string Game::squareName(int x, int y)
{
    char file = 'a' + y;
    char rank = '1' + (7 - x);
    std::string s;
    s += file;
    s += rank;
    return s;
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.clear();
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            target.draw(cells[i][j].square);
        }
    }
    target.draw(infoRestart);
    target.draw(textRestart);
    target.draw(turn);
    target.draw(situation);
    if(isOver == false)
        target.draw(chance);

    // --- move history panel (right side) ---
    sf::Text histHeader;
    histHeader.setFont(font);
    histHeader.setCharacterSize(22);
    histHeader.setStyle(sf::Text::Bold);
    histHeader.setFillColor(sf::Color::White);
    histHeader.setPosition(880.f, 250.f);
    histHeader.setString("Move History");
    target.draw(histHeader);

    float listStartY = 285.f;
    float lineHeight = 24.f;
    float bottomMargin = 790.f;
    int maxLines = (int)((bottomMargin - listStartY) / lineHeight);
    if (maxLines < 1) maxLines = 1;
    int totalPairs = (int)((moveHistory.size() + 1) / 2);
    int startPair = 0;
    if (totalPairs > maxLines)
        startPair = totalPairs - maxLines;
    for (int p = startPair; p < totalPairs; p++)
    {
        unsigned int wIdx = p * 2, bIdx = p * 2 + 1;
        std::string line = std::to_string(p + 1) + ". " + moveHistory[wIdx];
        if (bIdx < moveHistory.size())
            line += "   " + moveHistory[bIdx];
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(18);
        t.setFillColor(sf::Color::White);
        t.setPosition(880.f, listStartY + (p - startPair) * lineHeight);
        t.setString(line);
        target.draw(t);
    }

    // --- last move highlight: border only, on the two squares of the last move ---
    if (hasLastMove)
    {
        sf::RectangleShape hl;
        hl.setSize(sf::Vector2f(100.f, 100.f));
        hl.setFillColor(sf::Color::Transparent);
        hl.setOutlineColor(sf::Color(0xF7D34Bcc));
        hl.setOutlineThickness(-4.f);
        hl.setPosition(lastMoveFromY * 100.f, lastMoveFromX * 100.f);
        target.draw(hl);
        hl.setPosition(lastMoveToY * 100.f, lastMoveToX * 100.f);
        target.draw(hl);
    }

    for (int i = 0; i < moves.size(); i++)
    {
        target.draw(moves[i].square);
    }

    if ((selected_piece != NULL))
    {
        for (int i = 0; i < newmoves.size(); i++)
        {
            target.draw(newmoves[i]);
        }
    }
    for (int i = 0; i < whitePieces.size(); i++)
    {
        if (whitePieces[i]->isAlive)
        {
            target.draw(whitePieces[i]->piece);
        }
         /*if(!whitePieces[i]->isAlive){
              if(!w_king->isAlive){
                 sleep(3);
                 exit(0);
              }
         }*/
    }
    for (int i = 0; i < blackPieces.size(); i++)
    {
        if (blackPieces[i]->isAlive)
        {
            target.draw(blackPieces[i]->piece);
        }
    }

    // --- pawn promotion picker overlay, drawn on top of everything else ---
    if (awaitingPromotion)
    {
        for (int i = 0; i < 4; i++)
            target.draw(promoBoxes[i]);
        for (int i = 0; i < 4; i++)
            if (promoChoices[i] != NULL)
                target.draw(promoChoices[i]->piece);
    }
}

void Game::gameOver()
{
    isOver = true;
    if (whiteTurn == 0)
        situation.setString("CHECKMATE! \n White Won!!");
    else
        situation.setString("CHECKMATE! \n Black Won!!");
}

void Game::DrawPossibleMoves()
{
    if (selected_piece == NULL)
        return;
    newmoves.clear();
    moves.clear();
    moves = getLegalMoves(selected_piece);
    for (int i = 0; i < moves.size(); i++)
    {
        sf::RectangleShape tmp;
        tmp.setPosition(sf::Vector2f((moves[i].y) * 100.f, (moves[i].x) * 100.f));
        tmp.setSize(sf::Vector2f(100.f, 100.f));
        tmp.setFillColor(sf::Color(0x66b4ccaa));
        newmoves.push_back(tmp);
    }
    sf::RectangleShape tmp;
    tmp.setPosition(sf::Vector2f((selected_piece->y) * 100.f, (selected_piece->x) * 100.f));
    tmp.setSize(sf::Vector2f(100.f, 100.f));
    tmp.setFillColor(sf::Color(0x00000000));
    tmp.setOutlineColor(sf::Color::Red);
    tmp.setOutlineThickness(-3.f);
    newmoves.push_back(tmp);
}

bool Game::SelectPiece(Square Cells[][8], int x, int y)
{
    if (awaitingPromotion)
        return false;
    if (Cells[x][y].occupied_color == 0)
    {
        selected_piece = NULL;
        return false;
    }
    if (Cells[x][y].occupied_color == 1 && whiteTurn == 0 || Cells[x][y].occupied_color == -1 && whiteTurn == 1)
    {
        selected_piece = NULL;
        return false;
    }
    selected = true;
    // Search the moving side's own piece list for whichever piece sits on (x,y).
    // This also transparently covers promoted pieces (extra queens/rooks/etc.)
    // since they're pushed into these same vectors when created.
    vector<Pieces *> &side = (Cells[x][y].occupied_color == 1) ? whitePieces : blackPieces;
    selected_piece = NULL;
    for (int i = 0; i < side.size(); i++)
    {
        if (side[i]->isAlive && side[i]->x == x && side[i]->y == y)
        {
            selected_piece = side[i];
            break;
        }
    }
    DrawPossibleMoves();
    return true;
}

bool Game::getSelected()
{
    return selected;
}

void Game::moveSelected(Square Cells[][8], int x, int y)
{
    if (selected_piece == NULL)
        return;
    bool valid = false;
    vector<Square> legalMoves = getLegalMoves(selected_piece);
    for (int i = 0; i < legalMoves.size(); i++)
    {
        if (x == legalMoves[i].x && y == legalMoves[i].y)
        {
            valid = true;
            break;
        }
    }
    if (valid)
    {
        int a = selected_piece->x, b = selected_piece->y;
        int pieceVal = selected_piece->occupied_value;
        bool isCaptureMove = (Cells[x][y].occupied_color != 0 && Cells[x][y].occupied_color != Cells[a][b].occupied_color);
        bool isCastleMove = (pieceVal == 3 && abs(y - b) == 2);
        bool isPromotionMove = (pieceVal == -3) && ((whiteTurn && x == 0) || (!whiteTurn && x == 7));

        std::string notation;
        if (isCastleMove)
        {
            notation = (y == 6) ? "O-O" : "O-O-O";
        }
        else
        {
            char letter = 0;
            if (pieceVal == 3) letter = 'K';
            else if (pieceVal == 2) letter = 'Q';
            else if (pieceVal == 1) letter = 'R';
            else if (pieceVal == -1) letter = 'N';
            else if (pieceVal == -2) letter = 'B';
            if (letter) notation += letter;
            if (pieceVal == -3 && isCaptureMove)
                notation += char('a' + b);
            if (isCaptureMove) notation += "x";
            notation += squareName(x, y);
        }

        selected_piece->piece.setPosition(sf::Vector2f(100.f * y + 50.f, 100.f * x + 50.f));
        if (Cells[x][y].occupied_color != 0 && Cells[x][y].occupied_color != Cells[a][b].occupied_color)
        {
            if (Cells[x][y].occupied_color == 1)
            {
                for (int i = 0; i < whitePieces.size(); i++)
                {
                    if (whitePieces[i]->x == x && whitePieces[i]->y == y)
                    {
                        whitePieces[i]->isAlive = false;
                    }
                }
            }
            else
            {
                for (int i = 0; i < blackPieces.size(); i++)
                {
                    if (blackPieces[i]->x == x && blackPieces[i]->y == y)
                    {
                        blackPieces[i]->isAlive = false;
                    }
                }
            }
        }
        Cells[x][y].occupied_color = (whiteTurn == 1) ? 1 : -1;
        Cells[x][y].occupied_value = selected_piece->occupied_value;
        Cells[a][b].occupied_value = 0;
        Cells[selected_piece->x][selected_piece->y].occupied_color = 0;
        Cells[a][b].hasMoved = true;

        if (isCastleMove)
        {
            int rookFromY = (y == 6) ? 7 : 0;
            int rookToY = (y == 6) ? 5 : 3;
            Rook **rooks = whiteTurn ? w_rook : b_rook;
            Rook *castlingRook = (rooks[0]->y == rookFromY) ? rooks[0] : rooks[1];
            Cells[x][rookToY].occupied_color = Cells[x][rookFromY].occupied_color;
            Cells[x][rookToY].occupied_value = Cells[x][rookFromY].occupied_value;
            Cells[x][rookFromY].occupied_value = 0;
            Cells[x][rookFromY].occupied_color = 0;
            Cells[x][rookFromY].hasMoved = true;
            castlingRook->x = x;
            castlingRook->y = rookToY;
            castlingRook->piece.setPosition(sf::Vector2f(100.f * rookToY + 50.f, 100.f * x + 50.f));
        }

        // selected_piece already IS the exact object that moved (found by SelectPiece
        // scanning the piece list), so its identity never needs to be re-derived here -
        // this also makes promoted pieces (extra queens etc.) work with no special case.
        selected_piece->x = x;
        selected_piece->y = y;

        hasLastMove = true;
        lastMoveFromX = a;
        lastMoveFromY = b;
        lastMoveToX = x;
        lastMoveToY = y;

        if (isPromotionMove)
        {
            awaitingPromotion = true;
            promotionColor = whiteTurn ? 1 : 0;
            promotionX = x;
            promotionY = y;
            pendingPawn = selected_piece;
            pendingNotation = notation;

            promoChoices[0] = new Queen(promotionColor);
            promoChoices[1] = new Rook(promotionColor);
            promoChoices[2] = new Bishop(promotionColor);
            promoChoices[3] = new Knight(promotionColor);

            int row0 = (x == 0) ? 0 : 7;
            int rowStep = (x == 0) ? 1 : -1;
            for (int i = 0; i < 4; i++)
            {
                int row = row0 + rowStep * i;
                promoBoxes[i].setPosition(y * 100.f, row * 100.f);
                promoBoxes[i].setSize(sf::Vector2f(100.f, 100.f));
                promoBoxes[i].setFillColor(sf::Color(0xf0d878ee));
                promoBoxes[i].setOutlineColor(sf::Color::Black);
                promoBoxes[i].setOutlineThickness(2.f);
                promoChoices[i]->x = row;
                promoChoices[i]->y = y;
                promoChoices[i]->piece.setPosition(y * 100.f + 50.f, row * 100.f + 50.f);
                promoChoices[i]->piece.setOrigin(promoChoices[i]->piece.getTexture()->getSize().x / 2.f, promoChoices[i]->piece.getTexture()->getSize().y / 2.f);
                promoChoices[i]->piece.setScale(0.46875f, 0.46875f);
            }
        }

        else
        {
            moveHistory.push_back(notation);
            whiteTurn = !whiteTurn;
            SetRightSideofWindow();
            updateGameStatus();
            AppendMoveSuffix();
        }
    }
    selected_piece = NULL;
    selected = false;
}

vector<Square> Game::getLegalMoves(Pieces *piece)
{
    vector<Square> legal;
    if (piece == NULL || !piece->isAlive)
        return legal;
    vector<Square> pseudo = piece->getMoves(cells, piece->x, piece->y);
    int color = cells[piece->x][piece->y].occupied_color;
    for (int i = 0; i < pseudo.size(); i++)
    {
        Square dest = pseudo[i];
        Square temp[8][8];
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                temp[r][c] = cells[r][c];
        int a = piece->x, b = piece->y;
        temp[dest.x][dest.y].occupied_value = temp[a][b].occupied_value;
        temp[dest.x][dest.y].occupied_color = temp[a][b].occupied_color;
        temp[a][b].occupied_value = 0;
        temp[a][b].occupied_color = 0;
        int kx, ky;
        if (piece->occupied_value == 3)
        {
            kx = dest.x;
            ky = dest.y;
        }
        else
        {
            Pieces *king = (color == 1) ? (Pieces *)w_king : (Pieces *)b_king;
            kx = king->x;
            ky = king->y;
        }
        if (!w_king->isSquareAttacked(temp, kx, ky, -color))
            legal.push_back(dest);
    }
    return legal;
}

void Game::updateGameStatus()
{
    int color = whiteTurn ? 1 : -1;
    Pieces *king = whiteTurn ? (Pieces *)w_king : (Pieces *)b_king;
    bool inCheck = w_king->isSquareAttacked(cells, king->x, king->y, -color);
    vector<Pieces *> &side = whiteTurn ? whitePieces : blackPieces;
    bool hasMove = false;
    for (int i = 0; i < side.size() && !hasMove; i++)
    {
        if (!side[i]->isAlive)
            continue;
        if (!getLegalMoves(side[i]).empty())
            hasMove = true;
    }
    if (!hasMove)
    {
        isOver = true;
        if (inCheck)
            situation.setString(whiteTurn ? "CHECKMATE! \nBlack Won!!" : "CHECKMATE! \nWhite Won!!");
        else
            situation.setString("STALEMATE! \nDraw");
    }
    else if (inCheck)
    {
        situation.setString("CHECK!");
    }
    else
    {
        situation.setString("");
    }
}

void Game::AppendMoveSuffix()
{
    if (moveHistory.empty())
        return;
    std::string s = situation.getString().toAnsiString();
    if (s.find("CHECKMATE") != std::string::npos)
        moveHistory.back() += "#";
    else if (s.find("CHECK") != std::string::npos)
        moveHistory.back() += "+";
}

bool Game::isAwaitingPromotion()
{
    return awaitingPromotion;
}

bool Game::HandlePromotionClick(int mouseX, int mouseY)
{
    if (!awaitingPromotion)
        return false;
    for (int i = 0; i < 4; i++)
    {
        sf::FloatRect b = promoBoxes[i].getGlobalBounds();
        if (b.contains((float)mouseX, (float)mouseY))
        {
            ResolvePromotion(i);
            break;
        }
    }
    // Any click while the picker is open is consumed by it - the board underneath
    // stays frozen until a choice is made.
    return true;
}

void Game::ResolvePromotion(int choice)
{
    if (!awaitingPromotion)
        return;
    Pieces *chosen = promoChoices[choice];
    for (int i = 0; i < 4; i++)
    {
        if (i != choice)
            delete promoChoices[i];
        promoChoices[i] = NULL;
    }

    pendingPawn->isAlive = false;
    cells[promotionX][promotionY].occupied_value = chosen->occupied_value;

    if (promotionColor == 1)
        whitePieces.push_back(chosen);
    else
        blackPieces.push_back(chosen);

    char letter = 'Q';
    if (chosen->occupied_value == 1) letter = 'R';
    else if (chosen->occupied_value == -2) letter = 'B';
    else if (chosen->occupied_value == -1) letter = 'N';
    moveHistory.push_back(pendingNotation + "=" + letter);

    awaitingPromotion = false;
    whiteTurn = !whiteTurn;
    SetRightSideofWindow();
    updateGameStatus();
    AppendMoveSuffix();

    selected_piece = NULL;
    selected = false;
}
