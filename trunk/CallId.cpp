// CallId.cpp: implementation of the CCallId class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "CallId.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCallId::CCallId()
{
	Init();
}

CCallId::~CCallId()
{
	Free();
}

int CCallId::Init()
{
	number = NULL;
	host = NULL;

	return 0;	
}

void CCallId::Free()
{
	mm_free(number);
	number = NULL;
	mm_free(host);
	host = NULL;
}

int CCallId::Parse( const char* hvalue )
{
	const char* host;
	const char* end;
	
	this->number = NULL;
	this->host = NULL;
	
	host = strchr(hvalue, '@');  /* SEARCH FOR '@' */
	end = hvalue + strlen(hvalue);
	
	if (host == NULL)
		host = end;
	else
	{
		if (end - host + 1 < 2)
			return -1;
		this->host = (char *) mm_malloc(end - host);
		if (this->host == NULL)
			return -1;
		zstr_clrncpy(this->host, host + 1, end - host - 1);
	}
	if (host - hvalue + 1 < 2)
		return -1;
	this->number = (char *) mm_malloc(host - hvalue + 1);
	if (this->number == NULL)
		return -1;
	zstr_clrncpy(this->number, hvalue, host - hvalue);
	
	return 0;   				  /* ok */
}

/* returns the call_id as a string. 		 */
/* INPUT : osip_call_id_t *call_id | call_id.  */
/* returns null on error. */
int CCallId::ToStr(char** dest)
{
	*dest = NULL;
	if ((this->number == NULL))
		return -1;
	
	if (this->host == NULL)
	{
		*dest = (char *) mm_malloc(strlen(this->number) + 1);
		if (*dest == NULL)
			return -1;
		sprintf(*dest, "%s", this->number);
	}
	else
	{
		*dest = (char *) mm_malloc(strlen(this->number) +
			strlen(this->host) +
			2);
		if (*dest == NULL)
			return -1;
		sprintf(*dest, "%s@%s", this->number, this->host);
	}
	return 0;
}

int CCallId::Clone( CCallId** dest )
{
	CCallId* ci;
	
	*dest = NULL;

	if (this->number == NULL)
		return -1;
	
	ci = new CCallId;
	if (ci == NULL)				  /* allocation failed */
		return -1;
	ci->number = zstr_strdup(this->number);
	if (this->host != NULL)
		ci->host = zstr_strdup(this->host);
	
	*dest = ci;
	return 0;	
}

void CCallId::SetNumber( char* value )
{
	this->number = value;
}

char* CCallId::GetNumber()
{
	return number;
}

void CCallId::SetHost( char* value )
{
	host = value;	
}

char* CCallId::GetHost()
{
	return host;	
}

int CCallId::IdMatch( CCallId* callid )
{
	if (callid == NULL)
		return -1;
	if (this->number == NULL || callid->number == NULL)
		return -1;
	
	if (0 != strcmp(this->number, callid->number))
		return -1;
	
	if ((this->host == NULL) && (callid->host == NULL))
		return 0;
	if ((this->host == NULL) && (callid->host != NULL))
		return -1;
	if ((this->host != NULL) && (callid->host == NULL))
		return -1;
	if (0 != strcmp(this->host, callid->host))
		return -1;
	
	return 0;	
}
