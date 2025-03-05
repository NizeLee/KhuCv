cmake_minimum_required(VERSION 3.13)

message("SDI=${SDI}")
if(SDI) 
    message("SDI")
    project(KhuCvS)
else()
    message("MDI")
    project(KhuCv)
endif()

set(
  PROG_SRCS
  ../KhuCvApp/ChildFrame.cpp
  ../KhuCvApp/ClientView.cpp
  ../KhuCvApp/KhuCvApp.cpp
  ../KhuCvApp/MainDialog.cpp
  ../KhuCvApp/MainFrame.cpp
  ../KhuCvApp/Project.cpp
)

option(WX_ROOTDIR "wx root dir")
option(OPENCV_ROOTDIR "opencv root dir")

find_package(OpenCV REQUIRED PATHS ${OPENCV_ROOTDIR}) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Run)

if(SDI)
    ADD_DEFINITIONS(-D_KHUCV_SDI)
endif()

if (WIN32)
 string(APPEND CMAKE_CXX_FLAGS " /W4")
else()
 string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
 string(APPEND CMAKE_C_FLAGS " -Wall -Wextra")
endif()

include_directories("${WX_ROOTDIR}/include" "${WX_ROOTDIR}/include/msvc" ${OpenCV_INCLUDE_DIRS})
link_directories("${WX_ROOTDIR}/lib/vc_ReleaseX64_lib")

add_executable(${PROJECT_NAME} ${PROG_SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")

