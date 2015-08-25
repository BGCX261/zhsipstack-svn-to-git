// NictStateMachine.cpp: implementation of the CNictStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NictStateMachine.h"
#include "ThreadNet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNictStateMachine::CNictStateMachine(CSipTransaction *pTransaction)
{
	m_pTransaction = pTransaction;
}

CNictStateMachine::~CNictStateMachine()
{

}

int CNictStateMachine::Execute( CSipEvent *pEvent )
{
	if (m_pTransaction->state == NICT_PRE_TRYING && pEvent->type == SND_REQUEST)
	{
		SendRequest(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_TRYING && pEvent->type == TIMEOUT_F)
	{
		TimeoutFEvent(pEvent);
	}
	else if (m_pTransaction->state == NICT_TRYING && pEvent->type == TIMEOUT_E)
	{
		TimeoutEEvent(pEvent);
	}
	else if (m_pTransaction->state == NICT_TRYING && pEvent->type == RCV_STATUS_1XX)
	{
		Rcv1xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_TRYING && pEvent->type == RCV_STATUS_2XX)
	{
		Rcv23456xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_TRYING && pEvent->type == RCV_STATUS_3456XX)
	{
		Rcv23456xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_PROCEEDING && pEvent->type == TIMEOUT_F)
	{
		TimeoutFEvent(pEvent);
	}
	else if (m_pTransaction->state == NICT_PROCEEDING && pEvent->type == TIMEOUT_E)
	{
		TimeoutEEvent(pEvent);
	}
	else if (m_pTransaction->state == NICT_PROCEEDING && pEvent->type == RCV_STATUS_1XX)
	{
		Rcv1xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_PROCEEDING && pEvent->type == RCV_STATUS_2XX)
	{
		Rcv23456xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_PROCEEDING && pEvent->type == RCV_STATUS_3456XX)
	{
		Rcv23456xx(m_pTransaction,pEvent);
	}
	else if (m_pTransaction->state == NICT_COMPLETED && pEvent->type == TIMEOUT_K)
	{
		TimeoutKEvent(pEvent);
	}
	
	return 0;
}

void CNictStateMachine::SendRequest( CSipTransaction *m_pTransaction,CSipEvent* evt )
{
	if (evt == NULL || evt->sip == NULL || m_pTransaction == NULL)
	{
		return;
	}

	CSipMessage *pMessage = evt->sip;


	char *pcData = NULL;
	unsigned int iLen;
	pMessage->ToStr(&pcData,&iLen);
	
	int iSend = 0;
	CThreadNet *pThreadNet = CThreadNet::getInstance();
	iSend = pThreadNet->SendMessage(pcData,iLen);
	if (iSend <= 0)
	{
		m_pTransaction->SetState(NICT_TERMINATED);

		return;
	}
	
	if (MSG_IS_REGISTER(pMessage))
	{
	}
	else if (MSG_IS_BYE(pMessage))
	{
	}
	else if (MSG_IS_OPTIONS(pMessage))
	{
	}
	else if (MSG_IS_INFO(pMessage))
	{
	}
	else if (MSG_IS_CANCEL(pMessage))
	{
	}
	else if (MSG_IS_NOTIFY(pMessage))
	{
	}
	else if (MSG_IS_SUBSCRIBE(pMessage))
	{
	}
	else
	{

	}

	m_pTransaction->SetState(NICT_TRYING);

}

void CNictStateMachine::TimeoutEEvent( CSipEvent* evt )
{
	
}

void CNictStateMachine::TimeoutFEvent( CSipEvent* evt )
{
	
}

void CNictStateMachine::TimeoutKEvent( CSipEvent* evt )
{
	
}

void CNictStateMachine::Rcv1xx( CSipTransaction *m_pTransaction,CSipEvent* evt )
{
	
}

void CNictStateMachine::Rcv23456xx( CSipTransaction *m_pTransaction,CSipEvent* evt )
{
	if (m_pTransaction->last_response != NULL)
	{
		delete m_pTransaction->last_response;
		m_pTransaction->last_response = NULL;
	}

	m_pTransaction->last_response = evt->sip;

	if (EVT_IS_RCV_STATUS_2XX(evt))
	{
		
	}
	else if (MSG_IS_STATUS_3XX(m_pTransaction->last_response))
	{

	}
	else if (MSG_IS_STATUS_4XX(m_pTransaction->last_response))
	{

	}
	else if (MSG_IS_STATUS_5XX(m_pTransaction->last_response))
	{

	}
	else
	{

	}

	if (m_pTransaction->state != NICT_COMPLETED)    /* reset timer K */
	{
		
	}

	m_pTransaction->SetState(NICT_COMPLETED);

}
