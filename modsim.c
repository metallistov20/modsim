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

int main( )
{
	PortD_Prepare( );

	PortD_Toggle(  PD0 );

	while (1)
	{
		PortD_Toggle(  PD1 | PD0 );

		usleep (10);
	}
	return 0;
}




int _main ()
{
FILE *fp = NULL;

char cBuf [LARGE_BUF_SZ];

char cBf0 [SMALL_BUF_SZ], cBf1 [SMALL_BUF_SZ], cBf2 [SMALL_BUF_SZ];

float fltTM, fltDIn, fltDOut;

pTimepointType pTimeChain;

	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("can't open \n");

		return P_ERROR;
	}

/*

Function	Module Pin
LD0	30
LD1	29
LD2	28
LD3	27
LFLM	26
LLAP	25
LCLK	24
LACD	23 
*/

/*
TTL уровни "0" 0,4В и "1" 2,4В
Точнее - <0.4 и >2.4 сответственно

Как то раз посмотрел я документацию.
Питание действительно - 5 V
А вот данные - не более 3.6 V
Я и сам удивился, столь не удобной (на мой взгляд) конфигурацции.
Но это так.
Вот документация на один из жуков:
http://www.terraelectronica.ru/pdf/NSC/USBN9603-28.pdf

*/
	while ( ! (feof (fp) ) ) 
	{
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		char * cpTmp = cBuf;

			//printf("scanned: >> %s\n", cBuf);

			fltTM = fltDIn = fltDOut = 0.0f;

			/* memset (cBf0, 0, SMALL_BUF_SZ);
			memset (cBf1, 0, SMALL_BUF_SZ);
			memset (cBf2, 0, SMALL_BUF_SZ); */

			while (*cpTmp) {  if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
			//printf("changed: >> %s\n", cBuf);

			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
			//printf(" parced : >>  <%f> <%f> <%f>\n", fltTM, fltDIn, fltDOut );

			//_EnrollPoint("_AppendPoint", &pTimeChain, &fltTM, &fltDIn, &fltDOut, "emty");
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "emty");
			// usleep(500000);
		}
	}

	fclose(fp);

	printf("success 0 \n");

	//_ProcessPoints("_ProcessPoints", pTimeChain);
	ProcessPoints(pTimeChain);

	printf("success 1 \n");

	//_DeletePoints("_DeletePoints", &pTimeChain);
	DeletePoints(&pTimeChain);

	printf("success 2 \n");

	return P_SUCCESS;
}
