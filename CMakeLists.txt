cmake_minimum_required(VERSION 3.16)

project(GuessNumberGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG        v6.1.9 
)

FetchContent_MakeAvailable(ftxui)

add_executable(guess_number_game main.cpp)

target_link_libraries(guess_number_game PRIVATE ftxui::screen ftxui::dom ftxui::component)
