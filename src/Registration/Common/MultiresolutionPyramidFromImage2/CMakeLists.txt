cmake_minimum_required(VERSION 3.10.2)

project(MultiresolutionPyramidFromImage2)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


add_executable(MultiresolutionPyramidFromImage2 Code.cxx)
target_link_libraries(MultiresolutionPyramidFromImage2 ${ITK_LIBRARIES})

install(TARGETS MultiresolutionPyramidFromImage2
  DESTINATION bin/ITKExamples/Registration/Common
  COMPONENT Runtime
  )

install(FILES Code.cxx CMakeLists.txt
  DESTINATION share/ITKExamples/Code/Registration/Common/MultiresolutionPyramidFromImage2/
  COMPONENT Code
  )


enable_testing()
add_test(NAME MultiresolutionPyramidFromImage2Test
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MultiresolutionPyramidFromImage2)

