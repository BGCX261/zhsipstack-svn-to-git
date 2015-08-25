// SipTransactionManager.h: interface for the CSipTransactionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPTRANSACTIONMANAGER_H__D166AC25_6A0C_4046_A136_C0FF13962FE5__INCLUDED_)
#define AFX_SIPTRANSACTIONMANAGER_H__D166AC25_6A0C_4046_A136_C0FF13962FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Singleton.h"
#include "SipTransaction.h"
#include "SipEvent.h"
#include "SipMessage.h"
#include "ThreadBase.h"

class CSipTransactionManager : public Singleton<CSipTransactionManager>,public CThreadBase
{
public:
	int HandleInComming(CSipMessage *pMessage);
	int OutComming(CSipMessage *pMessage);
	CSipTransaction *FindTransactionAndAddEvent(CSipEvent *pEvent,int consume);

	int Execute();

	int SendOutMessage(CSipTransaction*tr, CSipMessage*sip, char*host, int iport, int outsocket);

	virtual bool ThreadWork();

	/* list of transactions for ict, ist, nict, nist */
	CZhList		osip_ict_transactions;   /**< list of ict transactions */
	CZhList		osip_ist_transactions;   /**< list of ist transactions */
	CZhList		osip_nict_transactions;  /**< list of nict transactions */
	CZhList		osip_nist_transactions;  /**< list of nist transactions */

	CZhList		m_listDialogs;

	CZhList		m_listCalls;

	CZhList		m_listSubscribes;

	CZhList		m_listNotifys;

	CCriticalSection m_sectIct;
	CCriticalSection m_sectNict;
	CCriticalSection m_sectIst;
	CCriticalSection m_sectNist;

private:
	CSipTransactionManager();
	virtual ~CSipTransactionManager();
	
	friend class Singleton<CSipTransactionManager>;

public:	
	CSipTransaction* FindTransaction(CZhList* transactions,CSipEvent* evt);
	int __osip_transaction_matching_response_osip_to_xict_17_1_3(CSipTransaction* tr, CSipMessage* response);
	void eXosip_update();
};

#endif // !defined(AFX_SIPTRANSACTIONMANAGER_H__D166AC25_6A0C_4046_A136_C0FF13962FE5__INCLUDED_)
