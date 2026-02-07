#ifndef GAME_ENHANCER_HPP
#define GAME_ENHANCER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <functional>

using namespace std;

// Використовуємо відносний шлях, оскільки файл arial.ttf лежить у папці з проектом
const string FONT_PATH2 = "";

class GameEnhancer {
private:
    vector<string> moveHistory;
    chrono::time_point<chrono::steady_clock> whiteStartTime;
    chrono::time_point<chrono::steady_clock> blackStartTime;

    float whiteElapsed = 0.0f;
    float blackElapsed = 0.0f;
    bool isWhiteTurn = true;

    sf::Font font;
    sf::Text whiteTimerText;
    sf::Text blackTimerText;
    sf::Text historyText;

    std::function<void()> restartCallback;

    // --- НОВІ ЗМІННІ ДЛЯ ПРОКРУТКИ ---
    float scrollOffset = 0.0f;
    float LINE_HEIGHT = 24.0f;
    float VIEW_HEIGHT = 500.0f;

public:
    bool gameOverDueToTime = false;
    bool timeAlertShown = false;

    GameEnhancer() {
        // Завантажуємо шрифт (має лежати поруч з .exe або в робочій директорії)
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Failed to load font arial.ttf\n";
        }

        whiteTimerText.setFont(font);
        blackTimerText.setFont(font);
        historyText.setFont(font);

        whiteTimerText.setCharacterSize(22);
        blackTimerText.setCharacterSize(22);
        historyText.setCharacterSize(18);

        whiteTimerText.setFillColor(sf::Color::White);
        blackTimerText.setFillColor(sf::Color::White);
        historyText.setFillColor(sf::Color(200, 200, 200)); // Світло-сірий для історії

        whiteTimerText.setPosition(820, 20);
        blackTimerText.setPosition(820, 60);

        // Початкова позиція тексту історії (буде змінюватися через View)
        historyText.setPosition(820, 0);

        whiteStartTime = chrono::steady_clock::now();
    }

    void setRestartCallback(const std::function<void()>& callback) {
        restartCallback = callback;
    }

    // Метод для обробки коліщатка миші
    void handleScroll(float delta) {
        float totalHeight = (moveHistory.size() + 2) * LINE_HEIGHT;

        // Прокручуємо тільки якщо ходів більше, ніж влазить в екран
        if (totalHeight > VIEW_HEIGHT) {
            scrollOffset -= delta * 20.0f; // 20.0f - швидкість

            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > totalHeight - VIEW_HEIGHT)
                scrollOffset = totalHeight - VIEW_HEIGHT;
        }
    }

    void recordMove(int fromX, int fromY, int toX, int toY) {
        string move = string(1, 'a' + fromX) + to_string(8 - fromY) + " -> " +
                      string(1, 'a' + toX) + to_string(8 - toY);
        moveHistory.push_back(move);

        auto now = chrono::steady_clock::now();
        if (isWhiteTurn) {
            whiteElapsed += chrono::duration<float>(now - whiteStartTime).count();
            blackStartTime = now;
        } else {
            blackElapsed += chrono::duration<float>(now - blackStartTime).count();
            whiteStartTime = now;
        }
        isWhiteTurn = !isWhiteTurn;

        // Автоматично прокручуємо вниз при новому ході
        float totalHeight = (moveHistory.size() + 2) * LINE_HEIGHT;
        if (totalHeight > VIEW_HEIGHT) {
            scrollOffset = totalHeight - VIEW_HEIGHT;
        }
    }

    void drawExtras(sf::RenderWindow& window) {
    // 1. Спочатку малюємо таймери у звичайному режимі (Default View)
    float wTime = whiteElapsed;
    float bTime = blackElapsed;
    auto now = chrono::steady_clock::now();
    if (isWhiteTurn) wTime += chrono::duration<float>(now - whiteStartTime).count();
    else bTime += chrono::duration<float>(now - blackStartTime).count();

    if ((wTime > 180 || bTime > 180) && !timeAlertShown) {
        gameOverDueToTime = true;
        showTimeOverDialog((wTime > 180) ? "White" : "Black");
        timeAlertShown = true;
    }

    stringstream wss, bss;
    wss << "White Time: " << static_cast<int>(wTime) << "s";
    bss << "Black Time: " << static_cast<int>(bTime) << "s";
    whiteTimerText.setString(wss.str());
    blackTimerText.setString(bss.str());

    window.draw(whiteTimerText);
    window.draw(blackTimerText);

    // --- ЛОГІКА ПРОКРУТКИ ТА ОБРІЗКИ ---

    float winW = (float)window.getSize().x;
    float winH = (float)window.getSize().y;

    // 2. Створюємо View для списку ходів.
    // Координати (0, scrollOffset) означають, що всередині цієї "камери" ми починаємо малювати з нуля.
    sf::View historyView(sf::FloatRect(0, scrollOffset, 350, VIEW_HEIGHT));

    // Встановлюємо в'юпорт: де цей прямокутник буде на реальному екрані.
    // Починаємо з 820 пікселя по ширині (край дошки) та 120 по висоті.
    historyView.setViewport(sf::FloatRect(820.f / winW, 120.f / winH, 350.f / winW, VIEW_HEIGHT / winH));
    window.setView(historyView);

    stringstream hist;
    hist << "Moves History:\n------------------\n";
    for (size_t i = 0; i < moveHistory.size(); ++i) {
        hist << i + 1 << ". " << moveHistory[i] << "\n";
    }

    historyText.setString(hist.str());
    // ВАЖЛИВО: Позиція 0,0 відносно нового View
    historyText.setPosition(0, 0);
    window.draw(historyText);

    // 3. ПОВЕРТАЄМО стандартний View для малювання повзунка
    window.setView(window.getDefaultView());

    // 4. Малюємо повзунок (scrollbar) у звичайних координатах вікна
    float totalHeight = (moveHistory.size() + 2) * LINE_HEIGHT;
    if (totalHeight > VIEW_HEIGHT) {
        // Доріжка
        sf::RectangleShape track(sf::Vector2f(4, VIEW_HEIGHT));
        track.setPosition(1185, 120);
        track.setFillColor(sf::Color(60, 60, 60));
        window.draw(track);

        // Повзунок (світлий прямокутник)
        float barHeight = (VIEW_HEIGHT / totalHeight) * VIEW_HEIGHT;
        float barPos = (scrollOffset / totalHeight) * VIEW_HEIGHT;
        sf::RectangleShape scrollbar(sf::Vector2f(6, barHeight));
        scrollbar.setPosition(1184, 120 + barPos);
        scrollbar.setFillColor(sf::Color(180, 180, 180));
        window.draw(scrollbar);
    }
}

    void showTimeOverDialog(const string& loserColor) {
        sf::RenderWindow timeoutWindow(sf::VideoMode(320, 150), "Time's up!");
        sf::Text message("Time's up! " + loserColor + " lost.", font, 24);
        message.setFillColor(sf::Color::Black);
        message.setPosition(20, 20);

        sf::RectangleShape button(sf::Vector2f(100, 40));
        button.setFillColor(sf::Color::Blue);
        button.setPosition(110, 80);

        sf::Text buttonText("Restart", font, 20);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(122, 88);

        while (timeoutWindow.isOpen()) {
            sf::Event event;
            while (timeoutWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) timeoutWindow.close();
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (button.getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
                        timeoutWindow.close();
                        reset();
                        if (restartCallback) restartCallback();
                    }
                }
            }
            timeoutWindow.clear(sf::Color::White);
            timeoutWindow.draw(message);
            timeoutWindow.draw(button);
            timeoutWindow.draw(buttonText);
            timeoutWindow.display();
        }
    }

    void reset() {
        gameOverDueToTime = false;
        timeAlertShown = false;
        whiteElapsed = 0.0f;
        blackElapsed = 0.0f;
        scrollOffset = 0.0f;
        moveHistory.clear();
        whiteStartTime = chrono::steady_clock::now();
        blackStartTime = chrono::steady_clock::now();
        isWhiteTurn = true;
    }
};

#endif