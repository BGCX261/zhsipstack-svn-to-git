// ZhSubscribe.cpp: implementation of the CZhSubscribe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhSubscribe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhSubscribe::CZhSubscribe()
{
	Init();
}

CZhSubscribe::~CZhSubscribe()
{
	Free();
}

int CZhSubscribe::Init()
{
	s_id = 0;
	s_ss_status = 0;
	s_ss_reason = 0;
	s_reg_period = 0;
	
	s_retry = 0;				/* avoid too many unsuccessfull retry */
	s_inc_tr = NULL;
	s_out_tr = NULL;
	
	return 0;
}

void CZhSubscribe::Free()
{
	CZhDialog* jd;
	
	if (s_inc_tr != NULL &&
		s_inc_tr->orig_request != NULL &&
		s_inc_tr->orig_request->m_callid != NULL &&
		s_inc_tr->orig_request->m_callid->number != NULL)
	{
		//_eXosip_delete_nonce(s_inc_tr->orig_request->call_id->number);
	}
	else if (s_out_tr != NULL &&
		s_out_tr->orig_request != NULL &&
		s_out_tr->orig_request->m_callid != NULL &&
		s_out_tr->orig_request->m_callid->number != NULL)
	{
		//_eXosip_delete_nonce(s_out_tr->orig_request->call_id->number);
	}
	
	int pos = 0;
	while (!s_dialogs.IsListEof(pos))
	{
		jd = (CZhDialog *) s_dialogs.GetAt(pos);
		s_dialogs.RemoveAt(pos);
		delete(jd);
	}
	
// 	__eXosip_delete_jinfo(s_inc_tr);
// 	__eXosip_delete_jinfo(s_out_tr);
// 	if (s_inc_tr != NULL)
// 		osip_list_add(eXosip.j_transactions, s_inc_tr, 0);
// 	if (s_out_tr != NULL)
// 		osip_list_add(eXosip.j_transactions, s_out_tr, 0);
	
}
