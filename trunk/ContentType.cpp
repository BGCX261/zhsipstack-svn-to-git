// ContentType.cpp: implementation of the CContentType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ContentType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentType::CContentType()
{
	Init();
}

CContentType::~CContentType()
{
	Free();
}

int CContentType::Parse( const char *hvalue )
{
	char* subtype;
	char* osip_content_type_params;

	/* How to parse:
	   we'll place the pointers:
	   subtype  			=>  beginning of subtype
	   osip_content_type_params  =>  beginning of params
	   examples:
	   application/multipart ; boundary=
	   ^		  ^
	 */
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;   				/* It's valid to add empty Accept header! */

	subtype = strchr(hvalue, '/');
	osip_content_type_params = strchr(hvalue, ';');

	if (subtype == NULL)
		return -1;  				/* do we really mind such an error */

	if (osip_content_type_params != NULL)
	{
		if (GenericParamParseAll(&(this->gen_params),osip_content_type_params) == -1)
		{
			return -1;
		}
	}
	else
	{
		osip_content_type_params = subtype + strlen(subtype);
	}

	if (subtype - hvalue + 1 < 2)
		return -1;
	this->type = (char *) mm_malloc(subtype - hvalue + 1);
	if (this->type == NULL)
		return -1;
	zstr_clrncpy(this->type, hvalue, subtype - hvalue);

	if (osip_content_type_params - subtype < 2)
		return -1;
	this->subtype = (char *) mm_malloc(osip_content_type_params -
										subtype);
	if (this->subtype == NULL)
		return -1;
	zstr_clrncpy(this->subtype, subtype + 1,
		osip_content_type_params - subtype - 1);

	return 0;
}
/* returns the content_type header as a string.  */
/* INPUT : osip_content_type_t *content_type | content_type header.   */
/* returns null on error. */
int CContentType::ToStr(char** dest)
{
	char* buf;
	char* tmp;
	size_t len;
	
	*dest = NULL;
	if ((this->type == NULL) ||
		(this->subtype == NULL))
		return -1;
	
	/* try to guess a long enough length */
	len = strlen(this->type) +
		strlen(this->subtype) +
		4		/* for '/', ' ', ';' and '\0' */ +
		10 * (this->gen_params.GetSize());
	
	buf = (char *) mm_malloc(len);
	tmp = buf;
	
	sprintf(tmp, "%s/%s", this->type, this->subtype);
	
	tmp = tmp + strlen(tmp);
	{
		int pos = 0;
		CUrlParam* u_param;
		
#if 0
		if (!this->gen_params.IsListEof(pos))
		{ 						/* needed for cannonical form! (authentication issue of rfc2543) */
			sprintf (tmp, " ");
			tmp++;
		}
#endif
		while (!this->gen_params.IsListEof(pos))
		{
			size_t tmp_len;
			
			u_param = (CUrlParam *) this->gen_params.GetAt(pos);
			if (u_param->gvalue == NULL)
			{
				mm_free(buf);
				return -1;
			}
			tmp_len = strlen(buf) +
				4 +
				strlen(u_param->gname) +
				strlen(u_param->gvalue) +
				1;
			if (len < tmp_len)
			{
				buf = (char*)mm_realloc(buf, tmp_len);
				len = tmp_len;
				tmp = buf + strlen(buf);
			}
			sprintf(tmp, "; %s=%s", u_param->gname, u_param->gvalue);
			tmp = tmp + strlen(tmp);
			pos++;
		}
	}
	*dest = buf;
	return 0;
}

int CContentType::Init()
{
	this->type = NULL;
	this->subtype = NULL;

	return 0;
}

void CContentType::Free()
{
	mm_free(this->type);
	mm_free(this->subtype);

	this->type = NULL;
	this->subtype = NULL;

}

int CContentType::Clone(CContentType** dest)
{
	int i;
	CContentType* ct;
	
	*dest = NULL;

	ct = new CContentType();	
	if (ct == NULL) 				  /* allocation failed */
		return -1;
	if (this->type != NULL)
		ct->type = zstr_strdup(this->type);
	if (this->subtype != NULL)
		ct->subtype = zstr_strdup(this->subtype);
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
