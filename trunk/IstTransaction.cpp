// IstTransaction.cpp: implementation of the CIstTransaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IstTransaction.h"
#include "IstStateMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIstTransaction::CIstTransaction(CSipMessage *pMessage)
{
	m_pStateMachine = new CIstStateMachine(this);
}

CIstTransaction::~CIstTransaction()
{

}

int CIstTransaction::CIst::Init(CSipMessage* invite)
{
	int i;

	/* for INVITE retransmissions */
	{
		CVia* via;
		char* proto;

		i = invite->GetVia(0, &via); /* get top via */
		if (i != 0)
		{
			return -1;
		}
		proto = via->GetProtocol();
		if (proto == NULL)
		{
			return -1;
		}

		if (zstr_strcasecmp(proto, "TCP") != 0 && zstr_strcasecmp(proto, "TLS") != 0 && zstr_strcasecmp(proto,"SCTP") != 0)
		{
			/* for other reliable protocol than TCP, the timer
					   must be desactived by the external application */
			m_TimerG.SetTimer(DEFAULT_T1,false);
			m_TimerI.SetTimer(DEFAULT_T4,false);
		}
		else
		{
			/* reliable protocol is used: */
			m_TimerG.SetTimer(-1,false);
			m_TimerI.SetTimer(0,false);
		}
	}

	m_TimerH.SetTimer(64 * DEFAULT_T1,false);

	return 0;
}

int CIstTransaction::Init( CSipMessage* invite )
{
	this->ctx_type = IST;
	this->state = IST_PRE_PROCEEDING;

	ist_context.Init(invite);

	return CSipTransaction::Init(invite);	
}

CSipEvent* CIstTransaction::TimerGTriger()
{
	return CSipEvent::TimerXTriger(&ist_context.m_TimerG,state == IST_COMPLETED,transactionid,TIMEOUT_G);
}

CSipEvent* CIstTransaction::TimerHTriger()
{
	return CSipEvent::TimerXTriger(&ist_context.m_TimerH,state == IST_COMPLETED,transactionid,TIMEOUT_H);
}

CSipEvent* CIstTransaction::TimerITriger()
{
	return CSipEvent::TimerXTriger(&ist_context.m_TimerI,state == IST_CONFIRMED,transactionid,TIMEOUT_I);
}

void CIstTransaction::ExecuteTimer()
{
#ifdef OSIP_MT
	osip_mutex_lock(ict_fastmutex);
#endif
	/* handle ict timers */
	
	CSipEvent* evt;
	
	evt = TimerITriger();
	if (evt != NULL)
		transactionff->Add(evt);
	else
	{
		evt = TimerHTriger();
		if (evt != NULL)
			transactionff->Add(evt);
		else
		{
			evt = TimerGTriger();
			if (evt != NULL)
				transactionff->Add(evt);
		}
	}
	
#ifdef OSIP_MT
	osip_mutex_unlock(ict_fastmutex);
#endif	
}
