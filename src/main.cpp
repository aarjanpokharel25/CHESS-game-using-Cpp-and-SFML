#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
label:
    // 1. Get the primary monitor's absolute native resolution
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // 2. Open the window in true Fullscreen mode
    sf::RenderWindow window(desktopMode, "Chess with SFML", sf::Style::Fullscreen);

    // 3. Create a Virtual Camera (View) matching your original game design size
    sf::View gameView(sf::FloatRect(0.f, 0.f, 1100.f, 800.f));
    window.setView(gameView); // Apply the view to the window

    Game chess(sf::Color(0xf1e8daff), sf::Color(0x64bfa4ff));

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            // Press Escape to exit fullscreen safely
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            if (e.type == sf::Event::MouseButtonPressed)
            {
                if (e.mouseButton.button == sf::Mouse::Left)
                {
                    // 4. Convert fullscreen screen pixels back into the 1100x800 layout coordinates
                    sf::Vector2i pixelPos(e.mouseButton.x, e.mouseButton.y);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    float mouseX = worldPos.x;
                    float mouseY = worldPos.y;

                    if (chess.isAwaitingPromotion())
                    {
                        chess.HandlePromotionClick(static_cast<int>(mouseX), static_cast<int>(mouseY));
                    }
                    else if (mouseX >= 0 && mouseX <= 800 && mouseY >= 0 && mouseY <= 800)
                    {
                        int x = static_cast<int>(mouseY / 100);
                        int y = static_cast<int>(mouseX / 100);

                        if (!chess.getSelected() && chess.isOver == false)
                        {
                            chess.SelectPiece(chess.cells, x, y);
                        }
                        else
                        {
                            chess.moveSelected(chess.cells, x, y);
                        }
                    }

                    // Restart button - jump back to label, fully rebuilding window + Game
                    // (avoids leaking the old Game's pieces via default assignment)
                    if (mouseX >= 850 && mouseX <= 1024 && mouseY >= 5 && mouseY <= 55)
                    {
                        goto label;
                    }
                }
            }
        }

        window.clear();

        // Ensure the view is set right before drawing so everything stretches to fit
        window.setView(gameView);
        window.draw(chess);

        window.display();
    }
    return 0;
}
