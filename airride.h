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
#ifndef INCLUDE_AIRRIDE_H
#define INCLUDE_AIRRIDE_H

#include "debug.h"
#include "corner.h"
#include "common.h"
#include "gmceeprom.h"

class CAirRide
{
    public:
    CAirRide();
    
    void Init();
    void Run();
    
    private:    
    void SetState(states_t s);
    void CaclulateLevel();
    bool DumpTank();
    bool Calibrate();
    void GetMode();
    void CheckEvents();
    bool AllDown(int left, int right);
    bool AllUp(int left, int right);
    void CalLED(bool on);
    
    CCorner CornerLR;
    CCorner CornerRR;
    CEEprom EEProm;

    //const int PinTilt;
    //const int PinDumpTank;
    //const int Mode1;
    //const int Mode2;
    //const int CalPin;

    states_t state;

    uint32_t SampleTime;    
    uint32_t CalDoneTime;

    //the height valuse that will level the coach to the horizon
    //when camping
    int16_t LeftLevel;
    int16_t RightLevel;

    //the current height readings
    int16_t LRheight;
    int16_t RRheight;
    int16_t SetPoint;
    int16_t Tilt;
    int16_t LeftHeight;
    int16_t RightHeight;
    int16_t LeftAuto;
    int16_t RightAuto;
    
    modes_t mode;// = MANUALMODE;

    int16_t LeftLowLimit;
    int16_t LeftHighLimit;
    int16_t RightLowLimit;
    int16_t RightHighLimit;

};

#endif