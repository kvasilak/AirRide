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
**    height.h
**************************************************************************/

#ifndef INCLUDE_HEIGHT_H
#define INCLUDE_HEIGHT_H

#include <stdint.h>

//defines height control parameters
class CHeight
{
public:

    int32_t FastHeight(Position p);
    int32_t SlowHeight(Position p);
    int32_t SetPoint(Position p);
    void Mode(states_t mode);
    void Run():
    
private:
    int32_t LeftFastHeight;
    int32_t LeftSlowHeight;
    int32_t LeftSetPoint;
    
    int32_t Left100[[100];
    int Left100Count;

    int32_t RightFastHeight;
    int32_t RightSlowHeight;
    int32_t RightSetPoint;
    
    int32_t Right100[100];
    int Right100Count;
    
};

#endif
