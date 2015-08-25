// IstTransaction.h: interface for the CIstTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTTRANSACTION_H__1E197C24_58AD_4ED6_A5B6_E0E41F93F00C__INCLUDED_)
#define AFX_ISTTRANSACTION_H__1E197C24_58AD_4ED6_A5B6_E0E41F93F00C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CIstTransaction : public CSipTransaction  
{
public:
	CIstTransaction(CSipMessage *pMessage);
	virtual ~CIstTransaction();

	virtual		int		Init(CSipMessage* invite);

	CSipEvent* TimerGTriger();
	CSipEvent* TimerHTriger();
	CSipEvent* TimerITriger();

	virtual     void		ExecuteTimer();
	
	class CIst  
	{
	public:
		CIst(){};
		virtual ~CIst(){};
		
		int		Init(CSipMessage* invite);
		
		CZhTimer m_TimerG;			 /**@internal G=MIN(T1*2,T2) for unreliable trans. */	
		CZhTimer m_TimerH;			 /**@internal H = 64* T1 */
		CZhTimer m_TimerI;			 /**@internal I = T4 for unreliable (or 0) */
	
	} ist_context;

	//CIst*		ist_context;	/**@internal */
};

#endif // !defined(AFX_ISTTRANSACTION_H__1E197C24_58AD_4ED6_A5B6_E0E41F93F00C__INCLUDED_)
