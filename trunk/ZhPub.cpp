// ZhPub.cpp: implementation of the CZhPub class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhPub.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhPub::CZhPub(const char* aor, const char* exp)
{
	Init(aor,exp);
}

CZhPub::~CZhPub()
{
	Free();
}

int CZhPub::Init(const char* aor, const char* exp)
{
	_snprintf(p_aor, 256, "%s", aor);
	
	p_expires = atoi(exp) + time(NULL);
	p_period = atoi(exp);

	p_id = -1;

	p_last_tr = NULL;

	return 0;		
}

void CZhPub::Free()
{
	
}
