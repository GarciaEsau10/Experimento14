#include <xc.h> //PIC hardware mapping

//SET CLOCK FREQUENCY AT 4 MHz
#define _XTAL_FREQ 4000000

// BEGIN CONFIGURATION BITS FOR THIS PROCESSOR
//END CONFIG

//READ ADC
unsigned int ADC_Read()
{
    ADCON0bits.ADCS = 0;         //Selecting the clk division factor = FOSC/2
    //ADCON1bits.ADCS2 = 0;      // (No existe en PIC16F887)
    ADCON1bits.ADFM = 1;         //Result right justified
    //ADCON1bits.PCFG = 0;       // (PCFG no existe como miembro de ADCON1 en PIC16F887)
    ADCON0bits.ADON = 1;         //Turns on ADC module
    //ADCON0bits.CHS = 0;        //Selects channel 0 (ADCON0bits.CHSAN0)
    //ADCON0bits.CHS = 0b000;    //Selects channel 0 (ADCON0bits.CHSAN0)
    ADCON0bits.CHS = 0b011;      //Selects channel 3 (ADCON0bits.CHSAN3)

    __delay_us(25);              //Waits for the acquisition to complete
    ADCON0bits.GO = 1;           //Starts ADC conversion

    while (ADCON0bits.nDONE) continue; //wait till ADC conversion is over
    return ((int)ADRESH << 8) | ADRESL; //Merging the MSB and LSB
    //ADCResult = (ADRESH<<8) + ADRESL; //Merging the MSB and LSB
}

void main()
{
    unsigned short ADCResult;

    TRISC = 0b00000000;        //Setting PORTC as output port
    PORTC = 0b00000000;        //Initializing PORTC
    //setup ADC

    //TIMER 2 PERIOD REGISTER
    //PR2 = 0xFF;              //Period=0.000226, Freq=4.42kHz. Too slow
    PR2 = 0x3F;                //Period=0.000064, Freq=15.63kHz. Better
    //PR2 = 0x1F;              //Period=0.000032, Freq=31.25kHz. Good
    //PR2 = 0x17;              //Period=0.000024, Freq=41.67kHz. Too noisy
    //PWM Period = [PR2+1]*4*Tosc*T2CKPS = [255+1]*4*(1/4000000Hz)*1

    //T2CONbits.T2CKPS = 0b00; //1:1 prescaler
    T2CON = 0b00000100;
    //T2CONbits.T2CKPS = 0b01; //1:4 prescaler
    //T2CONbits.T2CKPS = 0b11; //1:16 prescaler
    //T2CONbits.TMR2ON = 1;    //start the PWM

    //CAPTURE/COMPARE/PWM CONTROL REGISTER 2 //setup the PWM
    //CCP2CON = 0b00001111;   //PWM mode single output
    CCP2CON = 0x0F;

    while (1) {
        ADCResult = ADC_Read();
        CCPR2L = (ADCResult >> 2); //put the top 8 MSbs into CCPR2L
        CCP2CONbits.CCP2Y = ADCResult & 0x01; //1st LSb into CCP2Y register
        CCP2CONbits.CCP2X = ADCResult & 0x02; //2nd LSb into CCP2X register
        //PORTC = ADCResult >> 4; //GRAB TOP 4 MSBs
    }
}
