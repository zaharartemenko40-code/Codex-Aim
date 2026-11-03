cmake_minimum_required(VERSION 3.20)
project(CodexAim)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# --- Скачиваем Boost.PFR напрямую с GitHub ---
FetchContent_Declare(
    pfr
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG boost-1.84.0
)
FetchContent_MakeAvailable(pfr)
# ----------------------------------------------

# --- Скачиваем PreLoader SDK ---
FetchContent_Declare(
    preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
)
FetchContent_MakeAvailable(preloader_android)

add_library(CodexAim SHARED src/main.cpp)
# Привязываем библиотеку pfr к нашему модулю
target_link_libraries(CodexAim PRIVATE preloader pfr)