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
#include <string.h>
#include <stdlib.h>

#include "dstruct.h"
#include "dport.h"

//int iOldSec;
int iOldSecPRC;

int _EnrollPoint(const char * caller, pTimepointType * ppThisPointChain, 
#if !defined(ANTIFLOAT) 
	float * pfltTm, float * pfltX, float * pfltY, 
#else
	pQuasiFloatType pqfltTm, pQuasiFloatType pqfltX, pQuasiFloatType pqfltY, 
#endif /* !defined(ANTIFLOAT) */
	char * pcMrq)

{
pTimepointType pChild, pTempPointChain;

	if (NULL == *ppThisPointChain)
	{
		/* only one chain, for beginning */
		*ppThisPointChain = (pTimepointType) calloc ( 1, sizeof (TimepointType) );

		/* check if successful */
		if (NULL == *ppThisPointChain)
		{
#if !defined(ANTIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. %f: [X(%f),Y(%f)]  \n",
			__FILE__, caller, __func__,
			*pfltTm, *fltpX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. \n",
			__FILE__, caller, __func__);
#endif /* !defined(ANTIFLOAT) */

			return (-8);
		}

#if !defined(ANTIFLOAT)
		(*ppThisPointChain)->fltXval = *pfltX;
		(*ppThisPointChain)->fltYval = *pfltY;
		(*ppThisPointChain)->fltAbsTime = *pfltTm;
#else
		memcpy(& ((*ppThisPointChain)->qfltXval), pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltYval), pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );
#endif /* !defined(ANTIFLOAT) */

		(*ppThisPointChain)->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( (*ppThisPointChain)->pcMarquee, pcMrq);

#if DEBUG_DATA
#if !defined(ANTIFLOAT)
		printf("[%s] %s:%s : FIRST <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			(*ppThisPointChain)->fltAbsTime,
			(*ppThisPointChain)->fltXval,
			(*ppThisPointChain)->fltYval,
			(*ppThisPointChain)->pcMarquee
		);
#else
		printf("[%s] %s:%s : FIRST <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			(*ppThisPointChain)->qfltAbsTime.integer,(*ppThisPointChain)->qfltAbsTime.fraction,
			(*ppThisPointChain)->qfltAbsTime.sgn,(*ppThisPointChain)->qfltAbsTime.power,

			(*ppThisPointChain)->qfltXval.integer,(*ppThisPointChain)->qfltXval.fraction,
			(*ppThisPointChain)->qfltXval.sgn,(*ppThisPointChain)->qfltXval.power,

			(*ppThisPointChain)->qfltYval.integer,(*ppThisPointChain)->qfltYval.fraction,
			(*ppThisPointChain)->qfltYval.sgn,(*ppThisPointChain)->qfltYval.power,

			(*ppThisPointChain)->pcMarquee
		);
#endif /* !defined(ANTIFLOAT) */

#endif /* (DEBUG_DATA) */

	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisPointChain;

		pTempPointChain = (pTimepointType) calloc (1, sizeof (TimepointType) );

		if (NULL == pTempPointChain)
		{
#if !defined(ANTIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element. %f: [X(%f),Y(%f)]  \n", 
			__FILE__, caller, __func__,
			*pfltTm, *pfltX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element.\n", 
			__FILE__, caller, __func__);
#endif /* !defined(ANTIFLOAT) */

			return (-7);
		}

#if !defined(ANTIFLOAT)
		pTempPointChain->fltXval = *pfltX;
		pTempPointChain->fltYval = *pfltY;
		pTempPointChain->fltAbsTime = *fltTm;
#else
		memcpy(& ( pTempPointChain->qfltXval), 	pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltYval), 	pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );
#endif /* !defined(ANTIFLOAT) */

		pTempPointChain->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( pTempPointChain->pcMarquee, pcMrq);

#if DEBUG_DATA
#if !defined(ANTIFLOAT)
		printf("[%s] %s:%s : NEXT <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pTempPointChain->fltAbsTime,
			pTempPointChain->fltXval,
			pTempPointChain->fltYval,
			pTempPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : NEXT <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			pTempPointChain->qfltAbsTime.integer,pTempPointChain->qfltAbsTime.fraction,
			pTempPointChain->qfltAbsTime.sgn,pTempPointChain->qfltAbsTime.power,

			pTempPointChain->qfltXval.integer,pTempPointChain->qfltXval.fraction,
			pTempPointChain->qfltXval.sgn,pTempPointChain->qfltXval.power,

			pTempPointChain->qfltYval.integer,pTempPointChain->qfltYval.fraction,
			pTempPointChain->qfltYval.sgn,pTempPointChain->qfltYval.power,

			pTempPointChain->pcMarquee
		);
#endif /* !defined(ANTIFLOAT) */
#endif /* (DEBUG_DATA) */

		/* Skip everything, except last entry */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* . . */
			pChild = pChild->pNext;
		}

		/* before while we've already ensured that next chunk was created allright */

		/* attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempPointChain;

	}
	return (0);
}

#define NPROC "not processed"
#define MIN_THLD (-1.0)
#define MAX_THLD (6.0)

int _ProcessPoints(const char * caller, pTimepointType pPointChainPar)
{
pTimepointType pPointChain = pPointChainPar;

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{
#if DEBUG_DATA
#if !defined(ANTIFLOAT)
		printf("[%s] %s:%s : <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pPointChain->fltAbsTime,
			pPointChain->fltXval,
			pPointChain->fltYval,
			pPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			pPointChain->qfltAbsTime.integer,pPointChain->qfltAbsTime.fraction,
			pPointChain->qfltAbsTime.sgn,pPointChain->qfltAbsTime.power,

			pPointChain->qfltXval.integer,pPointChain->qfltXval.fraction,
			pPointChain->qfltXval.sgn,pPointChain->qfltXval.power,

			pPointChain->qfltYval.integer,pPointChain->qfltYval.fraction,
			pPointChain->qfltYval.sgn,pPointChain->qfltYval.power,

			pPointChain->pcMarquee
		);
#endif /* !defined(ANTIFLOAT) */
#endif /* (DEBUG_DATA) */

/*
TTL уровни "0" 0,4В и "1" 2,4В
Точнее - <0.4 и >2.4 сответственно

Как то раз посмотрел я документацию.
Питание действительно - 5 V
А вот данные - не более 3.6 V
Я и сам удивился, столь не удобной (на мой взгляд) конфигурацции. Но это так.

USBN9603-28.pdf

*/
if (0 == pPointChain->qfltAbsTime.power)
if (iOldSecPRC!= pPointChain->qfltAbsTime.integer)
{iOldSecPRC=pPointChain->qfltAbsTime.integer; printf("secPRC: %d; ", iOldSecPRC); fflush(stdout); }

//		PortD_Toggle( PD0 );

#if !defined(ANTIFLOAT)
		if (pPointChain->fltXval <= MIN_THLD) 
#else
		if ( ('-' == pPointChain->qfltXval.sgn) && (2 == pPointChain->qfltXval.power) 
		&& (4 == pPointChain->qfltXval.integer || (3 == pPointChain->qfltXval.integer && 0 != pPointChain->qfltXval.fraction) )  )
#endif /* !defined(ANTIFLOAT) */

			PortD_Down( PD0 );

		else
#if !defined(ANTIFLOAT)
			if (pPointChain->fltXval >= MAX_THLD) 
#else
			if ( ('-' == pPointChain->qfltXval.sgn) && (2 < pPointChain->qfltXval.power) )
#endif /* !defined(ANTIFLOAT) */

				PortD_Up( PD0 );
			else
			{
				pPointChain->pcMarquee = calloc (1, strlen (NPROC) +1 );
				strcpy( pPointChain->pcMarquee, NPROC);
/*
				printf("[%s] %s:%s :  <%s> \n", __FILE__, caller, __func__,
					pPointChain->pcMarquee	);
*/
			}



		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;
	}

	return 0;
}

void _DeletePoints(const char * caller, pTimepointType * ppThisPointChain)
{
pTimepointType pChild, pThisPointChain = *ppThisPointChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisPointChain)
	{
		/* if space to keep item's name is allocated */
		if (pThisPointChain->pcMarquee)
		
		    /* then release this space */
		    free(pThisPointChain->pcMarquee);

		/* preserve a pointer to next record */		    
		pChild = pThisPointChain->pNext;
		
		/* free space occupied by current record */
		free (pThisPointChain);
		
		/* Go to next record */
		pThisPointChain = pChild;
	}

	/* Dispose first element of chain */
	*ppThisPointChain = NULL;
}
