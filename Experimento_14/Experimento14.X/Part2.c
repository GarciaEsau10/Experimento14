#include <xc.h> //PIC hardware mapping

#define _XTAL_FREQ 4000000

// No definas otra función main() si ya existe en otro archivo
void PWM_Update(unsigned int DutyCycle)
{
    CCPR2L = (unsigned char)(DutyCycle >> 2);        // top 8 bits
    CCP2CONbits.CCP2Y = DutyCycle & 0x01;            // 1st LSB
    CCP2CONbits.CCP2X = DutyCycle & 0x02;            // 2nd LSB
}

void main() {
    unsigned int DutyCycle = 0;

    TRISC = 0b00000000;
    PORTC = 0b00000000;

    PR2 = 0x3F;
    T2CON = 0b00000100;
    CCP2CON = 0x0F;

    do {
        while (DutyCycle < 1023) {
            PWM_Update(DutyCycle);
            __delay_us(10000);
            DutyCycle += 5;

            if (DutyCycle > 1023) {
                DutyCycle = 0;
                PWM_Update(DutyCycle);
                __delay_us(5000);
            }
        }
    } while (1);
}
