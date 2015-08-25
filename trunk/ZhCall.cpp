// SipCall.cpp: implementation of the CZhCall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZHCall.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhCall::CZhCall()
{
	Init();
}

CZhCall::~CZhCall()
{
	Free();
}

int CZhCall::Init()
{
	c_inc_tr = NULL;
	c_out_tr = NULL;
	c_retry = 0;
	external_reference = NULL;
	response_auth = NULL;
	
	c_id = -1;   		  /* make sure the eXosip_update will assign a valid id to the call */
	return 0;	
}

void CZhCall::Free()
{
	CZhDialog* jd;
	
	if (response_auth != NULL)
		delete(response_auth);
	
	if (c_inc_tr != NULL &&
		c_inc_tr->orig_request != NULL &&
		c_inc_tr->orig_request->m_callid != NULL &&
		c_inc_tr->orig_request->m_callid->number != NULL)
	{
		//_eXosip_delete_nonce(c_inc_tr->orig_request->call_id->number);
	}
	else if (c_out_tr != NULL &&
		c_out_tr->orig_request != NULL &&
		c_out_tr->orig_request->m_callid != NULL &&
		c_out_tr->orig_request->m_callid->number != NULL)
	{
		//_eXosip_delete_nonce(c_out_tr->orig_request->call_id->number);
	}

	int pos = 0;
	while (!c_dialogs.IsListEof(pos))
	{
		jd = (CZhDialog *) c_dialogs.GetAt(pos);
		c_dialogs.RemoveAt(pos);
		delete(jd);
	}
	
	//__eXosip_delete_jinfo(c_inc_tr);
	//__eXosip_delete_jinfo(c_out_tr);
	if (c_inc_tr != NULL)
	{
		//osip_list_add(eXosip.j_transactions, c_inc_tr, 0);
	}
	if (c_out_tr != NULL)
	{
		//osip_list_add(eXosip.j_transactions, c_out_tr, 0);
	}
}

void CZhCall::RemoveDialogInCall( CZhDialog *pDailog )
{
	CZhDialog* _jd;
//	jinfo_t* ji;
	
	if (pDailog == NULL)
		return;
	
	int pos = 0;
	while (!c_dialogs.IsListEof(pos))
	{
		_jd = (CZhDialog *) c_dialogs.GetAt(pos);
		if (pDailog == _jd)
			break;
	}

	if (_jd == NULL)
	{
		/* dialog not found??? */
	}
	
// 	ji = osip_transaction_get_your_instance(jc->c_inc_tr);
// 	if (ji != NULL && ji->jd == jd)
// 		ji->jd = NULL;
// 	ji = osip_transaction_get_your_instance(jc->c_out_tr);
// 	if (ji != NULL && ji->jd == jd)
// 		ji->jd = NULL;	
}
