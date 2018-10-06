/*
 */


#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "n5110.h"

int8_t h = 0;
int8_t m = 0;
int8_t s = 0;

void    TIMER2_OVF_vect()
{
    s++;
    if(s>=60)
    {
        s = 0;
        m++;
        if(m>=60)
        {
            m = 0;
            h++;
            if(h>=24)
                h = 0;
        }
    }
    PORTC ^= 1;
    reti();
}

void write_TCNT2 (int8_t a)
{
    while (ASSR & (1<<TCN2UB)); //ожидание разрешения записи регистра TCNT2
    TCNT2 = a;
    return;
}

void write_OCR2 (int8_t b)
{
    while (ASSR & (1<<OCR2UB));
    OCR2 = b;
    return;
}

void write_TCCR2 (int8_t c)
{
    while (ASSR & (1<<TCR2UB));
    TCCR2 = c;
    return;
}

void init_t2 ()
{
    TIMSK &= ~((1<<TOIE2) | (1<<OCIE2));
    ASSR = (1<<AS2);
    write_TCNT2(0);
    write_OCR2(0);
    write_TCCR2(0);
    TIFR |= (1<<TOV2) | (1<<OCF2);
    TIMSK |= (1<<TOIE2) ; //| (1<<OCIE2);

    return;
}

int main(void)
{
    DDRC = 1;
    PORTC = 0;
    Lcd_init();
    init_t2();
    sei();
    write_TCCR2(0b00000101);
    while(1)
    ;

    return 0;
}
