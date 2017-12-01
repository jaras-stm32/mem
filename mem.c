/*
 * mem.c
 *
 *  Created on: 16.11.2017
 *      Author: jaras
 */

#include "mem.h"
#include <stdio.h>

uint8_t mem[MEM_SIZE] = {0, 0, 0};


void *MEM_Alloc(uint16_t size) {
	uint16_t i = 0;
	while(i < MEM_SIZE) {

		if(mem[i] == 0) {
			uint16_t len = mem[i +1] | (mem[i +2] << 8);

			if(len == 0) {
				if(MEM_SIZE - i - 3 >= size) {
					mem[i] = 1;
					mem[i +1] = size & 0xff;
					mem[i +2] = size >> 8;
					mem[i + size +3] = 0;
					mem[i + size +4] = 0;
					mem[i + size +5] = 0;

					return (void *)mem + i + 3;
				}
			}
			if(len == size) {
				mem[i] = 1;

				return (void *)mem + i + 3;
			}
			if(len > size) {
				mem[i] = 1;

				if(len - size > 11) {
					mem[i +1] = size & 0xff;
					mem[i +2] = size >> 8;
					uint16_t ns = len - size -3;
					mem[i + size +3] = 0;
					mem[i + size +4] = ns & 0xff;
					mem[i + size +5] = ns >> 8;
				}

				return (void *)mem + i + 3;
			}
		}

		i += (mem[i +1] | (mem[i +2] << 8)) + 3;

	}
	return 0;
}

void *MEM_AllocLast(uint16_t size) {
	uint16_t i = 0;
	while(i < MEM_SIZE) {
		if(mem[i] == 0 && mem[i +1] == 0 && mem[i +2] == 0) {
			if(MEM_SIZE - i - 3 >= size) {
				mem[i] = 1;
				mem[i +1] = size & 0xff;
				mem[i +2] = size >> 8;
				mem[i + size +3] = 0;
				mem[i + size +4] = 0;
				mem[i + size +5] = 0;

				return (void *)mem + i + 3;
			}
		}

		i += (mem[i +1] | (mem[i +2] << 8)) + 3;

	}
	return 0;
}

uint8_t MEM_Append(void *pointer, uint8_t *buffer, uint16_t length) {
	uint8_t *i = pointer -3;
	uint16_t len = *(i +1) | (*(i +2) << 8);
	uint16_t nlen = len + length;

	if(MEM_SIZE - (pointer - (void *)&mem) -3 < nlen) { //Memory too small
		return 1;
	}

	*(i +1) = nlen & 0xff;
	*(i +2) = nlen >> 8;

	for(uint16_t k = 0; k < length; k++) {
		*(i +len +k +3) = *(buffer + k);
	}

	*(i + nlen +3) = 0;
	*(i + nlen +4) = 0;
	*(i + nlen +5) = 0;

	return 0;
}

uint16_t MEM_Size(void *pointer) {
	uint8_t *i = pointer -3;
	if(*i == 0) {
		return 0;
	}
	return *(i +1) | (*(i +2) << 8);
}

void MEM_Free(void *pointer) {
	uint8_t *i = pointer -3;
	*i = 0;

	uint16_t size = *(i +1) | (*(i +2) << 8);

	if(*(i + size +3) == 0) {
		uint16_t ns = *(i + size +4) | (*(i + size +5) << 8);
		if(ns == 0) {
			*(i +1) = 0;
			*(i +2) = 0;
		}
		else {
			size += ns +3;
			*(i +1) = size & 0xff;
			*(i +2) = size >> 8;
		}
	}
}

uint16_t MEM_Dump(char *report) {
	uint16_t i = 0;
	uint16_t bytes = 0;

	while(i < MEM_SIZE) {
		bytes += sprintf(report + bytes, "%5u: ", i);

		uint16_t len = mem[i +1] | (mem[i +2] << 8);

		if(mem[i] == 0) {
			bytes += sprintf(report + bytes, "free %u", len);
		}
		else {
			bytes += sprintf(report + bytes, "used %u", len);
		}

		bytes += sprintf(report + bytes, "\n");

		if(len == 0) {
			return bytes;
		}
		i += (mem[i +1] | (mem[i +2] << 8)) + 3;
	}

	return bytes;
}
