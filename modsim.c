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

char cBf0 [SMALL_BUF_SZ], cBf1 [SMALL_BUF_SZ], cBf2 [SMALL_BUF_SZ];

#if !defined(ANTIFLOAT) 
	float fltTM, fltDIn, fltDOut;
#else
	QuasiFloatType qfltTM, qfltDIn, qfltDOut;

/*	int fltTM_i,fltTM_f,    fltDIn_i,fltDIn_f ,  fltDOut_i,fltDOut_f;
	int fltTM_power,    fltDIn_power,  fltDOut_power;
	char fltTM_sgn,    fltDIn_sgn,  fltDOut_sgn;
*/
#endif /* !defined(ANTIFLOAT) */

pTimepointType pTimeChain;


int main ()
{

	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("can't open \n");

		return P_ERROR;
	}

	while ( ! (feof (fp) ) ) 
	{
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		char * cpTmp = cBuf;

			printf("scanned: >> %s\n", cBuf);

#if !defined(ANTIFLOAT) 
			fltTM = fltDIn = fltDOut = 0.0f;
#else
			memset (&qfltTM, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDIn, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDOut, 0, sizeof (QuasiFloatType) ) ;
/*
			fltTM_i=fltTM_f=     fltDIn_i=fltDIn_f=   fltDOut_i=fltDOut_f = 0;
			fltTM_power=fltDIn_power=fltDOut_power=0;
			fltTM_sgn=fltDIn_sgn=fltDOut_sgn='?';
*/
#endif /* !defined(ANTIFLOAT) */

			/* memset (cBf0, 0, SMALL_BUF_SZ);
			memset (cBf1, 0, SMALL_BUF_SZ);
			memset (cBf2, 0, SMALL_BUF_SZ); */

			while (*cpTmp) {  if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
			//printf("changed: >> %s\n", cBuf);

#if !defined(ANTIFLOAT) 
			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
#else
			sscanf(cBuf, "%d.%de%c0%d %d.%de%c0%d %d.%de%c0%d,",
					&(qfltTM.integer),&(qfltTM.fraction),&(qfltTM.sgn),&(qfltTM.power),
					&(qfltDIn.integer),&(qfltDIn.fraction),&(qfltDIn.sgn),&(qfltDIn.power),
					&(qfltDOut.integer),&(qfltDOut.fraction),&(qfltDOut.sgn),&(qfltDOut.power)  );

#endif /* !defined(ANTIFLOAT) */

#if !defined(ANTIFLOAT) 
			printf(" parced : >>  <%f> <%f> <%f>\n", fltTM, fltDIn, fltDOut );
#else
			printf(" parced : >>  %d.%de%c0%d %d.%de%c0%d %d.%de%c0%d]\n",
					qfltTM.integer,qfltTM.fraction,qfltTM.sgn,qfltTM.power,
					qfltDIn.integer,qfltDIn.fraction,qfltDIn.sgn,qfltDIn.power,
					qfltDOut.integer,qfltDOut.fraction,qfltDOut.sgn,qfltDOut.power  );
#endif /* !defined(ANTIFLOAT) */

			//_EnrollPoint("_AppendPoint", &pTimeChain, &fltTM, &fltDIn, &fltDOut, "emty");
#if !defined(ANTIFLOAT) 
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "N/A");
#else
			EnrollPoint(&pTimeChain, &qfltTM, &qfltDIn, &qfltDOut, "N/A");
#endif /* !defined(ANTIFLOAT) */

			// usleep(500000);

		}
	}

	fclose(fp);

	printf("success 0 \n");

	//_ProcessPoints("_ProcessPoints", pTimeChain);
	//ProcessPoints(pTimeChain);

	printf("success 1 \n");

	//_DeletePoints("_DeletePoints", &pTimeChain);
	DeletePoints(&pTimeChain);

	printf("success 2 \n");

	return P_SUCCESS;
}
