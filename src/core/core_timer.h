#ifndef CORE_TIMER_H
#define CORE_TIMER_H

#ifdef pic12f675_xc8
#define CORETIMER_REQ      ((INTCON&(_INTCON_T0IF_MASK|_INTCON_T0IE_MASK)) == (_INTCON_T0IF_MASK|_INTCON_T0IE_MASK))
void coretimer_handler(void);
#endif

void (*volatile ct_handler0)(void);
void (*volatile ct_handler1)(void);
volatile uint8_t counter0, counter1;

void coretimer_routine1_start(uint8_t period, void (*handler)(void));
void coretimer_routine1_stop(void);


#endif