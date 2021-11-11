cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(cmake_and_cuda LANGUAGES CXX CUDA)
 
include(CTest)

set(EG_SOURCES 
  liteBase.c
  liteMemory.c
  liteGeom.c
  liteTopo.c
  liteAttrs.c
  liteImport.c
  liteString.c
  liteDevice.c
  ../src/egadsQuads.c
  ../src/egadsRobust.c
)

set(EGLITE_SOURCES
  ../src/egadsTess.c
  ../src/egadsTris.c 
  ../src/egadsTessInp.c
  ../src/egadsEffect.c
  ../src/egadsTessInp.c
  ../util/evaluateNR.c
  ../util/rational.c 
  ../util/regQuads.c
  liteUVmap.c
)
set_source_files_properties(${EGLITE_SOURCES} PROPERTIES COMPILE_DEFINITIONS LITE)

set(EMP_SOURCE ../util/emp.c)
set_source_files_properties(${EMP_SOURCE} PROPERTIES COMPILE_DEFINITIONS LINUX64_cuda)

set(ALL_SOURCES ${EG_SOURCES} ${EGLITE_SOURCES} ${EMP_SOURCE})

set_source_files_properties(${ALL_SOURCES} PROPERTIES LANGUAGE CUDA)

add_library(egadslite ${ALL_SOURCES})
target_include_directories(egadslite PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${CMAKE_CURRENT_SOURCE_DIR}/../util/
  ${CMAKE_CURRENT_SOURCE_DIR}/../util/uvmap)
 
# We need to explicitly state that we need all CUDA files in the 
# particle library to be built with -dc as the member functions 
# could be called by other libraries and executables
set_target_properties(egadslite PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_compile_definitions(egadslite PUBLIC REVISION=7.5)
 
add_executable(liteTest liteTest.c)
set_target_properties(liteTest PROPERTIES LANGUAGE CXX LINKER_LANGUAGE CXX)
target_link_libraries(liteTest PRIVATE egadslite)
