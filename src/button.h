#ifndef BUTTON_H
#define BUTTON_H
#define AMOUNT_LINE 4
#define SIZE_BUTTON_STACK 10
#define AMOUNT_KEY 12

void button_init(void);
void button_update(void);
int16_t button_get_key(void);
int16_t button_get_dec(void);
void button_stack_clear(void);

#endif