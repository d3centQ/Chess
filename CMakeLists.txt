cmake_minimum_required(VERSION 3.20)
project(chess)

set(CMAKE_CXX_STANDARD 17)

# Цей блок автоматично завантажить SFML з інтернету при першій компіляції
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(chess main.cpp ChessBoard.cpp )

# Підключаємо модулі SFML до вашої програми
target_link_libraries(chess sfml-graphics sfml-window sfml-system)