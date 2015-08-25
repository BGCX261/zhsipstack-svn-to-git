// ZhNotify.cpp: implementation of the CZhNotify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhNotify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhNotify::CZhNotify(CSipMessage* inc_subscribe)
{
	Init(inc_subscribe);
}

CZhNotify::~CZhNotify()
{
	Free();
}

int CZhNotify::Init(CSipMessage* inc_subscribe)
{
	CContact* co;
	
	co = (CContact*) inc_subscribe->m_contacts.GetAt(0);
	if (co == NULL || co->url == NULL)
		return -1;
	
	n_id = 0;
	n_online_status = 0;
	
	n_ss_status = 0;
	n_ss_reason = 0;
	n_ss_expires = 0;
	
	n_inc_tr = NULL;
	n_out_tr = NULL;

	return 0;
}

void CZhNotify::Free()
{
	
}
