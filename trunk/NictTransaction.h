// NictTransaction.h: interface for the CNictTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NICTTRANSACTION_H__02254421_D521_42CC_8B88_32BC76A3E093__INCLUDED_)
#define AFX_NICTTRANSACTION_H__02254421_D521_42CC_8B88_32BC76A3E093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CNictTransaction : public CSipTransaction  
{
public:
	CNictTransaction(CSipMessage *pMessage);
	virtual ~CNictTransaction();

	virtual		int		Init(CSipMessage* request);

	
	CSipEvent* TimerETriger();
	CSipEvent* TimerFTriger();
	CSipEvent* TimerKTriger();

	virtual     void		ExecuteTimer();

	class CNict  
	{
	public:
		CNict()
		{
			destination = NULL;
		};
		virtual ~CNict(){};
		
		int		Init(CSipMessage* request);
		
		int		SetDestination(char* destination, int port);
		
		CZhTimer m_TimerE; 		  /**@internal A=T1, A=2xT1... (unreliable only) */
		CZhTimer m_TimerF; 		  /**@internal B = 64* T1 */
		CZhTimer m_TimerK; 		  /**@internal K = T4 (else = 0) */

		char* destination;  		  /**@internal url used to send requests */
		int port;   				  /**@internal port of next hop */
	} nict_context;

	//CNict*		nict_context;  /**@internal */

};

#endif // !defined(AFX_NICTTRANSACTION_H__02254421_D521_42CC_8B88_32BC76A3E093__INCLUDED_)
