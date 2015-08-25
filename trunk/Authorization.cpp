// Authorization.cpp: implementation of the CAuthorization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Authorization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuthorization::CAuthorization()
{

}

CAuthorization::~CAuthorization()
{

}

int CAuthorization::Init()
{
	auth_type = NULL;
	username = NULL;
	realm = NULL;
	nonce = NULL;
	uri = NULL;
	response = NULL;
	digest = NULL; 	  /* DO NOT USE IT IN AUTHORIZATION_T HEADER?? */
	algorithm = NULL;    /* optionnal, default is "md5" */
	cnonce = NULL; 	  /* optionnal */
	opaque = NULL; 	  /* optionnal */
	message_qop = NULL;  /* optionnal */
	nonce_count = NULL;  /* optionnal */
	auth_param = NULL;   /* for other headers --NOT IMPLEMENTED-- */

	return 0;
}

void CAuthorization::Free()
{
	mm_free(auth_type);
	mm_free(username);
	mm_free(realm);
	mm_free(nonce);
	mm_free(uri);
	mm_free(response);
	mm_free(digest);
	mm_free(algorithm);
	mm_free(cnonce);
	mm_free(opaque);
	mm_free(message_qop);
	mm_free(nonce_count);

	auth_type = NULL;
	username = NULL;
	realm = NULL;
	nonce = NULL;
	uri = NULL;
	response = NULL;
	digest = NULL; 		
	algorithm = NULL;   
	cnonce = NULL; 		
	opaque = NULL; 		
	message_qop = NULL; 
	nonce_count = NULL; 
	auth_param = NULL;  

}

int CAuthorization::Parse( const char* hvalue )
{
	const char* space;
	const char* next = NULL;

	space = strchr(hvalue, ' '); /* SEARCH FOR SPACE */
	if (space == NULL)
		return -1;

	if (space - hvalue < 1)
		return -1;
	this->auth_type = (char *) mm_malloc(space - hvalue + 1);
	zstr_strncpy(this->auth_type, hvalue, space - hvalue);

	for (; ;)
	{
		int parse_ok = 0;

		if (zstr_quoted_string_set("username", space, &(this->username),
				&next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("realm", space, &(this->realm), &next))
			return -1;
		if (next == NULL)
			return 0;
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
		if (zstr_quoted_string_set("uri", space, &(this->uri), &next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("response", space, &(this->response),
				&next))
			return -1;
		if (next == NULL)
			return 0;   			/* end of header detected! */
		else if (next != space)
		{
			space = next;
			parse_ok++;
		}
		if (zstr_quoted_string_set("digest", space, &(this->digest), &next))
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
		if (zstr_quoted_string_set("cnonce", space, &(this->cnonce), &next))
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
		if (zstr_token_set("qop", space, &(this->message_qop), &next))
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
		/* nothing was recognized:
		   here, we should handle a list of unknown tokens where:
		   token1 = ( token2 | quoted_text ) */
		/* TODO */

		if (0 == parse_ok)
		{
			const char* quote1, * quote2, * tmp;

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

int CAuthorization::Clone( CAuthorization** dest )
{
	CAuthorization* au;

	*dest = NULL;
	/* to be removed?
	   if (this->auth_type==NULL) return -1;
	   if (this->username==NULL) return -1;
	   if (this->realm==NULL) return -1;
	   if (this->nonce==NULL) return -1;
	   if (this->uri==NULL) return -1;
	   if (this->response==NULL) return -1;
	   if (this->opaque==NULL) return -1;
	 */

	au = new CAuthorization();
	if (au == NULL)				  /* allocation failed */
		return -1;
	if (this->auth_type != NULL)
	{
		au->auth_type = zstr_strdup(this->auth_type);
		if (au->auth_type == NULL)
			goto ac_error;
	}
	if (this->username != NULL)
	{
		au->username = zstr_strdup(this->username);
		if (au->username == NULL)
			goto ac_error;
	}
	if (this->realm != NULL)
	{
		au->realm = zstr_strdup(this->realm);
		if (this->realm == NULL)
			goto ac_error;
	}
	if (this->nonce != NULL)
	{
		au->nonce = zstr_strdup(this->nonce);
		if (this->nonce == NULL)
			goto ac_error;
	}
	if (this->uri != NULL)
	{
		au->uri = zstr_strdup(this->uri);
		if (au->uri == NULL)
			goto ac_error;
	}
	if (this->response != NULL)
	{
		au->response = zstr_strdup(this->response);
		if (this->response == NULL)
			goto ac_error;
	}
	if (this->digest != NULL)
	{
		au->digest = zstr_strdup(this->digest);
		if (au->digest == NULL)
			goto ac_error;
	}
	if (this->algorithm != NULL)
	{
		au->algorithm = zstr_strdup(this->algorithm);
		if (this->algorithm == NULL)
			goto ac_error;
	}
	if (this->cnonce != NULL)
	{
		au->cnonce = zstr_strdup(this->cnonce);
		if (au->cnonce == NULL)
			goto ac_error;
	}
	if (this->opaque != NULL)
	{
		au->opaque = zstr_strdup(this->opaque);
		if (this->opaque == NULL)
			goto ac_error;
	}
	if (this->message_qop != NULL)
	{
		au->message_qop = zstr_strdup(this->message_qop);
		if (this->message_qop == NULL)
			goto ac_error;
	}
	if (this->nonce_count != NULL)
	{
		au->nonce_count = zstr_strdup(this->nonce_count);
		if (this->nonce_count == NULL)
			goto ac_error;
	}

	*dest = au;
	return 0;

	ac_error : delete (au);
	return -1;	
}

char* CAuthorization::GetAuthType()
{
	return auth_type;
}

void CAuthorization::SetAuthType( char* value )
{
	auth_type = value;
}

char* CAuthorization::GetUserName()
{
	return username;	
}

void CAuthorization::SetUserName( char* value )
{
	username = value;	
}

char* CAuthorization::GetRealm()
{
	return realm;
}

void CAuthorization::SetRealm( char* value )
{
	realm = value;	
}

char* CAuthorization::GetNonce()
{
	return nonce;	
}

void CAuthorization::SetNonce( char* value )
{
	nonce = value;	
}

char* CAuthorization::GetUri()
{
	return uri;
}

void CAuthorization::SetUri( char* value )
{
	uri = value;
}

char* CAuthorization::GetResponse()
{
	return response;	
}

void CAuthorization::SetResponse( char* value )
{
	response = value;
}

char* CAuthorization::GetDigest()
{
	return digest;	
}

void CAuthorization::SetDigest( char* value )
{
	digest = value;	
}

char* CAuthorization::GetAlgorithm()
{
	return algorithm;	
}

void CAuthorization::SetAlgorithm( char* value )
{
	algorithm = value;
}

char* CAuthorization::GetCnonce()
{
	return cnonce;	
}

void CAuthorization::SetCnonce( char* value )
{
	cnonce = value;
}

char* CAuthorization::GetOpaque()
{
	return opaque;
}

void CAuthorization::SetOpaque( char* value )
{
	opaque = value;
}

char* CAuthorization::GetMessageQop()
{
	return message_qop;
}

void CAuthorization::SetMessageQop( char* value )
{
	message_qop = value;
}

char* CAuthorization::GetNonceCount()
{
	return nonce_count;
}

void CAuthorization::SetNonceCount( char* value )
{
	nonce_count = value;
}

/* returns the authorization header as a string.		  */
/* INPUT : osip_authorization_t *authorization | authorization header.  */
/* returns null on error. */
int CAuthorization::ToStr(char** dest)
{
	size_t len;
	char* tmp;

	*dest = NULL;
	/* DO NOT REALLY KNOW THE LIST OF MANDATORY PARAMETER: Please HELP! */
	if ((this->auth_type == NULL) ||
		(this->realm == NULL) ||
		(this->nonce == NULL))
		return -1;

	len = strlen(this->auth_type) + 1;
	if (this->username != NULL)
		len = len + 10 + strlen(this->username);
	if (this->realm != NULL)
		len = len + 8 + strlen(this->realm);
	if (this->nonce != NULL)
		len = len + 8 + strlen(this->nonce);
	if (this->uri != NULL)
		len = len + 6 + strlen(this->uri);
	if (this->response != NULL)
		len = len + 11 + strlen(this->response);
	len = len + 2;
	if (this->digest != NULL)
		len = len + strlen(this->digest) + 9;
	if (this->algorithm != NULL)
		len = len + strlen(this->algorithm) + 12;
	if (this->cnonce != NULL)
		len = len + strlen(this->cnonce) + 9;
	if (this->opaque != NULL)
		len = len + 9 + strlen(this->opaque);
	if (this->nonce_count != NULL)
		len = len + strlen(this->nonce_count) + 5;
	if (this->message_qop != NULL)
		len = len + strlen(this->message_qop) + 6;

	tmp = (char *) mm_malloc(len);
	if (tmp == NULL)
		return -1;
	*dest = tmp;

	tmp = osip_str_append(tmp, this->auth_type);

	if (this->username != NULL)
	{
		tmp = osip_strn_append(tmp, " username=", 10);
		/* !! username-value must be a quoted string !! */
		tmp = osip_str_append(tmp, this->username);
	}

	if (this->realm != NULL)
	{
		tmp = osip_strn_append(tmp, ", realm=", 8);
		/* !! realm-value must be a quoted string !! */
		tmp = osip_str_append(tmp, this->realm);
	}
	if (this->nonce != NULL)
	{
		tmp = osip_strn_append(tmp, ", nonce=", 8);
		/* !! nonce-value must be a quoted string !! */
		tmp = osip_str_append(tmp, this->nonce);
	}

	if (this->uri != NULL)
	{
		tmp = osip_strn_append(tmp, ", uri=", 6);
		/* !! domain-value must be a list of URI in a quoted string !! */
		tmp = osip_str_append(tmp, this->uri);
	}
	if (this->response != NULL)
	{
		tmp = osip_strn_append(tmp, ", response=", 11);
		/* !! domain-value must be a list of URI in a quoted string !! */
		tmp = osip_str_append(tmp, this->response);
	}

	if (this->digest != NULL)
	{
		tmp = osip_strn_append(tmp, ", digest=", 9);
		/* !! domain-value must be a list of URI in a quoted string !! */
		tmp = osip_str_append(tmp, this->digest);
	}
	if (this->algorithm != NULL)
	{
		tmp = osip_strn_append(tmp, ", algorithm=", 12);
		tmp = osip_str_append(tmp, this->algorithm);
	}
	if (this->cnonce != NULL)
	{
		tmp = osip_strn_append(tmp, ", cnonce=", 9);
		tmp = osip_str_append(tmp, this->cnonce);
	}
	if (this->opaque != NULL)
	{
		tmp = osip_strn_append(tmp, ", opaque=", 9);
		tmp = osip_str_append(tmp, this->opaque);
	}
	if (this->message_qop != NULL)
	{
		tmp = osip_strn_append(tmp, ", qop=", 6);
		tmp = osip_str_append(tmp, this->message_qop);
	}
	if (this->nonce_count != NULL)
	{
		tmp = osip_strn_append(tmp, ", nc=", 5);
		tmp = osip_str_append(tmp, this->nonce_count);
	}
	return 0;
}

int osip_authorization_clone(const CAuthorization* auth,
	CAuthorization** dest)
{
	CAuthorization* au;

	*dest = NULL;
	if (auth == NULL)
		return -1;
	/* to be removed?
	   if (auth->auth_type==NULL) return -1;
	   if (auth->username==NULL) return -1;
	   if (auth->realm==NULL) return -1;
	   if (auth->nonce==NULL) return -1;
	   if (auth->uri==NULL) return -1;
	   if (auth->response==NULL) return -1;
	   if (auth->opaque==NULL) return -1;
	 */

	au = new CAuthorization();
	if (au == NULL)				  /* allocation failed */
		return -1;
	if (auth->auth_type != NULL)
	{
		au->auth_type = zstr_strdup(auth->auth_type);
		if (au->auth_type == NULL)
			goto ac_error;
	}
	if (auth->username != NULL)
	{
		au->username = zstr_strdup(auth->username);
		if (au->username == NULL)
			goto ac_error;
	}
	if (auth->realm != NULL)
	{
		au->realm = zstr_strdup(auth->realm);
		if (auth->realm == NULL)
			goto ac_error;
	}
	if (auth->nonce != NULL)
	{
		au->nonce = zstr_strdup(auth->nonce);
		if (auth->nonce == NULL)
			goto ac_error;
	}
	if (auth->uri != NULL)
	{
		au->uri = zstr_strdup(auth->uri);
		if (au->uri == NULL)
			goto ac_error;
	}
	if (auth->response != NULL)
	{
		au->response = zstr_strdup(auth->response);
		if (auth->response == NULL)
			goto ac_error;
	}
	if (auth->digest != NULL)
	{
		au->digest = zstr_strdup(auth->digest);
		if (au->digest == NULL)
			goto ac_error;
	}
	if (auth->algorithm != NULL)
	{
		au->algorithm = zstr_strdup(auth->algorithm);
		if (auth->algorithm == NULL)
			goto ac_error;
	}
	if (auth->cnonce != NULL)
	{
		au->cnonce = zstr_strdup(auth->cnonce);
		if (au->cnonce == NULL)
			goto ac_error;
	}
	if (auth->opaque != NULL)
	{
		au->opaque = zstr_strdup(auth->opaque);
		if (auth->opaque == NULL)
			goto ac_error;
	}
	if (auth->message_qop != NULL)
	{
		au->message_qop = zstr_strdup(auth->message_qop);
		if (auth->message_qop == NULL)
			goto ac_error;
	}
	if (auth->nonce_count != NULL)
	{
		au->nonce_count = zstr_strdup(auth->nonce_count);
		if (auth->nonce_count == NULL)
			goto ac_error;
	}

	*dest = au;
	return 0;

	ac_error : delete(au);
	return -1;
}