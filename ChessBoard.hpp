// ChessBoard.hpp
#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Piece.hpp"
#include "GameEnhancer.hpp"


class ChessBoard {
public:
    ChessBoard();
    ~ChessBoard();

    void initBoard();
    void draw(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);
    GameEnhancer& getEnhancer() {
        return enhancer;
    }
    void fullRestart();
    // Other functions used internally:
    std::vector<sf::Vector2i> getValidMoves(int x, int y);
    sf::Vector2i findKing(Piece::Color color);
    bool isCheckmate(Piece::Color color);
    bool isInCheck(Piece::Color color);
    bool wouldBeInCheck(Piece* movingPiece, int fromX, int fromY, int toX, int toY);
    bool willMovePreventCheck(int startX, int startY, int endX, int endY, Piece::Color color);
    void highlightValidMoves(const std::vector<sf::Vector2i>& moves);
    static void drawBoard(sf::RenderWindow &window);
    void drawPieces(sf::RenderWindow &window);
    void drawHints(sf::RenderWindow &window);
    void promotePawnIfNecessary(int y, int x);
    static Piece* choosePromotion(Piece::Color color);
    static Piece* showPromotionDialog(Piece::Color color);
    void handleCheckmate(Piece::Color winningColor);

private:
    std::vector<std::vector<Piece*>> board;// 8x8 board of pointers
    Piece::Color currentPlayer;
    bool pieceSelected;
    sf::Vector2i selectedPiece;
    Piece::Color currentTurn;
    std::vector<sf::CircleShape> moveHints;
    std::vector<sf::CircleShape> captureHints;
    GameEnhancer enhancer;

};

#endif // CHESSBOARD_HPP
