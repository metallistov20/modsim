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

#ifndef _DTASTR_H_
#define _DTASTR_H_

/* Old compiler does not operate such ANSI macros as __func__, et al */
#define __func__ "_func_"

/* List of D+(d.IN) and D-(d.OUT) values with relative time points */
typedef struct _TimepointType
{
	/* String to descibe this tm. point */
	char * pcMarquee;

	/* D- , yellow pin */
	float fltXval;

	/* D+ , blue pin */
	float fltYval;

	/* CVS's time stamp */
	float fltAbsTime;

	/* Next time point in the chain */
	struct _TimepointType * pNext; 

} TimepointType, *pTimepointType;


int _EnrollPoint(const char * caller, pTimepointType * ppThisPointChain, float * fltTm, float * fltX, float * fltY, char * pcMrq);

int _ProcessPoints(const char * caller, pTimepointType pPointChainPar);

void _DeletePoints(const char * caller, pTimepointType * ppThisPointChain);

#define EnrollPoint(x, y, s, t, u) _EnrollPoint(__func__, (x), (y), (s), (t), (u))

#define ProcessPoints(x) _ProcessPoints(__func__, (x))

#define DeletePoints(x) _DeletePoints(__func__, (x))


#endif /* _DTASTR_H_ */
