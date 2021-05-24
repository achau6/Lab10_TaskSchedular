/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEmo: https://youtu.be/OrT8VoURyd4
 */
#include <avr/io.h>
#include "timer.h"
#include "keypad.h"
#include "schedule.h"
#include "bit.h"
#include "pwn.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum combinations { start, init, key, right, wrong, lock } combination;
unsigned char keyUnlocker[] = { '#', '1', '2', '3', '4', '5' };
int countKey = 0;
int unlock(int state) {
	unsigned char y = GetKeypadKey();
	unsigned char button = ~PINB & 0x80;
	switch(state){
		case start:
			state = init;
		break;
		case init://#
			if(y == '#') {
				state = right;
				countKey ++;
			} else if((~PINB & 0x80) == 0x80){
				state = lock;	
			} else {
				state = init;
			}
		break;
		case right://1, 2, 3, 4, 5
			if(y == keyUnlocker[countKey]) {
				state = wrong;
				countKey++;
				if(countKey == 6){
					state = key;
				}
			} else if(y == '\0') {
				state = right;	
			} else if((~PINB & 0x80) == 0x80) {
				state = lock;	
			} else {
				state = init;
				countKey = 0;
			}	
		break;
		case wrong:
			if(y == '\0'){
				state = right;
			} else {
				state = wrong;
			}
		break;
		case key:
			if((~PINB & 0x80) == 0x80){
				state = lock;
			} else {
				state = key;
			}
		break;
		case lock:
			countKey = 0;
			state = init;
		break;
		default:
		break;
	}
	switch(state){
		case init:
			//to check current values to see if it currently working
			//PORTB = countKey;
		break;
		case wrong:
			//PORTB = countKey;
		break;
		case right:
			//PORTB = countKey;
		break;
		case key:
			PORTB = 0x01;
		break;
		case lock:
			PORTB = 0x00;
		break;
		default:
		break;
	}

	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x7F; PORTB = 0x80;
	DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
	static task task1;// task2, task3, task4;
	task *tasks[] = { &task1 };//  &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	const char start = 0;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &unlock;

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





