// Nist.cpp: implementation of the CNist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Nist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNist::CNist(CSipMessage* invite)
{
	Init(invite);
}

CNist::~CNist()
{

}

int CNist::Init(CSipMessage* invite)
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
			this->timer_j_length = 64 * DEFAULT_T1;
			this->timer_j_start.tv_sec = -1; 	/* not started */
		}
		else
		{
			/* reliable protocol is used: */
			this->timer_j_length = 0;	/* MUST do the transition immediatly */
			this->timer_j_start.tv_sec = -1; 	/* not started */
		}
	}
	
	return 0;
}
