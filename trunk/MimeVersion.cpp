// MimeVersion.cpp: implementation of the CMimeVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "MimeVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMimeVersion::CMimeVersion()
{
	Init();
}

CMimeVersion::~CMimeVersion()
{
	Free();
}

int CMimeVersion::Init()
{
	this->value = NULL;	
	
	return 0;
}

void CMimeVersion::Free()
{
	mm_free(this->value);
	this->value = NULL;
}

int CMimeVersion::Parse( const char *hvalue )
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

int CMimeVersion::Clone(CMimeVersion** dest)
{
	CMimeVersion* cl;

	*dest = NULL;
	/*
	   empty headers are allowed:
	   if (ctl->value==NULL) return -1;
	 */
	cl = new CMimeVersion();
	if (cl == NULL)				  /* allocation failed */
		return -1;
	if (this->value != NULL)
		cl->value = zstr_strdup(this->value);

	*dest = cl;
	return 0;
}

int CMimeVersion::ToStr(char** dest)
{
	*dest = zstr_strdup(this->value);
	return 0;
}