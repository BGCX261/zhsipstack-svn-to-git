// ZhAuthinfo.cpp: implementation of the CZhAuthinfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhAuthinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhAuthinfo::CZhAuthinfo()
{
	Init();
}

CZhAuthinfo::~CZhAuthinfo()
{
	Free();
}

int CZhAuthinfo::Init()
{
	memset(username,0,50);
	memset(userid,0,50);
	memset(passwd,0,50);
	memset(ha1,0,50);
	memset(realm,0,50);

	return 0;
}

void CZhAuthinfo::Free()
{
	
}
