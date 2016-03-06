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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "modsim.h"
#include "dstruct.h"
#include "dport.h"


FILE *fp = NULL;

char cBuf [LARGE_BUF_SZ];

#if !defined(ANTIFLOAT) 
	float fltTM, fltDIn, fltDOut;
#else
	QuasiFloatType qfltTM, qfltDIn, qfltDOut;
#endif /* !defined(ANTIFLOAT) */

pTimepointType pTimeChain;

int iOldSec;

int main ()
{
	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("can't open \n");

		return P_ERROR;
	}

	printf("Loading USB-curve-data via NFS from file <%s>\n", FILE_NAME);

	while ( ! (feof (fp) ) ) 
	{
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		char * cpTmp = cBuf;

#if DEBUG_DATA
			printf("scanned: >> %s\n", cBuf);
#endif /* (DEBUG_DATA) */

#if !defined(ANTIFLOAT) 
			fltTM = fltDIn = fltDOut = 0.0f;
#else

			memset (&qfltTM, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDIn, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDOut, 0, sizeof (QuasiFloatType) ) ;

#endif /* !defined(ANTIFLOAT) */

			while (*cpTmp) {  if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
			//printf("changed: >> %s\n", cBuf);

#if !defined(ANTIFLOAT) 
			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
#else
			sscanf(cBuf, "%d.%de%c0%d %d.%de%c0%d %d.%de%c0%d,",
					&(qfltTM.integer),&(qfltTM.fraction),&(qfltTM.sgn),&(qfltTM.power),
					&(qfltDIn.integer),&(qfltDIn.fraction),&(qfltDIn.sgn),&(qfltDIn.power),
					&(qfltDOut.integer),&(qfltDOut.fraction),&(qfltDOut.sgn),&(qfltDOut.power)  );

if (0 == qfltTM.power) if (iOldSec!= qfltTM.integer){iOldSec=qfltTM.integer; printf("sec: %d; ", iOldSec); fflush(stdout); }

#endif /* !defined(ANTIFLOAT) */

#if DEBUG_DATA
#if !defined(ANTIFLOAT) 
			printf(" parced : >>  <%f> <%f> <%f>\n", fltTM, fltDIn, fltDOut );
#else
			printf(" parced : >>  %d.%de%c0%d %d.%de%c0%d %d.%de%c0%d]\n",
					qfltTM.integer,qfltTM.fraction,qfltTM.sgn,qfltTM.power,
					qfltDIn.integer,qfltDIn.fraction,qfltDIn.sgn,qfltDIn.power,
					qfltDOut.integer,qfltDOut.fraction,qfltDOut.sgn,qfltDOut.power  );
#endif /* !defined(ANTIFLOAT) */
#endif /* (DEBUG_DATA) */

#if !defined(ANTIFLOAT) 
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "N/A");
#else
			EnrollPoint(&pTimeChain, &qfltTM, &qfltDIn, &qfltDOut, "N/A");
#endif /* !defined(ANTIFLOAT) */
		}
	}

	fclose(fp);

	printf("Issuing USB-curve-data on Pin #0 Port 'D'\n");

	PortD_Prepare( );
	ProcessPoints(pTimeChain);

	printf("Disposing memory allocations\n");

	DeletePoints(&pTimeChain);

	printf("Done (success) \n"); fflush(stdout);

	return P_SUCCESS;
}
