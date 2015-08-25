// Nict.cpp: implementation of the CNict class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Nict.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNict::CNict(CSipMessage* request)
{
	destination = NULL;
	Init(request);
}

CNict::~CNict()
{

}

int CNict::Init(CSipMessage* request)
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
			this->timer_e_length = DEFAULT_T1;
			this->timer_k_length = DEFAULT_T4;
			this->timer_e_start.tv_sec = -1;
			this->timer_k_start.tv_sec = -1; 	/* not started */
		}
		else
		{
			/* reliable protocol is used: */
			this->timer_e_length = -1;   /* E is not ACTIVE */
			this->timer_k_length = 0;	/* MUST do the transition immediatly */
			this->timer_e_start.tv_sec = -1;
			this->timer_k_start.tv_sec = -1; 	/* not started */
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

	this->timer_f_length = 64 * DEFAULT_T1;
	osip_gettimeofday(&this->timer_f_start, NULL);
	add_gettimeofday(&this->timer_f_start, this->timer_f_length);

	/* Oups! a Bug! */
	/*  (*nict)->port  = 5060; */

	return 0;
}

int CNict::SetDestination( char* destination, int port )
{
	if (this->destination != NULL)
	{
		mm_free(this->destination);
	}
	this->destination = destination;
	this->port = port;
	return 0;	
}
