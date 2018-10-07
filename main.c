/*
часы atmega8
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
    while (ASSR & (1<<OCR2UB)); //ожидание разрешения записи регистра OCR2
    OCR2 = b;
    return;
}

void write_TCCR2 (int8_t c) //ожидание разрешения записи регистра TCCR2
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
    TIMSK |= (1<<TOIE2);

    return;
}

int main(void)
{

    unsigned char p_m = 0, p_h = 0;

    Lcd_init();
    Lcd_clear();
    Lcd_update();

    DDRD = 0;
    PORTD = 0;

    DDRC = 1;
    PORTC = 0;
    init_t2();
    char clock[10];
    sei();
    write_TCCR2(0b00000101);
    while(1)
    {
        Lcd_clear();
        sprintf(clock, "%02d:%02d:%02d",h,m,s);
        Lcd_print(3,2,FONT_1X, (unsigned char*)clock);
        Lcd_update();

        if(PIND&1)
        {
            if(!p_m)
            {
                p_m = 1;
                m++;
            }

        }
            else p_m = 0;

            if(m<0) m = 0;
            if(m>59) m = 0;

            if(PIND&2)
            {
            if(!p_h)
            {
                p_h = 1;
                h++;
            }

            }
            else p_h = 0;

            if(h<0) h = 0;
            if(h>23) h = 0;
    }
    ;

    return 0;
}

