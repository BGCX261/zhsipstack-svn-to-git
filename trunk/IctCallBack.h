// IctCallBack.h: interface for the CIctCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICTCALLBACK_H__C14C60AD_AC3F_4BE5_B75A_3F1EBB9CA35F__INCLUDED_)
#define AFX_ICTCALLBACK_H__C14C60AD_AC3F_4BE5_B75A_3F1EBB9CA35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"

class CIctCallBack  
{
public:
	CIctCallBack();
	virtual ~CIctCallBack();
	
	virtual void icb_SendInvite(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_SendInviteAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_SendAck(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_SendAckAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev1xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev2xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev3xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev4xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev5xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev6xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void icb_Rev3456xx(int type, CSipTransaction* tr, CSipMessage* sip);

};

#endif // !defined(AFX_ICTCALLBACK_H__C14C60AD_AC3F_4BE5_B75A_3F1EBB9CA35F__INCLUDED_)
