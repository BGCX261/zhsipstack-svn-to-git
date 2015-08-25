// Fifo.cpp: implementation of the CFifo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Fifo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFifo::CFifo()
{
	Init();
}

CFifo::~CFifo()
{

}

/* always use this method to initiate osip_fifo_t.
*/
void CFifo::Init()
{
	state = osip_empty;
}

int CFifo::Add(void* el)
{
	if (state != osip_full)
	{
		/* ff->nb_elt++; */
		queue.Add(el, -1);  	  /* insert at end of queue */
	}
	else
	{
		return -1;  			  /* stack is full */
	}
	/* if (ff->nb_elt >= MAX_LEN) */
	if (queue.GetSize() >= MAX_LEN)
	{
		state = osip_full;
	}
	else
	{
		state = osip_ok;
	}
	
	return 0;
}

int CFifo::Insert(void* el)
{
	if (state != osip_full)
	{
		/* ff->nb_elt++; */
		queue.Add(el, 0); /* insert at end of queue */
	}
	else
	{
		return -1;  			  /* stack is full */
	}
	/* if (ff->nb_elt >= MAX_LEN) */
	if (queue.GetSize() >= MAX_LEN)
	{
		state = osip_full;
	}
	else
	{
		state = osip_ok;
	}
	
	return 0;
}

int CFifo::GetSize()
{
	int i;
	
	i = queue.GetSize();

	return i;
}

void* CFifo::GetFrist()
{
	void* el = NULL;
	
	if (state != osip_empty)
	{
		el = queue.GetAt(0);
		queue.RemoveAt(0);
		/* ff->nb_elt--; */
	}
	else
	{
		return 0;   			  /* pile vide */
	}
	/* if (ff->nb_elt <= 0) */
	if (queue.GetSize() <= 0)
	{
		state = osip_empty;
	}
	else
	{
		state = osip_ok;
	}
	
	return el;
}

void* CFifo::TryGetFrist()
{
	void* el = NULL;
	
	if (state == osip_empty)
	{
		return NULL;
	}

	if (state != osip_empty)
	{
		el = queue.GetAt(0);
		queue.RemoveAt(0);
		/* ff->nb_elt--; */
	}
	else
	{
		return 0;   			  /* pile vide */
	}
	/* if (ff->nb_elt <= 0) */
	if (queue.GetSize() <= 0)
	{
		state = osip_empty;
	}
	else
	{
		state = osip_ok;
	}
	
	return el;	
}

void CFifo::Free()
{
	
}
