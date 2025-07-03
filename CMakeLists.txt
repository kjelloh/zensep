cmake_minimum_required(VERSION 3.15)
project(zensep VERSION 0.0 LANGUAGES CXX)

# Get git version information
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NOT GIT_VERSION)
        set(GIT_VERSION "unknown")
    endif()
else()
    set(GIT_VERSION "no-git")
endif()

find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)

add_executable(zensep src/zensep.cpp src/main.cpp)

# Pass version information as compile definitions
target_compile_definitions(zensep PRIVATE 
    ZENSEP_VERSION="${PROJECT_VERSION}"
    ZENSEP_GIT_VERSION="${GIT_VERSION}"
)

target_link_libraries(zensep 
    spdlog::spdlog
    CLI11::CLI11
)



install(TARGETS zensep DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
