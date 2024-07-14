/*
 * RTTY.h
 *
 * Created: 16.04.2024 21:55:10
 *  Author: DL8MCG
 */ 

#ifndef RTTY_H_
#define RTTY_H_

#include <stdbool.h>
#define RttyTextLen  100       

void SetIdle();

void SetRttyText(const char * buf);

void InitRtty();

#endif /* RTTY_H_ */