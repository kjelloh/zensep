cmake_minimum_required(VERSION 3.15)
project(zensep CXX)



add_executable(zensep src/zensep.cpp src/main.cpp)



install(TARGETS zensep DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
