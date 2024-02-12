cmake_minimum_required(VERSION 2.8.12)
project(monitor)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wextra -Wall -Werror -ansi -pedantic -pthread --std=c++11")
include_directories(includes libs/c++ /opt/homebrew/opt/ncurses/include)
link_directories(/opt/homebrew/opt/ncurses/lib)
link_libraries(ncurses)

# set(CURSES_NEED_NCURSES TRUE)
# find_package(ncurses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
