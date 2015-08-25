// UrlParam.cpp: implementation of the CUrlParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "UrlParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUrlParam::CUrlParam()
{
	InitParam();
}

CUrlParam::~CUrlParam()
{
	FreeParam();
}

int CUrlParam::InitParam()
{
	gname = NULL;
	gvalue = NULL;		

	return 0;
}

void CUrlParam::FreeParam()
{
	free(gname);
	free(gvalue);
	gname = NULL;
	gvalue = NULL;	
}

int CUrlParam::SetParam( char* pname, char* pvalue )
{
	gname = pname;
	/* not needed for url, but for all other generic params */
	zstr_clrspace(gname);
	gvalue = pvalue;
	if (gvalue != NULL)
		zstr_clrspace(gvalue);

	return 0;
}

int osip_uri_param_add( CZhList* url_params, char* name, char* value )
{
	int i;
	CUrlParam* url_param = new CUrlParam;
	
	i = url_param->SetParam(name, value);
	if (i != 0)
	{
		delete url_param;
		return -1;
	}
	url_params->Add(url_param, -1);

	return 0;	
}

int osip_uri_param_set(CUrlParam* url_param, char* pname, char* pvalue)
{
	url_param->gname = pname;
	/* not needed for url, but for all other generic params */
	zstr_clrspace(url_param->gname);
	url_param->gvalue = pvalue;
	if (url_param->gvalue != NULL)
	{
		zstr_clrspace(url_param->gvalue);
	}
	return 0;
}

int osip_uri_param_clone(const CUrlParam* uparam,CUrlParam** dest)
{
	CUrlParam* up;
	
	*dest = NULL;
	if (uparam == NULL)
		return -1;
	if (uparam->gname == NULL)
		return -1;  				/* name is mandatory */
	
	up = new CUrlParam;
	if (up == NULL)
	{
		return -1;
	}

	up->gname = zstr_strdup(uparam->gname);
	if (uparam->gvalue != NULL)
		up->gvalue = zstr_strdup(uparam->gvalue);
	else
		up->gvalue = NULL;
	*dest = up;
	return 0;
}

int osip_uri_param_get_byname(CZhList* params, char* pname,CUrlParam** url_param)
{
	int pos = 0;
	size_t pname_len;
	CUrlParam* u_param;
	
	*url_param = NULL;
	if (pname == NULL)
		return -1;
	pname_len = strlen(pname);
	if (pname_len <= 0)
		return -1;
	while (!params->IsListEof(pos))
	{
		size_t len;
		
		u_param = (CUrlParam *)params->GetAt(pos);
		len = strlen(u_param->gname);
		if (pname_len == len && zstr_strncasecmp(u_param->gname, pname,
			strlen(pname)) == 0)
		{
			*url_param = u_param;
			return 0;
		}
		pos++;
	}
	return -1;
}

void osip_uri_param_freelist(CZhList* params)
{
	CUrlParam* u_param;
	
	while (!(params->IsListEof(0)))
	{
		u_param = (CUrlParam*)params->GetAt(0);
		params->RemoveAt(0);
		delete u_param;
		u_param = NULL;
	}
}