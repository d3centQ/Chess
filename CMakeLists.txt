cmake_minimum_required(VERSION 3.20)
project(chess)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(chess main.cpp ChessBoard.cpp )


target_link_libraries(chess sfml-graphics sfml-window sfml-system)
if (WIN32)
    add_custom_command(TARGET chess POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:chess>
            $<TARGET_FILE_DIR:chess>
            COMMAND_EXPAND_LISTS
    )
endif()