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
#include "common.h"


//This class is responsible for managing the height of a corner.
//There is one instance of this class for each corner of the vehicle usually 4
//This is where the decisions are made as to add or remove air from the suspension
CCorner::CCorner()://Position p):
  DeadBand(DEAD_BAND),
  CycleTime(100), //ms between updates
  State(Initing),
  PulseTime(250),
  LimitLow(100),            // set limits to 100 from high and low
  LimitHigh(924),
  LongFilter(false),
  IsAtHeight(false)
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
    int i;
    int32_t height = GetHeight();
    
    //initalize filters
    for(i=0; i<10; i++)
    {
        SmoothAvg[i] = height;      
    }
    
    for(i=0; i<100; i++)
    {
        HeightAvg[i] = height;      
    }
    
    filter_reg = (height << FILTER_SHIFT);

    count = 0;
    corner = p;
    SlowAt = 0;
    SmoothAt = 0;
    
    HeightSpeed = 0;
    OldHeight = height;
    SpeedTime = millis();
}
void CCorner::Limits(int16_t Low, int16_t high)
{
    LimitLow = Low +2;
    LimitHigh = high -2;
}

//Get the height of this corner
int32_t CCorner::GetHeight()
{
    int32_t height=0;
    
    switch(corner)
   {
        case LeftRear:
            height = (int32_t)analogRead(PINLRHEIGHT); 
            break;
        case RightRear:
            height = (int32_t)analogRead(PINRRHEIGHT);
            break;
   }   
   
   //calculate speed, counts/second
   if(IsTimedOut(1000, SpeedTime))
   {
        if(height > OldHeight)
        {
            HeightSpeed = height - OldHeight;
        }
        else
        {
            HeightSpeed = OldHeight - height;
        }
        
        OldHeight = height;
        SpeedTime = millis();
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
            Serial.print(F("Left,"));
            break;
        case RightRear:
            Serial.print(F("Right,"));
            break;
   } 
}

void CCorner::SetState(ValveOp s)
{
    const char s0[] PROGMEM = "Initing";
    const char s1[] PROGMEM = "Filling";
    const char s2[] PROGMEM = "FillPulse";
    const char s3[] PROGMEM = "Dumping";
    const char s4[] PROGMEM = "DumpPulse";
    const char s5[] PROGMEM = "HoldEntry";
    const char s6[] PROGMEM = "Holding";
    const char s7[] PROGMEM = "LastState";

    
    const char * const StateStrs[] PROGMEM = {s0, s1, s2, s3, s4, s5, s6, s7};
    
    char buffer[15];
    strcpy_P(buffer, (char*)pgm_read_word(&(StateStrs[s]))); // copy strings out of program space
    
    ValveOp laststate = Holding;

    Serial.print(F(">Corner,"));
    PrintCorner();
    Serial.print(buffer);
    Serial.println(F("<"));
    
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

uint16_t CCorner::Average(uint16_t value)
{
    uint32_t average = 0;
    
    //rollover if we hit the end
    if(SlowAt > 99) SlowAt = 0;
    
    //add new value to array
    HeightAvg[SlowAt] = value;
    
    SlowAt++;
    
    //Calculate new average
    for(int i=0; i<100; i++)
    {
        average += HeightAvg[i];
    }
    
    return (uint16_t)(average/100);
}

uint16_t CCorner::Smooth(uint16_t value)
{
    uint32_t average = 0;
    
    //rollover if we hit the end
    if(SmoothAt > 9) SmoothAt = 0;
    
    //add new value to array
    SmoothAvg[SmoothAt] = value;
    
    SmoothAt++;
    
    //Calculate new average
    for(int i=0; i<10; i++)
    {
        average += SmoothAvg[i];
    }
    
    return (uint16_t)(average/10);
}


void CCorner::SetLongFilter(bool slow)
{
    //reset filters if switching to long filter
    if((false == LongFilter) && (true == slow))
    {
        for(int i=0; i<100; i++)
        {
            HeightAvg[i] = SmoothHeight;      
        }
    }
    
    //Always Reset IIR filter
    filter_reg = (SmoothHeight << FILTER_SHIFT);
    
    LongFilter = slow;
}

void CCorner::AtHeight(bool at)
{
    IsAtHeight = at;
}

bool CCorner::AtHeight()
{
    return IsAtHeight;
}

//Uses the low pass IIR filter described in "Simple Software Lowpass Filter.pdf"
//And two simple FIR filters
void CCorner::DoHeight(int32_t height, int32_t setpoint)
{
	//sample Slowly
	if(IsTimedOut(CycleTime, LastTime) )
	{        
        if( LongFilter)
        {
            //fast smoothing filter to take out sampling noise
            SmoothHeight = Smooth(height);
            
            if(count >9)
            {
                count = 0;
            
                //Longer filter to take out bumps and roll
                AverageHeight = Average( SmoothHeight);

                //IIR filter to take out even more bump and roll
                // Update IIR filter with current sample.
                filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + AverageHeight;

                // Scale output for unity gain.
                slowheight = (filter_reg >> FILTER_SHIFT);
            }
            
            count++;
        }
        else
        {
            SmoothHeight = height;
            
            filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + height;

            // Scale output for unity gain.
            slowheight = (filter_reg >> FILTER_SHIFT);
        }

		LastTime = millis();        
        
        if(IsTimedOut(250, UpdateTime))
        {
           switch(corner)
           {
                case LeftRear:
                    Serial.print(F(">Corner, LError"));
                    Serial.print(height - setpoint);
                    Serial.println(F("<"));

                    Serial.print(F(">Corner, LeftHeight"));
                    Serial.print(height);
                    Serial.println(F("<"));

                    Serial.print(F(">Corner, LeftSlow"));
                    Serial.print(slowheight - setpoint);
                    Serial.println(F("<"));

                    break;
                case RightRear:
                    Serial.print(F(">Corner, RError"));
                    Serial.print(height - setpoint);
                    Serial.println(F("<"));

                    Serial.print(F(">Corner, RightHeight"));
                    Serial.print(height);
                    Serial.println(F("<"));

                    Serial.print(F(">Corner, RightSlow"));
                    Serial.print(slowheight - setpoint);
                    Serial.println(F("<"));                

                    break;
           } 
           UpdateTime = millis();
        }
    }
}



void CCorner::Run(int32_t setpoint)
{		
    int i;
    int32_t height 			= GetHeight();
    
    //prevent setpoint from exceeding cal limits
    if(setpoint > LimitHigh)
    {
        setpoint = LimitHigh;
    }
    else if(setpoint < LimitLow) 
    {
        setpoint = LimitLow;
    } 
    
    DoHeight(height, setpoint);

    switch (State)
    {
        case Initing:
            Fill(Closed);
            Dump(Closed);
            SetState(Holding);
            //filter_reg = (height << FILTER_SHIFT);
            HoldOffTime = millis();
            break;
        case HoldEntry:
            if(IsTimedOut(1000, HoldOffTime))
            {
                //Over ride the filter, force it to the current value
                //So the hold state doesn't keep changing
                filter_reg = (height << FILTER_SHIFT);
                
                for(i=0; i<100; i++)
                {
                    HeightAvg[i] = height;      
                }

                HoldOffTime = millis();
                DoPulse = false;
                CycleTime = 100;
                SetState(Holding);
            }
            break;
        case Holding:
            //react slowly if already within deadband
            
            //below setpoint
            if( slowheight < (setpoint - DeadBand))
            {
                //minimum hold time so we don't go crazy hunting
                //except in manual mode, react fast!
                if(!LongFilter || IsTimedOut(5000, HoldOffTime))
                {
                    SetState(Filling);
                    Fill(Open);
                    CycleTime = 100;
                    
                    //if within 5x deadband, only pulse the valve
                    //so we don't over shoot the setpoint due to the long lag time
                    
                    if(height > (setpoint -(DeadBand * 5)) )
                    {                        
                        //calc total pulse time as a multiple of deadbands from setpoint
                        //we know height < setpoint or we wouldn't be here
                        PulseTotal =(abs(setpoint - height) / DeadBand) * PulseTime; // pulsetime = 250ms
                        PulseStart = millis();

                        SetState(FillPulse);
                    }
                }
            }
            else if(slowheight > (setpoint + DeadBand)) //>524
            {
                if(!LongFilter || IsTimedOut(5000, HoldOffTime))
                {
                    SetState(Dumping);
                    Dump(Open);
                    
                    CycleTime = 100;
                    
                    //if within 5x deadband, only pulse the valve
                    //so we don't over shoot the setpoint due to the long lag time
                    
                    if(height < setpoint + (DeadBand * 5))
                    {                        
                        //calc total pulse time as a multiple of deadbands from setpoint
                        //we know height > setpoint or we wouldn't be here
                        PulseTotal = (abs(height - setpoint) / DeadBand) * PulseTime; // pulsetime = 250ms
                        PulseStart = millis();

                        SetState(DumpPulse);
                    }
                }
            }
            else
            {
                 //might not be perfectly at height, but should be pretty close
                 IsAtHeight = true;
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
            //Serial.println(">Corner,Default,State!!<");
            Fill(Closed);
            Dump(Closed);
            SetState(HoldEntry);
            HoldOffTime = millis();
    }
}

//used to calibrate height limits.
//the coach may not have travel from 0 to 1024
//wait for both heights to be all the way down
//determined by the height changing less that 2 counts in 1 second
//Waits 10 seconds for the cases where it is slow to start or stop moving
bool CCorner::IsMoving()
{
    bool ret = true;

    //Serial.print(">AirRide,msg,speed; ");
    //Serial.print(HeightSpeed);
    //Serial.println("<");
    
    if(HeightSpeed > 1)
    {
        ret =  true;
    }
    else
    {
        ret =  false;
    }
    return ret;
}




