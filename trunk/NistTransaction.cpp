// NistTransaction.cpp: implementation of the CNistTransaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NistTransaction.h"
#include "NistStateMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CNistTransaction::CNist::Init(CSipMessage* invite)
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
		
		if (zstr_strcasecmp(proto, "TCP") != 0 &&
			zstr_strcasecmp(proto, "TLS") != 0 &&
			zstr_strcasecmp(proto,"SCTP") != 0)
		{
			m_TimerJ.SetTimer(64 * DEFAULT_T1,false);
		}
		else
		{
			/* reliable protocol is used: */
			m_TimerJ.SetTimer(0,false);
		}
	}
	
	return 0;
}

CNistTransaction::CNistTransaction(CSipMessage *pMessage)
{
	m_pStateMachine = new CNistStateMachine(this);
}

CNistTransaction::~CNistTransaction()
{

}

int CNistTransaction::Init( CSipMessage* invite )
{
	this->ctx_type = NIST;
	
	this->state = NIST_PRE_TRYING;

	nist_context.Init(invite);

	return CSipTransaction::Init(invite);	
}

CSipEvent* CNistTransaction::TimerJTriger()
{
	return CSipEvent::TimerXTriger(&nist_context.m_TimerJ,state == NIST_COMPLETED,transactionid,TIMEOUT_J);
}

void CNistTransaction::ExecuteTimer()
{
#ifdef OSIP_MT
	osip_mutex_lock(ict_fastmutex);
#endif
	/* handle ict timers */
	
	CSipEvent* evt;
	
	evt = TimerJTriger();
	if (evt != NULL)
		transactionff->Add(evt);
	
#ifdef OSIP_MT
	osip_mutex_unlock(ict_fastmutex);
#endif
}
