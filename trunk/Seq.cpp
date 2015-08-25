// Seq.cpp: implementation of the CSeq class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Seq.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeq::CSeq()
{
	Init();
}

CSeq::~CSeq()
{
	Free();
}

int CSeq::Parse( const char *hvalue )
{
	char* method = NULL;
	const char* end = NULL;
	
	this->number = NULL;
	this->method = NULL;
	
	method = strchr(hvalue, ' ');   	 /* SEARCH FOR SPACE */
	end = hvalue + strlen(hvalue);
	
	if (method == NULL)
		return -1;
	
	if (method - hvalue + 1 < 2)
		return -1;
	this->number = (char *) mm_malloc(method - hvalue + 1);
	if (this->number == NULL)
		return -1;
	zstr_clrncpy(this->number, hvalue, method - hvalue);
	
	if (end - method + 1 < 2)
		return -1;
	this->method = (char *) mm_malloc(end - method + 1);
	if (this->method == NULL)
		return -1;
	zstr_clrncpy(this->method, method + 1, end - method);
	
	return 0;   				  /* ok */
}
/* returns the cseq header as a string. 		 */
/* INPUT : osip_cseq_t *cseq | cseq header.  */
/* returns null on error. */
int CSeq::ToStr(char** dest)
{
	size_t len;
	
	*dest = NULL;
	if ((this == NULL) || (this->number == NULL) || (this->method == NULL))
		return -1;
	len = strlen(this->method) + strlen(this->number) + 2;
	*dest = (char *) mm_malloc(len);
	if (*dest == NULL)
		return -1;
	sprintf(*dest, "%s %s", this->number, this->method);
	return 0;
}

int CSeq::Init()
{
	this->number = NULL;
	this->method = NULL;

	return 0;
}

void CSeq::Free()
{
	mm_free(this->number);	
	mm_free(this->method);	
	
	this->number = NULL;
	this->method = NULL;
}

int CSeq::Clone( CSeq** dest )
{
	CSeq* cs;
	
	*dest = NULL;

	if (this->method == NULL)
		return -1;
	if (this->number == NULL)
		return -1;
	
	cs = new CSeq;
	if (cs == NULL)
	{
		delete(cs);
		cs = NULL;
		return -1;
	}
	cs->method = zstr_strdup(this->method);
	cs->number = zstr_strdup(this->number);
	
	*dest = cs;
	return 0;	
}

void CSeq::SetNumber( char* value )
{
	number = value;	
}

char* CSeq::GetNumber()
{
	return number;
}

void CSeq::SetMethod( char* value )
{
	method = value;	
}

char* CSeq::GetMethod()
{
	return method;
}

int CSeq::Match( CSeq* cseq )
{
	if (cseq == NULL)
		return -1;
	if (this->number == NULL ||
		cseq->number == NULL ||
		this->method == NULL ||
		cseq->method == NULL)
		return -1;
	
	if (0 == strcmp(this->number, cseq->number))
	{
		if (0 == strcmp(cseq->method, "INVITE") || 0 == strcmp(cseq->method,
			"ACK"))
		{
			if (0 == strcmp(this->method, "INVITE") ||
				0 == strcmp(this->method, "ACK"))
			{
				return 0;
			}
		}
		else
		{
			if (0 == strcmp(this->method, cseq->method))
			{
				return 0;
			}
		}
	}
	return -1;
}
