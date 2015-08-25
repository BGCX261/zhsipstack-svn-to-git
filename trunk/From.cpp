// From.cpp: implementation of the CFrom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "From.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrom::CFrom()
{
	Init();
}

CFrom::~CFrom()
{
	Free();
}

int CFrom::Parse( const char *hvalue )
{
	const char* displayname;
	const char* url;
	const char* url_end;
	const char* gen_params;

	/* How to parse:
	   we'll place the pointers:
	   displayname  =>  beginning of displayname
	   url  		=>  beginning of url
	   url_end  	=>  end 	  of url
	   gen_params  =>  beginning of params
	   examples:
	   jack <sip:jack@atosc.org>;tag=34erZ
	   ^	 ^  			  ^ ^
	   sip:jack@atosc.org;tag=34erZ
	   ^				^^  	
	 */

	displayname = strchr(hvalue, '"');

	url = strchr(hvalue, '<');
	if (url != NULL)
	{
		url_end = strchr(url, '>');
		if (url_end == NULL)
			return -1;
	}

	/* SIPit day2: this case was not supported
	   first '"' is placed after '<' and after '>'
	   <sip:ixion@62.254.248.117;method=INVITE>;description="OPEN";expires=28800
	   if the fisrt quote is after '<' then
	   this is not a quote for a displayname.
	 */
	if (displayname != NULL)
	{
		if (displayname > url)
			displayname = NULL;
	}

	if ((displayname == NULL) && (url != NULL))
	{
		/* displayname IS A '*token' (not a quoted-string) */
		if (hvalue != url)  	  /* displayname exists */
		{
			if (url - hvalue + 1 < 2)
				return -1;
			this->displayname = (char *) mm_malloc(url - hvalue + 1);
			if (this->displayname == NULL)
				return -1;
			zstr_clrncpy(this->displayname, hvalue, url - hvalue);
		}
		url++;  				  /* place pointer on the beginning of url */
	}
	else
	{
		if ((displayname != NULL) && (url != NULL))
		{
			/* displayname IS A quoted-string (not a '*token') */
			const char* first;
			const char* second = NULL;

			/* search for quotes */
			first = zstr_quote_find(hvalue);
			if (first == NULL)
				return -1;  		/* missing quote */
			second = zstr_quote_find(first + 1);
			if (second == NULL)
				return -1;  		/* missing quote */
			if ((first > url))
				return -1;

			if (second - first + 2 >= 2)
			{
				this->displayname = (char *) mm_malloc(second - first + 2);
				if (this->displayname == NULL)
					return -1;
				zstr_strncpy(this->displayname, first, second - first + 1);
				/* osip_clrspace(from->displayname); *//*should we do that? */

				/* special case: "<sip:joe@big.org>" <sip:joe@really.big.com> */
			}   				/* else displayname is empty? */
			url = strchr(second + 1, '<');
			if (url == NULL)
				return -1;  		/* '<' MUST exist */
			url++;
		}
		else
			url = hvalue;   		/* field does not contains '<' and '>' */
	}

	/* DISPLAY-NAME SET   */
	/* START of URL KNOWN */

	url_end = strchr(url, '>');

	if (url_end == NULL)		  /* sip:jack@atosc.org;tag=023 */
	{
		/* We are sure ';' is the delimiter for from-parameters */
		char* host = strchr(url, '@');

		if (host != NULL)
			gen_params = strchr(host, ';');
		else
			gen_params = strchr(url, ';');
		if (gen_params != NULL)
			url_end = gen_params - 1;
		else
			url_end = url + strlen(url);
	}
	else						/* jack <sip:jack@atosc.org;user=phone>;tag=azer */
	{
		gen_params = strchr(url_end, ';');
		url_end--;  			  /* place pointer on the beginning of url */
	}

	if (gen_params != NULL) 	  /* now we are sure a param exist */
		if (GenericParamParseAll(&this->gen_params, gen_params) == -1)
		{
			return -1;
		}

	/* set the url */
	{
		char* tmp;
		int i = 0;

		if (url_end - url + 2 < 7)
			return -1;
		tmp = (char *) mm_malloc(url_end - url + 2);
		if (tmp == NULL)
			return -1;
		zstr_strncpy(tmp, url, url_end - url + 1);
		if (this->url == NULL)
		{
			this->url = new CUrl;
		}
		if (this->url != NULL)
		{
			this->url->Parse(tmp);
		}
		mm_free(tmp);
		if (i != 0)
			return -1;
	}
	return 0;
}
/* returns the from header as a string.  */
/* INPUT : osip_from_t *from | from header.   */
/* returns -1 on error. */
int CFrom::ToStr(char** dest)
{
	char* url;
	char* buf;
	int i;
	size_t len;

	*dest = NULL;
	if ((this->url == NULL))
		return -1;

	i = this->url->ToStr(&url);
	if (i != 0)
		return -1;

	if (this->displayname == NULL)
		len = strlen(url) + 5;
	else
		len = strlen(url) + strlen(this->displayname) + 5;

	buf = (char *) mm_malloc(len);
	if (buf == NULL)
	{
		mm_free(url);
		return -1;
	}

	if (this->displayname != NULL)
		sprintf(buf, "%s <%s>", this->displayname, url);
	else
		/* from rfc2543bis-04: for authentication related issue!
		   "The To and From header fields always include the < and >
		   delimiters even if the display-name is empty." */
		sprintf(buf, "<%s>", url);
	mm_free(url);
	{
		int pos = 0;
		CUrlParam* u_param;
		size_t plen;
		char* tmp;

		while (!this->gen_params.IsListEof(pos))
		{
			u_param = (CUrlParam *) this->gen_params.GetAt(pos);

			if (u_param->gvalue == NULL)
				plen = strlen(u_param->gname) + 2;
			else
				plen = strlen(u_param->gname) + strlen(u_param->gvalue) + 3;
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

int CFrom::Init()
{
	displayname = NULL;
	url = NULL;

	return 0;
}

void CFrom::Free()
{
	mm_free(displayname);
	mm_free(url);

}

int CFrom::Clone( CFrom** dest )
{
	int i;
	CFrom* fr;
	
	*dest = NULL;
	
	fr = new CFrom;
	if (fr == NULL)
	{
		return -1;
	}

	if (this->displayname != NULL)
		fr->displayname = zstr_strdup(this->displayname);
	
	if (this->url != NULL)
	{
		i = this->url->Clone(&(fr->url));
		if (i != 0)
		{
			delete(fr);
			fr = NULL;
			return -1;
		}
	}
	
	i = this->gen_params.Clone(&fr->gen_params,(int * (*) (void *, void *)) & osip_generic_param_clone);
	if (i != 0)
	{
		delete(fr);
		fr = NULL;
		return -1;
	}
	*dest = fr;
	return 0;
}

void CFrom::SetDisplayName( char* value )
{
	displayname = value;
}

char* CFrom::GetDisplayName( )
{
	return displayname;
}

void CFrom::SetUrl( CUrl* value )
{
	url = value;
}

CUrl* CFrom::GetUrl( )
{
	return url;	
}

int CFrom::GetParam( int pos,CUrlParam** dest )
{
	if (gen_params.GetSize() <= pos)
	{
		return -1;
	}
		
	*dest = (CUrlParam*)gen_params.GetAt(pos);

	return pos;
}

int CFrom::GetTag( CUrlParam** dest )
{
	return osip_uri_param_get_byname(&gen_params,"tag",dest);		
}

int CFrom::Compare( CFrom *from1 )
{
	char* tag1;
	char* tag2;

	if (from1 == NULL)
		return -1;
	if (from1->url == NULL || url == NULL)
		return -1;

	/* we could have a sip or sips url, but if string!=NULL,
	   host part will be NULL. */
	if (from1->url->host == NULL && url->host == NULL)
	{
		if (from1->url->m_pcOther == NULL || url->m_pcOther == NULL)
			return -1;
		if (0 == strcmp(from1->url->m_pcOther, url->m_pcOther))
			return 0;
	}
	if (from1->url->host == NULL || url->host == NULL)
		return -1;

	/* compare url including tag */
	if (0 != strcmp(from1->url->host, url->host))
		return -1;
	if (from1->url->username != NULL && url->username != NULL)
		if (0 != strcmp(from1->url->username, url->username))
			return -1;

	tag1 = NULL;
	tag2 = NULL;
	{
		int pos = 0;
		CUrlParam* u_param;

		while (!from1->gen_params.IsListEof(pos))
		{
			u_param = (CUrlParam *)from1->gen_params.GetAt(pos);
			if (0 == strncmp(u_param->gname, "tag", 3))
			{
				tag1 = u_param->gvalue;
				break;
			}
			pos++;
		}
	}
	{
		int pos = 0;
		CUrlParam* u_param;

		while (!gen_params.IsListEof(pos))
		{
			u_param = (CUrlParam*)gen_params.GetAt(pos);
			if (0 == strncmp(u_param->gname, "tag", 3))
			{
				tag2 = u_param->gvalue;
				break;
			}
			pos++;
		}
	}

	/* sounds like a BUG!
	   if tag2 exists and tag1 does not, then it will
	   return 0;
	   in the first request, (INVITE) the To field does not
	   contain any tag. The response contains one! and the
	   response must match the request....
	 */
	/* so we test the tags only when both exist! */
	if (tag1 != NULL && tag2 != NULL)
		if (0 != strcmp(tag1, tag2))
			return -1;

	/* We could return a special case, when */
	/* only one tag exists?? */

	return 0;   				  /* return code changed to 0 from release 0.6.1 */	
}

int osip_from_clone(CFrom* from, CFrom** dest)
{
	int i;
	CFrom* fr;
	
	*dest = NULL;
	if (from == NULL)
		return -1;
	
	fr = new CFrom();
	if (fr == NULL) 				  /* allocation failed */
		return -1;
	if (from->displayname != NULL)
		fr->displayname = zstr_strdup(from->displayname);
	
	if (from->url != NULL)
	{
		i = from->url->Clone(&(fr->url));
		if (i != 0)
		{
			delete (fr);
			return -1;
		}
	}
	
	i = from->gen_params.Clone(&(fr->gen_params),(int * (*) (void *, void *)) & osip_generic_param_clone);
	if (i != 0)
	{
		delete (fr);
		return -1;
	}
	*dest = fr;
	return 0;
}