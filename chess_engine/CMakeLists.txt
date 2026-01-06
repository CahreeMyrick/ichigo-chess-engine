cmake_minimum_required(VERSION 3.16)
project(chess_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(chess
	include/types.hpp
	include/move.hpp
	include/movelist.hpp
	include/position.hpp
	include/render.hpp
	include/parse.hpp
	include/rules.hpp
	include/movegen.hpp
	include/gamestate.hpp
	include/perft.hpp
	include/search.hpp
	include/eval.hpp
	src/position.cpp
	src/render.cpp
	src/parse.cpp
	src/rules.cpp
	src/movegen.cpp
	src/gamestate.cpp
	src/perft.cpp
	src/search.cpp
	src/eval.cpp
)

target_include_directories(chess PUBLIC include)

add_executable(ichigo_main src/main.cpp)
target_link_libraries(ichigo_main PRIVATE chess)

add_executable(test_pawn tests/test_pawn.cpp)
target_link_libraries(test_pawn PRIVATE chess)

add_executable(test_knight tests/test_knight.cpp)
target_link_libraries(test_knight PRIVATE chess)

add_executable(test_sliders tests/test_sliders.cpp)
target_link_libraries(test_sliders PRIVATE chess)

add_executable(test_king tests/test_king.cpp)
target_link_libraries(test_king PRIVATE chess)

add_executable(test_check tests/test_check.cpp)
target_link_libraries(test_check PRIVATE chess)

add_executable(test_movegen tests/test_movegen.cpp)
target_link_libraries(test_movegen PRIVATE chess)

add_executable(test_mate_stalemate tests/test_mate_stalemate.cpp)
target_link_libraries(test_mate_stalemate PRIVATE chess)

add_executable(test_castling tests/test_castling.cpp)
target_link_libraries(test_castling PRIVATE chess)

add_executable(test_enpassant tests/test_enpassant.cpp)
target_link_libraries(test_enpassant PRIVATE chess)

add_executable(test_promotion tests/test_promotion.cpp)
target_link_libraries(test_promotion PRIVATE chess)

add_executable(test_perft tests/test_perft.cpp)
target_link_libraries(test_perft PRIVATE chess)

