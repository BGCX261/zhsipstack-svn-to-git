// SipReg.h: interface for the CZhReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPREG_H__A9A13D37_4237_429D_B803_301DE7C4FCB6__INCLUDED_)
#define AFX_SIPREG_H__A9A13D37_4237_429D_B803_301DE7C4FCB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"
#include "SipDialog.h"

class CZhReg  
{
public:
	CZhReg(const char* from,const char* proxy, const char* contact);
	virtual ~CZhReg();
	
	void Free();

	static int r_id;
	
	int r_reg_period;   		/* delay between registration */
	char* r_aor;				/* sip identity */
	char* r_registrar;  		/* registrar */
	char* r_contact;			/* list of contacts string */
	
	char r_line[16];			/* line identifier */
	
	CSipTransaction* r_last_tr;
	int r_retry;				/* avoid too many unsuccessfull retry */
	
	SOCKADDR_IN addr;
	int len;
	
	
};

#endif // !defined(AFX_SIPREG_H__A9A13D37_4237_429D_B803_301DE7C4FCB6__INCLUDED_)
