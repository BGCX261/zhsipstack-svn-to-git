// ContentLength.cpp: implementation of the CContentLength class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ContentLength.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentLength::CContentLength()
{
	Init();
}

CContentLength::~CContentLength()
{
	Free();
}

int CContentLength::Init()
{
	this->value = NULL;	
	
	return 0;
}

void CContentLength::Free()
{
	mm_free(this->value);
	this->value = NULL;
}

int CContentLength::Parse( const char *hvalue )
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

int CContentLength::Clone(CContentLength** dest)
{
	CContentLength* cl;

	*dest = NULL;
	/*
	   empty headers are allowed:
	   if (ctl->value==NULL) return -1;
	 */
	cl = new CContentLength();
	if (cl == NULL)				  /* allocation failed */
		return -1;
	if (this->value != NULL)
		cl->value = zstr_strdup(this->value);

	*dest = cl;
	return 0;
}

int CContentLength::ToStr(char** dest)
{
	*dest = zstr_strdup(this->value);
	return 0;
}
