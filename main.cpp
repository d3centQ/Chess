#include <SFML/Graphics.hpp>
#include "ChessBoard.hpp"
#include "GameEnhancer.hpp"
using namespace sf;

int main() {


    RenderWindow window(VideoMode(1200, 800), "Chess Game", Style::Titlebar | Style::Close);

    ChessBoard chessBoard;

    chessBoard.getEnhancer().setRestartCallback([&chessBoard]() {
        chessBoard.getEnhancer().reset();
        chessBoard.initBoard();
    });

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::MouseWheelScrolled) {
                chessBoard.getEnhancer().handleScroll(event.mouseWheelScroll.delta);
            }

            if (event.type == Event::Closed)
                window.close();

            chessBoard.handleEvent(event);
        }

        window.clear();
        chessBoard.draw(window);
        window.display();
    }

    return 0;
}