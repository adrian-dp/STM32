#include <stdint.h>
#define GPIOC_BASE 0x40011000UL // GPIOC Base Address

int main(void) {
  volatile uint32_t *RCC_APB2ENR = (volatile uint32_t *) 0x40021018UL;// I/O Port C Enable address
  *RCC_APB2ENR |= (1U << 4);// Set bit 4 to 1
  
  volatile uint32_t *GPIOC_CRH = (volatile uint32_t *)(GPIOC_BASE + 0x04UL);// GPIOC Config Reg High
  *GPIOC_CRH &= ~(0xFU << 20);// Set bits 20-23 to 0000
  *GPIOC_CRH |= (0x1U << 20);// Set bits 20-23 to 0001

  volatile uint32_t *GPIOC_ODR = (volatile uint32_t *)(GPIOC_BASE + 0x0CUL);// GPIOC Output Data Reg
  *GPIOC_ODR &= ~(1U << 13);// Set bit 13 to 0
  //*GPIOC_ODR |= 1U << 13;// Set bit 13 to 1

  while (1) {
    //lol
  }
}