#include "clockConfig.h"
#include "stm32f091xc.h"

void clockConfig()
{
    // Enable HSE (external clock)
    RCC->CR |= RCC_CR_HSEON;
    // Wait until the crystal is stable
    while (!(RCC->CR & RCC_CR_HSERDY)) { }
    // Disable PLL
    RCC->CR &= ~RCC_CR_PLLON;
    // Wait until PLL is stopped
    while (RCC->CR & RCC_CR_PLLRDY) { }
    // PLL multiplication factor 6 - 6 * 8 MHz = 48 MHz
    RCC->CFGR |= RCC_CFGR_PLLMUL6;
    // PLL source HSE
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;
    // Use PLL for Sysclock
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    // Wait until PLL is started
    while (!(RCC->CR & RCC_CR_PLLRDY)) { }
    // Update value of SystemCoreClock
    SystemCoreClockUpdate();
}