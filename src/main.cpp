#include <stdint.h>
#define GPIOC_BASE 0x40011000UL // GPIOC Base Address (general purpose input-output)
#define GPIOA_BASE 0x40010800UL // GPIOA Base Address (general purpose input-output)
#define ADC1_BASE 0x40012400UL // ADC Base Address (analog to digital)
#define RCC_BASE 0x40021000UL // RCC Base Address (reset and clock control)

void ADC1_init () {
  volatile uint32_t *RCC_APB2ENR = (volatile uint32_t *)(RCC_BASE + 0x18UL); // RCC APB2 Enable Address
  *RCC_APB2ENR |= (1U << 9); // Enables the ADC clock

  volatile uint32_t *ADC1_CR2 = (volatile uint32_t *)(ADC1_BASE + 0x08UL); // ADC1 CR2 Address
  *ADC1_CR2 |= (1U << 0); // Wakes up ADC peripheral

  // Creates a delay for calibration
  volatile uint32_t delay = 8U;
  while (delay > 0U) {
    delay--;
  }

  volatile uint32_t *ADC1_SQR1 = (volatile uint32_t *)(ADC1_BASE + 0x2cUL); // ADC1 SQR1 Address
  *ADC1_SQR1 &= ~(0xFU << 20); // Set amount of conversions to 1

  volatile uint32_t *ADC1_SQR3 = (volatile uint32_t *)(ADC1_BASE + 0x34UL); // ADC1_SQR3 Address
  *ADC1_SQR3 &= ~(0x1FU << 0); // Sets Channel 0 as Rank 1

  volatile uint32_t *ADC1_SMPR2 = (volatile uint32_t *)(ADC1_BASE + 0x10UL); // ADC1_SMPR2 Address
  *ADC1_SMPR2 &= ~(0x7U << 0); // Clear the bits as 0
  *ADC1_SMPR2 |= (0x5U << 0); // Establish a sample time of 55.5 cycles

  *ADC1_CR2 |= (1U << 2); // Start calibration

  // CALIBRATION COMPLETION HANDLING
  while (*ADC1_CR2 & (1U << 2)) { // Wait until bit 2 and 1 = 0
    // Hold on
  }

  // ENABLE PA0 for ADC input
  *RCC_APB2ENR |= (1U << 2); // Enables port A clock

  volatile uint32_t *GPIOA_CRL = (volatile uint32_t *)(GPIOA_BASE + 0x00UL); // GPIOA Config Reg Low
  *GPIOA_CRL &= ~(0xFU << 0); // Sets MODE to input and CNF to analog

  // ENABLE SOFTWARE TRIGGER
  *ADC1_CR2 |= (1U << 20); // Enable external trigger
  *ADC1_CR2 |= (0x7U << 17); // Select software trigger
}

void ADC1_start_conv () {
  volatile uint32_t *ADC1_CR2 = (volatile uint32_t *)(ADC1_BASE + 0x08UL); // ADC1 CR2 address

  *ADC1_CR2 |= (1U << 22); // Start conversion
}

void LED13_off () {
  // LED TEST //
  volatile uint32_t *RCC_APB2ENR = (volatile uint32_t *)(RCC_BASE + 0x18UL); // RCC APB2 Enable Address
  *RCC_APB2ENR |= (1U << 4); // Set bit 4 to 1
  
  volatile uint32_t *GPIOC_CRH = (volatile uint32_t *)(GPIOC_BASE + 0x04UL);// GPIOC Config Reg High
  *GPIOC_CRH &= ~(0xFU << 20); // Set bits 20-23 to 0000
  *GPIOC_CRH |= (0x1U << 20); // Set bits 20-23 to 0001

  volatile uint32_t *GPIOC_ODR = (volatile uint32_t *)(GPIOC_BASE + 0x0CUL);// GPIOC Output Data Reg
  *GPIOC_ODR |= 1U << 13; // Set bit 13 to 1
}

void LED13_on () {
  // LED TEST //
  volatile uint32_t *RCC_APB2ENR = (volatile uint32_t *)(RCC_BASE + 0x18UL); // RCC APB2 Enable Address
  *RCC_APB2ENR |= (1U << 4); // Set bit 4 to 1
  
  volatile uint32_t *GPIOC_CRH = (volatile uint32_t *)(GPIOC_BASE + 0x04UL);// GPIOC Config Reg High
  *GPIOC_CRH &= ~(0xFU << 20); // Set bits 20-23 to 0000
  *GPIOC_CRH |= (0x1U << 20); // Set bits 20-23 to 0001

  volatile uint32_t *GPIOC_ODR = (volatile uint32_t *)(GPIOC_BASE + 0x0CUL);// GPIOC Output Data Reg
  *GPIOC_ODR &= ~(1U << 13); // Set bit 13 to 0
}

int main(void) {

  // LED TEST //

  LED13_off();

  LED13_on();

  // CONVERSION TEST
  volatile uint32_t *ADC1_SR = (volatile uint32_t *)(ADC1_BASE + 0x0UL); // ADC1 SR Address

  ADC1_init();

  ADC1_start_conv();

  while (!(*ADC1_SR & (1U << 1))) {
    // Still converting
  }

  volatile uint32_t *ADC1_DR = (volatile uint32_t *)(ADC1_BASE + 0x4CUL); // ADC1 DR Address
  uint32_t adc_value = *ADC1_DR;
}