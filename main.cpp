#include <SFML/Graphics.hpp>
#include "ChessBoard.hpp"
#include "GameEnhancer.hpp"
using namespace sf;

int main() {
    // Створюємо звичайне вікно (не Fullscreen)
    // 1200x800 - вистачить для дошки (800x800) та панелі (400 пікселів справа)
    RenderWindow window(VideoMode(1200, 800), "Chess Game", Style::Titlebar | Style::Close);

    ChessBoard chessBoard;

    chessBoard.getEnhancer().setRestartCallback([&chessBoard]() {
        chessBoard.getEnhancer().reset();
        chessBoard.initBoard();
    });

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // Прокрутка працюватиме і у вікні
            if (event.type == sf::Event::MouseWheelScrolled) {
                chessBoard.getEnhancer().handleScroll(event.mouseWheelScroll.delta);
            }

            if (event.type == Event::Closed)
                window.close();

            chessBoard.handleEvent(event);
        }

        window.clear();
        chessBoard.draw(window); // Тут малюється і дошка, і enhancer.drawExtras всередині
        window.display();
    }

    return 0;
}