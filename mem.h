/*
 * mem.h
 *
 *  Created on: 16.11.2017
 *      Author: jaras
 */

#ifndef MEM_H_
#define MEM_H_

#include <stdint.h>

#define MEM_SIZE 0x10000

void *MEM_Alloc(uint16_t size);
void MEM_Free(void *pointer);

void *MEM_AllocLast(uint16_t size);
uint8_t MEM_Append(void *pointer, uint8_t *buffer, uint16_t length);

uint16_t MEM_Size(void *pointer);

uint16_t MEM_Dump(char *report);

#endif /* MEM_H_ */
