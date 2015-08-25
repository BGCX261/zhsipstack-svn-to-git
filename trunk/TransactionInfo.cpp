// TransactionInfo.cpp: implementation of the CTransactionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "TransactionInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransactionInfo::CTransactionInfo()
{
	jd = NULL;
	jc = NULL;
#ifndef MINISIZE
	js = NULL;
	jn = NULL;
#endif
}

CTransactionInfo::~CTransactionInfo()
{

}
