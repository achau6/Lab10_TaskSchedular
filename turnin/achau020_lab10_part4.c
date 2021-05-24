/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo: https://youtu.be/3jffTJWoX-A
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
int keyCombination = 6;
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
				if(countKey == keyCombination){
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

double freq_array[55] = { 587, 587, 554, 554, 587, 587, 587, 587, 698.5, 698.5, 698.5, 698.5,554, 554, 554, 554, 554, 554, 554, 494, 494, 494, 494, 440, 440, 494, 494, 494, 494, 587, 587, 587, 587, 698.5, 698.5, 698.5, 698.5, 830.6, 830.6, 698.5, 698.5, 830.6, 830.6, 987.8, 987.8, 987.8, 987.8, 698.5, 698.5, 698.5, 698.5, 659.3, 659.3, 587, 587 };
int count = 0;
enum speaerkSMs { startsp, initsp, onsp, offsp };

int tick(int state){
	unsigned char button = (~PINA & 0x80);
	switch(state){
		case startsp:
			state = initsp;
		break;
		case initsp:
			if(button == 0x80){
				count = 0;
				state = onsp;
			} else {
				state = initsp;
			}
		break;
		case onsp:
			if(count < 55){
				state = onsp;
			} else {
				if(button == 0x80) {
					state = offsp;
				} else {
					state = initsp;
				}
			}
		break;
		case offsp:
			if(button == 0x80){
				state = offsp;
			} else {
				state = initsp;
			}
		break;
		default:
		break;

	}

	switch(state){
		case startsp:
		break;
		case initsp:
			set_PWM(0);
		break;
		case onsp:
			if(count < 55) {
				set_PWM(freq_array[count]);
				count ++;
			}
		break;
		case offsp:
			set_PWM(0);
		break;
		default:
		break;
	}
	return state;
}
enum locks {startL, initL, waitL, new };
unsigned char newLock[4] = {}; //{?, ?, ?, ? }
int countLock = 0;
int changeLock(int state){
	unsigned char x = GetKeypadKey();
	switch(state){
		case startL:
			state = initL;
		break;
		case initL:
			if(x == '*' && (~PINB & 0x80) == 0x80) {
				state = waitL;
				countLock = 0;
			} else {
				state = initL;
			}
		break;
		case new:
			if(countLock < 4){
				state = new;
			} else if(countLock == 4){
				state = initL;
			}
		break;
		case waitL:
			if(x == '\0'){
				state = new;
			}
		break;
		default:
		break;

	}
	switch(state){
		case initL:
		break;
		case new:
			if(x == '\0') {// || x == '*' || x == '#') {
			} else {
				//PORTB = 0x04;
				newLock[countLock] = x;
				countLock ++;
				state = waitL; 
			}
		break;
		default:
		break;
	}
	if(countLock == 4) {
		for(int i = 1; i < 5; i++){
			keyUnlocker[i] = newLock[i-1];
		}
		keyCombination = 5;
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
	DDRB = 0x7F; PORTB = 0x80;
	DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
	static task task1, task2, task3;// task4;
	task *tasks[] = { &task1, &task2, &task3};// &task4 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	const char start = 0;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &unlock;

	task2.state = startsp;
        task2.period = 100;
        task2.elapsedTime = task2.period;
        task2.TickFct = &tick;
	
        task3.state = startL;
        task3.period = 150;
        task3.elapsedTime = task3.period;
        task3.TickFct = &changeLock;
	/*
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
	PWM_on();
	unsigned short i;
    while (1) {
	for( i = 0; i < numTasks; i ++) {
		if( tasks[i]->elapsedTime >= tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += tasks[i]->period;
	}
	
	unsigned long int duration = 0;
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
