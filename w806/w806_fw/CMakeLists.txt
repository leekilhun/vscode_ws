cmake_minimum_required(VERSION 3.0.0)

#set(PRJ_NAME w806_fw)

project(w806_fw
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PROJECT_NAME}.elf)

# 해당 폴더의 파일만 찾는다.
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  *.cpp
)

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.S
  src/*.c  
  src/*.cpp  
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
  )

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DTLS_CONFIG_CPU_XT804=1 
  -DGCC_COMPILE=1 
  )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
  src 
  src/ap
  src/bsp
  src/hw
  src/common
  src/common/core
  src/common/hw/include

  src/lib/w806/include
  src/lib/w806/include/driver
  src/lib/w806/include/arch/xt804
  src/lib/w806/include/arch/xt804/csi_core
  src/lib/w806/include/arch/xt804/csi_dsp
  )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=ck804ef
  
  -fdata-sections
  -ffunction-sections
  -MMD
  -mhard-float

  -Wall
  -O0
  -g3
  )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE  
  -mcpu=ck804ef
  -mhard-float
  -nostartfiles

  -lm
  -Wl,-ckmap=${PROJECT_NAME}.map
  -Wl,--gc-sections
  -Xlinker -print-memory-usage -Xlinker

  -Wl,-zmax-page-size=1024
  -Wl,-T../src/bsp/ldscript/gcc_csky.ld
  )


add_custom_command(TARGET ${EXECUTABLE} 
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
  COMMENT "Invoking: Make Binary"
  )

add_custom_command(TARGET ${EXECUTABLE} 
  POST_BUILD
  COMMAND ../tools/wm_tool.exe ARGS -b ${PROJECT_NAME}.bin -o ${PROJECT_NAME} -it 1 -fc 0 -ra 8010400 -ih 8010000 -ua 8010000 -nh 0  -un 0
  COMMAND ../tools/wm_tool.exe ARGS -b ../tools/W806_secboot.bin -o W806_secboot -it 0 -fc 0 -ra 8002400 -ih 8002000 -ua 8010000 -nh 8010000  -un 0    
  COMMAND copy ARGS /y /b W806_secboot.img + ${PROJECT_NAME}.img ${PROJECT_NAME}.fls
  COMMENT "Invoking: Make Image"
  )


