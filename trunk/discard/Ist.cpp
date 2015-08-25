// Ist.cpp: implementation of the CIst class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Ist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIst::CIst(CSipMessage* invite)
{

	Init(invite);
}

CIst::~CIst()
{

}

int CIst::Init(CSipMessage* invite)
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
			this->timer_g_length = DEFAULT_T1;
			this->timer_i_length = DEFAULT_T4;
			this->timer_g_start.tv_sec = -1;  	/* not started */
			this->timer_i_start.tv_sec = -1;  	/* not started */
		}
		else
		{
			/* reliable protocol is used: */
			this->timer_g_length = -1;	/* A is not ACTIVE */
			this->timer_i_length = 0; 	/* MUST do the transition immediatly */
			this->timer_g_start.tv_sec = -1;  	/* not started */
			this->timer_i_start.tv_sec = -1;  	/* not started */
		}
	}

	this->timer_h_length = 64 * DEFAULT_T1;
	this->timer_h_start.tv_sec = -1;    /* not started */

	return 0;
}
