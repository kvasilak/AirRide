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
**    EEprom.h
**************************************************************************/
#ifndef INCLUDE_EEPROM_H
#define INCLUDE_EEPROM_H

#include <stdint.h>

class CEEprom
{
    public:
    CEEprom();
    
    void ReadEEprom();
    void WriteEEprom();
    
    int16_t GetLeftCal();
    int16_t GetRightCal();
    
};

#endif
