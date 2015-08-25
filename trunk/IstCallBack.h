// IstCallBack.h: interface for the CIstCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTCALLBACK_H__A0A1CF6C_35AE_4FD1_8724_2E67B9453F62__INCLUDED_)
#define AFX_ISTCALLBACK_H__A0A1CF6C_35AE_4FD1_8724_2E67B9453F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"

class CIstCallBack  
{
public:
	CIstCallBack();
	virtual ~CIstCallBack();

	virtual void isb_RcvInvite(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_RcvInviteAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_RcvAck(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_RcvAckAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send1xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send2xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send3xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send4xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send5xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send6xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void isb_Send3456xx(int type, CSipTransaction* tr, CSipMessage* sip);
};

#endif // !defined(AFX_ISTCALLBACK_H__A0A1CF6C_35AE_4FD1_8724_2E67B9453F62__INCLUDED_)
