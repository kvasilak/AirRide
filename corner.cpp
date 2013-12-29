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



//This class is responsible for managing the height of a corner.
//There is one instance of this class for each corner of the vehicle usually 4
//This is where the decisions are made as to add or remove air from the suspension
CCorner::CCorner(Position p):
DeadBand(DEAD_BAND),//ADC counts 0-1024
HoldDeadBand(HOLD_DEAD_BAND),
CycleTime(100), //ms between updates
State(LastState),
PinDumpRightRear(10), //set IO pins here
PinDumpLeftRear(8),
PinFillRightRear(11),
PinFillLeftRear(7),
PinHeightLeftRear(A0),   //analog in
PinHeightRghtRear(A2),   //analog in
PinSetpoint(A3),          //analong in setpoint pot
HoldOffTime(2000)
{
	corner = p;

	LastTime 		= millis();
    HoldOff         = millis();
	filter_reg 		= GetHeight() << FILTER_SHIFT;

    pinMode(PinDumpRightRear, OUTPUT);
    digitalWrite(PinDumpRightRear, HIGH);
    
    pinMode(PinDumpLeftRear, OUTPUT);
    digitalWrite(PinDumpLeftRear, HIGH);
    
    pinMode(PinFillRightRear, OUTPUT);
    digitalWrite(PinFillRightRear, HIGH);
    
    pinMode(PinFillLeftRear, OUTPUT);
    digitalWrite(PinFillLeftRear, HIGH);
    
    //analog in pins need no setup
}

//return true if it's been period ms since start
bool CCorner::IsTimedOut(uint32_t period, uint32_t start)
{
  uint32_t current = millis();
  return(current - start > period);
}

//Get the height of this corner
int16_t CCorner::GetHeight()
{
    int height=0;
    
    switch(corner)
   {
        case LeftRear:
            height = 1024 - analogRead(PinHeightLeftRear); 
            break;
        case RightRear:
            height = 1024 - analogRead(PinHeightRghtRear);
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
            digitalWrite(PinFillLeftRear, state);
            break;
        case RightRear:
            digitalWrite(PinFillRightRear, state);
            break;
   }   
}

//open or close the dump solenoid for this corner
void CCorner::Dump(Solenoid state)
{
    switch(corner)
   {
        case LeftRear:
            digitalWrite(PinDumpLeftRear, state);
            break;
        case RightRear:
            digitalWrite(PinDumpRightRear, state);
            break;
   }   
}

void CCorner::PrintCorner()
{
    switch(corner)
   {
        case LeftRear:
            Serial.print(">Left,");
            break;
        case RightRear:
            Serial.print(">Right,");
            break;
   } 
}

void CCorner::SetState(ValveOp s)
{
    ValveOp laststate = Holding;
    const char *StateStrs[] = {"Fill", "Dump", "Hold"};

    PrintCorner();
    Serial.print("state,");
    Serial.print(StateStrs[s]);
    Serial.print("<");
    
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
	int32_t setpoint 		= 512; //analogRead(PinSetpoint); //GetHeight(Height) + tilt;
	int32_t height; 			//= GetHeight();

	//sample Slowly
	if(IsTimedOut(CycleTime, LastTime) )
	{
        height	= GetHeight();
        
		// Update filter with current sample.
		filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + height;

		LastTime = millis();
        
        // Scale output for unity gain.
        slowheight = (filter_reg >> FILTER_SHIFT);



        switch (State)
        {
            case Holding:
                //react slowly if already within deadband
                if( slowheight < (setpoint - HoldDeadBand)) //<500
                {
                    //Dont switch states till we've waited holfOffTime
                    if(IsTimedOut(HoldOffTime, HoldOff))
                    {
                        SetState(Filling);
                        Fill(Open);
                        CycleTime = 1;
                        
                        //Over ride the filter, force it to the current value
                        //filter_reg = ((height) << FILTER_SHIFT);
                    }
                }
                else if(slowheight > (setpoint + HoldDeadBand)) //>524
                {
                    //Dont switch states till we've waited holfOffTime
                    if(IsTimedOut(HoldOffTime, HoldOff))
                    {
                        SetState(Dumping);
                        Dump(Open);
                        
                        CycleTime = 1;
                        
                        //Over ride the filter, force it to the current value
                        //filter_reg = (height << FILTER_SHIFT);
                    }
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
                Serial.println("Default State!!");
                Fill(Closed);
                Dump(Closed);
                SetState(Holding);
        }
    }
}

