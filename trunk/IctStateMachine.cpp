// IctStateMachine.cpp: implementation of the CIctStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IctStateMachine.h"
#include "SipTransactionManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIctStateMachine::CIctStateMachine(CSipTransaction *pTransaction)
{
	m_pTransaction = pTransaction;
}

CIctStateMachine::~CIctStateMachine()
{

}

int CIctStateMachine::Execute( CSipEvent *pEvent )
{
	if (m_pTransaction == NULL || pEvent == NULL)
	{
		return -1;
	}
	
	if (m_pTransaction->state == ICT_PRE_CALLING && pEvent->type == SND_REQINVITE)
	{
		SendInvite(pEvent);
	}
	else if (m_pTransaction->state == ICT_CALLING && pEvent->type == TIMEOUT_A)
	{
		TimeoutAEvent(pEvent);
	}
	else if (m_pTransaction->state == ICT_PRE_CALLING && pEvent->type == TIMEOUT_B)
	{
		TimeoutBEvent(pEvent);
	}
	else if (m_pTransaction->state == ICT_PRE_CALLING && pEvent->type == RCV_STATUS_1XX)
	{
		Rcv1xx(pEvent);
	}
	else if (m_pTransaction->state == ICT_PRE_CALLING && pEvent->type == RCV_STATUS_2XX)
	{
		Rcv2xx(pEvent);
	}
	else if (m_pTransaction->state == ICT_PRE_CALLING && pEvent->type == RCV_STATUS_3456XX)
	{
		Rcv3456xx(pEvent);
	}
	else if (m_pTransaction->state == ICT_PROCEEDING && pEvent->type == RCV_STATUS_1XX)
	{
		Rcv1xx(pEvent);
	}
	else if (m_pTransaction->state == ICT_PROCEEDING && pEvent->type == RCV_STATUS_2XX)
	{
		Rcv2xx(pEvent);
	}
	else if (m_pTransaction->state == ICT_PROCEEDING && pEvent->type == RCV_STATUS_3456XX)
	{
		Rcv3456xx(pEvent);	
	}
	else if (m_pTransaction->state == ICT_COMPLETED && pEvent->type == RCV_STATUS_3456XX)
	{
		RetransmitAck(pEvent);
	}
	else if (m_pTransaction->state == ICT_COMPLETED && pEvent->type == TIMEOUT_D)
	{
		TimeoutDEvent(pEvent);
	}
	
	return 0;	
}

void CIctStateMachine::SendInvite( CSipEvent* evt )
{
	CSipTransactionManager *psipManager = CSipTransactionManager::getInstance();
	
	psipManager->SendOutMessage(m_pTransaction,evt->sip,NULL,0,0);	
}

void CIctStateMachine::TimeoutAEvent( CSipEvent* evt )
{
	
}

void CIctStateMachine::TimeoutBEvent( CSipEvent* evt )
{
	
}

void CIctStateMachine::TimeoutDEvent( CSipEvent* evt )
{
	
}

void CIctStateMachine::Rcv1xx( CSipEvent* evt )
{
	if (m_pTransaction->last_response != NULL)
	{
		delete m_pTransaction->last_response;
	}

	m_pTransaction->last_response = evt->sip;
	
	icb_Rev1xx(0, m_pTransaction, evt->sip);

	m_pTransaction->SetState(ICT_PROCEEDING);
}

void CIctStateMachine::Rcv2xx( CSipEvent* evt )
{
	if (m_pTransaction->last_response != NULL)
	{
		delete m_pTransaction->last_response;
	}
	
	m_pTransaction->last_response = evt->sip;

	m_pTransaction->SetState(ICT_TERMINATED);

}

void CIctStateMachine::Rcv3456xx( CSipEvent* evt )
{
	
}

void CIctStateMachine::RetransmitAck( CSipEvent* evt )
{
	
}