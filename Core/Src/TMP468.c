/*
 * thermal.h
 *
 *	Functions to interface with the TMP468
 *  Created on: Jan 6, 2026
 *      Author: luan
 */


#include <TMP468.h>
#include "main.h"
#include "util.h"

#define TMP468_addr 0x48
#define blockRead_addr 0x80
//Number of blocks in block read mode. internal tmp + 8 peripherals
#define nBlocks 9
uint16_t temps[nBlocks]; //Holds the values read from i2c

void readTemps(){
	//write address to the pointer register

}
