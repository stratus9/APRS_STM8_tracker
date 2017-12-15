#ifndef __TIMER_H
#define __TIMER_H

void Delay(uint32_t);
void Timer1_Init();
void Timer1_1200Hz();
void Timer1_2200Hz();
void Timer2_Init();
void Timer2_ISR();

#endif