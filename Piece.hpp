#ifndef PIECES_HPP
#define PIECES_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
// Base class for all chess pieces.
const string FIGURE_PATH = R"(C:\project\chess\figures\)";
class Piece {
public:
    enum class Color { White, Black };

    Piece(Color color) : color(color) {}
    virtual ~Piece() {}

    // Returns the valid moves for this piece on the board.
    // The board is an 8x8 vector of Piece pointers.
    // (x,y) is the current board position.
    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const = 0;

    // Draw the piece onto the given SFML window.
    // Each board square is assumed to be 100x100 pixels.
    virtual void draw(sf::RenderWindow &window, int x, int y) const = 0;

    Color getColor() const { return color; }

protected:
    Color color;
};

//
// Pawn class
//
class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;
        int direction = (color == Color::White) ? -1 : 1;
        int startRow = (color == Color::White) ? 6 : 1;
        int newY = y + direction;
        // Single move forward.
        if (newY >= 0 && newY < 8 && board[newY][x] == nullptr)
            moves.push_back(sf::Vector2i(x, newY));
        // Double move from starting position.
        if (y == startRow && board[newY][x] == nullptr && board[newY + direction][x] == nullptr)
            moves.push_back(sf::Vector2i(x, newY + direction));
        // Diagonal captures.
        if (x - 1 >= 0 && newY >= 0 && newY < 8 &&
            board[newY][x - 1] != nullptr && board[newY][x - 1]->getColor() != color)
            moves.push_back(sf::Vector2i(x - 1, newY));
        if (x + 1 < 8 && newY >= 0 && newY < 8 &&
            board[newY][x + 1] != nullptr && board[newY][x + 1]->getColor() != color)
            moves.push_back(sf::Vector2i(x + 1, newY));
        return moves;
    }

    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        // Use two static textures, one for white and one for black.
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wP.png"))
                    cerr << "Error loading wP.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH +"bP.png"))
                    cerr << "Error loading bP.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

//
// Rook class
//
class Rook : public Piece {
public:
    bool hasMoved;
    Rook(Color color) : Piece(color), hasMoved(false) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;
        // Left
        for (int i = x - 1; i >= 0; i--) {
            if (board[y][i] == nullptr)
                moves.push_back(sf::Vector2i(i, y));
            else {
                if (board[y][i]->getColor() != color)
                    moves.push_back(sf::Vector2i(i, y));
                break;
            }
        }
        // Right
        for (int i = x + 1; i < 8; i++) {
            if (board[y][i] == nullptr)
                moves.push_back(sf::Vector2i(i, y));
            else {
                if (board[y][i]->getColor() != color)
                    moves.push_back(sf::Vector2i(i, y));
                break;
            }
        }
        // Up
        for (int j = y - 1; j >= 0; j--) {
            if (board[j][x] == nullptr)
                moves.push_back(sf::Vector2i(x, j));
            else {
                if (board[j][x]->getColor() != color)
                    moves.push_back(sf::Vector2i(x, j));
                break;
            }
        }
        // Down
        for (int j = y + 1; j < 8; j++) {
            if (board[j][x] == nullptr)
                moves.push_back(sf::Vector2i(x, j));
            else {
                if (board[j][x]->getColor() != color)
                    moves.push_back(sf::Vector2i(x, j));
                break;
            }
        }
        return moves;
    }

    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wR.png"))
                    cerr << "Error loading wR.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH + "bR.png"))
                    cerr << "Error loading bR.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

//
// Knight class
//
class Knight : public Piece {
public:
    Knight(Color color) : Piece(color) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;
        int offsets[8][2] = { {2,1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {-1,-2}, {1,-2}, {2,-1} };
        for (int i = 0; i < 8; i++) {
            int newX = x + offsets[i][0];
            int newY = y + offsets[i][1];
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                if (board[newY][newX] == nullptr || board[newY][newX]->getColor() != color)
                    moves.push_back(sf::Vector2i(newX, newY));
        }
        return moves;
    }

    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wN.png"))
                    cerr << "Error loading wN.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH + "bN.png"))
                    cerr << "Error loading bN.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

//
// Bishop class
//
class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;
        // Diagonally top-right.
        for (int d = 1; x + d < 8 && y - d >= 0; d++) {
            if (board[y - d][x + d] == nullptr)
                moves.push_back(sf::Vector2i(x + d, y - d));
            else {
                if (board[y - d][x + d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x + d, y - d));
                break;
            }
        }
        // Diagonally top-left.
        for (int d = 1; x - d >= 0 && y - d >= 0; d++) {
            if (board[y - d][x - d] == nullptr)
                moves.push_back(sf::Vector2i(x - d, y - d));
            else {
                if (board[y - d][x - d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x - d, y - d));
                break;
            }
        }
        // Diagonally bottom-right.
        for (int d = 1; x + d < 8 && y + d < 8; d++) {
            if (board[y + d][x + d] == nullptr)
                moves.push_back(sf::Vector2i(x + d, y + d));
            else {
                if (board[y + d][x + d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x + d, y + d));
                break;
            }
        }
        // Diagonally bottom-left.
        for (int d = 1; x - d >= 0 && y + d < 8; d++) {
            if (board[y + d][x - d] == nullptr)
                moves.push_back(sf::Vector2i(x - d, y + d));
            else {
                if (board[y + d][x - d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x - d, y + d));
                break;
            }
        }
        return moves;
    }

    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wB.png"))
                    cerr << "Error loading wB.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH + "bB.png"))
                    cerr << "Error loading bB.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

//
// Queen class
//
class Queen : public Piece {
public:
    Queen(Color color) : Piece(color) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;
        // Combine Rook and Bishop moves.

        // Rook-like: horizontal and vertical.
        // Left.
        for (int i = x - 1; i >= 0; i--) {
            if (board[y][i] == nullptr)
                moves.push_back(sf::Vector2i(i, y));
            else {
                if (board[y][i]->getColor() != color)
                    moves.push_back(sf::Vector2i(i, y));
                break;
            }
        }
        // Right.
        for (int i = x + 1; i < 8; i++) {
            if (board[y][i] == nullptr)
                moves.push_back(sf::Vector2i(i, y));
            else {
                if (board[y][i]->getColor() != color)
                    moves.push_back(sf::Vector2i(i, y));
                break;
            }
        }
        // Up.
        for (int j = y - 1; j >= 0; j--) {
            if (board[j][x] == nullptr)
                moves.push_back(sf::Vector2i(x, j));
            else {
                if (board[j][x]->getColor() != color)
                    moves.push_back(sf::Vector2i(x, j));
                break;
            }
        }
        // Down.
        for (int j = y + 1; j < 8; j++) {
            if (board[j][x] == nullptr)
                moves.push_back(sf::Vector2i(x, j));
            else {
                if (board[j][x]->getColor() != color)
                    moves.push_back(sf::Vector2i(x, j));
                break;
            }
        }

        // Bishop-like: diagonals.
        // Top-right.
        for (int d = 1; x + d < 8 && y - d >= 0; d++) {
            if (board[y - d][x + d] == nullptr)
                moves.push_back(sf::Vector2i(x + d, y - d));
            else {
                if (board[y - d][x + d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x + d, y - d));
                break;
            }
        }
        // Top-left.
        for (int d = 1; x - d >= 0 && y - d >= 0; d++) {
            if (board[y - d][x - d] == nullptr)
                moves.push_back(sf::Vector2i(x - d, y - d));
            else {
                if (board[y - d][x - d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x - d, y - d));
                break;
            }
        }
        // Bottom-right.
        for (int d = 1; x + d < 8 && y + d < 8; d++) {
            if (board[y + d][x + d] == nullptr)
                moves.push_back(sf::Vector2i(x + d, y + d));
            else {
                if (board[y + d][x + d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x + d, y + d));
                break;
            }
        }
        // Bottom-left.
        for (int d = 1; x - d >= 0 && y + d < 8; d++) {
            if (board[y + d][x - d] == nullptr)
                moves.push_back(sf::Vector2i(x - d, y + d));
            else {
                if (board[y + d][x - d]->getColor() != color)
                    moves.push_back(sf::Vector2i(x - d, y + d));
                break;
            }
        }
        return moves;
    }

    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wQ.png"))
                    cerr << "Error loading wQ.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH + "bQ.png"))
                    cerr << "Error loading bQ.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

//
// King class
//
class King : public Piece {
public:
    bool hasMoved;
    King(Color color) : Piece(color), hasMoved(false) {}

    virtual vector<sf::Vector2i> getValidMoves(const vector<vector<Piece*>> &board, int x, int y) const override {
        vector<sf::Vector2i> moves;

        // Normal king moves - one square in all directions
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;
                int newX = x + dx, newY = y + dy;
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (board[newY][newX] == nullptr || board[newY][newX]->getColor() != color)
                        moves.push_back(sf::Vector2i(newX, newY));
                }
            }
        }

        // Castling
        if (!hasMoved) {
            // Kingside castling
            if (x + 3 < 8) {
                Piece* rook = board[y][x + 3];
                if (rook != nullptr && dynamic_cast<Rook*>(rook) && !dynamic_cast<Rook*>(rook)->hasMoved) {
                    if (board[y][x + 1] == nullptr && board[y][x + 2] == nullptr) {
                        moves.push_back(sf::Vector2i(x + 2, y)); // Castling to the right side
                    }
                }
            }

            // Queenside castling
            if (x - 4 >= 0) {
                Piece* rook = board[y][x - 4];
                if (rook != nullptr && dynamic_cast<Rook*>(rook) && !dynamic_cast<Rook*>(rook)->hasMoved) {
                    if (board[y][x - 1] == nullptr && board[y][x - 2] == nullptr && board[y][x - 3] == nullptr) {
                        moves.push_back(sf::Vector2i(x - 2, y)); // Castling to the left side
                    }
                }
            }
        }

        return moves;
    }


    virtual void draw(sf::RenderWindow &window, int x, int y) const override {
        static sf::Texture whiteTexture;
        static sf::Texture blackTexture;
        static bool whiteLoaded = false;
        static bool blackLoaded = false;
        sf::Texture* texture = nullptr;
        if (color == Color::White) {
            if (!whiteLoaded) {
                if (!whiteLoaded && !whiteTexture.loadFromFile(FIGURE_PATH + "wK.png"))
                    cerr << "Error loading wK.png\n";
                whiteLoaded = true;
            }
            texture = &whiteTexture;
        } else {
            if (!blackLoaded) {
                if (!blackTexture.loadFromFile(FIGURE_PATH + "bK.png"))
                    cerr << "Error loading bK.png\n";
                blackLoaded = true;
            }
            texture = &blackTexture;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setPosition(x * 100 + 50, y * 100 + 50);
        sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
        window.draw(sprite);
    }
};

#endif // PIECES_HPP
