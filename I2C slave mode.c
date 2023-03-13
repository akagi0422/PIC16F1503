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
int a1=0;
int a2=0;
int a3=0;
int a4=0;
int a5=0x00;
int a6=0x02;
int a7=0x03;
int a8=0x04;
unsigned char rd_data[10] = 0;
unsigned char wt_data[10] = 0;
int i = 0;
unsigned char dummy;

void main()
{
    /**************************************************************************/
    
    //IO initializehhhhv
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
    SSPCON=0x36;
    SSPCON2=0x81;
    SSPCON3=0x00; //change form 0xf0
    SSPSTAT=0x00;
    SSPADD=0xA0;
    SSPCONbits.SSPM3=0;
    SSPCONbits.SSPM2=1;
    SSPCONbits.SSPM1=1;
    SSPCONbits.SSPM0=0;
    SSP1IF=0;
    
    LATAbits.LATA5=0;
    LATAbits.LATA4=1;
    __delay_ms(1000);
    LATAbits.LATA5=1;
    LATAbits.LATA4=0;
    __delay_ms(1000);
    LATAbits.LATA5=0;
    LATAbits.LATA4=0;
    
    /**************************************************************************/
    
    //i2c part start form here
    while(i <= 3)
    {
        if ((1 == SSPSTATbits.S)&&(0 == SSPSTATbits.D_nA)&&(0 == SSPSTATbits.R_nW)&&(1 == SSPSTATbits.BF))          //MASTER ADD WRITE
        {
            while(1 == SSP1IF)
            SSP1IF = 0;
            dummy = SSPBUF;
            while(0 == BF)
            CKP = 1;                                                                //write address
            i = 0;
        }
    
        else if ((1 == SSPSTATbits.S)&&(1 == SSPSTATbits.D_nA)&&(0 == SSPSTATbits.R_nW)&&(1 == SSPSTATbits.BF))    // MASTER DATA WRITE
        {
            ACKDT = 0;
            while(1 == SSP1IF)
            SSP1IF = 0;
            rd_data [i] = SSPBUF;
            i++;
            while(0 == BF)
            CKP = 1; 
        }
    
        else if ((1==SSPSTATbits.S)&&(0==SSPSTATbits.D_nA)&&(1==SSPSTATbits.R_nW))                                // MASTER ADDRESS READ
        {
            SSPBUF = rd_data [i];
            SSPCON1bits.CKP = 1;
            __delay_ms(100);
            while(1 == SSP1IF)
            SSP1IF=0;
            i++;
        }
    
        else if ((1 == SSPSTATbits.S)&&(1 == SSPSTATbits.D_nA)&&(1 == SSPSTATbits.R_nW))                         // MASTER DATA READ
        {
            SSPBUF = rd_data [i];
            SSPCON1bits.CKP = 1;
            __delay_ms(100);
            while(1 == SSP1IF)
            SSP1IF=0;
            i++;
        }
    
        else if ((1 == SSPSTATbits.S)&&(1 == SSPSTATbits.D_nA)&&(0 == SSPSTATbits.R_nW)&&(0 == SSPSTATbits.BF)) 
        {
            ;
        }
    
        else
	    {
	    	;								// Error case
	    }
    }
    //i2c part end
    
    /**************************************************************************/

    /***************************************************************************
     *                                                                        *
     * * * * * * * this part is a test, and now is useless. * * * * * * * * * *
     *                                                                        *

    while(1)
    {
    while((0 == SSPSTATbits.R_nW))
    {
        /*ACKDT = 0;
        while(1 == SSP1IF)
        SSP1IF = 0;
        dummy = SSPBUF;
        while(0 == BF)
        CKP = 1;
        
        ACKDT = 0;
        while(1 == SSP1IF)
        SSP1IF = 0;
        a1 = SSPBUF;
        while(0 == BF)
        CKP = 1;
        
        ACKDT = 0;
        while(1 == SSP1IF)
        SSP1IF = 0;
        a2 = SSPBUF;
        while(0 == BF)
        CKP = 1;
        
        ACKDT = 0;
        while(1 == SSP1IF)
        SSP1IF = 0;
        a3 = SSPBUF;
        while(0 == BF)
        CKP = 1;
        
        ACKDT = 0;
        while(1 == SSP1IF)
        SSP1IF = 0;
        a4 = SSPBUF;
        while(0 == BF)
        CKP = 1;
    }
    
    while((1 == SSPSTATbits.R_nW))
    {
        __delay_ms(100);
        SSP1IF = 0;
        dummy = SSPBUF;
        __delay_ms(100);
        
        SSPBUF = 5;
        SSPCON1bits.CKP = 1;
        __delay_ms(100);
        while(1 == SSP1IF)
        SSP1IF=0;
        
        SSPBUF = 6;
        SSPCON1bits.CKP = 1;
        __delay_ms(100);
        while(1 == SSP1IF)
        SSP1IF=0;
        
        SSPBUF = 7;
        SSPCON1bits.CKP = 1;
        __delay_ms(100);
        while(1 == SSP1IF)
        SSP1IF=0;
        
        SSPBUF = 8;
        SSPCON1bits.CKP = 1;
        __delay_ms(100);
        while(1 == SSP1IF)
        SSP1IF=0;
    }    
    }
    
     *                                                                        *
     * * * * * * * * * * * * * *the test is over* * * * * * * * * * * * * * * * 
     *                                                                        *

    ***************************************************************************/
    
    //Data judgment part start form here
    while(i == 4)
    {
        //sentence test start
        if(0x00 == rd_data [0] && 0x01 == rd_data [1] && 0x01 == rd_data [2] && 0x01 == rd_data [3])          
        {
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            __delay_ms(2000);
            LATAbits.LATA5=1;  
            LATAbits.LATA4=0;
            __delay_ms(2000);          //only red
            i = 0;
        }
        else if(0x00 == rd_data [0] && 0x02 == rd_data [1] && 0x02 == rd_data [2] && 0x02 == rd_data [3])
        {
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            __delay_ms(2000);
            LATAbits.LATA5=0;  
            LATAbits.LATA4=1;
            __delay_ms(2000);          //only green
            i = 0;
        }
        else if(0x00 == rd_data [0] && 0x03 == rd_data [1] && 0x03 == rd_data [2] && 0x03 == rd_data [3])
        {
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            __delay_ms(2000);
            LATAbits.LATA5=1;
            LATAbits.LATA4=1; 
            __delay_ms(2000);          //both green and red
            i =0;
        }
        else
        {
            i=0;
        }
        //sentence test end
        /*
        if(0x01 == rd_data [0] && 0x01 == rd_data [1] && 0x01 == rd_data [2] && 0x01 == rd_data [3] && PORTCbits.RC5==0)  //locked
       {
            if(PORTCbits.RC3==0)               //release now
            {
                LATAbits.LATA4=0;
                LATAbits.LATA5=0; 
                LATAbits.LATA2=1;
                LATCbits.LATC2=0;
                __delay_ms(9500);
            }
            else
           {
            LATAbits.LATA4=0;
            LATAbits.LATA5=1;
            LATAbits.LATA2=0;
            LATCbits.LATC2=0;
           }
         }
        if(0x02 == rd_data [0] && 0x02 == rd_data [1] && 0x02 == rd_data [2] && 0x02 == rd_data [3] && PORTCbits.RC4==0)  //released
        {
            if(PORTCbits.RC3==0)          //lock now
            {
            LATAbits.LATA4=0;
            LATAbits.LATA5=0;
            LATAbits.LATA2=0;
            LATCbits.LATC2=1;
            __delay_ms(9500);
            }
            else
            {
            LATAbits.LATA4=1;
            LATAbits.LATA5=0; 
            LATAbits.LATA2=0;
            LATCbits.LATC2=0;
            }
        }
        if(0x03 == rd_data [0] && 0x03 == rd_data [1] && 0x03 == rd_data [2] && 0x03 == rd_data [3])  //checking
        {
            if(PORTCbits.RC5==0)        //locked
            {
                ;
            }
            else if(PORTCbits.RC4==0)        //released
            {
                ;
            }
            else          //moving
            {
                ;
            }
        }
        */
    }
        //Data judgment part end
}
