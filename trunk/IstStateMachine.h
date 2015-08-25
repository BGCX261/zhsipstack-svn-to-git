// IstStateMachine.h: interface for the CIstStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTSTATEMACHINE_H__FC9A7486_FE84_4748_9B74_A646D0DC7291__INCLUDED_)
#define AFX_ISTSTATEMACHINE_H__FC9A7486_FE84_4748_9B74_A646D0DC7291__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StateMachine.h"
#include "SipTransaction.h"

class CIstStateMachine : public CStateMachine  
{
public:
	CIstStateMachine(CSipTransaction *pTransaction);
	virtual ~CIstStateMachine();
	
	virtual int Execute( CSipEvent *pEvent );


	void RcvInvite(CSipEvent* evt);
	void TimeoutGEvent(CSipEvent* evt);
	void TimeoutHEvent(CSipEvent* evt);
	void TimeoutIEvent(CSipEvent* evt);
	void Send1xx(CSipEvent* evt);
	void Send2xx(CSipEvent* evt);
	void Send3456xx(CSipEvent* evt);
	void RcvAck(CSipEvent* evt);

};

#endif // !defined(AFX_ISTSTATEMACHINE_H__FC9A7486_FE84_4748_9B74_A646D0DC7291__INCLUDED_)
