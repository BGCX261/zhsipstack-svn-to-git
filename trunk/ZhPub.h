// ZhPub.h: interface for the CZhPub class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHPUB_H__10816D5F_0A15_4F30_9326_DC19F215F9D5__INCLUDED_)
#define AFX_ZHPUB_H__10816D5F_0A15_4F30_9326_DC19F215F9D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CZhPub  
{
public:
	CZhPub(const char* aor, const char* exp);
	virtual ~CZhPub();

	int Init(const char* aor, const char* exp);
	void Free();
	
	int p_id;
	
	time_t p_expires;   		/* expiration date (started+period) */
	int p_period;   			/* delay between registration */
	char p_aor[256];			/* sip identity */
	char p_sip_etag[64];		/* sip_etag from 200ok */
	
	CSipTransaction* p_last_tr;

};

#endif // !defined(AFX_ZHPUB_H__10816D5F_0A15_4F30_9326_DC19F215F9D5__INCLUDED_)
