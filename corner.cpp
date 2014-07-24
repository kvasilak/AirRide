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
  PulseTime(250),
  LimitLow(100),            // set limits to 100 from high and low
  LimitHigh(924)
{

	LastTime 		= millis();
	filter_reg 		= GetHeight() << FILTER_SHIFT;

    pinMode(PINDUMP_RIGHTREAR, OUTPUT);
    digitalWrite(PINDUMP_RIGHTREAR, LOW);
    
    pinMode(PINDUMP_LEFTREAR, OUTPUT);
    digitalWrite(PINDUMP_LEFTREAR, LOW);
    
    pinMode(PINFILL_RIGHTREAR, OUTPUT);
    digitalWrite(PINFILL_RIGHTREAR, LOW);
    
    pinMode(PINFILL_LEFTREAR, OUTPUT);
    digitalWrite(PINFILL_LEFTREAR, LOW);
    
    UpdateTime = millis();
    
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
int32_t CCorner::GetHeight()
{
    int height=0;
    
    switch(corner)
   {
        case LeftRear:
            height = (int32_t)analogRead(PINLRHEIGHT); 
            break;
        case RightRear:
            height = (int32_t)analogRead(PINRRHEIGHT);
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

void CCorner::FillExit()
{
    Fill(Closed);
    SetState(HoldEntry);
    HoldOffTime = millis();
}

void CCorner::DumpExit()
{
    Dump(Closed);
    SetState(HoldEntry);
    HoldOffTime = millis();
}

//Uses the low pass filter described in "Simple Software Lowpass Filter.pdf"
void CCorner::Run(int32_t setpoint)
{
	int32_t slowheight; 		
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

        if(IsTimedOut(250, UpdateTime))
        {
           switch(corner)
           {
                case LeftRear:
                    Log("Corner", "LError", height - setpoint);
                    Log("Corner", "LeftHeight", height);
                    Log("Corner", "LeftSlow", slowheight - setpoint);
                    break;
                case RightRear:
                    Log("Corner", "RError", height - setpoint);
                    Log("Corner", "RightHeight", height);
                    Log("Corner", "RightSlow", slowheight - setpoint);
                    break;
           } 
           UpdateTime = millis();
        }
        


        switch (State)
        {
            case Initing:
                Fill(Closed);
                Dump(Closed);
                SetState(Holding);
                filter_reg = (height << FILTER_SHIFT);
                HoldOffTime = millis();
                break;
            case HoldEntry:
                if(IsTimedOut(1000, HoldOffTime))
                {
                    //Over ride the filter, force it to the current value
                    //So the hold state doesn't keep changing
                    //maybe not....filter_reg = (height << FILTER_SHIFT);
                    HoldOffTime = millis();
                    DoPulse = false;
                    CycleTime = 250;
                    SetState(Holding);
                }
                break;
            case Holding:
                //react slowly if already within deadband
                
                //below setpoint
                if( slowheight < (setpoint - HoldDeadBand))
                {
                    //minimum hold time so we don't go crazy hunting
                    if(IsTimedOut(5000, HoldOffTime))
                    {
                        SetState(Filling);
                        Fill(Open);
                        CycleTime = 10;
                        
                        //if within 5x deadband, only pulse the valve
                        //so we don't over shoot the setpoint due to the long lag time
                        
                        if(height > (setpoint -(DeadBand * 5)) )
                        {                        
                            //calc total pulse time as a multiple of deadbands from setpoint
                            //we know height < setpoint or we wouldn't be here
                            PulseTotal = ((setpoint - height) / DeadBand) * PulseTime; // pulsetime = 250ms
                            PulseStart = millis();

                            SetState(FillPulse);
                        }
                    }
                }
                else if(slowheight > (setpoint + HoldDeadBand)) //>524
                {
                    if(IsTimedOut(5000, HoldOffTime))
                    {
                        SetState(Dumping);
                        Dump(Open);
                        
                        CycleTime = 10;
                        
                        //if within 5x deadband, only pulse the valve
                        //so we don't over shoot the setpoint due to the long lag time
                        
                        if(height < setpoint + (DeadBand * 5))
                        {                        
                            //calc total pulse time as a multiple of deadbands from setpoint
                            //we know height > setpoint or we wouldn't be here
                            PulseTotal = ((height - setpoint) / DeadBand) * PulseTime; // pulsetime = 250ms
                            PulseStart = millis();

                            SetState(DumpPulse);
                        }
                    }
                }
                break;
            case FillPulse:
                if(IsTimedOut(PulseTotal, PulseStart))
                {
                    FillExit();
                }
                break;
            case Filling:
                if(height >= (setpoint-DeadBand))
                {    
                    FillExit();
                }
                break;
            case DumpPulse:
                if(IsTimedOut(PulseTotal, PulseStart))
                {
                    DumpExit();
                }
                break;
            case Dumping:
                if(height <= (setpoint + DeadBand))
                {    
                    DumpExit();
                }
                break;

            default:
                Serial.println(">Corner,Default,State!!<");
                Fill(Closed);
                Dump(Closed);
                SetState(HoldEntry);
                HoldOffTime = millis();
        }
    }
}

