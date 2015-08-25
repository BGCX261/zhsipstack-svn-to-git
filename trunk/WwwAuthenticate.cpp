// WwwAuthenticate.cpp: implementation of the CWwwAuthenticate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "WwwAuthenticate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWwwAuthenticate::CWwwAuthenticate()
{

}

CWwwAuthenticate::~CWwwAuthenticate()
{

}

int CWwwAuthenticate::Init()
{
	auth_type = NULL;
	realm = NULL;
	domain = NULL;
	nonce = NULL;
	opaque = NULL;
	stale = NULL;
	algorithm = NULL;
	qop_options = NULL;
	auth_param = NULL;


	return 0;
}

int CWwwAuthenticate::Parse( const char* hvalue )
{
	const char* space;
	const char* next = NULL;

	space = strchr(hvalue, ' '); /* SEARCH FOR SPACE */
	if (space == NULL)
		return -1;

	if (space - hvalue + 1 < 2)
		return -1;
	this->auth_type = (char *) mm_malloc(space - hvalue + 1);
	if (this->auth_type == NULL)
		return -1;
	zstr_strncpy(this->auth_type, hvalue, space - hvalue);

	for (; ;)
	{
		int parse_ok = 0;

		if (zstr_quoted_string_set("realm", space, &(this->realm), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("domain", space, &(this->domain), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("nonce", space, &(this->nonce), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("opaque", space, &(this->opaque), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_token_set("stale", space, &(this->stale), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_token_set("algorithm", space, &(this->algorithm), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("qop", space, &(this->qop_options), &next))
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
/* returns the www_authenticate header as a string. 		 */
/* INPUT : osip_www_authenticate_t *www_authenticate | www_authenticate header.  */
/* returns null on error. */
int CWwwAuthenticate::ToStr(char** dest)
{
	size_t len;
	char* tmp;
	
	*dest = NULL;
	if ((this == NULL) || (this->auth_type == NULL))
		return -1;
	
	len = strlen(this->auth_type) + 1;
	
	if (this->realm != NULL)
		len = len + strlen(this->realm) + 7;
	if (this->nonce != NULL)
		len = len + strlen(this->nonce) + 8;
	len = len + 2;
	if (this->domain != NULL)
		len = len + strlen(this->domain) + 9;
	if (this->opaque != NULL)
		len = len + strlen(this->opaque) + 9;
	if (this->stale != NULL)
		len = len + strlen(this->stale) + 8;
	if (this->algorithm != NULL)
		len = len + strlen(this->algorithm) + 12;
	if (this->qop_options != NULL)
		len = len + strlen(this->qop_options) + 6;
	
	tmp = (char *) mm_malloc(len);
	if (tmp == NULL)
		return -1;
	*dest = tmp;
	
	tmp = osip_str_append(tmp, this->auth_type);
	
	if (this->realm != NULL)
	{
		tmp = osip_strn_append(tmp, " realm=", 7);
		tmp = osip_str_append(tmp, this->realm);
	}
	if (this->domain != NULL)
	{
		tmp = osip_strn_append(tmp, ", domain=", 9);
		tmp = osip_str_append(tmp, this->domain);
	}
	if (this->nonce != NULL)
	{
		tmp = osip_strn_append(tmp, ", nonce=", 8);
		tmp = osip_str_append(tmp, this->nonce);
	}
	if (this->opaque != NULL)
	{
		tmp = osip_strn_append(tmp, ", opaque=", 9);
		tmp = osip_str_append(tmp, this->opaque);
	}
	if (this->stale != NULL)
	{
		tmp = osip_strn_append(tmp, ", stale=", 8);
		tmp = osip_str_append(tmp, this->stale);
	}
	if (this->algorithm != NULL)
	{
		tmp = osip_strn_append(tmp, ", algorithm=", 12);
		tmp = osip_str_append(tmp, this->algorithm);
	}
	if (this->qop_options != NULL)
	{
		tmp = osip_strn_append(tmp, ", qop=", 6);
		tmp = osip_str_append(tmp, this->qop_options);
	}
	
	if (this->realm == NULL)
	{
		/* remove comma */
		len = strlen(this->auth_type);
		if ((*dest)[len] == ',')
			(*dest)[len] = ' ';
	}
	
	return 0;
}

/* deallocates a osip_www_authenticate_t structure.  */
/* INPUT : osip_www_authenticate_t *www_authenticate | www_authenticate. */
void CWwwAuthenticate::Free()
{
	mm_free(auth_type);
	mm_free(realm);
	mm_free(domain);
	mm_free(nonce);
	mm_free(opaque);
	mm_free(stale);
	mm_free(algorithm);
	mm_free(qop_options);

}

char* CWwwAuthenticate::GetAuthType()
{
	return auth_type;
}

void CWwwAuthenticate::SetAuthType( char* value )
{
	auth_type = value;
}

char* CWwwAuthenticate::GetRealm()
{
	return realm;	
}

void CWwwAuthenticate::SetRealm( char* value )
{
	realm = value;
}

char* CWwwAuthenticate::GetDomain()
{
	return domain;
}

void CWwwAuthenticate::SetDomain( char* value )
{
	domain = value;	
}

char* CWwwAuthenticate::GetNonce()
{
	return nonce;
}

void CWwwAuthenticate::SetNonce( char* value )
{
	nonce = value;	
}

char* CWwwAuthenticate::GetOpaque()
{
	return opaque;
}

void CWwwAuthenticate::SetOpaque( char* value )
{
	opaque = value;
}

char* CWwwAuthenticate::GetStale()
{
	return stale;
}

void CWwwAuthenticate::SetStale( char* value )
{
	stale = value;
}

char* CWwwAuthenticate::GetAlgorithm()
{
	return algorithm;
}

void CWwwAuthenticate::SetAlgorithm( char* value )
{
	algorithm = value;
}

char* CWwwAuthenticate::GetQopOptions()
{
	return qop_options;
}

void CWwwAuthenticate::SetQopOptions( char* value )
{
	qop_options = value;
}

int osip_www_authenticate_clone(const CWwwAuthenticate* wwwa,
								CWwwAuthenticate** dest)
{
	CWwwAuthenticate* wa;
	
	*dest = NULL;
	if (wwwa == NULL)
		return -1;
	if (wwwa->auth_type == NULL)
		return -1;
	
	wa = new CWwwAuthenticate();
	if (wa == NULL)				  /* allocation failed */
		return -1;
	wa->auth_type = zstr_strdup(wwwa->auth_type);
	if (wwwa->realm != NULL)
		wa->realm = zstr_strdup(wwwa->realm);
	if (wwwa->domain != NULL)
		wa->domain = zstr_strdup(wwwa->domain);
	if (wwwa->nonce != NULL)
		wa->nonce = zstr_strdup(wwwa->nonce);
	if (wwwa->opaque != NULL)
		wa->opaque = zstr_strdup(wwwa->opaque);
	if (wwwa->stale != NULL)
		wa->stale = zstr_strdup(wwwa->stale);
	if (wwwa->algorithm != NULL)
		wa->algorithm = zstr_strdup(wwwa->algorithm);
	if (wwwa->qop_options != NULL)
		wa->qop_options = zstr_strdup(wwwa->qop_options);
	
	*dest = wa;
	return 0;
}
