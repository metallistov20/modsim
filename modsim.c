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

#if !defined(QUASIFLOAT) 
	float fltTM, fltDIn, fltDOut;
#else
	QuasiFloatType qfltTM, qfltDIn, qfltDOut;
#endif /* !defined(QUASIFLOAT) */

pTimepointType pTimeChain;

int iOldSec;

int main ()
{
	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("[%s] %s: can't open file <%s> \n", __FILE__, __func__ , FILE_NAME);

		return P_ERROR;
	}

	printf("[%s] %s: loading USB-curve-data via NFS from file <%s>\n", __FILE__, __func__, FILE_NAME);

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
			printf("[%s] %s: scanned: < %s >\n", __FILE__, __func__, cBuf);
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			fltTM = fltDIn = fltDOut = 0.0f;
#else

			memset (&qfltTM, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDIn, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDOut, 0, sizeof (QuasiFloatType) ) ;

#endif /* !defined(QUASIFLOAT) */

			/* In the string obtained */
			while (*cpTmp)

				/* replace all commas with spaces, to let the <scanf()> parse it */
				{ if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
	

#if !defined(QUASIFLOAT) 
			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
#else
			sscanf(cBuf, "%d.%de%c0%d %d.%de%c0%d %d.%de%c0%d,",
					&(qfltTM.integer),&(qfltTM.fraction),&(qfltTM.sgn),&(qfltTM.power),
					&(qfltDIn.integer),&(qfltDIn.fraction),&(qfltDIn.sgn),&(qfltDIn.power),
					&(qfltDOut.integer),&(qfltDOut.fraction),&(qfltDOut.sgn),&(qfltDOut.power)  );

if (0 == qfltTM.power) if (iOldSec!= qfltTM.integer){iOldSec=qfltTM.integer; printf("sec: %d; ", iOldSec); fflush(stdout); }

#endif /* !defined(QUASIFLOAT) */

#if DEBUG_DATA
#if !defined(QUASIFLOAT) 
			printf("[%s] %s: parsed :  <%f> <%f> <%f>\n", __FILE__, __func__, fltTM, fltDIn, fltDOut );
#else
			printf("[%s] %s: parsed :  <%d.%de%c0%d>  <%d.%de%c0%d>  <%d.%de%c0%d> \n",
					__FILE__, __func__, 
					qfltTM.integer,qfltTM.fraction,qfltTM.sgn,qfltTM.power,
					qfltDIn.integer,qfltDIn.fraction,qfltDIn.sgn,qfltDIn.power,
					qfltDOut.integer,qfltDOut.fraction,qfltDOut.sgn,qfltDOut.power  );
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "N/A");
#else
			EnrollPoint(&pTimeChain, &qfltTM, &qfltDIn, &qfltDOut, "N/A");
#endif /* !defined(QUASIFLOAT) */
		}
	}

	fclose(fp);

	printf("\n[%s] %s: issuing USB-curve-data on Pin #0 Port 'D'\n", __FILE__, __func__);

	PortD_Prepare( );
	ProcessPoints(pTimeChain);

	printf("\n[%s] %s: disposing memory allocations\n", __FILE__, __func__);

	DeletePoints(&pTimeChain);

	printf("[%s] %s: done (success) \n", __FILE__, __func__); fflush(stdout);

	return P_SUCCESS;
}
