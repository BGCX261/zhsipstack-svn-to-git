// IstStateMachine.cpp: implementation of the CIstStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IstStateMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIstStateMachine::CIstStateMachine(CSipTransaction *pTransaction)
{
	m_pTransaction = pTransaction;
}

CIstStateMachine::~CIstStateMachine()
{

}

int CIstStateMachine::Execute( CSipEvent *pEvent )
{
	if (m_pTransaction->state == IST_PRE_PROCEEDING && pEvent->type == RCV_REQINVITE)
	{
		RcvInvite(pEvent);
	}
	else if (m_pTransaction->state == IST_PROCEEDING && pEvent->type == RCV_REQINVITE)
	{
		RcvInvite(pEvent);
	}
	else if (m_pTransaction->state == IST_COMPLETED && pEvent->type == RCV_REQINVITE)
	{
		RcvInvite(pEvent);
	}
	else if (m_pTransaction->state == IST_COMPLETED && pEvent->type == TIMEOUT_G)
	{
		TimeoutGEvent(pEvent);
	}
	else if (m_pTransaction->state == IST_COMPLETED && pEvent->type == TIMEOUT_H)
	{
		TimeoutHEvent(pEvent);
	}
	else if (m_pTransaction->state == IST_PROCEEDING && pEvent->type == SND_STATUS_1XX)
	{
		Send1xx(pEvent);
	}
	else if (m_pTransaction->state == IST_PROCEEDING && pEvent->type == SND_STATUS_2XX)
	{
		Send2xx(pEvent);
	}
	else if (m_pTransaction->state == IST_PROCEEDING && pEvent->type == SND_STATUS_3456XX)
	{
		Send3456xx(pEvent);
	}
	else if (m_pTransaction->state == IST_COMPLETED && pEvent->type == RCV_REQACK)
	{
		RcvAck(pEvent);
	}
	else if (m_pTransaction->state == IST_CONFIRMED && pEvent->type == RCV_REQACK)
	{
		RcvAck(pEvent);
	}
	else if (m_pTransaction->state == IST_CONFIRMED && pEvent->type == TIMEOUT_I)
	{
		TimeoutIEvent(pEvent);
	}
	
	return 0;
}

void CIstStateMachine::RcvInvite( CSipEvent* evt )
{
	
}

void CIstStateMachine::TimeoutGEvent( CSipEvent* evt )
{
	
}

void CIstStateMachine::TimeoutHEvent( CSipEvent* evt )
{
	
}

void CIstStateMachine::TimeoutIEvent( CSipEvent* evt )
{
	
}

void CIstStateMachine::Send1xx( CSipEvent* evt )
{
	
}

void CIstStateMachine::Send2xx( CSipEvent* evt )
{
	
}

void CIstStateMachine::Send3456xx( CSipEvent* evt )
{
	
}

void CIstStateMachine::RcvAck( CSipEvent* evt )
{
	
}