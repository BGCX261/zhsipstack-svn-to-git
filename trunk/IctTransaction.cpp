// IctTransaction.cpp: implementation of the CIctTransaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IctTransaction.h"
#include "IctStateMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CIctTransaction::CIct::Init(CSipMessage* invite)
{
	CRoute* route;
	int i;
	time_t now;

	now = time(NULL);
	/* for INVITE retransmissions */
	{
		CVia* via;
		char* proto;

		i = invite->GetVia(0,&via); /* get top via */
		if (i != 0 || via == NULL)
		{
			return -1;
		}
		proto = via->m_pcProtocol;
		if (proto == NULL)
		{
			return -1;
		}

		if (zstr_strcasecmp(proto, "TCP") != 0 && zstr_strcasecmp(proto, "TLS") != 0 &&	zstr_strcasecmp(proto,"SCTP") != 0)
		{
			/* for other reliable protocol than TCP, the timer
					   must be desactived by the external application */
			if (64 * DEFAULT_T1 < 32000)
				m_TimerD.SetTimer(32000,false);
			else
				m_TimerD.SetTimer(64 * DEFAULT_T1,false);

			m_TimerA.SetTimer(DEFAULT_T1,true);
		}
		else
		{
			/* reliable protocol is used: */
			m_TimerA.ReSetTimer();
			m_TimerD.ReSetTimer();
		}
	}

	/* for PROXY, the destination MUST be set by the application layer,
	   this one may not be correct. */
	invite->GetRoute(0,&route);
	if (route != NULL && route->url != NULL)
	{
		CUrlParam* lr_param;

		osip_uri_uparam_get_byname(route->url, "lr", &lr_param);
		if (lr_param == NULL)
		{
			/* using uncompliant proxy: destination is the request-uri */
			route = NULL;
		}
	}

	if (route != NULL)
	{
		int port = 5060;

		if (route->url->port != NULL)
			port = zstr_atoi(route->url->port);
		this->SetDestination(zstr_strdup(route->url->host), port);
	}
	else
	{
		int port = 5060;
		/* search for maddr parameter */
		CUrlParam* maddr_param = NULL;

		port = 5060;
		if (invite->m_pRequestUrl->port != NULL)
		{
			port = zstr_atoi(invite->m_pRequestUrl->port);
		}

		osip_uri_uparam_get_byname(invite->m_pRequestUrl, "maddr", &maddr_param);
		if (maddr_param != NULL && maddr_param->gvalue != NULL)
		{
			this->SetDestination(zstr_strdup(maddr_param->gvalue),port);
		}
		else
		{
			this->SetDestination(zstr_strdup(invite->m_pRequestUrl->host), port);
		}
	}
	
	m_TimerB.SetTimer(64 * DEFAULT_T1,true);
	/* Oups! A bug! */
	/*  ict_context->port  = 5060; */

	return 0;
}

int CIctTransaction::CIct::SetDestination( char* destination, int port )
{
	if (this->destination != NULL)
	{
		mm_free(this->destination);
	}
	this->destination = destination;
	this->port = port;
	return 0;
	
}

CIctTransaction::CIctTransaction(CSipMessage *pMessage)
{
	Init(pMessage);

	m_pStateMachine = new CIctStateMachine(this);
}

CIctTransaction::~CIctTransaction()
{

}

int CIctTransaction::Init( CSipMessage* invite )
{
	this->ctx_type = ICT;

	this->state = ICT_PRE_CALLING;

	ict_context.Init(invite);
	
	return CSipTransaction::Init(invite);
}

CSipEvent* CIctTransaction::TimerATriger()
{
	return CSipEvent::TimerXTriger(&ict_context.m_TimerA,state == ICT_CALLING,transactionid,TIMEOUT_A);
}

CSipEvent* CIctTransaction::TimerBTriger()
{
	return CSipEvent::TimerXTriger(&ict_context.m_TimerB,state == ICT_CALLING,transactionid,TIMEOUT_B);
}

CSipEvent* CIctTransaction::TimerDTriger()
{
	return CSipEvent::TimerXTriger(&ict_context.m_TimerD,state == ICT_COMPLETED,transactionid,TIMEOUT_D);
}

void CIctTransaction::ExecuteTimer()
{	
#ifdef OSIP_MT
	osip_mutex_lock(ict_fastmutex);
#endif
	/* handle ict timers */

		CSipEvent* evt;
		
		if (1 <= transactionff->GetSize())
		{
			
		}
		else
		{
			evt = TimerBTriger();
			if (evt != NULL)
				transactionff->Add(evt);
			else
			{
				evt = TimerATriger();
				if (evt != NULL)
					transactionff->Add(evt);
				else
				{
					evt = TimerDTriger();
					if (evt != NULL)
						transactionff->Add(evt);
				}
			}
		}

#ifdef OSIP_MT
	osip_mutex_unlock(ict_fastmutex);
#endif
}
