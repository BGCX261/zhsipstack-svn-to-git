// IctTransaction.h: interface for the CIctTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICTTRANSACTION_H__89928FBB_BA76_4FA9_8778_0353E16B70C5__INCLUDED_)
#define AFX_ICTTRANSACTION_H__89928FBB_BA76_4FA9_8778_0353E16B70C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CIctTransaction : public CSipTransaction  
{
public:
	CIctTransaction(CSipMessage *pMessage);
	virtual ~CIctTransaction();

	
	virtual		int		Init(CSipMessage* invite);

	CSipEvent* TimerATriger();
	CSipEvent* TimerBTriger();
	CSipEvent* TimerDTriger();

	virtual     void		ExecuteTimer();

	class CIct  
	{
	public:
		CIct()
		{
			destination = NULL;
		};
		virtual ~CIct(){};
		
		int		Init(CSipMessage* invite);
		
		int		SetDestination(char* destination, int port);
		
		CZhTimer m_TimerA;
		CZhTimer m_TimerB;
		CZhTimer m_TimerD;

		char* destination;  		  /**@internal url used to send requests */
		int port;   				  /**@internal port of next hop */
		
	} ict_context;

	//CIct*		ict_context;	/**@internal */

};

#endif // !defined(AFX_ICTTRANSACTION_H__89928FBB_BA76_4FA9_8778_0353E16B70C5__INCLUDED_)
