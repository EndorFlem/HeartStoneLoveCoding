cmake_minimum_required(VERSION 3.16)
project(heartstone VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Собираем main.cpp и cpp-файлы из src/
file(GLOB PROJECT_SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_executable(student_solution
    ${PROJECT_SOURCE_DIR}/studentSolution/main.cpp
)

# Разрешаем подключения вида #include "hero.h" для заголовков из include/
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(student_solution
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Опционально: включаем стандартные предупреждения для удобства разработки
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
    target_compile_options(student_solution PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(student_solution PRIVATE -Wall -Wextra -Wpedantic)
endif()
