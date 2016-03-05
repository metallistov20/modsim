/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

#ifndef _DPORT_H_
#define _DPORT_H_

/*
Function	Module Pin
LD0		30
LD1		29
LD2		28
LD3		27
*/
#define PD0 0x80
#define PD1 0x40
#define PD2 0x20
#define PD3 0x10
#define PD0orPD1 (PD0 | PD1)

/*
LFLM		26
LLAP		25
LCLK		24
LACD		23 
*/
#define FLM 0x08
#define LAP 0x04
#define LCK 0x02
#define ACD 0x01

#endif /* _DPORT_H_ */


