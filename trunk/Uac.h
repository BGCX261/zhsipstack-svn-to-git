// Uac.h: interface for the CUac class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UAC_H__A30DF271_45AB_4386_B9FE_55D49003C490__INCLUDED_)
#define AFX_UAC_H__A30DF271_45AB_4386_B9FE_55D49003C490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipMessage.h"
#include "Utils.h"

class CUac  
{
public:
	CUac();
	virtual ~CUac();

	//void SetPeerCall(const char *pcCall);

	int Invite(const char *to,const char* from, const char* route, const char* subject);

};

#endif // !defined(AFX_UAC_H__A30DF271_45AB_4386_B9FE_55D49003C490__INCLUDED_)
