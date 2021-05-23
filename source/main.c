/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "keypad.h"
#include "schedule.h"
#include "bit.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

/*unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum pauseButtonSM_States { pauseButton_wait, pauseButton_press, pauseButton_release } ;

int pauseButtonSMTick(int state) {
	unsigned char press = ~PINA & 0x01;
	switch(state) {
		case pauseButton_wait:
			state = press == 0x01? pauseButton_press: pauseButton_wait ; break;
		case pauseButton_press:
			state = pauseButton_release; break;
		case pauseButton_release:
			state = press = 0x00? pauseButton_wait: pauseButton_press; break;
		default: state = pauseButton_wait; break;
	}
	switch(state) {
		case pauseButton_wait: break;
		case pauseButton_press:
			pause = (pause == 0) ? 1 : 0;
		break;
		case pauseButton_release: break;
	}
	return state;
}

enum toggleLED0_States { toggleLed0_wait, toggleLED0_blink };

int toggleLED0SMTick(int state){
	switch(state) {
		case toggleLED0_wait: state = !pause? toggleLED0_blink: toggleLED0_wait; break;
		case toggleLED0_blink: state = pause? toggleLED0_wait: toggleLED0_blink; break;
		default: state = toggleLED0_wait; break;
	}
	switch(state) {
		case toggleLED0_wait: break;
		case toggleLED0_blink:
			led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return state;
}

enum toggleLED1_States { toggleLED1_wait, toggleLED1_blink };

int toggleLED1SMTick(int state) {
	switch(state) {
		case toggleLED1_wait: state = !pause? toggleLED1_blink: toggleLED1_wait; break;
		case toggleLED1_blink: state = pause? toggleLED1_wait: toggleLED1_blink; break;
		default: state = toggleLED1_wait; break;
	}	
	switch(state) {
		case toggleLED1_wait: break;
		case toggleLED1_blink:
			led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return state;
}

enum display_States { display_display };

int displaySMTick(int state) {
	unsigned char output;
	switch(state) {
		case display_display: state = display_display; break;
		default: state = display_display; break;
	}
	switch(state) {
		case display_display:
			output = led0_output | led1_output << 1;
			break;
	}
	PORTB = output;
	return state;
}*/

enum displays { start, init } display;
int tick(int state){
	unsigned char x = GetKeypadKey();
	switch(state) {
		case start: 	state = init; 
				break;
		case init:	state = init;	
				break;
		default:	state = start;
				break;
	}
	switch(state) {
		case start:	break;
		case init:
			switch(x) {
				case '\0':	PORTB = 0x1F; break;
				case '1':	PORTB = 0x01; break;
				case '2':	PORTB = 0x02; break;
				case '3':	PORTB = 0x03; break;
				case '4':	PORTB = 0x04; break;
				case '5':	PORTB = 0x05; break;
				case '6':	PORTB = 0x06; break;
				case '7':	PORTB = 0x07; break;
				case '8':	PORTB = 0x08; break;
				case '9':	PORTB = 0x09; break;
				case 'A':	PORTB = 0x0A; break;
				case 'B':	PORTB = 0x0B; break;
				case 'C':	PORTB = 0x0C; break;
				case 'D':	PORTB = 0x0D; break;
				case '*':	PORTB = 0x0E; break;
				case '0':	PORTB = 0x00; break;
				case '#':	PORTB = 0x0F; break;
				default:	PORTB = 0x1B; break;
			}
			
			break;
				
		default:	break;
	}
	return state;
}

/*unsigned long int findGCD(unsigned long int a, unsigned long in b){
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a=b;
		b=c;
	}
	return 0;
}*/
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
	static task task1;// task2, task3, task4;
	task *tasks[] = { &task1 };//  &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	const char start = 0;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &tick;

	/*task2.state = start;
        task2.period = 500;
        task2.elapsedTime = task2.period;
        task2.TickFct = &toggleLED0SMTick;

        task3.state = start;
        task3.period = 1000;
        task3.elapsedTime = task3.period;
        task3.TickFct = &toggleLED1SMTick;

        task4.state = start;
        task4.period = 10;
        task4.elapsedTime = task4.period;
        task4.TickFct = &displaySMTick;*/

	unsigned long GCD = tasks[0]->period;
	for(int j = 0; j < numTasks; j ++){
		GCD = findGCD(GCD, tasks[j]->period);
	}

	TimerSet(50);
	TimerOn();
	unsigned short i;
    while (1) {
	for( i = 0; i < numTasks; i ++) {
		if( tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 50;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
