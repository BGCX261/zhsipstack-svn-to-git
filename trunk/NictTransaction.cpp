// NictTransaction.cpp: implementation of the CNictTransaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NictTransaction.h"
#include "NictStateMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CNictTransaction::CNict::Init(CSipMessage* request)
{
	CRoute* route;
	int i;
	time_t now;

	now = time(NULL);
	/* for REQUEST retransmissions */
	{
		CVia* via;
		char* proto;

		i = request->GetVia(0, &via); 	   /* get top via */
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
			m_TimerE.SetTimer(DEFAULT_T1,false);
			m_TimerK.SetTimer(DEFAULT_T4,false);
		}
		else
		{
			/* reliable protocol is used: */
			m_TimerE.SetTimer(-1,false);
			m_TimerK.SetTimer(0,false);
		}
	}

	/* for PROXY, the destination MUST be set by the application layer,
	   this one may not be correct. */
	request->GetRoute(0, &route);
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
		if (request->m_pRequestUrl->port != NULL)
		{
			port = zstr_atoi(request->m_pRequestUrl->port);
		}

		osip_uri_uparam_get_byname(request->m_pRequestUrl, "maddr", &maddr_param);
		if (maddr_param != NULL && maddr_param->gvalue != NULL)
		{
			this->SetDestination(zstr_strdup(maddr_param->gvalue), port);
		}
		else
		{
			this->SetDestination(zstr_strdup(request->m_pRequestUrl->host), port);
		}
	}

	m_TimerF.SetTimer(64 * DEFAULT_T1,true);
	/* Oups! a Bug! */
	/*  (*nict)->port  = 5060; */

	return 0;
}

int CNictTransaction::CNict::SetDestination( char* destination, int port )
{
	if (this->destination != NULL)
	{
		mm_free(this->destination);
	}
	this->destination = destination;
	this->port = port;
	return 0;	
}

CNictTransaction::CNictTransaction(CSipMessage *pMessage)
{
	Init(pMessage);

	m_pStateMachine = new CNictStateMachine(this);
}

CNictTransaction::~CNictTransaction()
{

}

int CNictTransaction::Init( CSipMessage* request )
{
	this->ctx_type = NICT;
	this->state = NICT_PRE_TRYING;

	nict_context.Init(request);

	return CSipTransaction::Init(request);	
}

CSipEvent* CNictTransaction::TimerETriger()
{
	return CSipEvent::TimerXTriger(&nict_context.m_TimerE,state == NICT_PROCEEDING || state == NICT_TRYING,
		transactionid,TIMEOUT_A);
}

CSipEvent* CNictTransaction::TimerFTriger()
{
	return CSipEvent::TimerXTriger(&nict_context.m_TimerF,state == NICT_PROCEEDING || state == NICT_TRYING,
		transactionid,TIMEOUT_B);
}

CSipEvent* CNictTransaction::TimerKTriger()
{
	return CSipEvent::TimerXTriger(&nict_context.m_TimerK,state == NICT_COMPLETED,transactionid,TIMEOUT_D);
}

void CNictTransaction::ExecuteTimer()
{
#ifdef OSIP_MT
	osip_mutex_lock(ict_fastmutex);
#endif
	/* handle ict timers */
	
	CSipEvent* evt;
	
	
	evt = TimerKTriger();
	if (evt != NULL)
		transactionff->Add(evt);
	else
	{
		evt = TimerFTriger();
		if (evt != NULL)
			transactionff->Add(evt);
		else
		{
			evt = TimerETriger();
			if (evt != NULL)
				transactionff->Add(evt);
		}
	}
	
	
#ifdef OSIP_MT
	osip_mutex_unlock(ict_fastmutex);
#endif	
}
