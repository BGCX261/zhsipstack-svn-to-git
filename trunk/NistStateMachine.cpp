// NistStateMachine.cpp: implementation of the CNistStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NistStateMachine.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNistStateMachine::CNistStateMachine(CSipTransaction *pTransaction)
{
	m_pTransaction = pTransaction;
}

CNistStateMachine::~CNistStateMachine()
{

}

int CNistStateMachine::Execute( CSipEvent *pEvent )
{
	if (m_pTransaction->state == NIST_PRE_TRYING && pEvent->type == RCV_REQUEST)
	{
		RcvRequest(pEvent);
	}
	else if (m_pTransaction->state == NIST_TRYING && pEvent->type == SND_STATUS_1XX)
	{
		Send1xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_TRYING && pEvent->type == SND_STATUS_2XX)
	{
		Send23456xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_TRYING && pEvent->type == SND_STATUS_3456XX)
	{
		Send23456xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_PROCEEDING && pEvent->type == SND_STATUS_1XX)
	{
		Send1xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_PROCEEDING && pEvent->type == SND_STATUS_2XX)
	{
		Send23456xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_PROCEEDING && pEvent->type == SND_STATUS_3456XX)
	{
		Send23456xx(pEvent);
	}
	else if (m_pTransaction->state == NIST_PROCEEDING && pEvent->type == RCV_REQUEST)
	{
		RcvRequest(pEvent);
	}
	else if (m_pTransaction->state == NIST_COMPLETED && pEvent->type == TIMEOUT_J)
	{
		TimeoutJEvent(pEvent);
	}
	else if (m_pTransaction->state == NIST_COMPLETED && pEvent->type == RCV_REQUEST)
	{
		RcvRequest(pEvent);
	}
	
	
	return 0;
}

void CNistStateMachine::RcvRequest( CSipEvent* evt )
{
	
}

void CNistStateMachine::Send1xx( CSipEvent* evt )
{
	
}

void CNistStateMachine::Send23456xx( CSipEvent* evt )
{
	
}

void CNistStateMachine::TimeoutJEvent( CSipEvent* evt )
{
	
}
