// ContentEncoding.cpp: implementation of the CContentEncoding class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ContentEncoding.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentEncoding::CContentEncoding()
{
	Init();
}

CContentEncoding::~CContentEncoding()
{
	Free();
}

int CContentEncoding::Init()
{
	this->value = NULL;	
	
	return 0;
}

void CContentEncoding::Free()
{
	mm_free(this->value);
	this->value = NULL;
}

int CContentEncoding::Parse( const char *hvalue )
{
	size_t len;
	
	if (hvalue == NULL)
		return -1;
	len = strlen(hvalue);
	if (len + 1 < 2)
		return -1;
	this->value = (char *) mm_malloc(len + 1);
	if (this->value == NULL)
		return -1;
	zstr_strncpy(this->value, hvalue, len);
	return 0;
}

int CContentEncoding::ToStr(char** dest)
{
	*dest = zstr_strdup(this->value);
	return 0;
}

int CContentEncoding::Clone(CContentEncoding** dest)
{
	CContentEncoding* cl;

	*dest = NULL;
	/*
	   empty headers are allowed:
	   if (ctl->value==NULL) return -1;
	 */
	cl = new CContentEncoding();
	if (cl == NULL)				  /* allocation failed */
		return -1;
	if (this->value != NULL)
		cl->value = zstr_strdup(this->value);

	*dest = cl;
	return 0;
}
