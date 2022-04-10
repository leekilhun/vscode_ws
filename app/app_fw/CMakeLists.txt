cmake_minimum_required(VERSION 3.12)
set(PRJ_NAME stm32g070_app)

project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PRJ_NAME}.elf)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/bsp/*.c
  src/hw/*.c
  src/module/*.c
  src/hw/driver/*.c
  src/module/driver/*.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.cpp
  src/ap/*.cpp
  src/ap/*.c
  src/common/*.c    
  lib/STM32G0xx_HAL_Driver/Src/*.c
  lib/FreeRTOS/*.c
)


add_executable(${EXECUTABLE} 
  src/main.cpp
  src/bsp/startup/startup_stm32g070rbtx.s

  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)


target_include_directories(${EXECUTABLE} PRIVATE 
  src  
  src/ap
  src/bsp
  src/common 
  src/common/core
  src/common/comm
  src/common/inc_hw
  src/common/inc_module 
  src/hw
  src/module
  lib
  
  lib/CMSIS/Include
  lib/CMSIS/Device/ST/STM32G0xx/Include
  lib/CMSIS/Include
  lib/STM32G0xx_HAL_Driver/Inc

  lib/FreeRTOS
  lib/FreeRTOS/Source/CMSIS_RTOS
  lib/FreeRTOS/Source/portable/GCC/ARM_CM0
  lib/FreeRTOS/Source/include

)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32G070xx
  -D_USE_HW_RTOS
  )

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m0plus
  -mthumb
  -mfloat-abi=soft
  -fdata-sections
  -ffunction-sections
  -Wall

  -Os
  )


target_link_options(${EXECUTABLE} PRIVATE
  -T../src/bsp/ldscript/STM32G070RBTX_FLASH.ld
  -mcpu=cortex-m0plus
  -mthumb
  -mfloat-abi=soft
  -specs=nano.specs
  -lc
  -lm
  -lstdc++
  -lsupc++
  -lnosys
  -Wl,-Map=${PRJ_NAME}.map,--cref
  -Wl,--gc-sections
  -Xlinker -print-memory-usage -Xlinker
  )


add_custom_command(TARGET ${EXECUTABLE} 
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
    COMMENT "Invoking: Make Binary"
    )

#set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")
#add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#        COMMAND arm-none-eabi-size --format=berkeley --totals "$<TARGET_FILE:${EXECUTABLE}>"
#        COMMENT "Invoking: Cross ARM GNU Print Size")