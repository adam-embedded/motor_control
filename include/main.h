#ifndef __MAIN_H
#define __MAIN_H

//PWM motor pins
#define RIGHTFWD 3
#define RIGHTRWD 11
#define LEFTFWD 10
#define LEFTRWD 9

//
/* Serial Control bytes */
//
//Status
#define STATUSBIT 0xA0
#define STATUSOK 0xA1
#define STATUSFAIL 0xF0
#define STATUSTIME 0xF1
#define MOTORBIT 0xA2

//Motors
#define RIGHTFWDCMD 0x01
#define RIGHTRWDCMD 0x02
#define LEFTFWDCMD 0x03
#define LEFTRWDCMD 0x04
#define FORWARDCMD 0x06
#define BACKWARDS 0x07
#define STOPCMD 0x08

//
/* Serial Control Parameters */
//
#define MAXWAIT 2000

#endif