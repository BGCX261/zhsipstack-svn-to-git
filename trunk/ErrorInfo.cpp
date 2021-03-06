// ErrorInfo.cpp: implementation of the CErrorInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ErrorInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorInfo::CErrorInfo()
{
	Init();
}

CErrorInfo::~CErrorInfo()
{
	Free();
}

int CErrorInfo::Init()
{
	element = NULL;

	return 0;
}

void CErrorInfo::Free()
{
	mm_free(element);
	element = NULL;
}

int CErrorInfo::Parse( const char* hvalue )
{
	const char* osip_call_info_params;
	
	osip_call_info_params = strchr(hvalue, '<');
	if (osip_call_info_params == NULL)
		return -1;
	
	osip_call_info_params = strchr(osip_call_info_params + 1, '>');
	if (osip_call_info_params == NULL)
		return -1;
	
	osip_call_info_params = strchr(osip_call_info_params + 1, ';');
	
	if (osip_call_info_params != NULL)
	{
		if (GenericParamParseAll(&this->gen_params,
			osip_call_info_params) == -1)
			return -1;
	}
	else
		osip_call_info_params = hvalue + strlen(hvalue);
	
	if (osip_call_info_params - hvalue + 1 < 2)
		return -1;
	this->element = (char *) mm_malloc(osip_call_info_params -
		hvalue +
		1);
	if (this->element == NULL)
		return -1;
	zstr_clrncpy(this->element, hvalue, osip_call_info_params - hvalue);
	
	return 0;	
}

int CErrorInfo::ToStr( char** dest )
{
	char* buf;
	char* tmp;
	size_t len;
	size_t plen;
	
	*dest = NULL;
	if ((this->element == NULL))
		return -1;
	
	len = strlen(this->element) + 2;
	buf = (char *) mm_malloc(len);
	if (buf == NULL)
		return -1;
	*dest = buf;
	
	sprintf(buf, "%s", this->element);
	{
		int pos = 0;
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
				sprintf(tmp, ";%s", u_param->gname);
			else
				sprintf(tmp, ";%s=%s", u_param->gname, u_param->gvalue);
			pos++;
		}
	}
	*dest = buf;
	return 0;
}

int CErrorInfo::Clone( CErrorInfo** dest )
{
	int i;
	CErrorInfo* ct;
	
	*dest = NULL;
	
	if (this->element == NULL)
		return -1;
	
	ct = new CErrorInfo();
	if (ct != NULL) 				  /* allocation failed */
		return -1;
	ct->element = zstr_strdup(this->element);
	
	i = this->gen_params.Clone(&(ct->gen_params),(int * (*) (void *, void *)) & osip_generic_param_clone);
	if (i != 0)
	{
		delete (ct);
		return -1;
	}
	*dest = ct;
	return 0;	
}

char* CErrorInfo::GetUrl()
{
	return element;
}

void CErrorInfo::SetUrl( char* uri )
{
	element = uri;
}
