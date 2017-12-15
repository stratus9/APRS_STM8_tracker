/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include <stdlib.h>
#include "timer.h"

#define AX25_FLAG 0x7E

void Delay(uint32_t nCount){
    /* Decrement nCount value */
    while (nCount != 0){
        nCount--;
    }
}

void Timer1_Init(){
  TIM1_DeInit();
  /* Time Base configuration */
  /*
  TIM1_Period = 4095
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */
  TIM1_TimeBaseInit(1221, TIM1_COUNTERMODE_UP, 5, 0);
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  */
  TIM1_OC3Init(TIM1_OCMODE_PWM1, 
               TIM1_OUTPUTSTATE_ENABLE, 
               TIM1_OUTPUTNSTATE_ENABLE,
               3, 
               TIM1_OCPOLARITY_LOW, 
               TIM1_OCNPOLARITY_HIGH, 
               TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 
  
  //TIM1_CCPreloadControl(ENABLE);
  TIM1_ITConfig(TIM1_IT_COM, DISABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
  
  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void Timer1_1200Hz(){
  uint16_t TIM1_Prescaler = 2221;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer1_2200Hz(){
  uint16_t TIM1_Prescaler = 1221;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer2_Init(){
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_2, 13333);
  
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

  /* TIM1 counter enable */
  TIM2_Cmd(ENABLE);
}

void Timer2_ISR(){
  /* byte to send (LSB first) */
  static unsigned char bits = 0;

  /* number of bits in 'bits' sent */
  static unsigned char bit_count = 0;

  /* count the 1's sent -- used for AX.25 but stuffing */
  static unsigned char ones_count = 0;

  /* Does 'bits' contain the AX.25 Flag? Used to avoid bit stuffing in that case. */
  static unsigned char sending_ax25_flag;

  /* index in the tones array */
  static unsigned char tones_index = 0;

  /* Is the buffer 'bits' empty? If so, get more bits */
  if (bit_count == 0) {

          /* Do we have any bytes that need to be sent? */
          if (tx_buffer_empty()) {

                  /* The buffer is empty. We can stop sending now.
                   * The loop in main() will detect an empty buffer
                   * and disable this interrupt for us.
                   */
                  return;
          }

          bits = tx_buffer_dequeue();
          bit_count = 8;
          sending_ax25_flag = (bits == AX25_FLAG);
  }

  if (!(bits & 0x01)) { /* is current bit 0? */

          /* if the current bit is a 0, then toggle */
          tones_index = !tones_index;
          if(tones_index == 0) Timer1_1200Hz();
          else Timer1_2200Hz();
          ones_count = 0;

          bits >>= 1;
          bit_count--;

  } else if (ones_count++ >= 5 && !sending_ax25_flag) {

          /* bit stuff */
          tones_index = !tones_index;
          if(tones_index == 0) Timer1_1200Hz();
          else Timer1_2200Hz();
          ones_count = 0;

  } else { /* current bit is 1 (don't toggle) */

          bits >>= 1;
          bit_count--;
  }
}