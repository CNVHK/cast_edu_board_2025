include(FetchContent)

# Requirement for C/C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# App sources
include_directories(App/Inc)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "App/*.*")
target_sources(${PROJECT_NAME}.elf PRIVATE ${APP_SOURCES})

# U8G2 v2.35.7
# FetchContent_Declare(u8g2 GIT_REPOSITORY https://github.com/olikraus/u8g2 GIT_TAG b7a9144f56d113171fc834a376775e238aad8b28)
# FetchContent_MakeAvailable(u8g2)

set(U8G2_LOCAL_PATH "${CMAKE_SOURCE_DIR}/lib/u8g2")

if(NOT EXISTS "${U8G2_LOCAL_PATH}/csrc/u8g2.h")
    message(FATAL_ERROR "u8g2 not found at ${U8G2_LOCAL_PATH}. "
            "Please manually download u8g2 from https://github.com/olikraus/u8g2 "
            "and extract it to this location.")
endif()

include_directories(
        "${U8G2_LOCAL_PATH}/csrc"
)

file(GLOB U8G2_SOURCES
        "${U8G2_LOCAL_PATH}/csrc/*.c"
        "${U8G2_LOCAL_PATH}/csrc/*.h"
)

add_library(u8g2 STATIC ${U8G2_SOURCES})

target_compile_options(u8g2 PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
)

include_directories(App/Inc)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "App/*.*")
target_sources(${PROJECT_NAME}.elf PRIVATE ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME}.elf u8g2)