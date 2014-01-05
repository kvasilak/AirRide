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
**    Airride.cpp
**************************************************************************/
#include <Arduino.h>
#include "corner.h"
#include "debug.h"
#include "common.h"
#include "eeprom.h"
#include "airride.h"


#define MODULE "Main"
#define MAINSTATE "MainState"

CAirRide::CAirRide() :
PinTilt(5),
PinDumpTank(A5),
Mode1(5),
Mode2(6)
{

}

void CAirRide::Init()
{
    Serial.println("*******GMC Air Ride Controller V1.0*********");

    SampleTime = millis();

    SetState(RUNMANUAL);

    pinMode(2, INPUT_PULLUP);
  
    CornerLR.Init(LeftRear); 
    CornerRR.Init(RightRear);
  
}

void CAirRide::SetState(states_t s)
{
    static states_t laststate = DUMPINGTANK;
    
    if(s != laststate)
    {
        //Log(MODULE, "MainState", StateStrs[s]);
        laststate = s;
    }
}

//calculate the Left and right height values that will make the coach level
void CAirRide::CaclulateLevel()
{
//todo...implement it!
//handle not enough suspesion rage issues
//handle travel limits, ranges stored in EEPROM
    LeftLevel = 512;
    RightLevel = 512;
}

//input is analog
//if at zero ( <342) it it means calibrate
//if at 1/2 ( 342><682 ) it means dump tanks
//if > 682 it means nothing is pressed
bool CAirRide::DumpTank()
{
    int a = analogRead(A4);
    bool pressed = false;
    
    if( ( a > 342) && (a < 682) )
    {
        pressed = true;
    }
}

//only called when appropriate
//button must transition as follows
//not pressed
//pressed for 5 seconds
//released
//once released were in calibrate mode
bool CAirRide::Calibrate()
{
    static int calstate = 0;
    bool docal = false;
    bool pressed = false;
    uint32_t pressstart;
    
    if(analogRead(A4) < 342)
    {
        pressed = true;
    }
    
    switch(calstate)
    {
        case 0: //unpressed
            if(!pressed)
            {
                pressstart = millis();
                calstate++;
            }
            break;
        case 1: //pressed
            if(pressed)
            {
                if(IsTimedOut(5000, pressstart))
                {
                    calstate++;
                }
            }
            else
            {
                //user let go too soon
                calstate = 0;
            }
            break;
        case 2: //wait for release
            if(!pressed)
            {
                docal = true;
                calstate=0;
            }
            break;
    }
    
    return docal;
}

void CAirRide::GetMode()
{
    int m = digitalRead(Mode1);
    
    m &= digitalRead(Mode2) << 2;
    
    mode = (states_t)m;
}

//read all the inputs and change states accordingly
void CAirRide::CheckEvents()
{
    GetMode();
    
    switch(state)
    {
        //only switch states if in these states;
        case RUNTRAVEL:
            switch(mode)
            {
                case MANUALMODE:
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                //No mode change
                case TRAVELMODE:
                //find the high and low limits
                if(Calibrate())
                {
                    SetState(CALLIMITS);
                }
                //what are the chances of dumptank and calibrate being pressed at the exact same time?
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNMANUAL:
            switch(mode)
            {
                case TRAVELMODE:
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                case MANUALMODE:
                //save the current height as the travel height
                if(Calibrate())
                {
                    EEProm.SaveLeftHeight(LeftHeight); 
                    EEProm.SaveRightHeight(RightHeight);
                    EEProm.WriteEEprom();
                    SetState(CALDONE);
                }
                
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNAUTO:
            switch(mode)//no calibration to do
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                case AUTOMODE:
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNAUTOCAL:
            switch(mode)
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOMODE:
                SetState(mode);
                break;
                case AUTOCALMODE:
                //Save the accelerometer values for level
                if(Calibrate())
                { 
                    EEProm.SaveLeftAuto(LeftAuto);
                    EEProm.SaveRightAuto(RightAuto);
                    SetState(CALDONE);
                }
                
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        //no mode change allowed until the user lets go of the dump button, or we'd just end up back here
        case DUMPTANK:
        case DUMPINGTANK:
        //no change allowed during limit calibration
        case CALLIMITS: 
        case CALLOW:
        case CALHIGH:
        case CALDONE:
        break; 
        //now back to our current mode
        case CALCOMPLETE:
            switch(mode)
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
            }
        break;
    }   
}

void CAirRide::Run() 
{
    LRheight = 1024 - analogRead(A0);
    RRheight = 1024 - analogRead(A2);
    SetPoint = analogRead(A3);
    Tilt = analogRead(A1)-512;
        
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    if(IsTimedOut(100, SampleTime))
    {      
        Log(MODULE, "LRHeight", LRheight);
        Log(MODULE, "RRHeight", RRheight);
        Log(MODULE, "SetPoint", SetPoint);
        Log(MODULE, "Tilt", Tilt);
       
        SampleTime = millis();
    }

    CheckEvents();
    
    switch(state)
    {
        //Manual leveling
        case RUNMANUAL: 
            CornerLR.Run(0-Tilt);  
            CornerRR.Run(Tilt);
            break;
            
        //Run at the calibrated travel height
        //todo change update frequency once car moves
        case RUNTRAVEL:
            CornerLR.Run(EEProm.GetLeftCal());  
            CornerRR.Run(EEProm.GetRightCal());  
            break;
            
        //Auto level to the horizon
        case RUNAUTO:
            //read accel
            CaclulateLevel();
            CornerLR.Run(LeftLevel);  
            CornerRR.Run(RightLevel);
            break;
            
        //manually level the coach to the horizon
        //then press caibrate to save the acellerometer values
        //to use for autoleveling
        case RUNAUTOCAL:
            CornerLR.Run(0-Tilt);  
            CornerRR.Run(Tilt);
            break;
            
        //open all valves and dump all air from the system
        //as long as switch is closed
        case DUMPTANK:
            //open all valves to dump all air from system
            CornerLR.Fill(Open);
            CornerLR.Dump(Open);
        
            CornerRR.Fill(Open);
            CornerRR.Dump(Open);
            
            SetState(DUMPINGTANK);
            break;
            
         case DUMPINGTANK:
            //wait for user to release button, then close all valves
            if(!DumpTank())
            {
                CornerLR.Fill(Closed);
                CornerLR.Dump(Closed);
            
                CornerRR.Fill(Closed);
                CornerRR.Dump(Closed);
                
                //allow the event handler to switch us back to the current state
                SetState(CALCOMPLETE);
            }
            break;
            
        //raise and lower the coach to find the upper and lower limits of travel 
        //must be in travelmode before pressing cal button
        case CALLIMITS:
            SetState(CALLOW);
            break;
        case CALLOW:
            SetState(CALHIGH);
            break;
        case CALHIGH:
            CalDoneTime = millis();
            SetState(CALDONE);
            break;
        
         //done with cal, show cal LED for 1 sec
        case CALDONE:
            //can we light up an LED on entry??
            if(IsTimedOut(1000, CalDoneTime))
            {
                SetState(CALCOMPLETE);
            }
            break;
        //wait for user to release cal button
        case CALCOMPLETE:
            break;
        default:
            SetState(RUNMANUAL);
    }
  
}
