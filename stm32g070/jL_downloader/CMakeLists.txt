cmake_minimum_required(VERSION 3.12)
set(PRJ_NAME jL_downloader)

project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PROJECT_NAME})

# 해당 폴더의 파일만 찾는다.
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/hw/*.c
  src/hw/driver/*.c
  src/sys/*.c
  src/common/core/*.c
)

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.S
  src/*.c  
  src/*.cpp
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  src/main.cpp
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
  )

target_compile_definitions(${EXECUTABLE} PRIVATE
  )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
  src 
  src/ap
  src/ap/include
  src/sys
  src/hw
  src/common
  src/common/core
  src/common/hw/include
  )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
  
  -fdata-sections
  -ffunction-sections

  -Wall
  -O0
  -g3
  )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
  
  -lm
  )

