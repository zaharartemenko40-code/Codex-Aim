cmake_minimum_required(VERSION 3.20)
project(CodexAim)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Скачиваем только нужную библиотеку PFR для Boost
FetchContent_Declare(
    pfr
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG boost-1.84.0
)
FetchContent_MakeAvailable(pfr)

# Скачиваем SDK PreLoader
FetchContent_Declare(
    preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
)
FetchContent_MakeAvailable(preloader_android)

add_library(CodexAim SHARED src/main.cpp)

# Линкуем наш мод с PreLoader и PFR
target_link_libraries(CodexAim PRIVATE preloader)
target_include_directories(CodexAim PRIVATE ${pfr_SOURCE_DIR}/include)