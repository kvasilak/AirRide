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
**    Corner.h

todo
there is a lagtime from opening/closing the valve to a height change.
need to predict movement or use shorter bursts to sneak up on height

**************************************************************************/

#ifndef INCLUDE_CORNER_H
#define INCLUDE_CORNER_H

#include <stdint.h>
#include "debug.h"

enum Position { 
        LeftFront, 
        RightFront, 
        LeftRear, 
        RightRear, 
        Height};
  
enum LEDS { OK_LED, Error_LED, IN_Position_LED };

//energised is open
//so 1 is open, 0 closed
enum Solenoid { Closed, Open }; 

//enum ValveOp { Filling, Dumping, Holding, LastState };
#define VALVE_STATES_LIST(macro)\
    macro(Initing)      \
    macro(Filling)    \
    macro(Dumping)     \
    macro(Holding)      \
    macro(LastState)
    
enum ValveOp
{
  VALVE_STATES_LIST(ENUMIFY)
};

// Parameter K for the filter
#define FILTER_SHIFT    5//8
#define HEIGHT_NOMINAL  512
#define DEAD_BAND       10 //5
#define HOLD_DEAD_BAND  20 //8

class CCorner 
{
	public:
	CCorner();//Position c);
	void Run(int16_t tilt);
    void Fill(Solenoid state);
    void Dump(Solenoid state);
    void PrintCorner();
    void Init(Position p);
    void Limits(int16_t Low, int16_t high);
  
	private:
	int16_t DeadBand;			//how far the corner has to move before we try to adjust the position
	int16_t HoldDeadBand; 	    //deadband + hysterysis
	Position corner;
	uint32_t CycleTime;		    //The minimum time between valve state changes. Prevents valve chatter
	ValveOp State;
	uint32_t LastTime;		    //the last time valve changed state
	int32_t filter_reg; 
    uint32_t HoldOff;           //The time till the next state change
    uint32_t HoldOffTime;       //Min time between state changes
    
    //int PinHeightLeftRear;
    //int PinHeightRghtRear;
    //int PinDumpRightRear;
    //int PinDumpLeftRear;
    //int PinFillLeftRear;
    //int PinFillRightRear;
    //int PinSetpoint;
    
    int16_t LimitLow;
    int16_t LimitHigh;
    
    int16_t GetHeight();
    void SetState(ValveOp s);

};
#endif //INCLUDE_CORNER_H
