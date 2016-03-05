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


int _EnrollPoint(const char * caller, pTimepointType * ppThisPointChain, float * fltTm, float * fltX, float * fltY, char * pcMrq)
{
pTimepointType pChild, pTempPointChain;

#if 0
	printf(" %s called: <%f> <%f> <%f> <%s> \n", caller, *fltTm, *fltX, *fltY, pcMrq);//printf(" called: <%f> <%f> <%f>  \n",  *fltTm, *fltX, *fltY );
	return 0;
#else
	if (NULL == *ppThisPointChain)
	{
		/* only one chain, for beginning */
		*ppThisPointChain = (pTimepointType) calloc ( 1, sizeof (TimepointType) );

		/* check if successful */
		if (NULL == *ppThisPointChain)
		{
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. %f: [X(%f),Y(%f)]  \n",
			__FILE__, caller, __func__,
			*fltTm, *fltX, *fltY);

			return (-8);
		}
		
		(*ppThisPointChain)->fltXval = *fltX;
		(*ppThisPointChain)->fltYval = *fltY;
		(*ppThisPointChain)->fltAbsTime = *fltTm;

		(*ppThisPointChain)->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( (*ppThisPointChain)->pcMarquee, pcMrq);

#if DEBUG_DATA
		printf("[%s] %s:%s : FIRST <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			(*ppThisPointChain)->fltAbsTime,
			(*ppThisPointChain)->fltXval,
			(*ppThisPointChain)->fltYval,
			(*ppThisPointChain)->pcMarquee
		);
#endif /* (DEBUG_DATA) */

	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisPointChain;

		pTempPointChain = (pTimepointType) calloc (1, sizeof (TimepointType) );

		if (NULL == pTempPointChain)
		{
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element. %f: [X(%f),Y(%f)]  \n", 
			__FILE__, caller, __func__,
			*fltTm, *fltX, *fltY);

			return (-7);
		}


		pTempPointChain->fltXval = *fltX;
		pTempPointChain->fltYval = *fltY;
		pTempPointChain->fltAbsTime = *fltTm;

		pTempPointChain->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( pTempPointChain->pcMarquee, pcMrq);

#if DEBUG_DATA
		printf("[%s] %s:%s : NEXT <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pTempPointChain->fltAbsTime,
			pTempPointChain->fltXval,
			pTempPointChain->fltYval,
			pTempPointChain->pcMarquee
		);
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
#endif /* #if 0, #else */
}

int _ProcessPoints(const char * caller, pTimepointType pPointChainPar)
{
pTimepointType pPointChain = pPointChainPar;

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{
//#if DEBUG_DATA
		printf("[%s] %s:%s : <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pPointChain->fltAbsTime,
			pPointChain->fltXval,
			pPointChain->fltYval,
			pPointChain->pcMarquee
		);
//#endif /* (DEBUG_DATA) */

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
