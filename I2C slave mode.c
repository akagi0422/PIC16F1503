// PIC16F1503 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic.h>
#include <stdint.h>             //define uint_8
#define _XTAL_FREQ 310000UL     //maybe 500kHz
unsigned char rd_data[2] = 0;
int i = 0;
unsigned char dummy;

void main()
{
    /**************************************************************************/
    
    //IO initialize
    TRISAbits.TRISA5=0;   //A5 output
    TRISAbits.TRISA4=0;   //A4 output
    TRISCbits.TRISC3=1;   //C3 input
    ANSELCbits.ANSC3=0;   //C3 math input
    TRISCbits.TRISC4=1;   //C4 input
    TRISCbits.TRISC5=1;   //C5 input
    TRISAbits.TRISA2=0;   //A2 output
    TRISCbits.TRISC2=0;   //C2 output
    ANSELCbits.ANSC2=0;   //C2 math output
    TRISAbits.TRISA0=1;   //A0 input
    TRISAbits.TRISA1=1;   //A1 input
    TRISCbits.TRISC0=1;   //C0 input
    ANSELCbits.ANSC0=0;   //C0 math input
    TRISCbits.TRISC1=1;   //C1 in put
    ANSELCbits.ANSC1=0;   //C1 math input
    
    /**************************************************************************/
    
    //i2c initialize
    SSPBUF=0x00;
    SSPCON=0x36;////36!!
    SSPCON2=0x81;
    SSPCON3=0x00; //change form 0xf0 change form 0xf7
    SSPSTAT=0x00;
    SSPADD=0x52;
    SSPCONbits.SSPM3=0;
    SSPCONbits.SSPM2=1;
    SSPCONbits.SSPM1=1; 
    SSPCONbits.SSPM0=0;
    SSP1IF=0;
    i = 0;
    
    LATAbits.LATA5=0;
    LATAbits.LATA4=1;
    //__delay_ms(1000);
    LATAbits.LATA5=1;
    LATAbits.LATA4=0;
    //__delay_ms(1000);
    LATAbits.LATA5=0;
    LATAbits.LATA4=0;
    LATAbits.LATA2=0;
    LATCbits.LATC2=0;
    /**************************************************************************/
    
    //i2c part start form here
    while(i < 1)
    {
         if ((SSPSTATbits.S == 1) && (SSPSTATbits.D_nA == 0) && (SSPSTATbits.R_nW == 0) && (SSPSTATbits.BF == 1))          //MASTER ADD WRITE
        {
            while(SSP1IF==1)
            SSP1IF = 0;
            dummy = SSPBUF;
            while(BF==0)
            CKP = 1;                                                                //write address
            i = 0;
        }
    
        else if ((SSPSTATbits.S == 1) && (SSPSTATbits.D_nA == 1) && (SSPSTATbits.R_nW == 0) && (SSPSTATbits.BF == 1))    // MASTER DATA WRITE
        {
            while(SSP1IF==1)
            SSP1IF = 0;
            rd_data[i] = SSPBUF;
            if(rd_data[0]==0x52||rd_data[0]==0x18)
            {
                i++;
                while(BF==0)
                CKP = 1;
            }
            else if(rd_data[0]==0x8E)    //read reinitialize
            {
                SSPCON=0x36;//DO NOT TOUCH THIS LINE
                SSP1IF=0;
                i = 0;
            }
            else
            {
                i = 0;
                while(BF==0)
                CKP = 1;
            }
        }
    
        else if ((1 == SSPSTATbits.S) && (0 == SSPSTATbits.D_nA) && (1 == SSPSTATbits.R_nW) && (SSPSTATbits.BF == 1))        // MASTER ADDRESS READ
        {
            if(PORTCbits.RC5==0 && PORTCbits.RC4==1)       //locking
            {
                while(SSP1IF == 1)
                SSP1IF = 0;
                while(BF == 1)
                dummy = SSPBUF;
                //while(BF == 0)
                SSPBUF = 0x89;
                SSPCON1bits.CKP = 1;
                
                i = 0;
                LATAbits.LATA5=1;
                LATAbits.LATA4=1;
                __delay_ms(1000);
                break;
            }
            else if(PORTCbits.RC4 == 0 && PORTCbits.RC5 == 1/* && SSPSTATbits.BF == 0*/)   //releasing
            {
                while(1 == SSP1IF)
                SSP1IF = 0;
                while(BF == 1)
                dummy = SSPBUF;
                //while(BF == 0)
                SSPBUF = 0x8A;
                SSPCON1bits.CKP = 1;
                
                i = 0;
                LATAbits.LATA5=1;
                LATAbits.LATA4=1;
                __delay_ms(1000);
                break;
            }
            else if(PORTCbits.RC4==1 && PORTCbits.RC4==1)   //not releasing nor locking
            {
                while(1 == SSP1IF)
                SSP1IF = 0;
                while(1 == BF)
                dummy = SSPBUF;
                //while(BF == 0)
                SSPBUF = 0x8B;
                SSPCON1bits.CKP = 1;

                i = 0;
                LATAbits.LATA5=1;
                LATAbits.LATA4=1;
                __delay_ms(1000);
                break;
            }
            else                                //error case
            {
                i = 0;
                break;
            }
        }
    }
    //i2c part end

    //Data judgment part start form here
    while(i == 1)
    {
        if(PORTCbits.RC5 == 0 && PORTCbits.RC4 == 1)    //locked
        {
            if(0x52 == rd_data[0]/* && 0xAD == rd_data[1]*/)          //release
            {
                while(PORTCbits.RC4 != 0)
                {
                    LATAbits.LATA2=1;
                    LATCbits.LATC2=0;
                    LATAbits.LATA5=1;
                    LATAbits.LATA4=1;
                }
                LATAbits.LATA2=0;
                LATCbits.LATC2=0;
                i = 0;
                rd_data[0] = 0;
                rd_data[1] = 0;
                rd_data[2] = 0;
                break;
            }
            else
            {
                i = 0;
                rd_data[0] = 0;
                rd_data[1] = 0;
                rd_data[2] = 0;
                LATAbits.LATA5=1;
                LATAbits.LATA4=1;
                __delay_ms(250);
                LATAbits.LATA5=0;
                LATAbits.LATA4=1;
                __delay_ms(250);
                LATAbits.LATA5=1;
                LATAbits.LATA4=0;
                __delay_ms(250);
                LATAbits.LATA5=1;
                LATAbits.LATA4=0;
                __delay_ms(250);
                break;
            }
        }
        if(PORTCbits.RC4 == 0 && PORTCbits.RC5 == 1)   //released
        {
            if(rd_data[0] == 0x18/* && rd_data[1] == 0xe7*/)    //lock
            {
                while(PORTCbits.RC5 != 0)
                {
                    LATAbits.LATA2=0;
                    LATCbits.LATC2=1;
                    LATAbits.LATA5=1;
                    LATAbits.LATA4=1;
                    __delay_ms(10000);//10000
                }
                i = 0;
                rd_data[0] = 0;
                rd_data[1] = 0;
                rd_data[2] = 0;
                break;
            }
            else
            {
                i = 0;
                LATAbits.LATA5=1;
                LATAbits.LATA4=1;
                __delay_ms(250);
                LATAbits.LATA5=0;
                LATAbits.LATA4=1;
                __delay_ms(250);
                LATAbits.LATA5=1;
                LATAbits.LATA4=0;
                __delay_ms(250);
                LATAbits.LATA5=1;
                LATAbits.LATA4=0;
                __delay_ms(250);
                rd_data[0] = 0;
                rd_data[1] = 0;
                rd_data[2] = 0;
                break;
            }
        }
        else
        {
            i=0;
            break;
        }
    }
    //Data judgment part end
}
