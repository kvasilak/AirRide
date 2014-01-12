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
**    corner.cpp
**************************************************************************/

#include <Arduino.h>
#include <stdint.h>
#include "corner.h"
#include "debug.h"
#include "common.h"


//This class is responsible for managing the height of a corner.
//There is one instance of this class for each corner of the vehicle usually 4
//This is where the decisions are made as to add or remove air from the suspension
CCorner::CCorner()://Position p):
  DeadBand(DEAD_BAND),//ADC counts 0-1024
  HoldDeadBand(HOLD_DEAD_BAND),
  CycleTime(100), //ms between updates
  State(Initing),
  //PinDumpRightRear(10), //set IO pins here
  //PinDumpLeftRear(8),
  //PinFillRightRear(11),
  //PinFillLeftRear(7),
  //PinHeightLeftRear(A0),   //analog in
  //PinHeightRghtRear(A2),   //analog in
  //PinSetpoint(A3),          //analog in setpoint pot
  HoldOffTime(2000),
  LimitLow(100),            // set limits to 100 from high and low
  LimitHigh(924)
{

	LastTime 		= millis();
    HoldOff         = millis();
	filter_reg 		= GetHeight() << FILTER_SHIFT;

    pinMode(PINDUMP_RIGHTREAR, OUTPUT);
    digitalWrite(PINDUMP_RIGHTREAR, HIGH);
    
    pinMode(PINDUMP_LEFTREAR, OUTPUT);
    digitalWrite(PINDUMP_LEFTREAR, HIGH);
    
    pinMode(PINFILL_RIGHTREAR, OUTPUT);
    digitalWrite(PINFILL_RIGHTREAR, HIGH);
    
    pinMode(PINFILL_LEFTREAR, OUTPUT);
    digitalWrite(PINFILL_LEFTREAR, HIGH);
    
    //analog in pins need no setup
}

void CCorner::Init(Position p)
{
    corner = p;
}
void CCorner::Limits(int16_t Low, int16_t high)
{
    LimitLow = Low +2;
    LimitHigh = high -2;
}

//Get the height of this corner
int16_t CCorner::GetHeight()
{
    int height=0;
    
    switch(corner)
   {
        case LeftRear:
            height = 1024 - analogRead(PINLRHEIGHT); 
            break;
        case RightRear:
            height = 1024 - analogRead(PINRRHEIGHT);
            break;
   }   
   
   return height;
}

//open or close the fill solenoid for this corner
void CCorner::Fill(Solenoid state)
{
    switch(corner)
   {
        case LeftRear:
            digitalWrite(PINFILL_LEFTREAR, state);
            break;
        case RightRear:
            digitalWrite(PINFILL_RIGHTREAR, state);
            break;
   }   
}

//open or close the dump solenoid for this corner
void CCorner::Dump(Solenoid state)
{
    switch(corner)
   {
        case LeftRear:
            digitalWrite(PINDUMP_LEFTREAR, state);
            break;
        case RightRear:
            digitalWrite(PINDUMP_RIGHTREAR, state);
            break;
   }   
}

void CCorner::PrintCorner()
{
    switch(corner)
   {
        case LeftRear:
            Serial.print("Left,");
            break;
        case RightRear:
            Serial.print("Right,");
            break;
   } 
}

void CCorner::SetState(ValveOp s)
{
    ValveOp laststate = Holding;
    
    static char *StateStrs[] = {VALVE_STATES_LIST(STRINGIFY)};

    Serial.print(">Corner,");
    PrintCorner();
    Serial.print(StateStrs[s]);
    Serial.println("<");
    
    if(laststate != s)
    {
        laststate = s;
    }
    
    State = s;
}

//Uses the low pass filter described in "Simple Software Lowpass Filter.pdf"
void CCorner::Run(int16_t tilt)
{
	int32_t slowheight; 		
    	
	//the setpoint pot raises and lowers both corners
	//the tilt pot causes one corner to raise while the other lowers
	//This is used to level the motorhome while camping
	//setpoint + tilt is still limited to +-
	int32_t setpoint 		= analogRead(PINSETPOINT) + tilt;
	int32_t height 			= GetHeight();
    
    //prevent setpoint from exceeding cal limits
/*     if(setpoint > LimitHigh)
    {
        setpoint = LimitHigh;
    }
    
    if(setpoint < LimitLow) 
    {
        setpoint = LimitLow;
    } */

	//sample Slowly
	if(IsTimedOut(CycleTime, LastTime) )
	{
		// Update filter with current sample.
		filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + height;

		LastTime = millis();
        
        // Scale output for unity gain.
        slowheight = (filter_reg >> FILTER_SHIFT);

        
        switch(corner)
       {
            case LeftRear:
                Log("Corner", "LRError", height - setpoint);
                Log("Corner", "LeftHeight", height);
                Log("Corner", "LeftSet", setpoint);
                break;
            case RightRear:
                Log("Corner", "RRError", height - setpoint);
                Log("Corner", "RightHeight", height);
                Log("Corner", "RightSet", setpoint);
                break;
       } 
        
        


        switch (State)
        {
            case Initing:
                Fill(Closed);
                Dump(Closed);
                SetState(Holding);
                break;
            case Holding:
                //react slowly if already within deadband
                if( slowheight < (setpoint - HoldDeadBand)) //<500
                {
                    //Dont switch states till we've waited holdfOffTime
                    //if(IsTimedOut(HoldOffTime, HoldOff))
                    //{
                        SetState(Filling);
                        Fill(Open);
                        CycleTime = 1;
                    //}
                }
                else if(slowheight > (setpoint + HoldDeadBand)) //>524
                {
                    //Dont switch states till we've waited holfOffTime
                    //if(IsTimedOut(HoldOffTime, HoldOff))
                    //{
                        SetState(Dumping);
                        Dump(Open);
                        
                        CycleTime = 1;
                    //}
                }
                break;
            case Filling:
                //Respond quickly when filling
                if(height > (setpoint-DeadBand)) //>509
                {	
                    SetState(Holding);
                    Fill(Closed);
                    
                    //need to wait HoldOffTime till next state change
                    HoldOff  = millis();
                    
                    CycleTime = 100;
                    
                    //Over ride the filter, force it to the current value
                    //So the hold state doesn't keep changing
                    filter_reg = (height << FILTER_SHIFT);
                }
                break;
            case Dumping:
                //respond quickly when dumping
                if(height < (setpoint + DeadBand)) //<515
                {
                    SetState(Holding);
                    Dump(Closed);
                    
                    CycleTime = 100;
                    filter_reg = (height << FILTER_SHIFT);
                    
                    //need to wait HoldOffTime till next state change
                    HoldOff = millis();
                }
                break;
            default:
                Serial.println(">Corner,Default,State!!<");
                Fill(Closed);
                Dump(Closed);
                SetState(Holding);
        }
    }
}

