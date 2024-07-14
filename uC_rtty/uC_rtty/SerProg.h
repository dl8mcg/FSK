/*
 * SerProg.h
 *
 * Created: 04.09.2022 18:28:08
 *  Author: DL8MCG
 */ 

#include <stdbool.h>

#ifndef SERPROG_H_
#define SERPROG_H_

void SerInit();
void InitFSK(uint64_t freq0, uint64_t freq1);		// set mark / space frequencies of AD9833
void CallSmSer();
void SerStart();

bool isSer();

void FskMark();										// select mark frequency
void FskSpace();									// select space frequency

#endif /* SERPROG_H_ */