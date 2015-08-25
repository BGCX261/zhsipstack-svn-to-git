// SipTransactionManager.cpp: implementation of the CSipTransactionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipTransactionManager.h"
#include "IctTransaction.h"
#include "NictTransaction.h"
#include "ThreadNet.h"
#include "ZhCall.h"
#include "ZhDialog.h"
#include "ZhList.h"
#include "ZhReg.h"
#include "ZhSubscribe.h"
#include "ZhNotify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipTransactionManager::CSipTransactionManager()
{

}

CSipTransactionManager::~CSipTransactionManager()
{

}

int CSipTransactionManager::Execute()
{
	CSipTransaction* transaction;
	int i = 0;

	{
		CSingleLock lock(&m_sectIct,TRUE);
		for (i = 0; i < osip_ict_transactions.GetSize(); i++)
		{
			transaction = (CSipTransaction*)osip_ict_transactions.GetAt(0);
			if (transaction != NULL)
			{
				transaction->Execute();
			}
	
			//osip_ict_transactions.RemoveAt(0);
		}
	}
	{
		CSingleLock lock(&m_sectNict,TRUE);
		for (i = 0; i < osip_nict_transactions.GetSize(); i++)
		{
			transaction = (CSipTransaction*)osip_nict_transactions.GetAt(0);
			if (transaction != NULL)
			{
				transaction->Execute();
			}
			
			//osip_nict_transactions.RemoveAt(0);
		}
	}	
	{
		CSingleLock lock(&m_sectIst,TRUE);
		for (i = 0; i < osip_ist_transactions.GetSize(); i++)
		{
			transaction = (CSipTransaction*)osip_ist_transactions.GetAt(0);
			if (transaction != NULL)
			{
				transaction->Execute();
			}
			
			//osip_nict_transactions.RemoveAt(0);
		}
	}
	{
		CSingleLock lock(&m_sectNist,TRUE);
		for (i = 0; i < osip_nist_transactions.GetSize(); i++)
		{
			transaction = (CSipTransaction*)osip_nist_transactions.GetAt(0);
			if (transaction != NULL)
			{
				transaction->Execute();
			}
			
			//osip_nict_transactions.RemoveAt(0);
		}
	}

	return 0;	
}

CSipTransaction* CSipTransactionManager::FindTransactionAndAddEvent( CSipEvent *evt,int consume)
{
	
	if (evt == NULL || evt->sip == NULL || evt->sip->m_seq == NULL)
		return NULL;
	
	CZhList *pTransactionList = NULL;
	CSipTransaction* transaction = NULL;
	CCriticalSection *pSect;

	if (EVT_IS_INCOMINGMSG(evt))
	{
		if (MSG_IS_REQUEST(evt->sip))
		{
			if (0 == strcmp(evt->sip->m_seq->method, "INVITE") ||
				0 == strcmp(evt->sip->m_seq->method, "ACK"))
			{
				pTransactionList = &osip_ist_transactions;
				pSect = &m_sectIst;
			}
			else
			{
				pTransactionList = &osip_nist_transactions;
				pSect = &m_sectNist;
			}
		}
		else
		{
			if (0 == strcmp(evt->sip->m_seq->method, "INVITE"))
			{
				pTransactionList = &osip_ict_transactions;
				pSect = &m_sectIct;
			}
			else
			{
				pTransactionList = &osip_nict_transactions;
				pSect = &m_sectNict;
			}
		}
	}
	else if (EVT_IS_OUTGOINGMSG(evt))
	{
		if (MSG_IS_RESPONSE(evt->sip))
		{
			if (0 == strcmp(evt->sip->m_seq->method, "INVITE"))
			{
				pTransactionList = &osip_ist_transactions;
				pSect = &m_sectIst;
			}
			else
			{
				pTransactionList = &osip_nist_transactions;
				pSect = &m_sectNist;
			}
		}
		else
		{
			if (0 == strcmp(evt->sip->m_seq->method, "INVITE") ||
				0 == strcmp(evt->sip->m_seq->method, "ACK"))
			{
				pTransactionList = &osip_ict_transactions;
				pSect = &m_sectIct;
			}
			else
			{
				pTransactionList = &osip_nict_transactions;
				pSect = &m_sectNict;
			}
		}
	}

	if (pTransactionList == NULL)
		return NULL;				/* not a message??? */

	CSingleLock(pSect,TRUE);

	transaction = FindTransaction(pTransactionList, evt);
	if (consume == 1)
	{
		/* we add the event before releasing the mutex!! */
		if (transaction != NULL)
		{
			transaction->AddEvent(evt);
			return transaction;
		}
	}

	return transaction;
}

CSipTransaction* CSipTransactionManager::FindTransaction(CZhList* transactions,CSipEvent* evt)
{
	ListIterator iterator;
	CSipTransaction* transaction;

	transaction = (CSipTransaction*) transactions->GetFirstPos(&iterator);

	if (EVT_IS_INCOMINGREQ(evt))
	{
		transaction = (CSipTransaction *) transactions->GetFirstPos(&iterator);
		while (IsListIteratorHasElem(iterator))
		{
// 			if (0 == __osip_transaction_matching_request_osip_to_xist_17_2_3(transaction,
// 					evt->sip))
// 			{
// 				return transaction;
// 			}
			transaction = (CSipTransaction *) transactions->GetNextPos(&iterator);
		}
	}
	else if (EVT_IS_INCOMINGRESP(evt))
	{
		transaction = (CSipTransaction *) transactions->GetFirstPos(&iterator);
		while (IsListIteratorHasElem(iterator))
		{
			if (0 ==
				__osip_transaction_matching_response_osip_to_xict_17_1_3(transaction,
					evt->sip))
				return transaction;
			transaction = (CSipTransaction *)transactions->GetNextPos(&iterator);
		}
	}
	else						/* handle OUTGOING message */
	{
		/* THE TRANSACTION ID MUST BE SET */
		transaction = (CSipTransaction *) transactions->GetFirstPos(&iterator);
		while (IsListIteratorHasElem(iterator))
		{
			if (transaction->transactionid == evt->transactionid)
				return transaction;
			transaction = (CSipTransaction *)transactions->GetNextPos(&iterator);
		}
	}
	return NULL;
}

int CSipTransactionManager::__osip_transaction_matching_response_osip_to_xict_17_1_3(CSipTransaction* tr,
	CSipMessage* response)
{
	CUrlParam* b_request;
	CUrlParam* b_response;
	CVia* topvia_response;

	/* some checks to avoid crashing on bad requests */
	if (tr == NULL ||
		/*(tr->ict_context == NULL && tr->nict_context == NULL) || */
	/* only ict and nict can match a response */
		response == NULL ||
		response->m_seq == NULL ||
		response->m_seq->method == NULL)
		return -1;

	topvia_response = (CVia*)response->m_listVias.GetAt(0);
	if (topvia_response == NULL)
	{
		return -1;
	}
	osip_via_param_get_byname(tr->topvia, "branch", &b_request);
	if (b_request == NULL)
	{
		return -1;
	}
	osip_via_param_get_byname(topvia_response, "branch", &b_response);
	if (b_response == NULL)
	{
	}

	/*
	   A response matches a client transaction under two
	   conditions:
	   1.   If the response has the same value of the branch parameter
	   in the top Via header field as the branch parameter in the
	   top Via header field of the request that created the
	   transaction.
	 */
	if (0 != strcmp(b_request->gvalue, b_response->gvalue))
	{
		return -1;
	}
	/*  
	   2.   If the method parameter in the CSeq header field matches
	   the method of the request that created the transaction. The
	   method is needed since a CANCEL request constitutes a
	   different transaction, but shares the same value of the
	   branch parameter.
	   AMD NOTE: cseq->method is ALWAYS the same than the METHOD of the request.
	 */
	if (0 == strcmp(response->m_seq->method, tr->cseq->method))   /* general case */
	{
		return 0;
	}
	return -1;
}

int CSipTransactionManager::HandleInComming( CSipMessage *pMessage )
{
	if (pMessage == NULL)
	{
		return -1;
	}
	
	CSipEvent *pEvent = CSipEvent::InComingEventNew(pMessage);
	
	CSipTransaction* pTransaction = NULL;
	
	pTransaction = FindTransactionAndAddEvent(pEvent,1);
	
	if (pTransaction == NULL)
	{
		return -1;
	}
	
	
	return 0;
}

int CSipTransactionManager::OutComming( CSipMessage *pMessage )
{
	CSipTransaction *pTransaction = NULL;

	if (MSG_IS_REQUEST(pMessage))
	{
		if (MSG_IS_INVITE(pMessage))
		{
			pTransaction = new CIctTransaction(pMessage);
			osip_ict_transactions.Add(pTransaction);
		}
		else
		{
			pTransaction = new CNictTransaction(pMessage);
			osip_nict_transactions.Add(pTransaction);
		}
	}
	
	if (pTransaction == NULL)
	{
		return -1;
	}

	CSipEvent *pEvent = CSipEvent::OutGoingEventNew(pMessage);
	pEvent->transactionid = pTransaction->transactionid;
	
	pTransaction->AddEvent(pEvent);
	
	return 0;
}

bool CSipTransactionManager::ThreadWork()
{
	if (!OnThreadCreate())
		return false;	
	
	while (!m_bThreadExit)
	{
		Execute();		
		
		Sleep(1);
	}
	
	return true;
}

int CSipTransactionManager::SendOutMessage( CSipTransaction*tr, CSipMessage*sip, char*host, int iport, int outsocket )
{
		CThreadNet *pNet = CThreadNet::getInstance();
	if (pNet == NULL)
	{
		return 0;
	}
	
	int i;
	CVia* via;

#ifndef MINISIZE
	if (sip->m_iStatusCode == 101)
		return 0;
#else
	if (sip->m_iStatusCode == 101)
		return 0;
#endif

	via = (CVia *) sip->m_listVias.GetAt(0);
	if (via == NULL || via->m_pcProtocol == NULL)
		return -1;

	if (host == NULL)
	{
		if (MSG_IS_REQUEST(sip))
		{
			CRoute* route;

			sip->GetRoute(0, &route);
			if (route != NULL)
			{
				CUrlParam* lr_param = NULL;

				osip_uri_uparam_get_byname(route->url, "lr", &lr_param);
				if (lr_param == NULL)
					route = NULL;
			}

			if (route != NULL)
			{
				iport = 5060;
				if (route->url->port != NULL)
					iport = zstr_atoi(route->url->port);
				host = route->url->host;
			}
			else
			{
				/* search for maddr parameter */
				CUrlParam* maddr_param = NULL;
				osip_uri_uparam_get_byname(sip->m_pRequestUrl, "maddr", &maddr_param);
				host = NULL;
				if (maddr_param != NULL && maddr_param->gvalue != NULL)
					host = maddr_param->gvalue;

				iport = 5060;
				if (sip->m_pRequestUrl->port != NULL)
					iport = zstr_atoi(sip->m_pRequestUrl->port);

				if (host == NULL)
					host = sip->m_pRequestUrl->host;
			}
		}
		else
		{
			CUrlParam* maddr;
			CUrlParam* received;
			CUrlParam* rport;

			osip_via_param_get_byname(via, "maddr", &maddr);
			osip_via_param_get_byname(via, "received", &received);
			osip_via_param_get_byname(via, "rport", &rport);
			if (maddr != NULL && maddr->gvalue != NULL)
				host = maddr->gvalue;
			else if (received != NULL && received->gvalue != NULL)
				host = received->gvalue;
			else
				host = via->m_pcHost;

			if (rport == NULL || rport->gvalue == NULL)
			{
				if (via->m_pcPort != NULL)
					iport = zstr_atoi(via->m_pcPort);
				else
					iport = 5060;
			}
			else
				iport = zstr_atoi(rport->gvalue);
		}
	}

	i = -1;
	if (zstr_strcasecmp(via->m_pcProtocol, "udp") == 0)
	{
		i = pNet->SendOutMessage(tr,sip,host,iport,outsocket);
	}
	
	if (i != 0)
	{
		return -1;
	}

	return 0;
}

void CSipTransactionManager::eXosip_update()
{
	static int static_id = 1;
	CZhCall* jc;
#ifndef MINISIZE
	CZhSubscribe* js;
	CZhNotify* jn;
#endif
	CZhDialog* jd;
	time_t now;
	
	if (static_id > 100000)
		static_id = 1;  			/* loop */
	
	now = time(NULL);
	int pos = 0;

	while (!m_listCalls.IsListEof(pos))
	{
		jc = (CZhCall *) m_listCalls.GetAt(pos);
		if (jc->c_id < 1)
		{
			jc->c_id = static_id;
			static_id++;
		}

		int ipos = 0;
		while (!jc->c_dialogs.IsListEof(ipos))
		{
			if (jd->d_dialog != NULL)     /* finished call */
			{
				if (jd->d_id < 1)
				{
					jd->d_id = static_id;
					static_id++;
				}
			}
			else
				jd->d_id = -1;

			ipos++;
		}
		pos++;
	}
	
#ifndef MINISIZE
	pos = 0;
	while (!m_listSubscribes.IsListEof(pos))
	{
		js = (CZhSubscribe *) m_listSubscribes.GetAt(pos);
		if (js->s_id < 1)
		{
			js->s_id = static_id;
			static_id++;
		}
		
		int ipos = 0;
		while (!js->s_dialogs.IsListEof(ipos))
		{
			jd = (CZhDialog*) js->s_dialogs.GetAt(ipos);
			if (jd->d_dialog != NULL)     /* finished call */
			{
				if (jd->d_id < 1)
				{
					jd->d_id = static_id;
					static_id++;
				}
			}
			else
				jd->d_id = -1;
			
			ipos++;
		}
		pos++;
	}

	pos = 0;
	while (!m_listNotifys.IsListEof(pos))
	{
		jn = (CZhNotify *) m_listNotifys.GetAt(pos);
		if (jn->n_id < 1)
		{
			jn->n_id = static_id;
			static_id++;
		}
		
		int ipos = 0;
		while (!jn->n_dialogs.IsListEof(ipos))
		{
			jd = (CZhDialog*) jn->n_dialogs.GetAt(ipos);
			if (jd->d_dialog != NULL)     /* finished call */
			{
				if (jd->d_id < 1)
				{
					jd->d_id = static_id;
					static_id++;
				}
			}
			else
				jd->d_id = -1;
			
			ipos++;
		}
		pos++;
	}
#endif
}