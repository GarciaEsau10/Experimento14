#include <xc.h> //PIC hardware mapping

//SET CLOCK FREQUENCY AT 4 MHz
#define _XTAL_FREQ 4000000

// BEGIN CONFIGURATION BITS FOR THIS PROCESSOR

//END CONFIG

void main()
{
    unsigned int DutyCycle = 0;

    TRISC = 0b00000000;             //Setting PORTC as output port
    PORTC = 0b00000000;             //Initializing PORTC

    //TIMER 2 PERIOD REGISTER
    PR2 = 0x3F;                     //Period=0.000064, Freq=15.63kHz. Better

    //TIMER2 CONTROL REGISTER
    //T2CONbits.T2CKPS = 0b00;      //1:1 prescaler
    T2CON = 0b00000100;             //1:1 prescaler

    //CAPTURE/COMPARE/PWM CONTROL REGISTER 2 - setup the PWM
    CCP2CON = 0x0F;

    do {
        while (DutyCycle < 1023)
        {
            //CAPTURE/COMPARE/PWM REGISTER 2
            CCPR2L = (DutyCycle >> 2);        //put the top 8 MSbs into CCPR2L

            //CCP2CONbits.DC2B = DutyCycle & 0x03; //2 LSbs into DC2B register
            CCP2CONbits.CCP2Y = DutyCycle & 0x01;  //1st LSb into CCP2Y register
            CCP2CONbits.CCP2X = DutyCycle & 0x02;  //2nd LSb into CCP2X register

            __delay_us(10000);
            DutyCycle = DutyCycle + 5;

            if (DutyCycle >= 1023)
            {
                DutyCycle = 0;
                CCPR2L = (DutyCycle >> 2);        //put the top 8 MSbs into CCPR2L

                //CCP2CONbits.DC2B = DutyCycle & 0x03; //2 LSbs into DC2B register
                CCP2CONbits.CCP2Y = DutyCycle & 0x01;  //1st LSb into CCP2Y register
                CCP2CONbits.CCP2X = DutyCycle & 0x02;  //2nd LSb into CCP2X register

                __delay_us(5000);
            }
        }
    } while (1);
}
