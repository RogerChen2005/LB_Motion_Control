# Adds motion, UI, and official display drivers and emits flashable firmware images.
target_compile_definitions(stm32cubemx INTERFACE
    HAL_TIM_MODULE_ENABLED HAL_SRAM_MODULE_ENABLED HAL_UART_MODULE_ENABLED)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    App/Src/board_clock.c
    App/Src/board_pins.c
    App/Src/control.c
    App/Src/display_panel.c
    App/Src/keypad.c
    App/Src/lb_app.c
    App/Src/motion_pwm.c
    App/Src/serial_diag.c
    App/Src/touch_input.c
    App/Src/ui.c
    App/Port/SYSTEM/delay/delay.c
    Drivers/BSP/ATK_MD0280/atk_md0280.c
    Drivers/BSP/ATK_MD0280/atk_md0280_fsmc.c
    Drivers/BSP/ATK_MD0280/atk_md0280_touch.c
    Drivers/BSP/ATK_MD0280/atk_md0280_touch_spi.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE App/Inc App/Port Drivers)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/LB.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/LB.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)
