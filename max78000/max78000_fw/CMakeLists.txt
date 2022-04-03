cmake_minimum_required(VERSION 3.12)

set(PRJ_NAME max78000_fw)


project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)


set(EXECUTABLE ${PRJ_NAME}.elf)


file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/bsp/*.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/ap/*.cpp
  src/bsp/cmsis/*.c
  src/common/*.c
  src/hw/*.c  
)

add_executable(${EXECUTABLE} 
  src/main.cpp
  src/bsp/startup/startup_max78000.S

  ${SRC_FILES}
  ${SRC_FILES_RECURSE}


  src/lib/PeriphDrivers/Source/SYS/mxc_assert.c
  src/lib/PeriphDrivers/Source/SYS/mxc_delay.c
  src/lib/PeriphDrivers/Source/SYS/mxc_lock.c
  src/lib/PeriphDrivers/Source/SYS/pins_ai85.c
  src/lib/PeriphDrivers/Source/SYS/sys_ai85.c
  src/lib/PeriphDrivers/Source/SYS/nvic_table.c

  src/lib/PeriphDrivers/Source/ICC/icc_me17.c
  src/lib/PeriphDrivers/Source/ICC/icc_reva.c
 
  src/lib/PeriphDrivers/Source/GPIO/gpio_common.c
  src/lib/PeriphDrivers/Source/GPIO/gpio_ai85.c
  src/lib/PeriphDrivers/Source/GPIO/gpio_reva.c

  src/lib/PeriphDrivers/Source/RTC/rtc_me17.c
  src/lib/PeriphDrivers/Source/RTC/rtc_reva.c  

  src/lib/PeriphDrivers/Source/UART/uart_ai85.c
  src/lib/PeriphDrivers/Source/UART/uart_common.c
  src/lib/PeriphDrivers/Source/UART/uart_revb.c  

)



target_include_directories(${EXECUTABLE} PRIVATE 
  src  
  src/ap
  src/bsp
  src/common 
  src/common/core
  src/common/hw/include 
  src/hw
  src/lib

  src/bsp/cmsis/device/Include
  src/bsp/cmsis/Include
  src/lib/PeriphDrivers/Include
  src/lib/PeriphDrivers/Include/MAX78000
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DTARGET=MAX78000
  -DTARGET_REV=0x4131
  -D_GLIBCXX_INCLUDE_NEXT_C_HEADERS
  -DLFS_THREADSAFE
  )

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m4
  -mthumb  
  -mfloat-abi=softfp
  -mfpu=fpv4-sp-d16
  -Wa,-mimplicit-it=thumb

  -fdata-sections
  -ffunction-sections

  -Wall

  -O0
  )

target_link_options(${EXECUTABLE} PRIVATE
  -T../src/bsp/ldscript/max78000.ld
  -mcpu=cortex-m4
  -mthumb
  -mfpu=fpv4-sp-d16
  -mfloat-abi=softfp
  -specs=nano.specs
  -specs=nosys.specs
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
