cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(egadslitecuda LANGUAGES CXX C CUDA)

include(GNUInstallDirs)
 
include(CTest)

set(EG_HEADERS
  include/egads.h
  include/egadsTypes.h
  include/egadsErrors.h
)

set(EG_SOURCES 
  lite/liteBase.c
  lite/liteMemory.c
  lite/liteGeom.c
  lite/liteTopo.c
  lite/liteAttrs.c
  lite/liteImport.c
  lite/liteString.c
  lite/liteDevice.c
  src/egadsQuads.c
  src/egadsRobust.c
)

set(EGLITE_SOURCES
  src/egadsTess.c
  src/egadsTris.c 
  src/egadsTessInp.c
  src/egadsEffect.c
  src/egadsTessInp.c
  util/evaluateNR.c
  util/rational.c 
  util/regQuads.c
  lite/liteUVmap.c
)
set_source_files_properties(${EGLITE_SOURCES} PROPERTIES COMPILE_DEFINITIONS LITE)

set(EMP_SOURCE util/emp.c)
set_source_files_properties(${EMP_SOURCE} PROPERTIES COMPILE_DEFINITIONS LINUX64_cuda)

set(ALL_SOURCES ${EG_SOURCES} ${EGLITE_SOURCES} ${EMP_SOURCE})

set_source_files_properties(${ALL_SOURCES} PROPERTIES LANGUAGE CUDA)

add_library(egadslite ${ALL_SOURCES})
target_include_directories(egadslite PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/lite
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/util
  ${CMAKE_CURRENT_SOURCE_DIR}/util/uvmap)
target_include_directories(egadslite PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
 
# We need to explicitly state that we need all CUDA files in the 
# particle library to be built with -dc as the member functions 
# could be called by other libraries and executables
set_target_properties(egadslite PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_compile_definitions(egadslite PUBLIC REVISION=7.5)
 
add_executable(liteTestCuda lite/liteTestCuda.c)
set_target_properties(liteTestCuda PROPERTIES LANGUAGE CUDA LINKER_LANGUAGE CUDA)
target_link_libraries(liteTestCuda PRIVATE egadslite)

install(FILES ${EG_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS egadslite LIBRARY)
install(TARGETS liteTestCuda RUNTIME)
