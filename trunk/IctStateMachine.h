// IctStateMachine.h: interface for the CIctStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICTSTATEMACHINE_H__52270EBB_0046_4901_8A3C_4306657CE583__INCLUDED_)
#define AFX_ICTSTATEMACHINE_H__52270EBB_0046_4901_8A3C_4306657CE583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StateMachine.h"
#include "SipTransaction.h"
#include "IctCallBack.h"

class CIctStateMachine : public CStateMachine , public CIctCallBack 
{
public:
	CIctStateMachine(CSipTransaction *pTransaction);
	virtual ~CIctStateMachine();
	
	virtual int Execute( CSipEvent *pEvent );

	void		SendInvite(CSipEvent* evt);
	void		TimeoutAEvent(CSipEvent* evt);
	void		TimeoutBEvent(CSipEvent* evt);
	void		TimeoutDEvent(CSipEvent* evt);
	
	void		Rcv1xx(CSipEvent* evt);
	void		Rcv2xx(CSipEvent* evt);
	void		Rcv3456xx(CSipEvent* evt);
	void		RetransmitAck(CSipEvent* evt);	

	
};

#endif // !defined(AFX_ICTSTATEMACHINE_H__52270EBB_0046_4901_8A3C_4306657CE583__INCLUDED_)
