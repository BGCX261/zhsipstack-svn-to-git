// AuthorizationInfo.cpp: implementation of the CAuthorizationInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "AuthorizationInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuthorizationInfo::CAuthorizationInfo()
{
	Init();
}

CAuthorizationInfo::~CAuthorizationInfo()
{
	Free();
}

int CAuthorizationInfo::Init()
{
	nextnonce = NULL;
	qop_options = NULL;
	rspauth = NULL;
	cnonce = NULL;
	nonce_count = NULL;
	return 0;
}

void CAuthorizationInfo::Free()
{
	mm_free(nextnonce);
	mm_free(qop_options);
	mm_free(rspauth);
	mm_free(cnonce);
	mm_free(nonce_count);

	nextnonce = NULL;
	qop_options = NULL;
	rspauth = NULL;
	cnonce = NULL;
	nonce_count = NULL;
}

int CAuthorizationInfo::Parse( const char* hvalue )
{
	const char* space;
	const char* next = NULL;
	
	space = hvalue;
	
	for (; ;)
	{
		int parse_ok = 0;
		
		if (zstr_quoted_string_set("nextnonce", space, &(this->nextnonce),
			&next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("cnonce", space, &(this->cnonce), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("rspauth", space, &(this->rspauth),
			&next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_token_set("nc", space, &(this->nonce_count), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_token_set("qop", space, &(this->qop_options), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (0 == parse_ok)
		{
			char* quote1, * quote2, * tmp;
			
			/* CAUTION */
			/* parameter not understood!!! I'm too lazy to handle IT */
			/* let's simply bypass it */
			if (strlen(space) < 1)
				return 0;
			tmp = strchr(space + 1, ',');
			if (tmp == NULL)	  /* it was the last header */
				return 0;
			quote1 = zstr_quote_find(space);
			if ((quote1 != NULL) && (quote1 < tmp)) 	  /* this may be a quoted string! */
			{
				quote2 = zstr_quote_find(quote1 + 1);
				if (quote2 == NULL)
					return -1;  	/* bad header format... */
				if (tmp < quote2) /* the comma is inside the quotes! */
					space = strchr(quote2, ',');
				else
					space = tmp;
				if (space == NULL)  	  /* it was the last header */
					return 0;
			}
			else
				space = tmp;
			/* continue parsing... */
		}
	}
	return 0;   				  /* ok */
}

/* returns the authentication_info header as a string.  		*/
/* INPUT : osip_authentication_info_t *authentication_info | authentication_info header.  */
/* returns null on error. */
int CAuthorizationInfo::ToStr(char** dest)
{
	size_t len;
	char* tmp;
	
	*dest = NULL;

	len = 0;
	if (this->nextnonce != NULL)
		len = len + strlen(this->nextnonce) + 11;
	if (this->rspauth != NULL)
		len = len + strlen(this->rspauth) + 10;
	if (this->cnonce != NULL)
		len = len + strlen(this->cnonce) + 9;
	if (this->nonce_count != NULL)
		len = len + strlen(this->nonce_count) + 5;
	if (this->qop_options != NULL)
		len = len + strlen(this->qop_options) + 6;
	
	if (len == 0)
		return -1;
	
	tmp = (char *) mm_malloc(len);
	if (tmp == NULL)
		return -1;
	*dest = tmp;
	
	if (this->qop_options != NULL)
	{
		tmp = osip_strn_append(tmp, "qop=", 4);
		tmp = osip_str_append(tmp, this->qop_options);
	}
	if (this->nextnonce != NULL)
	{
		if (tmp != *dest)
		{
			tmp = osip_strn_append(tmp, ", ", 2);
		}
		tmp = osip_strn_append(tmp, "nextnonce=", 10);
		tmp = osip_str_append(tmp, this->nextnonce);
	}
	if (this->rspauth != NULL)
	{
		if (tmp != *dest)
		{
			tmp = osip_strn_append(tmp, ", ", 2);
		}
		tmp = osip_strn_append(tmp, "rspauth=", 8);
		tmp = osip_str_append(tmp, this->rspauth);
	}
	if (this->cnonce != NULL)
	{
		if (tmp != *dest)
		{
			tmp = osip_strn_append(tmp, ", ", 2);
		}
		tmp = osip_strn_append(tmp, "cnonce=", 7);
		tmp = osip_str_append(tmp, this->cnonce);
	}
	if (this->nonce_count != NULL)
	{
		if (tmp != *dest)
		{
			tmp = osip_strn_append(tmp, ", ", 2);
		}
		tmp = osip_strn_append(tmp, "nc=", 3);
		tmp = osip_str_append(tmp, this->nonce_count);
	}
	
	return 0;
}

int CAuthorizationInfo::Clone( CAuthorizationInfo** dest )
{
	CAuthorizationInfo* wa;
	
	*dest = NULL;

	wa = new CAuthorizationInfo();
	if (wa == NULL)				  /* allocation failed */
		return -1;
	if (this->nextnonce != NULL)
		wa->nextnonce = zstr_strdup(this->nextnonce);
	if (this->cnonce != NULL)
		wa->cnonce = zstr_strdup(this->cnonce);
	if (this->rspauth != NULL)
		wa->rspauth = zstr_strdup(this->rspauth);
	if (this->nonce_count != NULL)
		wa->nonce_count = zstr_strdup(this->nonce_count);
	if (this->qop_options != NULL)
		wa->qop_options = zstr_strdup(this->qop_options);
	
	*dest = wa;
	return 0;	
}

char* CAuthorizationInfo::GetNextNonce()
{
	return nextnonce;
}

void CAuthorizationInfo::SetNextNonce( char* value )
{
	nextnonce = value;
}

char* CAuthorizationInfo::GetCnonce()
{
	return cnonce;
}

void CAuthorizationInfo::SetCnonce( char* value )
{
	cnonce = value;
}

char* CAuthorizationInfo::GetQopOptions()
{
	return qop_options;	
}

void CAuthorizationInfo::SetQopOptions( char* value )
{
	qop_options = value;
}

char* CAuthorizationInfo::GetRspauth()
{
	return rspauth;
}

void CAuthorizationInfo::SetRspauth( char* value )
{
	rspauth = value;
}

char* CAuthorizationInfo::GetNonceCount()
{
	return nonce_count;	
}

void CAuthorizationInfo::SetNonceCount( char* value )
{
	nonce_count = value;
}
