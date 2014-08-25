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
    bool AllDown(int *oldleft, int *oldright, int left, int right);
    bool AllUp(int *oldleft, int *oldright, int left, int right);
    bool IsRising(int *OldLeft, int *Oldright, int LRheight, int RRheight);
    void CalLED(bool on);
    void LISInit();
    void LISGetID();
    void LISGetXYZ();
    
    CCorner CornerL;
    CCorner CornerR;
    CEEprom EEProm;

    states_t state;

    uint32_t SampleTime;    

    //the height valuse that will level the coach to the horizon
    //when camping
    int16_t LeftLevel;
    int16_t RightLevel;

    //the current height readings
    int16_t LRheight;
    int16_t RRheight;
    int16_t SetPoint;
    int16_t Tilt;
    //int16_t LeftHeight;
    //int16_t RightHeight;
    int16_t LeftAuto;
    int16_t RightAuto;
    
    int16_t LTravelHeight;
    int16_t RTravelHeight; 
    
    modes_t mode;// = MANUALMODE;

    int16_t LeftLowLimit;
    int16_t LeftHighLimit;
    int16_t RightLowLimit;
    int16_t RightHighLimit;
    
   //timeouts for all up and all down
    uint32_t timeout;
    uint32_t LeftMinTime;
    uint32_t RightMinTime;
    uint32_t MoveTimeOut;
    int OldLeft;
    int OldRight;
};

#endif