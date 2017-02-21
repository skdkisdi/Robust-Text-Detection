cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Robust_Text_Detection)


#find_package(PkgConfig REQUIRED)

#pkg_check_modules(OpenCV REQUIRED opencv)

set(CMAKE_PREFIX_PATH "/usr/local/opencv320/")

find_package(OpenCV 3.2.0 EXACT REQUIRED)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} include)
#message(${OpenCV_INCLUDE_DIRS})

AUX_SOURCE_DIRECTORY(src DIR_SRCS)  

#SET(CMAKE_C_COMPILER g++)
#SET(CMAK_CXX_COMPILER g++)
add_compile_options(-std=c++11)

add_executable(${PROJECT_NAME} ${DIR_SRCS} )
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS} tesseract)
