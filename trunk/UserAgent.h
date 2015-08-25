// UserAgent.h: interface for the CUserAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERAGENT_H__7685D868_8A8C_4DF5_B9E5_C7C4A73A2354__INCLUDED_)
#define AFX_USERAGENT_H__7685D868_8A8C_4DF5_B9E5_C7C4A73A2354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipMessage.h"

class CUserAgent  
{
public:
	CUserAgent();
	virtual ~CUserAgent();

	
	CSipMessage*	SetInvitePhone(const char *pcTo);
	bool			SendInvite(CSipMessage *pMessageSip);
};

#endif // !defined(AFX_USERAGENT_H__7685D868_8A8C_4DF5_B9E5_C7C4A73A2354__INCLUDED_)
