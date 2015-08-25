// SipTransaction.cpp: implementation of the CSipTransaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipTransaction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipTransaction::CSipTransaction()
{
	m_pStateMachine = NULL;
}

CSipTransaction::~CSipTransaction()
{

}

int CSipTransaction::SetTopVia(CVia* topvia)
{
	int i;
	
	i = topvia->Clone(&(this->topvia));
	if (i == 0)
		return 0;
	this->topvia = NULL;
	return -1;
}

int CSipTransaction::SetFrom(CFrom* from)
{
	int i;
	
	i = from->Clone(&(this->from));
	if (i == 0)
		return 0;
	this->from = NULL;
	return -1;
}

int CSipTransaction::SetTo(CTo* to)
{
	int i;
	
	i = to->Clone(&(this->to));
	if (i == 0)
		return 0;
	this->to = NULL;
	return -1;
}

int CSipTransaction::SetCallId(CCallId* call_id)
{
	int i;
	
	i = call_id->Clone(&(this->callid));
	if (i == 0)
		return 0;
	this->callid = NULL;
	return -1;
}

int CSipTransaction::SetCseq(CSeq* cseq)
{
	int i;
	
	i = cseq->Clone(&(this->cseq));
	if (i == 0)
		return 0;
	this->cseq = NULL;
	return -1;
}

int CSipTransaction::Init( CSipMessage* request )
{
	last_response = NULL;
	your_instance = NULL;

	static int itransactionid = 1;
	CVia* topvia;

	int i;
	time_t now;

	if (request == NULL)
		return -1;
	if (request->m_callid == NULL)
		return -1;
	if (request->m_callid->number == NULL)
		return -1;

	now = time(NULL);

	birth_time = now;
	transactionid = itransactionid;
	itransactionid++;

	topvia = (CVia*)request->m_listVias.GetAt(0);
	if (topvia == NULL)
		goto ti_error_1;

	i = SetTopVia(topvia);
	if (i != 0)
		goto ti_error_1;

	/* In some situation, some of those informtions might
	   be useless. Mostly, I prefer to keep them in all case
	   for backward compatibility. */
	i = SetFrom(request->m_from);
	if (i != 0)
		goto ti_error_2;
	i = SetTo(request->m_to);
	if (i != 0)
		goto ti_error_3;
	i = SetCallId(request->m_callid);
	if (i != 0)
		goto ti_error_4;
	i = SetCseq(request->m_seq);
	if (i != 0)
		goto ti_error_5;
	/* RACE conditions can happen for server transactions */
	/* (*transaction)->orig_request = request; */
	this->orig_request = NULL;

	(this)->transactionff = new CFifo;
	if ((this)->transactionff == NULL)
		goto ti_error_6;

	return 0;

	ti_error_6 : delete ((this)->cseq);
	ti_error_5 : delete ((this)->callid);
	ti_error_4 : delete ((this)->to);
	ti_error_3 : delete ((this)->from);
	ti_error_2 : delete ((this)->topvia);
	ti_error_1 : delete (this);
	
	return -1;
}

int CSipTransaction::AddEvent( CSipEvent *pEvent )
{
	if (transactionff == NULL)
	{
		return -1;
	}
		
	transactionff->Add(pEvent);

	return transactionff->GetSize();
}

int CSipTransaction::Execute()
{
	ExecuteTimer();

	if (m_pStateMachine == NULL)
	{
		return -1;
	}
	CSipEvent* se;
	int more_event;

	more_event = 1;
	do
	{
		se = (CSipEvent *) transactionff->TryGetFrist();
		if (se == NULL) 	  /* no more event for this transaction */
			more_event = 0;
		else
		{
			m_pStateMachine->Execute(se);
		}
	} while (more_event == 1);

	return 0;
}

void CSipTransaction::ExecuteTimer()
{
	
	
}

int CSipTransaction::ExecuteTrsanction()
{
	
	return 0;
}

int CSipTransaction::SetState( eTransactionState state )
{
	this->state = state;	
	
	return this->state;
}

int CSipTransaction::SetInstance(void* instance)
{
	your_instance = instance;
	return 0;
}

void* CSipTransaction::GetInstance()
{
	return your_instance;
}