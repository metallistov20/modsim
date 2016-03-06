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

#include <sys/time.h>

#include "dstruct.h"
#include "dport.h"

int iOldSecPRC;

/* Time-measurement variables */
struct timeval starttimePROC, endtimePROC;

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
			*pfltTm, *pfltX, *pfltY);
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
		pTempPointChain->fltAbsTime = *pfltTm;
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



int ProcRealAndRel(
#if !defined(ANTIFLOAT)
	float fltRealTime /*, float fltRelTime */
#else
	QuasiFloatType qfltRealTime /*, QuasiFloatType qfltRelTime */
#endif /* !defined(ANTIFLOAT) */
	)
{
#if !defined(ANTIFLOAT)
float fltJiffy = 1.0;
float fltRelTime;
float _left, _right;
#else
QuasiFloatType qfltJiffy; 
QuasiFloatType qfltRelTime;
int _left, _right;
#endif /* !defined(ANTIFLOAT) */


#if !defined(ANTIFLOAT)
	/* Operate uSeconds multiplied by 10e6 because <usleep> accepts	integer parameters only */
	fltRealTime = fltRealTime*1000000;

	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %f\n", __FILE__, /* caller, */	fltRealTime	);

	do 
	{	/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		fltRelTime = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
			+ endtimePROC.tv_usec - starttimePROC.tv_usec;

		if (fltRelTime < 0)
			 _right = fltRelTime, _left = fltRealTime;
		else
			 _left = fltRelTime, _right = fltRealTime;


		/* Wait for relative <fltRelTime> to catch up with absolute <fltRealTime>  */
		usleep (fltJiffy);

		printf("[%s] : <TIME SHIFTING> real tm.: %f, shiftable tm.: %f \n", __FILE__, fltRealTime,	fltRelTime ); 

	} while (/*fltRelTime*/ _right < /*fltRealTime*/ _left);
	
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%f>, is same as <%f> \n", __FILE__,
		fltRealTime,
		fltRelTime );
#else
		qfltJiffy.fraction = 1;

		qfltRealTime.integer = 			(qfltRealTime.integer < 0)?
			(qfltRealTime.integer * 1000000) - (qfltRealTime.fraction / 10):
			(qfltRealTime.integer * 1000000) + (qfltRealTime.fraction / 10);

// first cycle must be skipable on UCSIMM (start)
	/* Take current time */
	gettimeofday(&endtimePROC,0);

	/* Compute how much time elapsed since head of list processing till now */
	qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
		+ endtimePROC.tv_usec - starttimePROC.tv_usec;
// first cycle must be skipable on UCSIMM (end)


#if defined(FAST_UCSIMM)
	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		qfltRealTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */


// first cycle must be skipable on UCSIMM
// 	do 	

	if (qfltRelTime.integer < 0)
		 _right = qfltRelTime.integer, _left = qfltRealTime.integer;
	else
		 _left = qfltRelTime.integer, _right = qfltRealTime.integer;

	if (0 != qfltRealTime.integer) while (/* qfltRelTime.integer*/ _right < /* qfltRealTime.integer */ _left )
	{
		/* Wait for relative <fltRelTime> to catch up with absolute <fltRealTime>  */
		usleep (qfltJiffy.fraction);


		/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
			+ endtimePROC.tv_usec - starttimePROC.tv_usec;

		if (qfltRelTime.integer < 0)
			 _right = qfltRelTime.integer, _left = qfltRealTime.integer;
		else
			 _left = qfltRelTime.integer, _right = qfltRealTime.integer;

#if defined(FAST_UCSIMM)
		printf("[%s] : <TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		qfltRealTime.integer,	qfltRelTime.integer );
#else
		printf(",");
#endif /* defined(FAST_UCSIMM) */
	}
// first cycle must be skipable on UCSIMM
//	} while (qfltRelTime.integer < qfltRealTime.integer);

#if defined(FAST_UCSIMM)
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%d>, is same as <%d> \n", __FILE__,
		qfltRealTime.integer,
		qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */


#endif /* !defined(ANTIFLOAT) */

}

int _ProcessPoints(const char * caller, pTimepointType pPointChainPar)
{
pTimepointType pPointChain = pPointChainPar;
float fltAbsTime;

double timeusePROC;

	/* Take first time */
	gettimeofday(&starttimePROC,0);

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
TTL levels "0" 0,4V и "1" 2,4V
More precise - <0.4 and  >2.4 correspondingly

Data - not more than 3.6 V. Some details: USBN9603-28.pdf
*/


#if !defined(ANTIFLOAT)

		if (0.0 != pPointChain->fltAbsTime )
			ProcRealAndRel(pPointChain->fltAbsTime);

		/* printf("[%s] %s:  <%f> : relative time elapsed: %f microseconds\n", __FILE__, caller, 
			fltAbsTime,
			timeusePROC ); */
#else
		ProcRealAndRel(pPointChain->qfltAbsTime);
#endif /* !defined(ANTIFLOAT) */


#if !defined(ANTIFLOAT)
		if (pPointChain->fltXval <= MIN_THLD) 
#else
		if (0 == pPointChain->qfltAbsTime.power)
		if (iOldSecPRC!= pPointChain->qfltAbsTime.integer)
		{iOldSecPRC=pPointChain->qfltAbsTime.integer; printf("secPRC: %d; ", iOldSecPRC); fflush(stdout); }

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
DEBUG: this_will_mince_the_very_idea_behind_current_RT_process
#if DEBUG_DATA
				printf("[%s] %s:%s :  <%s> \n", __FILE__, caller, __func__,
					pPointChain->pcMarquee	);
#endif (DEBUG_DATA) */
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
