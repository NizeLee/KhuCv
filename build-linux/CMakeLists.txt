cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Run)

find_package(wxWidgets COMPONENTS core base xrc aui REQUIRED)
find_package(OpenCV REQUIRED)

if(SDI)
    ADD_DEFINITIONS(-D_KHUCV_SDI)
endif()
ADD_DEFINITIONS(-DUNICODE)

SET (CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")

include(${wxWidgets_USE_FILE})
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROG_SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

