// NistTransaction.h: interface for the CNistTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NISTTRANSACTION_H__76898B09_BED4_4C9D_92DF_F2582FF0A515__INCLUDED_)
#define AFX_NISTTRANSACTION_H__76898B09_BED4_4C9D_92DF_F2582FF0A515__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CNistTransaction : public CSipTransaction  
{
public:
	CNistTransaction(CSipMessage *pMessage);
	virtual ~CNistTransaction();

	virtual		int		Init(CSipMessage* invite);

	CSipEvent*			TimerJTriger();

	virtual     void	ExecuteTimer();

	class CNist  
	{
	public:
		CNist()
		{
		};
		virtual ~CNist(){};
		
		int		Init(CSipMessage* invite);
		
		CZhTimer m_TimerJ; 		   /**@internal J = 64*T1 (else 0) */
	} nist_context;

	//CNist*		nist_context;  /**@internal */
};

#endif // !defined(AFX_NISTTRANSACTION_H__76898B09_BED4_4C9D_92DF_F2582FF0A515__INCLUDED_)
