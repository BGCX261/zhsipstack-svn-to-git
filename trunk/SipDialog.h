// SipDialog.h: interface for the CSipDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPDIALOG_H__86028552_8E78_44B3_98E7_727122C8255B__INCLUDED_)
#define AFX_SIPDIALOG_H__86028552_8E78_44B3_98E7_727122C8255B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "From.h"
#include "To.h"
#include "Contact.h"
#include "siptypes.h"
#include "SipMessage.h"

class CSipDialog  
{
public:
	typedef enum _enDialog_Type
	{
		CALLER,
		CALLEE
	} enDialog_Type;

	CSipDialog();
	virtual ~CSipDialog();
	void Free();

	void SetInstance(void *pInstance);
	void *GetInstance();

	int Init(CSipMessage* invite,CSipMessage* response, CFrom* local, CTo* remote,CSipMessage* remote_msg);
	int InitAsUac(CSipMessage* response);
	int InitAsUacWithRemoteRequest(CSipMessage* next_request, int local_cseq);
	int InitAsUas(CSipMessage* invite, CSipMessage* response);
	void SetState(eTransactionState type);
	int UpdateRouteSetAsUas(CSipMessage* invite);
	int UpdateCseqAsUas(CSipMessage* request);
	int MatchAsUac(CSipMessage* response);
	int UpdateTagAsUac(CSipMessage* response);
	int UpdateRouteSetAsUac(CSipMessage* response);
	int MatchAsUas(CSipMessage* request);
	int IsOriginator();
	int IsCallee();

	char* call_id;  					 /**< Call-ID*/
	char* local_tag;					 /**< local tag */
	char* remote_tag;   				 /**< remote tag */
	CZhList route_set;  			/**< route set */
	int local_cseq; 					 /**< last local cseq */
	int remote_cseq;					 /**< last remote cseq*/
	CFrom* remote_uri;  			 /**< remote_uri */
	CFrom* local_uri; 			 /**< local_uri */
	CContact* remote_contact_uri;  /**< remote contact_uri */
	int secure; 						 /**< use secure transport layer */
	
	enDialog_Type type;			 /**< type of dialog (CALLEE or CALLER) */
	eTransactionState state;  					 /**< DIALOG_EARLY || DIALOG_CONFIRMED || DIALOG_CLOSED */
	void* your_instance;				 /**< for application data reference */

};

#endif // !defined(AFX_SIPDIALOG_H__86028552_8E78_44B3_98E7_727122C8255B__INCLUDED_)
