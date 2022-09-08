# CMakeList.txt : CMake project for SpaceShooterX, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (SpaceShooterX "src/main.cpp"  "src/animated_object.cpp" "src/animated_object.h" "src/audio_player.cpp" "src/audio_player.h" "src/base_console.cpp" "src/base_console.h" "src/common.cpp" "src/common.h" "src/game.cpp" "src/game.h" "src/game_object.cpp" "src/game_object.h" "src/game_ui.cpp" "src/game_ui.h" "src/player.cpp" "src/player.h" "src/projectile.cpp" "src/projectile.h" "src/random_extension.h" "src/repair_pack.cpp" "src/repair_pack.h" "src/spaceship.cpp" "src/spaceship.h" "src/math/rect.h" "src/math/vector_2.cpp" "src/math/vector_2.h" "src/math/vector_2i.cpp" "src/math/vector_2i.h")

target_link_libraries(SpaceShooterX dsound.lib)

add_compile_definitions(UNICODE)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceShooterX PROPERTY CXX_STANDARD 20)
endif()

file(GLOB ASSET_SOUNDS "assets/*")
file(COPY ${ASSET_SOUNDS} DESTINATION "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/assets/")
