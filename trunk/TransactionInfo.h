// TransactionInfo.h: interface for the CTransactionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSACTIONINFO_H__3D4DF3A6_DFAE_4F7C_B5CF_44664CECCCD9__INCLUDED_)
#define AFX_TRANSACTIONINFO_H__3D4DF3A6_DFAE_4F7C_B5CF_44664CECCCD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZhDialog.h"
#include "ZhCall.h"
#include "ZhSubscribe.h"
#include "ZhNotify.h"

class CTransactionInfo  
{
public:
	CTransactionInfo();
	virtual ~CTransactionInfo();

	CZhDialog* jd;
	CZhCall* jc;
#ifndef MINISIZE
	CZhSubscribe* js;
	CZhNotify* jn;
#endif

};

#endif // !defined(AFX_TRANSACTIONINFO_H__3D4DF3A6_DFAE_4F7C_B5CF_44664CECCCD9__INCLUDED_)
