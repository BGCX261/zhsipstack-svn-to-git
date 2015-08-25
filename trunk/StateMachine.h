// StateMachine.h: interface for the CStateMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATEMACHINE_H__3221C35A_CB93_4EA4_96A9_C66C17CC3503__INCLUDED_)
#define AFX_STATEMACHINE_H__3221C35A_CB93_4EA4_96A9_C66C17CC3503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SipEvent.h"

class CSipTransaction;

class CStateMachine  
{
public:
	CStateMachine();
	virtual ~CStateMachine();

	virtual int Execute(CSipEvent *pEvent) = 0;

	CSipTransaction *m_pTransaction;

};

#endif // !defined(AFX_STATEMACHINE_H__3221C35A_CB93_4EA4_96A9_C66C17CC3503__INCLUDED_)
