// NictCallBack.h: interface for the CNictCallBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NICTCALLBACK_H__CE55E17F_E85D_4083_82AE_0B0FBBE56B45__INCLUDED_)
#define AFX_NICTCALLBACK_H__CE55E17F_E85D_4083_82AE_0B0FBBE56B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"

class CNictCallBack  
{
public:
	CNictCallBack();
	virtual ~CNictCallBack();

	virtual void nicb_RegisterSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_ByeSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_OptionsSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_InfoSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_CancelSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_NotifySend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_SubscribeSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_UnknowRequestSend(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_RequestSendAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev1xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev2xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev2xxAgain(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev3xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev4xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev5xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev6xx(int type, CSipTransaction* tr, CSipMessage* sip);
	virtual void nicb_Rev3456xx(int type, CSipTransaction* tr, CSipMessage* sip);
};

#endif // !defined(AFX_NICTCALLBACK_H__CE55E17F_E85D_4083_82AE_0B0FBBE56B45__INCLUDED_)
