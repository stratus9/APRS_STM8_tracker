#ifndef __MAIN_H
#define __MAIN_H

/* Private function prototypes -----------------------------------------------*/
void GPIO_Initialization(void);


char tx_buffer_dequeue();
void tx_buffer_queue(char);
void tx_buffer_string(char * c);
char tx_buffer_empty(void);

#endif /* __MAIN_H */