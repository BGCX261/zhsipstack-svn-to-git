// SipMessageParse.cpp: implementation of the CSipMessageParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipMessageParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipMessageParse::CSipMessageParse()
{


}

CSipMessageParse::~CSipMessageParse()
{

}

int CSipMessageParse::SipMessageParse(CSipMessage* dest,const char* buf,
									 size_t length, int sipfrag)
{
	int i;
	const char* next_header_index;
	char* tmp;
	char* beg;
	
	tmp = new char[length + 2];
	if (tmp == NULL)
	{
		return -1;
	}
	beg = tmp;
	memcpy(tmp, buf, length);    /* may contain binary data */
	tmp[length] = '\0';
	ReplaceAllLws(tmp);
	/* parse request or status line */
	i = SipMessageStartLineParse(dest, tmp, &next_header_index);
	if (i == -1 && !sipfrag)
	{
		
		free(beg);
		return -1;
	}
	tmp = (char *) next_header_index;
	
	/* parse headers */
	i = MessageHeadersParse(dest, tmp, &next_header_index);
	if (i == -1)
	{
		free(beg);
		return -1;
	}
	tmp = (char *) next_header_index;
	
	/* this is a *very* simple test... (which handle most cases...) */
	if (tmp[0] == '\0' || tmp[1] == '\0' || tmp[2] == '\0')
	{
		/* this is mantory in the oSIP stack */
		if (dest->m_contentLength == NULL)
		{
			CContentLength *p = new CContentLength();
			if (p == NULL)
			{
				return -1;
			}
			if ( 0 == p->Parse("0"))
			{
				dest->m_contentLength = p;
			}
		}

		free(beg);
		return 0;   			  /* no body found */
	}
	
	i = SipBodyParse(dest, tmp, &next_header_index, length - (tmp - beg));
	free(beg);
	if (i == -1)
	{
		return -1;
	}
	
	/* this is mandatory in the oSIP stack */
	if (dest->m_contentLength == NULL)
	{
		dest->SetContentLength("0");
	}
	
	return 0;
}

void CSipMessageParse::ReplaceAllLws(char* sip_message)
{
	/* const char *end_of_message; */
	char* tmp;
	
	if (sip_message == NULL)
		return;
	
	/* end_of_message = sip_message + strlen (sip_message); */
	
	tmp = sip_message;
	for (; tmp[0] != '\0'; tmp++)
	{
		if (('\0' == tmp[0]) ||
			('\0' == tmp[1]) ||
			('\0' == tmp[2]) ||
			('\0' == tmp[3]))
			return;
		
		if ((('\r' == tmp[0]) &&
			('\n' == tmp[1]) &&
			('\r' == tmp[2]) &&
			('\n' == tmp[3])) ||
			(('\r' == tmp[0]) && ('\r' == tmp[1])) ||
			(('\n' == tmp[0]) && ('\n' == tmp[1])))
			return; 				/* end of message */
		
		if ((('\r' == tmp[0]) &&
			('\n' == tmp[1]) &&
			((' ' == tmp[2]) || ('\t' == tmp[2]))) ||
			(('\r' == tmp[0]) && ((' ' == tmp[1]) || ('\t' == tmp[1]))) ||
			(('\n' == tmp[0]) && ((' ' == tmp[1]) || ('\t' == tmp[1]))))
		{
			/* replace line end and TAB symbols by SP */
			tmp[0] = ' ';
			tmp[1] = ' ';
			tmp = tmp + 2;
			/* replace all following TAB symbols */
			for (; ('\t' == tmp[0] || ' ' == tmp[0]);)
			{
				tmp[0] = ' ';
				tmp++;
			}
		}
	}
}

int CSipMessageParse::SipMessageStartLineParseResp(CSipMessage* dest,
											  const char* buf, const char** headers)
{
	const char* statuscode;
	const char* reasonphrase;
	
	dest->m_strMethod = "";
	
	*headers = buf;
	
	statuscode = strchr(buf, ' ');  	 /* search for first SPACE */
	if (statuscode == NULL)
		return -1;
	dest->m_strVersion = (char *) MallocAndZero(statuscode - (*headers) + 1);
	strncpy(dest->m_strVersion, *headers, statuscode - (*headers));
	
	reasonphrase = strchr(statuscode + 1, ' ');
	if (reasonphrase == NULL)
	{
 		free(dest->m_strVersion);
 		dest->m_strVersion = NULL;
		return -1;
	}
	/* dest->status_code = (char *) MallocAndZero (reasonphrase - statuscode); */
	/* strncpy (dest->status_code, statuscode + 1, reasonphrase - statuscode - 1); */
	if (sscanf(statuscode + 1, "%d", &dest->m_iStatusCode) != 1)
	{
		/* Non-numeric status code */
		return -1;
	}
	
	if (dest->m_iStatusCode == 0)
		return -1;
	{
		const char* hp = reasonphrase;
		
		while ((*hp != '\r') && (*hp != '\n'))
		{
			if (*hp)
				hp++;
			else
			{
				return -1;
			}
		}
		dest->m_strReasonPhrase = (char *) MallocAndZero(hp - reasonphrase);
		strncpy(dest->m_strReasonPhrase, reasonphrase + 1,
			hp - reasonphrase - 1);
		
		hp++;
		if ((*hp) && ('\r' == hp[-1]) && ('\n' == hp[0]))
			hp++;
		(*headers) = hp;
	}
	return 0;
}

int CSipMessageParse::SipMessageStartLineParseReq(CSipMessage* dest,
	const char* buf, const char** headers)
{
	const char* p1;
	const char* p2;
	char* requesturi;
	int i = 0;

	dest->m_strMethod = NULL;
	dest->m_iStatusCode = 0;
	dest->m_strReasonPhrase = NULL;

	*headers = buf;

	/* The first token is the method name: */
	p2 = strchr(buf, ' ');
	if (p2 == NULL)
		return -1;
	if (*(p2 + 1) == '\0' || *(p2 + 2) == '\0')
		return -1;
	if (p2 - buf == 0)
	{
		return -1;
	}
	dest->m_strMethod = (char *) MallocAndZero(p2 - buf + 1);
	if (dest->m_strMethod == NULL)
	{
		return -1;
	}
	memset(dest->m_strMethod,0,p2 - buf + 1);
	strncpy(dest->m_strMethod, buf, p2 - buf);

	/* The second token is a sip-url or a uri: */
	p1 = strchr(p2 + 2, ' ');    /* no space allowed inside sip-url */
	if (p1 == NULL)
	{
		free(dest->m_strMethod);
		dest->m_strMethod = NULL;
		return -1;
	}
	if (p1 - p2 < 2)
	{
		free(dest->m_strMethod);
		dest->m_strMethod = NULL;
		return -1;
	}

	requesturi = (char *) MallocAndZero(p1 - p2);
	if (requesturi == NULL)
	{
		return -1;
	}
	memset(requesturi,0,p1 - p2);
	strncpy(requesturi, p2 + 1, (p1 - p2 - 1));

	CUrl *pUrl = new CUrl;
	if (pUrl == NULL)
	{
		return -1;
	}
	i = pUrl->Parse(requesturi);
	free(requesturi);
	if (i != 0)
	{
		free(dest->m_strMethod);
		dest->m_strMethod = NULL;
		return -1;
	}
	dest->m_pRequestUrl = pUrl;

	/* find the the version and the beginning of headers */
	{
		const char* hp = p1;

		while ((*hp != '\r') && (*hp != '\n'))
		{
			if (*hp)
				hp++;
			else
			{
				free(dest->m_strMethod);
				dest->m_strMethod = NULL;

				return -1;
			}
		}
		if (hp - p1 < 2)
		{
			free(dest->m_strMethod);
			dest->m_strMethod = NULL;

			return -1;
		}

		dest->m_strVersion = (char *) MallocAndZero(hp - p1);
		memset(dest->m_strVersion,0,hp - p1);
		strncpy(dest->m_strVersion, p1 + 1, (hp - p1 - 1));

		if (0 != _stricmp(dest->m_strVersion, "SIP/2.0"))
		{
			return -1;
		}

		hp++;
		if ((*hp) && ('\r' == hp[-1]) && ('\n' == hp[0]))
			hp++;
		(*headers) = hp;
	}
	return 0;
}

int CSipMessageParse::FindNextCrlf(const char* start_of_header,
	const char** end_of_header)
{
	const char* soh = start_of_header;

	*end_of_header = NULL;  	  /* AMD fix */

	while (('\r' != *soh) && ('\n' != *soh))
	{
		if (*soh)
			soh++;
		else
		{
			return -1;
		}
	}

	if (('\r' == soh[0]) && ('\n' == soh[1]))
		/* case 1: CRLF is the separator
		   case 2 or 3: CR or LF is the separator */
		soh = soh + 1;


	/* VERIFY if TMP is the end of header or LWS.   		 */
	/* LWS are extra SP, HT, CR and LF contained in headers. */
	if ((' ' == soh[1]) || ('\t' == soh[1]))
	{
		/* From now on, incoming message that potentially
			   contains LWS must be processed with
			   -> void osip_util_replace_all_lws(char *)
			   This is because the parser methods does not
			   support detection of LWS inside. */
		return -1;
	}

	*end_of_header = soh + 1;
	return 0;
}


int CSipMessageParse::SipParseHeader(CSipMessage* sip, const char* hname,
									const char* hvalue)
{
	if (strncmp(hname,VIA,3) == 0)
	{
		CVia *pH = new CVia();
		pH->Parse(hvalue);
		sip->m_listVias.Add(pH,0);
	}
	else if (strncmp(hname,FROM,7) == 0)
	{
		CFrom *pH = new CFrom;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_from = pH;
		}
	}
	else if (strncmp(hname,TO,2) == 0)
	{
		CTo *pH = new CTo;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_to = pH;
		}
	}
	else if (strncmp(hname,CALL_ID,7) == 0)
	{
		CCallId *pH = new CCallId;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_callid = pH;
		}	
	}
	else if (strncmp(hname,CSEQ,4) == 0)
	{
		CSeq *pH = new CSeq;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_seq = pH;
		}	
	}
	else if (strncmp(hname,CONTACT,6) == 0)
	{
		CContact *pH = new CContact;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_contacts.Add(pH);
		}
	}
	else if (strncmp(hname,ALLOW,5) == 0)
	{
		CAllow *pH = new CAllow;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_listAllow.Add(pH);
		}
	}
	else if (strncmp(hname,ACCEPT_ENCODING,15) == 0)
	{
		CAcceptEncoding *pH = new CAcceptEncoding;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_listAcceptEncoding.Add(pH);
		}
	}
	else if (strncmp(hname,ACCEPT,6) == 0)
	{
		CAccept *pH = new CAccept;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_listAccept.Add(pH);
		}
	}
	else if (strncmp(hname,CONTENT_TYPE,12) == 0)
	{
		CContentType *pH = new CContentType;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_contentType = pH;
		}
	}
	else if (strncmp(hname,CONTENT_LENGTH,14) == 0)
	{
		CContentLength *pH = new CContentLength;
		if ( pH->Parse(hvalue) == 0)
		{
			sip->m_contentLength = pH;
		}
	}

	return 0;
	ParseMethod pMethod = FindParseMethod(hname);
	
	if (pMethod != NULL)
	{
		pMethod(sip,hvalue);
	}

	return 0;
}

int CSipMessageParse::SipParseMultipleHeader(CSipMessage* sip, char* hname,
	char* hvalue)
{
	int i;
	char* ptr;  				  /* current location of the search */
	char* comma;				  /* This is the separator we are elooking for */
	char* beg;  				  /* beg of a header */
	char* end;  				  /* end of a header */
	char* quote1;   			  /* first quote of a pair of quotes   */
	char* quote2;   			  /* second quuote of a pair of quotes */
	size_t hname_len;

	/* Find header based upon lowercase comparison */
	zstr_tolower(hname);

	if (hvalue == NULL)
	{
		i = SipParseHeader(sip, hname, hvalue);
		if (i == -1)
			return -1;
		return 0;
	}

	ptr = hvalue;
	comma = strchr(ptr, ',');

	hname_len = strlen(hname);

	if (comma ==
		NULL ||
		(hname_len == 4 && strncmp(hname, "date", 4) == 0) ||
		(hname_len == 2 && strncmp(hname, "to", 2) == 0) ||
		(hname_len == 4 && strncmp(hname, "from", 4) == 0) ||
		(hname_len == 7 && strncmp(hname, "call-id", 7) == 0) ||
		(hname_len == 4 && strncmp(hname, "cseq", 4) == 0) ||
		(hname_len == 7 && strncmp(hname, "subject", 7) == 0) ||
		(hname_len == 7 && strncmp(hname, "expires", 7) == 0) ||
		(hname_len == 6 && strncmp(hname, "server", 6) == 0) ||
		(hname_len == 10 && strncmp(hname, "user-agent", 10) == 0) ||
		(hname_len == 16 && strncmp(hname, "www-authenticate", 16) == 0) ||
		(hname_len == 19 && strncmp(hname, "authentication-info", 19) == 0) ||
		(hname_len == 18 && strncmp(hname, "proxy-authenticate", 18) == 0) ||
		(hname_len == 19 && strncmp(hname, "proxy-authorization", 19) == 0) ||
		(hname_len == 25 &&
		strncmp(hname, "proxy-authentication-info", 25) == 0) ||
		(hname_len == 12 && strncmp(hname, "organization", 12) == 0) ||
		(hname_len == 13 && strncmp(hname, "authorization", 13) == 0))
		/* there is no multiple header! likely  	*/
		/* to happen most of the time...			*/
		/* or hname is a TEXT-UTF8-TRIM and may 	*/
		/* contain a comma. this is not a separator */
		/* THIS DOES NOT WORK FOR UNKNOWN HEADER!!!! */
	{
		i = SipParseHeader(sip, hname, hvalue);
		if (i == -1)
			return -1;
		return 0;
	}

	beg = hvalue;
	end = NULL;
	quote2 = NULL;
	while (comma != NULL)
	{
		quote1 = zstr_quote_find(ptr);
		if (quote1 != NULL)
		{
			quote2 = zstr_quote_find(quote1 + 1);
			if (quote2 == NULL)
				return -1;  		/* quotes comes by pair */
			ptr = quote2 + 1;
		}

		if ((quote1 == NULL) || (quote1 > comma))
		{
			/* We must search for the next comma which is not
					   within quotes! */
			end = comma;

			if (quote1 != NULL && quote1 > comma)
			{
				/* comma may be within the quotes */
				/* ,<sip:usera@host.example.com>;methods=\"INVITE,BYE,OPTIONS,ACK,CANCEL\",<sip:userb@host.blah.com> */
				/* we want the next comma after the quotes */
				char* tmp_comma;
				char* tmp_quote1;
				char* tmp_quote2;

				tmp_quote1 = quote1;
				tmp_quote2 = quote2;
				tmp_comma = strchr(comma + 1, ',');
				while (1)
				{
					if (tmp_comma < tmp_quote1)
						break;  	/* ok (before to quotes) */
					if (tmp_comma < tmp_quote2)
					{
						tmp_comma = strchr(tmp_quote2 + 1, ',');
					}
					tmp_quote1 = zstr_quote_find(tmp_quote2 + 1);
					if (tmp_quote1 == NULL)
						break;
					tmp_quote2 = zstr_quote_find(tmp_quote1 + 1);
					if (tmp_quote2 == NULL)
						break;  	/* probably a malformed message? */
				}
				comma = tmp_comma;  	  /* this one is not enclosed within quotes */
			}
			else
				comma = strchr(comma + 1, ',');
			if (comma != NULL)
				ptr = comma + 1;
		}
		else if ((quote1 < comma) && (quote2 < comma))
		{
			/* quotes are located before the comma, */
			/* continue the search for next quotes  */
			ptr = quote2 + 1;
		}
		else if ((quote1 < comma) && (comma < quote2))
		{
			/* if comma is inside the quotes... */
			/* continue with the next comma.	*/
			ptr = quote2 + 1;
			comma = strchr(ptr, ',');
			if (comma == NULL)
						/* this header last at the end of the line! */
			{
				/* this one does not need an allocation... */
#if 0
			  if (strlen (beg) < 2)
				return 0;   	/* empty header */
#else
			  if (beg[0] == '\0' || beg[1] == '\0')
				return 0;   	/* empty header */
#endif
				zstr_clrspace(beg);
				i = SipParseHeader(sip, hname, beg);
				if (i == -1)
					return -1;
				return 0;
			}
		}

		if (end != NULL)
		{
			char* avalue;

			if (end - beg + 1 < 2)
				return -1;
			avalue = (char *) MallocAndZero(end - beg + 1);
			zstr_clrncpy(avalue, beg, end - beg);
			/* really store the header in the sip structure */
			i = SipParseHeader(sip, hname, avalue);
			free(avalue);
			if (i == -1)
				return -1;
			beg = end + 1;
			end = NULL;
			if (comma == NULL)
						/* this header last at the end of the line! */
			{
				/* this one does not need an allocation... */
#if 0
			  if (strlen (beg) < 2)
				return 0;   	/* empty header */
#else
			  if (beg[0] == '\0' || beg[1] == '\0')
				return 0;   	/* empty header */
#endif
				zstr_clrspace(beg);
				i = SipParseHeader(sip, hname, beg);
				if (i == -1)
					return -1;
				return 0;
			}
		}
	}
	return -1;  				  /* if comma is NULL, we should have already return 0 */
}

int CSipMessageParse::MessageHeadersParse(CSipMessage* sip, const char* start_of_header,
	const char** body)
{
	const char* colon_index;	  /* index of ':' */
	char* hname;
	char* hvalue;
	const char* end_of_header;
	int i;

	for (; ;)
	{
		if (start_of_header[0] == '\0')
		{
			/* final CRLF is missing */
			return 0;
		}

		i = FindNextCrlf(start_of_header, &end_of_header);
		if (i == -1)
		{
			return -1;  		  /* this is an error case! 	*/
		}

		/* the list of headers MUST always end with  */
		/* CRLFCRLF (also CRCR and LFLF are allowed) */
		if ((start_of_header[0] == '\r') || (start_of_header[0] == '\n'))
		{
			*body = start_of_header;
			return 0;   		  /* end of header found		*/
		}

		/* find the header name */
		colon_index = strchr(start_of_header, ':');
		if (colon_index == NULL)
		{
			return -1;  		  /* this is also an error case */
		}
		if (colon_index - start_of_header + 1 < 2)
			return -1;
		if (end_of_header <= colon_index)
		{
			return -1;
		}
		hname = (char *) MallocAndZero(colon_index - start_of_header + 1);
		zstr_clrncpy(hname, start_of_header, colon_index - start_of_header);
		{
			const char* end;

			/* END of header is (end_of_header-2) if header separation is CRLF */
			/* END of header is (end_of_header-1) if header separation is CR or LF */
			if ((end_of_header[-2] == '\r') || (end_of_header[-2] == '\n'))
				end = end_of_header - 2;
			else
				end = end_of_header - 1;
			if ((end) - colon_index < 2)
				hvalue = NULL;  	  /* some headers (subject) can be empty */
			else
			{
				hvalue = (char *) MallocAndZero((end) - colon_index + 1);
				zstr_clrncpy(hvalue, colon_index + 1, (end) - colon_index - 1);
			}
		}

		/* hvalue MAY contains multiple value. In this case, they   */
		/* are separated by commas. But, a comma may be part of a   */
		/* quoted-string ("here, and there" is an example where the */
		/* comma is not a separator!) */
		i = SipParseMultipleHeader(sip, hname, hvalue);

		free(hname);
		if (hvalue != NULL)
			free(hvalue);
		if (i == -1)
		{
			return -1;
		}

		/* continue on the next header */
		start_of_header = end_of_header;
	}

	return -1;
}

int CSipMessageParse::SipBodyParse(CSipMessage* sip, const char* start_of_buf,
	const char** next_body, size_t length)
{
	const char* start_of_body;
	const char* end_of_body;
	const char* end_of_buf;
	char* tmp;
	int i;

	char* sep_boundary;
	size_t len_sep_boundary;
	CUrlParam* ct_param;

	if (sip->m_contentType == NULL ||
		sip->m_contentType->type == NULL ||
		sip->m_contentType->subtype == NULL)
		return 0;   				/* no body is attached */

	if (0 != zstr_strcasecmp(sip->m_contentType->type, "multipart"))
	{
		size_t osip_body_len;

		if (start_of_buf[0] == '\0')
			return -1;  			/* final CRLF is missing */
		/* get rid of the first CRLF */
		if ('\r' == start_of_buf[0])
		{
			if ('\n' == start_of_buf[1])
				start_of_body = start_of_buf + 2;
			else
				start_of_body = start_of_buf + 1;
		}
		else if ('\n' == start_of_buf[0])
			start_of_body = start_of_buf + 1;
		else
			return -1;  			/* message does not end with CRLFCRLF, CRCR or LFLF */

		/* update length (without CRLFCRLF */
		length = length - (start_of_body - start_of_buf); /* fixed 24 08 2004 */
		if (length <= 0)
			return -1;

		if (sip->m_contentLength != NULL)
			osip_body_len = zstr_atoi(sip->m_contentLength->value);
		else
		{
			/* if content_length does not exist, set it. */
			char tmp[16];

			/* case where content-length is missing but the
					   body only contains non-binary data */
			if (0 == zstr_strcasecmp(sip->m_contentType->type, "application") &&
				0 == zstr_strcasecmp(sip->m_contentType->subtype,"sdp"))
			{
				osip_body_len = strlen(start_of_body);
				sprintf(tmp, "%i", osip_body_len);
				i = sip->SetContentLength(tmp);
				if (i != 0)
					return -1;
			}
			else
				return -1;  		/* Content-type may be non binary data */
		}

		if (length < osip_body_len)
		{
			return -1;
		}

		end_of_body = start_of_body + osip_body_len;
		tmp = MallocAndZero(end_of_body - start_of_body + 2);
		if (tmp == NULL)
			return -1;
		memcpy(tmp, start_of_body, end_of_body - start_of_body);
		tmp[end_of_body - start_of_body] = '\0';

		i = SipSetBody(sip, tmp, end_of_body - start_of_body);
		free(tmp);
		if (i != 0)
			return -1;
		return 0;
	}

	/* find the boundary */
	i = osip_generic_param_get_byname(&sip->m_contentType->gen_params,
			"boundary", &ct_param);
	if (i != 0)
		return -1;

	if (ct_param == NULL)
		return -1;
	if (ct_param->gvalue == NULL)
		return -1;  				/* No boundary but multiple headers??? */
	{
		const char* boundary_prefix = "\n--";

		size_t len = strlen(ct_param->gvalue);

		sep_boundary = (char *) MallocAndZero(len + sizeof(boundary_prefix));
		sprintf(sep_boundary, boundary_prefix);
		if (ct_param->gvalue[0] == '"' && ct_param->gvalue[len - 1] == '"')
			strncat(sep_boundary, ct_param->gvalue + 1, len - 2);
		else
			strncat(sep_boundary, ct_param->gvalue, len);
	}

	len_sep_boundary = strlen(sep_boundary);

	*next_body = NULL;
	start_of_body = start_of_buf;

	end_of_buf = start_of_buf + length;

	for (; ;)
	{
		size_t body_len = 0;

		i = zstr_find_next_occurence(sep_boundary, start_of_body,
				&start_of_body, end_of_buf);
		if (i == -1)
		{
			free(sep_boundary);
			return -1;
		}

		i = zstr_find_next_occurence(sep_boundary,
				start_of_body + len_sep_boundary, &end_of_body, end_of_buf);
		if (i == -1)
		{
			free(sep_boundary);
			return -1;
		}

		/* this is the real beginning of body */
		start_of_body = start_of_body + len_sep_boundary + 1;
		if ('\n' == start_of_body[0] || '\r' == start_of_body[0])
			start_of_body++;

		body_len = end_of_body - start_of_body;

		/* Skip CR before end boundary. */
		if (*(end_of_body - 1) == '\r')
			body_len--;

		tmp = MallocAndZero(body_len + 2);
		if (tmp == NULL)
		{
			free(sep_boundary);
			return -1;
		}
		memcpy(tmp, start_of_body, body_len);
		tmp[body_len] = '\0';

		i = SipSetBodyMime(sip, tmp, body_len);
		free(tmp);
		if (i == -1)
		{
			free(sep_boundary);
			return -1;
		}

		if (strncmp(end_of_body + len_sep_boundary, "--", 2) == 0)
		{
			/* end of all bodies */
			*next_body = end_of_body;
			free(sep_boundary);
			return 0;
		}

		/* continue on the next body */
		start_of_body = end_of_body;
	}
	/* Unreachable code */
	/* free (sep_boundary); */
	return -1;
	
	return 0;
}


int CSipMessageParse::SipMessageStartLineParse(CSipMessage* dest,
										  const char* buf, const char** headers)
{
	if (0 == strncmp((const char *) buf, (const char *) "SIP/", 4))
		return SipMessageStartLineParseResp(dest, buf, headers);
	else
		return SipMessageStartLineParseReq(dest, buf, headers);
}

map<string,ParseMethod> CSipMessageParse::m_mapParseMethod;


int CSipMessageParse::InitParsers()
{
//	m_mapParseMethod[VIA] = CVia::Parse;
// 	m_mapParseMethod[CSEQ] = CSeq::Parse;
// 	m_mapParseMethod[CONTACT] = CContact::Parse;
// 	m_mapParseMethod[FROM] = CFrom::Parse;
// 	m_mapParseMethod[TO] = CTo::Parse;
	
	return m_mapParseMethod.size();	
}

ParseMethod CSipMessageParse::FindParseMethod(const char *pcMethod)
{
	map<string,ParseMethod>::iterator it; 
	it = m_mapParseMethod.find(pcMethod);
	if (it == m_mapParseMethod.end())
	{
		return NULL;
	}
	return m_mapParseMethod[pcMethod];
}

int CSipMessageParse::Parse( CSipMessage **dest,char *pcBuffer,int iBufferLen )
{
	CSipMessage *pSip = new CSipMessage();

	SipMessageParse(pSip,pcBuffer,iBufferLen);
		
	*dest = pSip;

	return 0;
}

int CSipMessageParse::SipSetBody( CSipMessage* sip, const char* buf, size_t length )
{
	int i;
	CBody *pBody = new CBody;
	if (pBody == NULL)
	{
		return -1;
	}

	i = pBody->Parse(buf,length);
	if (i != 0)
	{
		return -1;
	}
	
	sip->m_listBodys.Add(pBody);

	return 0;
}

int CSipMessageParse::SipSetBodyMime( CSipMessage* sip, const char* buf, size_t length )
{
	CBody* pBody = new CBody;
	int i;
	
	i = pBody->ParseMime(buf, length);
	if (i != 0)
	{
		delete pBody;
		pBody = NULL;
		return -1;
	}
	sip->m_listBodys.Add(pBody, -1);

	return 0;
	
}

