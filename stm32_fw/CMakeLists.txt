cmake_minimum_required(VERSION 3.0.0)


project(stm32_fw 
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/src/STM32F103RCTX_FLASH.ld)

# 해당 폴더의 파일만 찾는다.
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/*.c
)

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/Core/*.c
  src/Drivers/*.c
  src/Middlewares/*.c #ST/STM32_USB_Device_Library/Class/CDC/Src/
  src/USB_DEVICE/*.c
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}

  src/Core/Startup/startup_stm32f103rctx.s
  )

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32F103xE
  )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
  src 
  src/Core/Inc
  src/Drivers/CMSIS/Device/ST/STM32F1xx/Include
  src/Drivers/CMSIS/Include
  src/Drivers/STM32F1xx_HAL_Driver/Inc
  src/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
  src/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
  src/USB_DEVICE/App
  src/USB_DEVICE/Target

  )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m3
  -mthumb
  
  #-mfpu=fpv4-sp-d16
  #-mfloat-abi=hard

  -fdata-sections
  -ffunction-sections

  -Wall
  -O0
  -g3
  )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
  -T${LINKER_FILE}
  -mcpu=cortex-m3
  -mthumb
  #-mfpu=fpv4-sp-d16
  #-mfloat-abi=hard
  -specs=nano.specs
  -lc
  -lm
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
  -Xlinker -print-memory-usage -Xlinker
  )