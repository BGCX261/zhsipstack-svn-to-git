// NistCallBack.h: interface for the CNistCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NISTCALLBACK_H__0EA0E733_D850_432E_A55C_447399B7E136__INCLUDED_)
#define AFX_NISTCALLBACK_H__0EA0E733_D850_432E_A55C_447399B7E136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"

class CNistCallBack  
{
public:
	CNistCallBack();
	virtual ~CNistCallBack();

	virtual void nisb_RegisterRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_ByeRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_OptionsRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_InfoRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_CancelRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_NotifyRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_SubscribeRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_UnknowRequestRcv(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_RequestRcvAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send1xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send2xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send2xxAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send3xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send4xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send5xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send6xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nisb_Send3456xxAgain(int type, CSipTransaction* tr, CSipMessage* sip);
};

#endif // !defined(AFX_NISTCALLBACK_H__0EA0E733_D850_432E_A55C_447399B7E136__INCLUDED_)
