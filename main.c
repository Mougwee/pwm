/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         C Template for the MC Car
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          12.09.2014
 *
 * \b Language:   Ansi-C \n\n
 * \b Target:     MC-Car  \n
 *
 * \par Description:
 *
 * $Id: main.c 904 2014-09-12 10:58:43Z chj-hslu $
 *--------------------------------------------------------------------
 */
#include "platform.h" /* include peripheral declarations */

#define TIMER1_MODULO 31249
#define DUTY_CYCLE_DIVISOR 20

interrupt void timerOverflowInt(void){
	//Timer-Overflow zurücksetzen, das System würde sonst ewig wiederholend in die ISR springen
	TPM1SC_TOF = 0;
}

interrupt void channel2OverflowInt(void){
	TPM1C2SC_CH2F = 0;
}

void initPorts(void){
	//Lichter initialisieren
	PTDD_PTDD2 = 1;		//Rücklichter einschalten (Ein = 1)
	PTDDD_PTDDD2 = 1;	//Rücklicht-Ports als Output
}

void initTimer(void){
	//Timer einstellen
	// (MOD + 1) * PS / f
	TPM1SC_PS = 5;				//Prescaler 32
	TPM1MOD = TIMER1_MODULO;	//Modulo
	//Timer Reset, zur Sicherheit
	TPM1CNT = 0;				//clear counter register
	TPM1SC_TOF = 0;				//clear overflow flag
	//Timer-Interrupt einschalten, aktiviert ISR (gemäss Linker) wenn TOF = 1
	TPM1SC_TOIE = 1;
	
	//Channel einstellen, Buzzer an TPM1CH5
	TPM1C2V = (TIMER1_MODULO / DUTY_CYCLE_DIVISOR);
	TPM1C2SC_MS2x = 2;	//Mode: edge-aligned PWM
	TPM1C2SC_ELS2x = 2;	//Mode: clear output on compare
	TPM1C2SC_CH2F = 0;
	//Channel-Interrupt
	TPM1C2SC_CH2IE = 1;
	
	
	/*
	 * Der Timer fängt an zu zählen, sobald die Clock gewählt wird,
	 * deshalb wird das zuletzt eingestellt
	 */
	TPM1SC_CLKSx = 2;	//Fixed System Clock 1MHz
}


/**
 * main program
 */  
void main(void) 
{
    initPorts();
    
    initTimer();
	
	// Interrupts aktivieren, I-Bit in CCR-Register (0 = eingeschaltet)
    EnableInterrupts;
    
    
    for(;;) 
    {   
      __RESET_WATCHDOG();  /* feeds the dog */
    }
    
  /* please make sure that you never leave main */
}

