cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
# ----this was throwing errors in my local computer, so have commented it out
# include_directories(${CURSES_INCLUDE_DIRS})
include_directories(/usr/local/opt/ncurses/include)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
