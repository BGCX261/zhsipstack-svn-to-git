// NictStateMachine.h: interface for the CNictStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NICTSTATEMACHINE_H__CDC75BD1_A074_4727_AD86_05B49666FCDE__INCLUDED_)
#define AFX_NICTSTATEMACHINE_H__CDC75BD1_A074_4727_AD86_05B49666FCDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StateMachine.h"
#include "SipTransaction.h"

class CNictStateMachine : public CStateMachine  
{
public:
	CNictStateMachine(CSipTransaction *pTransaction);
	virtual ~CNictStateMachine();
	
	virtual int Execute( CSipEvent *pEvent );


	void SendRequest(CSipTransaction *pTransaction,CSipEvent* evt);
	void TimeoutEEvent(CSipEvent* evt);
	void TimeoutFEvent(CSipEvent* evt);
	void TimeoutKEvent(CSipEvent* evt);
	void Rcv1xx( CSipTransaction *pTransaction,CSipEvent* evt );
	void Rcv23456xx( CSipTransaction *pTransaction,CSipEvent* evt );
};

#endif // !defined(AFX_NICTSTATEMACHINE_H__CDC75BD1_A074_4727_AD86_05B49666FCDE__INCLUDED_)
