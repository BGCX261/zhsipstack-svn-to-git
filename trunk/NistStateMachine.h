// NistStateMachine.h: interface for the CNistStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NISTSTATEMACHINE_H__8F92EF1F_1F39_4A82_9C27_BB87977E4951__INCLUDED_)
#define AFX_NISTSTATEMACHINE_H__8F92EF1F_1F39_4A82_9C27_BB87977E4951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StateMachine.h"
#include "SipTransaction.h"

class CNistStateMachine : public CStateMachine
{
public:
	CNistStateMachine(CSipTransaction *pTransaction);
	virtual ~CNistStateMachine();
	
	virtual int Execute( CSipEvent *pEvent );

	void RcvRequest(CSipEvent* evt);
	void Send1xx(CSipEvent* evt);
	void Send23456xx(CSipEvent* evt);
	void TimeoutJEvent(CSipEvent* evt);

};

#endif // !defined(AFX_NISTSTATEMACHINE_H__8F92EF1F_1F39_4A82_9C27_BB87977E4951__INCLUDED_)
