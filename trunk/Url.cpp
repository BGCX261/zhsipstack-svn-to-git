// Url.cpp: implementation of the CUrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Url.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _ALPHANUM_ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\0"
#define _RESERVED_ ";/?:@&=+$\0"
#define _MARK_ "-_.!~*'()\0"

#define _MARK__USER_UNRESERVED_ "-_.!~*'()&=+$,;?/\0"
#define _MARK__PWORD_UNRESERVED_ "-_.!~*'()&=+$,\0"
#define _MARK__URI_PARAM_UNRESERVED_ "-_.!~*'()[]/:&+$\0"
#define _MARK__HEADER_PARAM_UNRESERVED_ "-_.!~*'()[]/?:+$\0"

#define osip_is_alphanum(in) (  \
	   (in >= 'a' && in <= 'z') || \
	   (in >= 'A' && in <= 'Z') || \
(in >= '0' && in <= '9'))

char* __osip_uri_escape_nonascii_and_nondef(const char* string,
											const char* def)
{
	size_t alloc = strlen(string) + 1;
	size_t length;
	char* ns = (char*)mm_malloc(alloc);
	unsigned char in;
	size_t newlen = alloc;
	int index = 0;
	const char* tmp;
	int i;
	
	length = alloc - 1;
	while (length--)
	{
		in = *string;
		
		i = 0;
		tmp = NULL;
		if (osip_is_alphanum(in))
			tmp = string;
		else
		{
			for (; def[i] != '\0' && def[i] != in; i++)
			{
			}
			if (def[i] != '\0')
				tmp = string;
		}
		if (tmp == NULL)
		{
			/* encode it */
			newlen += 2;		  /* the size grows with two, since this'll become a %XX */
			if (newlen > alloc)
			{
				alloc *= 2;
				ns = (char*)mm_realloc(ns, alloc);
				if (!ns)
					return NULL;
			}
			sprintf(&ns[index], "%%%02X", in);
			index += 3;
		}
		else
		{
			/* just copy this */
			ns[index++] = in;
		}
		string++;
	}
	ns[index] = 0;  			  /* terminate it */
	return ns;
}

/* user =  *( unreserved / escaped / user-unreserved ) */
const char* userinfo_def = /* implied _ALPHANUM_ */ _MARK__USER_UNRESERVED_;
char* __osip_uri_escape_userinfo(const char* string)
{
	return __osip_uri_escape_nonascii_and_nondef(string, userinfo_def);
}

/* user =  *( unreserved / escaped / user-unreserved ) */
const char* password_def = _MARK__PWORD_UNRESERVED_;
char* __osip_uri_escape_password(const char* string)
{
	return __osip_uri_escape_nonascii_and_nondef(string, password_def);
}

const char* uri_param_def = _MARK__URI_PARAM_UNRESERVED_;
char* __osip_uri_escape_uri_param(char* string)
{
	return __osip_uri_escape_nonascii_and_nondef(string, uri_param_def);
}

const char* header_param_def = _MARK__HEADER_PARAM_UNRESERVED_;
char* __osip_uri_escape_header_param(char* string)
{
	return __osip_uri_escape_nonascii_and_nondef(string, header_param_def);
}

CUrl::CUrl()
{
	Init();
}

CUrl::~CUrl()
{
	Free();
}

int CUrl::Init()
{
	scheme = NULL;
	username = NULL;
	password = NULL;
	host = NULL;
	port = NULL;

	m_pcOther = NULL;
	m_pcOriginal = NULL;
		
	return 0;
}

void CUrl::Free()
{
	free(scheme);
	free(username);
	free(password);
	free(host);
	free(port);
	free(m_pcOther);
	free(m_pcOriginal);
	scheme = NULL;
	username = NULL;
	password = NULL;
	host = NULL;
	port = NULL;
	m_pcOther = NULL;
	m_pcOriginal = NULL;
}

void CUrl::UriUnescape(char* pcParam)
{
	size_t alloc = strlen(pcParam) + 1;
	unsigned char in;
	int index = 0;
	unsigned int hex;
	char* ptr;
	
	ptr = pcParam;
	while (--alloc > 0)
	{
		in = *ptr;
		if ('%' == in)
		{
			/* encoded part */
			if (alloc > 2 && sscanf(ptr + 1, "%02X", &hex) == 1)
			{
				in = (unsigned char) hex;
				if (*(ptr + 2) &&
					((*(ptr + 2) >= '0' && *(ptr + 2) <= '9') ||
					(*(ptr + 2) >= 'a' && *(ptr + 2) <= 'f') ||
					(*(ptr + 2) >= 'A' && *(ptr + 2) <= 'F')))
				{
					alloc -= 2;
					ptr += 2;
				}
				else
				{
					alloc -= 1;
					ptr += 1;
				}
			}
			else
			{
				break;
			}
		}
		
		pcParam[index++] = in;
		ptr++;
	}
	pcParam[index] = 0;  		  /* terminate it */
}

int CUrl::UriParseHeaders(const char* headers)
{
	char* and;
	char* equal;
	
	/* find '=' wich is the separator for one header */
	/* find ';' wich is the separator for multiple headers */
	
	equal = strchr(headers, '=');
	and = strchr(headers + 1, '&');
	
	if (equal == NULL)  		  /* each header MUST have a value */
		return -1;
	
	do
	{
		char* hname;
		char* hvalue;
		
		hname = (char *) MallocAndZero(equal - headers);
		if (hname == NULL)
			return -1;
		zstr_strncpy(hname, headers + 1, equal - headers - 1);
		UriUnescape(hname);
		
		if (and != NULL)
		{
			if (and - equal < 2)
			{
				mm_free(hname);
				return -1;
			}
			hvalue = (char *) mm_malloc(and - equal);
			if (hvalue == NULL)
			{
				mm_free(hname);
				return -1;
			}
			zstr_strncpy(hvalue, equal + 1, and - equal - 1);
			UriUnescape(hvalue);
		}
		else
		{
			/* this is for the last header (no and...) */
			if (headers + strlen(headers) - equal + 1 < 2)
			{
				mm_free(hname);
				return -1;
			}
			hvalue = (char *) mm_malloc(headers +
				strlen(headers) -
				equal +
				1);
			if (hvalue == NULL)
			{
				mm_free(hname);
				return -1;
			}
			zstr_strncpy(hvalue, equal + 1, headers + strlen(headers) - equal);
			UriUnescape(hvalue);
		}
		
		osip_uri_uheader_add(this, hname, hvalue);
		
		if (and == NULL)		  /* we just set the last header */
			equal = NULL;
		else					  /* continue on next header */
		{
			headers = and;
			equal = strchr(headers, '=');
			and = strchr(headers + 1, '&');
			if (equal == NULL)    /* each header MUST have a value */
				return -1;
		}
	}
	while (equal != NULL);
	return 0;
}

int CUrl::Parse( const char *buf )
{
	char* username;
	char* password;
	char* host;
	const char* port;
	const char* params;
	const char* headers;
	const char* tmp;

	/* basic tests */
	if (buf == NULL || buf[0] == '\0')
		return -1;

	tmp = strchr(buf, ':');
	if (tmp == NULL)
		return -1;

	if (tmp - buf < 2)
		return -1;
	this->scheme = (char *) MallocAndZero(tmp - buf + 1);
	if (this->scheme == NULL)
		return -1;
	zstr_strncpy(this->scheme, buf, tmp - buf);

	if (strlen(this->scheme) <
		3 ||
		(0 != zstr_strncasecmp(this->scheme, "sip", 3) &&
		0 != zstr_strncasecmp(this->scheme,
																"sips", 4)))
	{
		/* Is not a sipurl ! */
		size_t i = strlen(tmp + 1);

		if (i < 2)
			return -1;
		this->m_pcOther = (char *) mm_malloc(i + 1);
		if (this->m_pcOther == NULL)
			return -1;
		zstr_strncpy(this->m_pcOther, tmp + 1, i);
		return 0;
	}

	/*  law number 1:
	   if ('?' exists && is_located_after '@')
	   or   if ('?' exists && '@' is not there -no username-)
	   =====>  HEADER_PARAM EXIST
	   =====>  start at index(?)
	   =====>  end at the end of url
	 */

	/* find the beginning of host */
	username = strchr(buf, ':');
	/* if ':' does not exist, the url is not valid */
	if (username == NULL)
		return -1;

	host = strchr(buf, '@');

	if (host == NULL)
		host = username;
	else if (username[1] == '@')  /* username is empty */
		host = username + 1;
	else
		/* username exists */
	{
		password = next_separator(username + 1, ':', '@');
		if (password == NULL)
			password = host;
		else
				/* password exists */
		{
			if (host - password < 2)
				return -1;
			this->password = (char *) mm_malloc(host - password);
			if (this->password == NULL)
				return -1;
			zstr_strncpy(this->password, password + 1, host - password - 1);
			UriUnescape(this->password);
		}
		if (password - username < 2)
			return -1;
		{
			this->username = (char *) mm_malloc(password - username);
			if (this->username == NULL)
				return -1;
			zstr_strncpy(this->username, username + 1, password - username - 1);
			UriUnescape(this->username);
		}
	}


	/* search for header after host */
	headers = strchr(host, '?');

	if (headers == NULL)
		headers = buf + strlen(buf);
	else
		/* headers exist */
		UriParseHeaders(headers);


	/* search for params after host */
	params = strchr(host, ';');  /* search for params after host */
	if (params == NULL)
		params = headers;
	else
		/* params exist */
	{
		char* tmpbuf;

		if (headers - params + 1 < 2)
			return -1;
		tmpbuf = MallocAndZero(headers - params + 1);
		if (tmpbuf == NULL)
			return -1;
		tmpbuf = zstr_strncpy(tmpbuf, params, headers - params);
		UriParseParams(tmpbuf);
		mm_free(tmpbuf);
	}

	port = params - 1;
	while (port > host && *port != ']' && *port != ':')
		port--;
	if (*port == ':')
	{
		if (host == port)
			port = params;
		else
		{
			if ((params - port < 2) || (params - port > 8))
				return -1;  		/* error cases */
			this->port = (char *) MallocAndZero(params - port);
			if (this->port == NULL)
				return -1;
			zstr_clrncpy(this->port, port + 1, params - port - 1);
		}
	}
	else
		port = params;
	/* adjust port for ipv6 address */
	tmp = port;
	while (tmp > host && *tmp != ']')
		tmp--;
	if (*tmp == ']')
	{
		port = tmp;
		while (host < port && *host != '[')
			host++;
		if (host >= port)
			return -1;
	}

	if (port - host < 2)
		return -1;
	this->host = (char *) MallocAndZero(port - host);
	if (this->host == NULL)
		return -1;
	zstr_clrncpy(this->host, host + 1, port - host - 1);

	return 0;	
}

int CUrl::ToStr(char** dest)
{
	char* buf;
	size_t len;
	size_t plen;
	char* tmp;
	const char* scheme;

	*dest = NULL;
	if (this == NULL)
		return -1;
	if (this->host == NULL && this->m_pcOther == NULL)
		return -1;
	if (this->scheme == NULL && this->m_pcOther != NULL)
		return -1;
	if (this->m_pcOther == NULL && this->scheme == NULL)
		scheme = "sip"; 			/* default is sipurl */
	else
		scheme = this->scheme;

	if (this->m_pcOther != NULL)
	{
		buf = (char *) mm_malloc(strlen(scheme) + strlen(this->m_pcOther) + 3);
		if (buf == NULL)
			return -1;
		*dest = buf;
		sprintf(buf, "%s:", scheme);
		buf = buf + strlen(scheme) + 1;
		sprintf(buf, "%s", this->m_pcOther);
		buf = buf + strlen(this->m_pcOther);
		return 0;
	}

	len = strlen(scheme) + 1 + strlen(this->host) + 5;
	if (this->username != NULL)
		len = len + (strlen(this->username) * 3) + 1;	   /* count escaped char */
	if (this->password != NULL)
		len = len + (strlen(this->password) * 3) + 1;
	if (this->port != NULL)
		len = len + strlen(this->port) + 3;

	buf = (char *) mm_malloc(len);
	if (buf == NULL)
		return -1;
	tmp = buf;

	sprintf(tmp, "%s:", scheme);
	tmp = tmp + strlen(tmp);

	if (this->username != NULL)
	{
		char* tmp2 = __osip_uri_escape_userinfo(this->username);

		sprintf(tmp, "%s", tmp2);
		mm_free(tmp2);
		tmp = tmp + strlen(tmp);
	}
	if ((this->password != NULL) && (this->username != NULL))
	{
		/* be sure that when a password is given, a username is also given */
		char* tmp2 = __osip_uri_escape_password(this->password);

		sprintf(tmp, ":%s", tmp2);
		mm_free(tmp2);
		tmp = tmp + strlen(tmp);
	}
	if (this->username != NULL)
	{
		/* we add a '@' only when username is present... */
		sprintf(tmp, "@");
		tmp++;
	}
	if (strchr(this->host, ':') != NULL)
	{
		sprintf(tmp, "[%s]", this->host);
		tmp = tmp + strlen(tmp);
	}
	else
	{
		sprintf(tmp, "%s", this->host);
		tmp = tmp + strlen(tmp);
	}
	if (this->port != NULL)
	{
		sprintf(tmp, ":%s", this->port);
		tmp = tmp + strlen(tmp);
	}
	{
		int pos = 0;
		CUrlParam* u_param;

		while (!this->url_params.IsListEof(pos))
		{
			char* tmp1;
			char* tmp2 = NULL;

			u_param = (CUrlParam *) this->url_params.GetAt(pos);

			tmp1 = __osip_uri_escape_uri_param(u_param->gname);
			if (u_param->gvalue == NULL)
				plen = strlen(tmp1) + 2;
			else
			{
				tmp2 = __osip_uri_escape_uri_param(u_param->gvalue);
				plen = strlen(tmp1) + strlen(tmp2) + 3;
			}
			len = len + plen;
			buf = (char *) mm_realloc(buf, len);
			tmp = buf;
			tmp = tmp + strlen(tmp);
			if (u_param->gvalue == NULL)
				sprintf(tmp, ";%s", tmp1);
			else
			{
				sprintf(tmp, ";%s=%s", tmp1, tmp2);
				mm_free(tmp2);
			}
			mm_free(tmp1);
			pos++;
		}
	}
	{
		int pos = 0;
		CUrlParam* u_header;

		while (!this->url_headers.IsListEof(pos))
		{
			char* tmp1;
			char* tmp2;

			u_header = (CUrlParam *) this->url_headers.GetAt(pos);
			tmp1 = __osip_uri_escape_header_param(u_header->gname);

			if (tmp1 == NULL)
			{
				mm_free(buf);
				return -1;
			}

			tmp2 = __osip_uri_escape_header_param(u_header->gvalue);

			if (tmp2 == NULL)
			{
				mm_free(tmp1);
				mm_free(buf);
				return -1;
			}
			plen = strlen(tmp1) + strlen(tmp2) + 4;

			len = len + plen;
			buf = (char *) mm_realloc(buf, len);
			tmp = buf;
			tmp = tmp + strlen(tmp);
			if (pos == 0)
				sprintf(tmp, "?%s=%s", tmp1, tmp2);
			else
				sprintf(tmp, "&%s=%s", tmp1, tmp2);
			mm_free(tmp1);
			mm_free(tmp2);
			pos++;
		}
	}

	*dest = buf;
	return 0;
}

int CUrl::UriParseParams(const char* params)
{
	char* pname;
	char* pvalue;
	
	const char* comma;
	const char* equal;
	
	/* find '=' wich is the separator for one param */
	/* find ';' wich is the separator for multiple params */
	
	equal = next_separator(params + 1, '=', ';');
	comma = strchr(params + 1, ';');
	
	while (comma != NULL)
	{
		if (equal == NULL)
		{
			equal = comma;
			pvalue = NULL;
		}
		else
		{
			if (comma - equal < 2)
				return -1;
			pvalue = (char *) MallocAndZero(comma - equal);
			if (pvalue == NULL)
				return -1;
			zstr_strncpy(pvalue, equal + 1, comma - equal - 1);
			UriUnescape(pvalue);
		}
		
		if (equal - params < 2)
		{
			mm_free(pvalue);
			return -1;
		}
		pname = (char *) mm_malloc(equal - params);
		if (pname == NULL)
		{
			mm_free(pvalue);
			return -1;
		}
		zstr_strncpy(pname, params + 1, equal - params - 1);
		UriUnescape(pname);
		
		osip_uri_uparam_add(this, pname, pvalue);
		
		params = comma;
		equal = next_separator(params + 1, '=', ';');
		comma = strchr(params + 1, ';');
	}
	
	/* this is the last header (comma==NULL) */
	comma = params + strlen(params);
	
	if (equal == NULL)
	{
		equal = comma;  		  /* at the end */
		pvalue = NULL;
	}
	else
	{
		if (comma - equal < 2)
			return -1;
		pvalue = (char *) MallocAndZero(comma - equal);
		if (pvalue == NULL)
			return -1;
		zstr_strncpy(pvalue, equal + 1, comma - equal - 1);
	}
	
	if (equal - params < 2)
	{
		mm_free(pvalue);
		return -1;
	}
	pname = (char *) MallocAndZero(equal - params);
	if (pname == NULL)
	{
		mm_free(pvalue);
		return -1;
	}
	zstr_strncpy(pname, params + 1, equal - params - 1);
	
	osip_uri_uparam_add(this, pname, pvalue);
	
	return 0;
}


int CUrl::Clone( CUrl** dest )
{
	int i;
	CUrl* ur;
	
	*dest = NULL;

	if (this->host == NULL && this->m_pcOther == NULL)
		return -1;
	
	ur = new CUrl;
	if (ur == NULL)				  /* allocation failed */
		return -1;
	if (this->scheme != NULL)
		ur->scheme = zstr_strdup(this->scheme);
	if (this->username != NULL)
		ur->username = zstr_strdup(this->username);
	if (this->password != NULL)
		ur->password = zstr_strdup(this->password);
	if (this->host != NULL)
		ur->host = zstr_strdup(this->host);
	if (this->port != NULL)
		ur->port = zstr_strdup(this->port);
	if (this->m_pcOther != NULL)
		ur->m_pcOther = zstr_strdup(this->m_pcOther);
	
	i = this->url_params.Clone(&ur->url_params,(int * (*) (void *, void *)) & osip_uri_param_clone);
	if (i != 0)
	{
		delete ur;
		ur = NULL;
		return -1;
	}
	i = this->url_headers.Clone(&ur->url_headers,(int * (*) (void *, void *)) & osip_uri_param_clone);
	if (i != 0)
	{
		delete ur;
		ur = NULL;
		return -1;
	}
	*dest = ur;
	return 0;		
}

void CUrl::SetScheme( char* value )
{
	scheme = value;
}

char* CUrl::GetScheme()
{
	return scheme;
}

void CUrl::SetHost( char* value )
{
	host = value;
}

char* CUrl::GetHost()
{
	return host;
}

void CUrl::SetUsername( char* value )
{
	username = value;
}

char* CUrl::GetUsername()
{
	return username;
}

void CUrl::SetPassword( char* value )
{
	password = value;
}

char* CUrl::GetPassword()
{
	return password;
}

void CUrl::SetPort( char* value )
{
	port = value;
}

char* CUrl::GetPort()
{
	return port;
}
