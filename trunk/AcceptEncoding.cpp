// AcceptEncoding.cpp: implementation of the CAcceptEncoding class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "AcceptEncoding.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptEncoding::CAcceptEncoding()
{
	Init();
}

CAcceptEncoding::~CAcceptEncoding()
{
	Free();
}

int CAcceptEncoding::Init()
{
	this->element = NULL;
	
	return 0;
}

void CAcceptEncoding::Free()
{
	mm_free(this->element);
	osip_generic_param_freelist(&this->gen_params);
	this->element = NULL;
}

int CAcceptEncoding::Parse( const char *hvalue )
{
	const char* osip_accept_encoding_params;
	
	osip_accept_encoding_params = strchr(hvalue, ';');
	
	if (osip_accept_encoding_params != NULL)
	{
		if (GenericParamParseAll(&(this->gen_params),osip_accept_encoding_params) == -1)
		{
			return -1;
		}
	}
	else
	{
		osip_accept_encoding_params = hvalue + strlen(hvalue);
	}
	
	if (osip_accept_encoding_params - hvalue + 1 < 2)
		return -1;
	this->element = (char *) mm_malloc(osip_accept_encoding_params - hvalue + 1);
	if (this->element == NULL)
		return -1;
	zstr_clrncpy(this->element, hvalue,osip_accept_encoding_params - hvalue);
	
	return 0;
}

int CAcceptEncoding::Clone( CAcceptEncoding** dest )
{
	int i;
	CAcceptEncoding* ct;
	
	*dest = NULL;

	if (this->element == NULL)
		return -1;
	
	ct = new CAcceptEncoding();
	if (ct == NULL) 				  /* allocation failed */
		return -1;
	ct->element = zstr_strdup(this->element);
	if (this->element != NULL && ct->element == NULL)
	{
		delete (ct);
		return -1;
	}
	{
		int pos = 0;
		CUrlParam* u_param;
		CUrlParam* dest_param;
		
		while (!this->gen_params.IsListEof(pos))
		{
			u_param = (CUrlParam *) this->gen_params.GetAt(pos);
			i = osip_generic_param_clone(u_param, &dest_param);
			if (i != 0)
			{
				delete (ct);
				return -1;
			}
			ct->gen_params.Add(dest_param, -1);
			pos++;
		}
	}
	*dest = ct;
	return 0;	
}

void CAcceptEncoding::SetElement( char* value )
{
	element = value;
}

char* CAcceptEncoding::GetElement()
{
	return element;
}

int CAcceptEncoding::ToStr( char** dest )
{
	char* buf;
	char* tmp;
	size_t len;
	
	*dest = NULL;
	if ( this->element == NULL)
		return -1;
	
	len = strlen(this->element) + 2;
	buf = (char *) mm_malloc(len);
	if (buf == NULL)
		return -1;
	
	sprintf(buf, "%s", this->element);
	{
		int pos = 0;
		size_t plen;
		CUrlParam* u_param;
		
		while (!this->gen_params.IsListEof(pos))
		{
			u_param = (CUrlParam *) this->gen_params.GetAt(pos);
			if (u_param->gvalue == NULL)
			{
				plen = strlen(u_param->gname) + 2;
			}
			else
			{
				plen = strlen(u_param->gname) + strlen(u_param->gvalue) + 3;
			}
			len = len + plen;
			buf = (char *) mm_realloc(buf, len);
			tmp = buf;
			tmp = tmp + strlen(tmp);
			if (u_param->gvalue == NULL)
			{
				sprintf(tmp, ";%s", u_param->gname);
			}
			else
			{
				sprintf(tmp, ";%s=%s", u_param->gname, u_param->gvalue);
			}
			pos++;
		}
	}
	*dest = buf;
	return 0;	
}
