/*************************************************************************
**   Air ride controller for the MSGPIO hardware
**
** Copyright 2013 Keith Vasilakes
**
** This file is part of MegaRide an Air ride controller.
**
** MegaRide is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation, either version 3 of the License, or (at your option) 
** any later version.
**
** MegaRide is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
** implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public 
** License for more details.
**
** You should have received a copy of the GNU General Public License along with MegaRide. If not, see
** http://www.gnu.org/licenses/.
**
**    common.h
**************************************************************************/
#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#include "debug.h"

#define STATES_LIST(macro)\
    macro(RUNAUTOCAL)               \
    macro(RUNCAMP)                \
    macro(RUNMANUAL)                  \
    macro(RUNTRAVEL)                \
    macro(DUMPTANK)                 \
    macro(DUMPINGTANK)              \
    macro(CALLIMITS)                \
    macro(CALLOW)                   \
    macro(CALWAITHIGH)              \
    macro(CALHIGH)                  \
    macro(CALSAVELIMITS)            \
    macro(CALDONELED)               \
    macro(CALDONE)                  \
    macro(CALCOMPLETE)              \

    
enum states_t
{
  STATES_LIST(ENUMIFY)
};

//inverted logic on these pins
//0 is ON!
#define MODES_LIST(macro)\
    macro(AUTOCALMODE)  \
    macro(CAMPMODE)   \
    macro(MANUALMODE)     \
    macro(TRAVELMODE)   \

    
enum modes_t
{
  MODES_LIST(ENUMIFY)
};



bool IsTimedOut(uint32_t period, uint32_t start);
  
//IO map
//=== Analog ===================
#define PINSETPOINT A0
//#define PINTILT     A1 //this input is bad
#define PINRRHEIGHT A2
#define PINLRHEIGHT A3
#define PINDUMP     A4
#define WIPERSPEED  A5
#define PINTILT  A6
//#define  A7

// === Digital =======================
#define SERVO               2
#define LED                 3       //LED and cal share a pin
#define PINCAL              3
#define PINMODE1            6
#define PINMODE2            7
#define PINFILL_LEFTREAR    8
#define PINDUMP_LEFTREAR    9
#define PINDUMP_RIGHTREAR   10
#define PINFILL_RIGHTREAR   11
#define ACCEL_I2C_DATA      12
#define ACCEL_I2C_CLOCK     13

enum Position 
{ 
    LeftRear, 
    RightRear
};

#endif
