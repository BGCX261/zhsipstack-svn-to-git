// Ict.cpp: implementation of the CIct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Ict.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIct::CIct(CSipMessage* invite)
{
	destination = NULL;
	Init(invite);
}

CIct::~CIct()
{

}

int CIct::Init(CSipMessage* invite)
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
			this->timer_a_length = DEFAULT_T1;
			if (64 * DEFAULT_T1 < 32000)
				this->timer_d_length = 32000;
			else
				this->timer_d_length = 64 * DEFAULT_T1;
			osip_gettimeofday(&this->timer_a_start, NULL);
			add_gettimeofday(&this->timer_a_start, this->timer_a_length);
			this->timer_d_start.tv_sec = -1;  	/* not started */
		}
		else
		{
			/* reliable protocol is used: */
			this->timer_a_length = -1;	/* A is not ACTIVE */
			this->timer_d_length = 0; 	/* MUST do the transition immediatly */
			this->timer_a_start.tv_sec = -1;  	/* not started */
			this->timer_d_start.tv_sec = -1;  	/* not started */
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

	this->timer_b_length = 64 * DEFAULT_T1;
	osip_gettimeofday(&this->timer_b_start, NULL);
	add_gettimeofday(&this->timer_b_start, this->timer_b_length);

	/* Oups! A bug! */
	/*  ict_context->port  = 5060; */

	return 0;
}

int CIct::SetDestination( char* destination, int port )
{
	if (this->destination != NULL)
	{
		mm_free(this->destination);
	}
	this->destination = destination;
	this->port = port;
	return 0;
	
}
