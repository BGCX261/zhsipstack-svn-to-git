// UserAgent.cpp: implementation of the CUserAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "UserAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserAgent::CUserAgent()
{

}

CUserAgent::~CUserAgent()
{

}

CSipMessage *CUserAgent::SetInvitePhone( const char *pcTo )
{

	if (pcTo == NULL)
	{
		return NULL;
	}

	CSipMessage *pMessageSip = new CSipMessage();
	if (pMessageSip == NULL)
	{
		return NULL;
	}
	
	return pMessageSip;
}

bool CUserAgent::SendInvite( CSipMessage *pMessageSip )
{
	if (pMessageSip == NULL)
	{
		return false;
	}
	
	
	return true;
}
