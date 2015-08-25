// MessageSip.cpp: implementation of the CSipMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const char* osip_protocol_version = "SIP/2.0";

static int _osip_message_realloc(char** message, char** dest, size_t needed,
								 size_t* malloc_size)
{
	size_t size = *message - *dest;
	
	if (*malloc_size < (size_t) (size + needed + 100))
	{
		*malloc_size = size + needed + 100;
		*dest = (char*)mm_realloc(*dest, *malloc_size);
		if (*dest == NULL)
			return -1;
		*message = *dest + size;
	}
	
	return 0;
}
static int strcat_simple_header(char** _string, size_t* malloc_size,
	char** _message, CNodeRoot *pNode, char* header_name,
	size_t size_of_header, char** next)
{
	char* string;
	char* message;
	char* tmp;
	int i;

	string = *_string;
	message = *_message;

	if (pNode != NULL)
	{
		if (*malloc_size < message - string + 100 + size_of_header)
				/* take some memory avoid to mm_realloc too much often */
		{
			/* should not happen often */
			size_t size = message - string;

			*malloc_size = message - string + size_of_header + 100;
			string = (char*)mm_realloc(string, *malloc_size);
			if (string == NULL)
			{
				*_string = NULL;
				*_message = NULL;
				return -1;
			}
			message = string + size;
		}
		message = osip_strn_append(message, header_name, size_of_header);

		i = pNode->ToStr(&tmp);
		if (i == -1)
		{
			*_string = string;
			*_message = message;
			*next = NULL;
			return -1;
		}
		if (*malloc_size < message - string + strlen(tmp) + 100)
		{
			size_t size = message - string;

			*malloc_size = message - string + strlen(tmp) + 100;
			string = (char*)mm_realloc(string, *malloc_size);
			if (string == NULL)
			{
				*_string = NULL;
				*_message = NULL;
				return -1;
			}
			message = string + size;
		}

		message = osip_str_append(message, tmp);
		mm_free(tmp);
		message = osip_strn_append(message, CRLF, 2);
	}
	*_string = string;
	*_message = message;
	*next = message;
	return 0;
}

static int strcat_headers_one_per_line(char** _string, size_t* malloc_size,
	char** _message, CZhList* headers, char* header,
	size_t size_of_header, char** next)
{
	char* string;
	char* message;
	char* tmp;
	int pos = 0;
	int i;

	string = *_string;
	message = *_message;

	while (!headers->IsListEof(pos))
	{
		CNodeRoot* elt;

		elt = (CNodeRoot *) headers->GetAt(pos);

		if (*malloc_size < message - string + 100 + size_of_header)
				/* take some memory avoid to mm_realloc too much often */
		{
			/* should not happen often */
			size_t size = message - string;

			*malloc_size = message - string + size_of_header + 100;
			string = (char*)mm_realloc(string, *malloc_size);
			if (string == NULL)
			{
				*_string = NULL;
				*_message = NULL;
				return -1;
			}
			message = string + size;
		}
		zstr_strncpy(message, header, size_of_header);
		i = elt->ToStr(&tmp);
		if (i == -1)
		{
			*_string = string;
			*_message = message;
			*next = NULL;
			return -1;
		}
		message = message + strlen(message);

		if (*malloc_size < message - string + strlen(tmp) + 100)
		{
			size_t size = message - string;

			*malloc_size = message - string + strlen(tmp) + 100;
			string = (char*)mm_realloc(string, *malloc_size);
			if (string == NULL)
			{
				*_string = NULL;
				*_message = NULL;
				return -1;
			}
			message = string + size;
		}
		message = osip_str_append(message, tmp);
		mm_free(tmp);
		message = osip_strn_append(message, CRLF, 2);
		pos++;
	}
	*_string = string;
	*_message = message;
	*next = message;
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipMessage::CSipMessage()
{
	Init();
}

CSipMessage::~CSipMessage()
{
	Free();
}

int CSipMessage::Init()
{
	m_strMethod = NULL;
	m_strVersion = NULL;
	m_iStatusCode = 0;
	m_strReasonPhrase = NULL;	
	
	m_pRequestUrl = NULL;

	m_callid = NULL;
	m_from = NULL;
	m_to = NULL;
	m_seq = NULL;
	m_contentType = NULL;
	m_contentLength = NULL;
	m_mime_version = NULL;

	message = NULL;
	application_data = NULL;

	return 0;
}

void CSipMessage::Free()
{
	mm_free(m_strMethod);
	mm_free(m_strReasonPhrase);
	m_iStatusCode = 0;

	mm_free(m_pRequestUrl);
	m_pRequestUrl = NULL;

	m_strMethod = NULL;
	m_strVersion = NULL;
	m_iStatusCode = 0;
	m_strReasonPhrase = NULL;	

	mm_free(m_callid);
	mm_free(m_from);
	mm_free(m_to);
	mm_free(m_seq);
	mm_free(m_contentType);
	mm_free(m_contentLength);
	mm_free(m_mime_version);
	
	m_callid = NULL;
	m_from = NULL;
	m_to = NULL;
	m_seq = NULL;
	m_contentType = NULL;
	m_contentLength = NULL;
	m_mime_version = NULL;

	mm_free(message);
	mm_free(application_data);
	message = NULL;
	application_data = NULL;
}

int CSipMessage::Clone( CSipMessage** dest )
{
	CSipMessage* copy;
	int pos = 0;
	int i;

	*dest = NULL;

	copy = new CSipMessage();
	if (copy == NULL)
		return -1;

	copy->m_strMethod = zstr_strdup(this->m_strMethod);
	copy->m_strVersion = zstr_strdup(this->m_strVersion);
	copy->m_iStatusCode = this->m_iStatusCode;
	copy->m_strReasonPhrase = zstr_strdup(this->m_strReasonPhrase);
	if (this->m_pRequestUrl != NULL)
	{
		i = this->m_pRequestUrl->Clone(&(copy->m_pRequestUrl));
		if (i != 0)
			goto mc_error1;
	}

#ifndef MINISIZE
	{
		CAccept* accept;
		CAccept* accept2;

		pos = 0;
		while (!this->m_listAccept.IsListEof(pos))
		{
			accept = (CAccept *)this->m_listAccept.GetAt(pos);
			i = accept->Clone(&accept2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAccept.Add(accept2, -1); 	/* insert as last element */
			pos++;
		}
	}
	{
		CAcceptEncoding* accept_encoding;
		CAcceptEncoding* accept_encoding2;

		pos = 0;
		while (!this->m_listAcceptEncoding.IsListEof(pos))
		{
			accept_encoding = (CAcceptEncoding *)this->m_listAcceptEncoding.GetAt(pos);
			i = accept_encoding->Clone(&accept_encoding2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAcceptEncoding.Add(accept_encoding2, -1);
			pos++;
		}
	}
	{
		CAcceptLanguage* accept_language;
		CAcceptLanguage* accept_language2;

		pos = 0;
		while (!this->m_listAcceptLanguages.IsListEof(pos))
		{
			accept_language = (CAcceptLanguage *)this->m_listAcceptLanguages.GetAt(pos);
			i = accept_language->Clone(&accept_language2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAcceptLanguages.Add(accept_language2, -1);
			pos++;
		}
	}
	{
		CAlertInfo* alert_info;
		CAlertInfo* alert_info2;

		pos = 0;
		while (!this->m_listAlertInfos.IsListEof(pos))
		{
			alert_info = (CAlertInfo *)this->m_listAlertInfos.GetAt(pos);
			i = alert_info->Clone(&alert_info2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAlertInfos.Add(alert_info2, -1);
			pos++;
		}
	}
	{
		CAllow* allow;
		CAllow* allow2;

		pos = 0;
		while (!this->m_listAllow.IsListEof(pos))
		{
			allow = (CAllow *) this->m_listAllow.GetAt(pos);
			i = allow->Clone(&allow2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAllow.Add(allow2, -1);
			pos++;
		}
	}
	{
		CAuthorizationInfo* authentication_info;
		CAuthorizationInfo* authentication_info2;

		pos = 0;
		while (!this->m_listAuthenticationInfos.IsListEof(pos))
		{
			authentication_info = (CAuthorizationInfo *) this->m_listAuthenticationInfos.GetAt(pos);
			i = authentication_info->Clone(&authentication_info2);
			if (i != 0)
				goto mc_error1;
			copy->m_listAuthenticationInfos.Add(authentication_info2,-1);
			pos++;
		}
	}
	{
		CCallInfo* call_info;
		CCallInfo* call_info2;

		pos = 0;
		while (!this->m_listCallInfos.IsListEof(pos))
		{
			call_info = (CCallInfo*) this->m_listCallInfos.GetAt(pos);
			i = call_info->Clone(&call_info2);
			if (i != 0)
				goto mc_error1;
			copy->m_listCallInfos.Add(call_info2, -1);
			pos++;
		}
	}
	{
		CContentEncoding* content_encoding;
		CContentEncoding* content_encoding2;

		pos = 0;
		while (!this->m_listContentEncodings.IsListEof(pos))
		{
			content_encoding = (CContentEncoding *) this->m_listContentEncodings.GetAt(pos);
			i = content_encoding->Clone(&content_encoding2);
			if (i != 0)
				goto mc_error1;
			copy->m_listContentEncodings.Add(content_encoding2, -1);
			pos++;
		}
	}
	{
		CErrorInfo* error_info;
		CErrorInfo* error_info2;

		pos = 0;
		while (!this->m_listErrorInfos.IsListEof(pos))
		{
			error_info = (CErrorInfo *) this->m_listErrorInfos.GetAt(pos);
			i = error_info->Clone(&error_info2);
			if (i != 0)
				goto mc_error1;
			copy->m_listErrorInfos.Add(error_info2, -1);
			pos++;
		}
	}
	{
		CProxyAuthenticationInfo* proxy_authentication_info;
		CProxyAuthenticationInfo* proxy_authentication_info2;

		pos = 0;
		while (!this->m_lisProxyAuthenticationInfos.IsListEof(pos))
		{
			proxy_authentication_info = (CProxyAuthenticationInfo *) this->m_lisProxyAuthenticationInfos.GetAt(pos);
			i = proxy_authentication_info->Clone(&proxy_authentication_info2);
			if (i != 0)
				goto mc_error1;
			copy->m_lisProxyAuthenticationInfos.Add(proxy_authentication_info2, -1);
			pos++;
		}
	}
#endif
	i = this->m_listAuthorizations.Clone(&copy->m_listAuthorizations,(int * (*) (void *, void *)) & osip_authorization_clone);
	if (i != 0)
		goto mc_error1;
	if (this->m_callid != NULL)
	{
		i = this->m_callid->Clone(&(copy->m_callid));
		if (i != 0)
			goto mc_error1;
	}
	i = this->m_contacts.Clone(&copy->m_contacts,(int * (*) (void *, void *)) & osip_contact_clone);
	if (i != 0)
		goto mc_error1;
	if (this->m_contentLength != NULL)
	{
		i = this->m_contentLength->Clone(&(copy->m_contentLength));
		if (i != 0)
			goto mc_error1;
	}
	if (this->m_contentType != NULL)
	{
		i = this->m_contentType->Clone(&(copy->m_contentType));
		if (i != 0)
			goto mc_error1;
	}
	if (this->m_seq != NULL)
	{
		i = this->m_seq->Clone(&(copy->m_seq));
		if (i != 0)
			goto mc_error1;
	}
	if (this->m_from != NULL)
	{
		i = this->m_from->Clone(&(copy->m_from));
		if (i != 0)
			goto mc_error1;
	}
	if (this->m_mime_version != NULL)
	{
		i = this->m_mime_version->Clone(&(copy->m_mime_version));
		if (i != 0)
			goto mc_error1;
	}
	i = this->m_lisProxyAuthenticates.Clone(&copy->m_lisProxyAuthenticates,(int * (*) (void *, void *)) & osip_proxy_authenticate_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_listProxyAuthorizations.Clone(&copy->m_listProxyAuthorizations,(int * (*) (void *, void *)) & osip_proxy_authorization_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_listRecordRoutes.Clone(&copy->m_listRecordRoutes,(int * (*) (void *, void *)) & osip_record_route_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_listRoutes.Clone(&copy->m_listRoutes,(int * (*) (void *, void *)) & osip_route_clone);
	if (i != 0)
		goto mc_error1;
	if (this->m_to != NULL)
	{
		i = this->m_to->Clone(&(copy->m_to));
		if (i != 0)
			goto mc_error1;
	}
	i = this->m_listVias.Clone(&copy->m_listVias,(int * (*) (void *, void *)) & osip_via_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_listWwwAuthenticates.Clone(&copy->m_listWwwAuthenticates,(int * (*) (void *, void *)) & osip_www_authenticate_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_headers.Clone(&copy->m_headers,(int * (*) (void *, void *)) & osip_header_clone);
	if (i != 0)
		goto mc_error1;
	i = this->m_listBodys.Clone(&copy->m_listBodys,(int * (*) (void *, void *)) & osip_body_clone);
	if (i != 0)
		goto mc_error1;

	copy->message_length = this->message_length;
	copy->message = zstr_strdup(this->message);
	copy->message_property = this->message_property;

	*dest = copy;
	return 0;
	mc_error1 : delete(copy);
	return -1;	
}

void CSipMessage::SetReasonPhrase( char* reason )
{
	m_strReasonPhrase = reason;
}

char* CSipMessage::GetReasonPhrase()
{
	return m_strReasonPhrase;
}

void CSipMessage::SetStatusCode( int statuscode )
{
	m_iStatusCode = statuscode;
}

int CSipMessage::GetStatusCode()
{
	return m_iStatusCode;
}

void CSipMessage::SetMethod( char* method )
{
	m_strMethod = method;
}

char* CSipMessage::GetMethod()
{
	return m_strMethod;
}

void CSipMessage::SetVersion( char* version )
{
	m_strVersion = version;
}

char* CSipMessage::GetVersion()
{
	return m_strVersion;
}

void CSipMessage::SetUri( CUrl* uri )
{
	m_pRequestUrl = uri;
}

CUrl* CSipMessage::GetUri()
{
	return m_pRequestUrl;	
}

int CSipMessage::SetFrom( const char* hvalue )
{
	if (m_from != NULL)
	{
		return -1;
	}

	m_from = new CFrom();
	if (m_from == NULL)
	{
		return -1;
	}
	m_from->Parse(hvalue);

	return 0;
}

CTo* CSipMessage::GetFrom()
{
	return m_from;	
}

int CSipMessage::SetTo( const char* hvalue )
{
	if (m_to != NULL)
	{
		return -1;
	}
	
	m_to = new CTo();
	if (m_to == NULL)
	{
		return -1;
	}
	m_to->Parse(hvalue);
	
	return 0;
}

CTo* CSipMessage::GetTo()
{
	return m_to;	
}

int CSipMessage::SetHeader( const char* hname,const char* hvalue )
{
	CHeader* h;
	
	if (hname == NULL)
		return -1;
	
	h = new CHeader();
	if (h == NULL)
		return -1;
	
	h->hname = (char *) mm_malloc(strlen(hname) + 1);
	
	if (h->hname == NULL)
	{
		delete (h);
		return -1;
	}
	zstr_clrncpy(h->hname, hname, strlen(hname));
	
	if (hvalue != NULL)
	{
		/* some headers can be null ("subject:") */
		h->hvalue = (char *) mm_malloc(strlen(hvalue) + 1);
		if (h->hvalue == NULL)
		{
			delete (h);
			return -1;
		}
		zstr_clrncpy(h->hvalue, hvalue, strlen(hvalue));
	}
	else
		h->hvalue = NULL;
	message_property = 2;
	m_headers.Add(h, -1);

	return 0;   				  /* ok */
}

/* adds the route header to message.		 */
/* INPUT : const char *hvalue | value of header.	*/
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetRoute(const char* hvalue)
{
	CRoute* route;
	int i;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
#ifdef __VXWORKS_OS__
	i = osip_route_init2(&route);
#else
	route = new CRoute();
#endif
	if (route == NULL)
		return -1;
	i = route->Parse(hvalue);
	if (i != 0)
	{
		delete (route);
		return -1;
	}
	message_property = 2;
	m_listRoutes.Add(route, -1);
	return 0;
}

/* returns the route header.	*/
/* INPUT : osip_message_t *sip | sip message.   */
/* returns null on error. */
int CSipMessage::GetRoute(int pos,CRoute** dest)
{
	CRoute* route;
	
	*dest = NULL;
	if (this->m_listRoutes.GetSize() <= pos)
		return -1;  				/* does not exist */
	route = (CRoute*) this->m_listRoutes.GetAt(pos);
	*dest = route;
	return pos;
}

/* fills the call_id of message.					*/
/* INPUT : const char *hvalue | value of header.	*/
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetCallId(const char* hvalue)
{
	int i;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
	if (this->m_callid != NULL)
		return -1;
	this->m_callid = new CCallId();
	if (this->m_callid == NULL)
		return -1;
	this->message_property = 2;
	i = this->m_callid->Parse(hvalue);
	if (i != 0)
	{
		delete this->m_callid;
		this->m_callid= NULL;
		return -1;
	}
	return 0;
}

/* adds the content_length header to message.   	*/
/* INPUT : const char *hvalue | value of header.	*/
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetContentLength(const char* hvalue)
{
	int i;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
	if (this->m_contentLength != NULL)
		return -1;
	this->m_contentLength = new CContentLength();
	if (this->m_contentLength == NULL)
		return -1;
	this->message_property = 2;
	i = this->m_contentLength->Parse(hvalue);
	if (i != 0)
	{
		delete this->m_contentLength;
		this->m_contentLength = NULL;
		return -1;
	}
	
	return 0;
}

/* adds the content_type header to message. 			 */
/* INPUT : char *hvalue | value of header.    */
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetContentType(const char* hvalue)
{
	int i;
	
	if (this->m_contentType != NULL)
		return -1;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
	this->m_contentType = new CContentType();
	if (this->m_contentType == NULL)
		return -1;
	this->message_property = 2;
	i = this->m_contentType->Parse(hvalue);
	if (i != 0)
	{
		delete this->m_contentType;
		this->m_contentType = NULL;
	}
	return 0;
}

/* fills the cseq header of message.			   */
/* INPUT :  char *hvalue | value of header.   */
/* OUTPUT: osip_message_t *sip | structure to save results. */
/* returns -1 on error. */
int CSipMessage::SetCseq(const char* hvalue)
{
	int i;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
	if (this->m_seq != NULL)
		return -1;
	this->m_seq = new CSeq();
	if (this->m_seq == NULL)
		return -1;
	this->message_property = 2;
	i = this->m_seq->Parse(hvalue);
	if (i != 0)
	{
		delete this->m_seq;
		this->m_seq = NULL;
		return -1;
	}
	return 0;
}

/* adds the via header to message.  			*/
/* INPUT : const char *hvalue | value of header.	*/
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetVia(const char* hvalue)
{
	CVia* via;
	int i;
	
	if (hvalue == NULL || hvalue[0] == '\0')
		return 0;
	
	via = new CVia();
	if (via == NULL)
		return -1;
	i = via->Parse(hvalue);
	if (i != 0)
	{
		delete (via);
		return -1;
	}
	message_property = 2;
	m_listVias.Add(via, -1);
	return 0;
}

int CSipMessage::GetVia(int pos, CVia** dest)
{
	*dest = NULL;
	if (this == NULL)
		return -1;
	if (this->m_listVias.GetSize() <= pos)
		return -1;
	*dest = (CVia*) this->m_listVias.GetAt(pos);
	
	return pos;
}

int CSipMessage::AddVia(const char* transport, const char* locip)
{
	char tmp[200];
	const char* ip = NULL;
	char firewall_ip[65];
	char firewall_port[10];
	
	if (this == NULL)
		return -1;
	
	if (this->m_callid == NULL)
		return -1;
	
	if (locip == NULL && this->m_callid->host == NULL)
		return -1;
	
	if (locip != NULL)
		ip = locip;
	else if (this->m_callid->host != NULL)
		ip = this->m_callid->host;
	
	firewall_ip[0] = '\0';
	firewall_port[0] = '\0';
	
	if (firewall_port[0] == '\0')
	{
		_snprintf(firewall_port, sizeof(firewall_port), "5067");
	}
	
	_snprintf(tmp, 200, "SIP/2.0/%s %s:%s;rport;branch=z9hG4bK%u","UDP",ip, firewall_port, via_branch_new_random());
	
	this->SetVia(tmp);
	
	return 0;
}

/* adds the accept header to message.   		   */
/* INPUT : char *hvalue | value of header.    */
/* OUTPUT: osip_message_t *sip | structure to save results.  */
/* returns -1 on error. */
int CSipMessage::SetAccept(const char* hvalue)
{
	CAccept* accept;
	int i;
	
	accept = new CAccept();
	if (accept == NULL)
		return -1;
	i = accept->Parse(hvalue);
	if (i != 0)
	{
		delete (accept);
		return -1;
	}
	message_property = 2;
	
	m_listAccept.Add(accept, -1);
	return 0;
}

int CSipMessage::SetAllow( const char* hvalue )
{
	return SetHeader("Allow",hvalue);
}

int CSipMessage::SetSupported( const char* hvalue )
{
	return SetHeader("Supported",hvalue);
}

int CSipMessage::StartlineToStrReq(char** dest)
{
	const char* sip_version;
	char* tmp;
	char* rquri;
	int i;
	
	*dest = NULL;
	if ((this == NULL) || (this->m_pRequestUrl == NULL) || (this->m_strMethod == NULL))
		return -1;
	
	i = this->m_pRequestUrl->ToStr(&rquri);
	if (i != 0)
		return -1;
	
	if (this->m_strVersion == NULL)
		sip_version = osip_protocol_version;
	else
		sip_version = this->m_strVersion;
	
	*dest = (char *) mm_malloc(strlen(this->m_strMethod) +
		strlen(rquri) +
		strlen(sip_version) +
		3);
	tmp = *dest;
	
	tmp = osip_str_append(tmp, this->m_strMethod);
	*tmp = ' ';
	tmp++;
	tmp = osip_str_append(tmp, rquri);
	*tmp = ' ';
	tmp++;
	strcpy(tmp, sip_version);
	
	mm_free(rquri);
	return 0;
}

int CSipMessage::StartlineToStrResp(char** dest)
{
	char* tmp;
	const char* sip_version;
	char status_code[5];
	
	*dest = NULL;
	if ((this == NULL) ||
		(this->m_strReasonPhrase == NULL) ||
		(this->m_iStatusCode < 100) ||
		(this->m_iStatusCode > 699))
		return -1;
	
	if (this->m_strVersion == NULL)
		sip_version = osip_protocol_version;
	else
		sip_version = this->m_strVersion;
	
	sprintf(status_code, "%u", this->m_iStatusCode);
	
	*dest = (char *) mm_malloc(strlen(sip_version) +
		3 +
		strlen(this->m_strReasonPhrase) +
		4);
	tmp = *dest;
	
	tmp = osip_str_append(tmp, sip_version);
	*tmp = ' ';
	tmp++;
	
	tmp = osip_strn_append(tmp, status_code, 3);
	*tmp = ' ';
	tmp++;
	strcpy(tmp, this->m_strReasonPhrase);
	
	return 0;
}

int CSipMessage::StartlineToStr(char** dest)
{
	if (this->m_strMethod != NULL)
		return StartlineToStrReq(dest);
	if (this->m_iStatusCode != 0)
		return StartlineToStrResp(dest);
	
	return -1;  
}

int CSipMessage::ToStr(char** dest, size_t* message_length, int sipfrag)
{
	size_t malloc_size;
	size_t total_length = 0;

	/* Added at SIPit day1 */
	char* start_of_bodies;
	char* content_length_to_modify = NULL;

	char* message;
	char* next;
	char* tmp;
	int pos;
	int i;
	char* boundary = NULL;

	malloc_size = SIP_MESSAGE_MAX_LENGTH;

	*dest = NULL;
	if ((this == NULL))
		return -1;
	{
		if (1 == this->GetMessageProperty())
		{
			/* message is already available in "message" */

			*dest = (char*)mm_malloc(this->message_length + 1);
			if (*dest == NULL)
				return -1;
			memcpy(*dest, this->message, this->message_length);
			(*dest)[this->message_length] = '\0';
			if (message_length != NULL)
				*message_length = this->message_length;
			return 0;
		}
		else
		{
			/* message should be rebuilt: delete the old one if exists. */
			mm_free(this->message);
			this->message = NULL;
		}
	}

	message = (char *) mm_malloc(SIP_MESSAGE_MAX_LENGTH); 	 /* ???? message could be > 4000  */
	if (message == NULL)
		return -1;
	*dest = message;

	/* add the first line of message */
	i = this->StartlineToStr(&tmp);
	if (i == -1)
	{
		if (!sipfrag)
		{
			mm_free(*dest);
			*dest = NULL;
			return -1;
		}

		/* A start-line isn't required for message/sipfrag parts. */
	}
	else
	{
		message = osip_str_append(message, tmp);
		mm_free(tmp);
		message = osip_strn_append(message, CRLF, 2);
	}
	{
// 		pos = 0;
// 		while (table[pos].header_name[0] != '\0')
// 		{
// 			if (table[13].header_list == NULL)
// 				i = strcat_simple_header(dest, &malloc_size, &message,
// 						table[pos].header_data, table[pos].header_name,
// 						table[pos].header_length,
// 						((int (*) (void *, char * *)) table[pos].to_str),
// 						&next);
// 			i = strcat_headers_one_per_line(dest, &malloc_size, &message,
// 					table[pos].header_list, table[pos].header_name,
// 					table[pos].header_length,
// 					((int (*) (void *, char * *)) table[pos].to_str), &next);
// 			if (i != 0)
// 			{
// 				mm_free(*dest);
// 				*dest = NULL;
// 				return -1;
// 			}
// 			message = next;
// 
// 			pos++;
// 		}
		
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listVias, "Via: ", 5,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listRecordRoutes, "Record-Route: ", 14,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listRoutes, "Route: ", 7,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_from, "From: ", 6,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_to, "To: ", 4,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_callid, "Call-ID: ", 9,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_seq, "CSeq: ", 6,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_contacts, "Contact: ", 9,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAuthorizations, "Authorization: ", 15,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listWwwAuthenticates, "WWW-Authenticate: ", 18,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_lisProxyAuthenticates, "Proxy-Authenticate: ", 20,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listProxyAuthorizations, "Proxy-Authorization: ", 21,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_contentType, "Content-Type: ", 14,&next);
		i = strcat_simple_header(dest, &malloc_size, &message,this->m_mime_version, "Mime-Version: ", 14,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAllow, "Allow: ", 7,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listContentEncodings, "Content-Encoding: ", 18,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listCallInfos, "Call-Info: ", 11,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAlertInfos, "Alert-Info: ", 12,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listErrorInfos, "Error-Info: ", 12,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAccept, "Accept: ", 8,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAcceptEncoding, "Accept-Encoding: ", 17,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAcceptLanguages, "Accept-Language: ", 17,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listAuthenticationInfos, "Authentication-Info: ", 21,&next);
		i = strcat_headers_one_per_line(dest, &malloc_size, &message,&this->m_listProxyAuthorizations, "Proxy-Authentication-Info: ", 27,&next);
	}

	pos = 0;
	while (!this->m_headers.IsListEof(pos))
	{
		CHeader* header;
		size_t header_len = 0;

		header = (CHeader *) this->m_headers.GetAt(pos);
		i = header->ToStr(&tmp);
		if (i == -1)
		{
			mm_free(*dest);
			*dest = NULL;
			return -1;
		}

		header_len = strlen(tmp);

		if (_osip_message_realloc(&message, dest, header_len + 3, &malloc_size) <
			0)
		{
			mm_free(tmp);
			*dest = NULL;
			return -1;
		}

		message = osip_str_append(message, tmp);
		mm_free(tmp);
		message = osip_strn_append(message, CRLF, 2);

		pos++;
	}

	/* we have to create the body before adding the contentlength */
	/* add enough lenght for "Content-Length: " */

	if (_osip_message_realloc(&message, dest, 16, &malloc_size) < 0)
		return -1;

	if (sipfrag && this->m_listBodys.IsListEof(0))
	{
		/* end of headers */
		zstr_strncpy(message, CRLF, 2);
		message = message + 2;

		/* same remark as at the beginning of the method */
		this->message_property = 1;
		this->message = zstr_strdup(*dest);
		this->message_length = message - *dest;
		if (message_length != NULL)
			*message_length = message - *dest;

		return 0;   			  /* it's all done */
	}

	zstr_strncpy(message, "Content-Length: ", 16);
	message = message + 16;

	/* SIPit Day1
	   ALWAYS RECALCULATE?
	   if (this->contentlength!=NULL)
	   {
	   i = osip_content_length_to_str(this->contentlength, &tmp);
	   if (i==-1) {
	   mm_free(*dest);
	   *dest = NULL;
	   return -1;
	   }
	   zstr_strncpy(message,tmp,strlen(tmp));
	   mm_free(tmp);
	   }
	   else
	   { */
	if (this->m_listBodys.IsListEof(0))   /* no body */
	{
		message = osip_strn_append(message, "0", 1);
	}
	else
	{
		/* BUG: p130 (rfc2543bis-04)
		   "No SP after last token or quoted string"
		   In fact, if extra spaces exist: the stack can't be used
		   to make user-agent that wants to make authentication...
		   This should be changed...
		 */

		content_length_to_modify = message;
		message = osip_str_append(message, "     ");
	}
	/*  } */

	message = osip_strn_append(message, CRLF, 2);


	/* end of headers */
	message = osip_strn_append(message, CRLF, 2);

	start_of_bodies = message;
	total_length = start_of_bodies - *dest;

	if (this->m_listBodys.IsListEof(0))
	{
		/* same remark as at the beginning of the method */
		this->message_property = 1;
		this->message = zstr_strdup(*dest);
		this->message_length = total_length;
		if (message_length != NULL)
			*message_length = total_length;

		return 0;   			  /* it's all done */
	}

	if (this->m_mime_version != NULL &&
		this->m_contentType &&
		this->m_contentType->type &&
		!zstr_strcasecmp(this->m_contentType->type,"multipart"))
	{
		CUrlParam* ct_param = NULL;

		/* find the boundary */
		i = osip_generic_param_get_byname(&this->m_contentType->gen_params,"boundary", &ct_param);
		if ((i >= 0) && ct_param && ct_param->gvalue)
		{
			size_t len = strlen(ct_param->gvalue);

			if (len > MIME_MAX_BOUNDARY_LEN)
			{
				mm_free(*dest);
				*dest = NULL;
				return -1;
			}

			boundary = (char*)mm_malloc(len + 5);

			zstr_strncpy(boundary, CRLF, 2);
			zstr_strncpy(boundary + 2, "--", 2);

			if (ct_param->gvalue[0] == '"' && ct_param->gvalue[len - 1] == '"')
				zstr_strncpy(boundary + 4, ct_param->gvalue + 1, len - 2);
			else
				zstr_strncpy(boundary + 4, ct_param->gvalue, len);
		}
	}

	pos = 0;
	while (!this->m_listBodys.IsListEof(pos))
	{
		CBody* body;
		size_t body_length;

		body = (CBody *) this->m_listBodys.GetAt(pos);

		if (boundary)
		{
			/* Needs at most 77 bytes,
			   last realloc allocate at least 100 bytes extra */
			message = osip_str_append(message, boundary);
			message = osip_strn_append(message, CRLF, 2);
		}

		i = body->ToStr(&tmp, &body_length);
		if (i != 0)
		{
			mm_free(*dest);
			*dest = NULL;
			if (boundary)
				mm_free(boundary);
			return -1;
		}

		if (malloc_size < message - *dest + 100 + body_length)
		{
			size_t size = message - *dest;
			int offset_of_body;
			int offset_content_length_to_modify = 0;

			offset_of_body = (int) (start_of_bodies - *dest);
			if (content_length_to_modify != NULL)
				offset_content_length_to_modify = (int)
					(content_length_to_modify - *dest);
			malloc_size = message - *dest + body_length + 100;
			*dest = (char*)mm_realloc(*dest, malloc_size);
			if (*dest == NULL)
			{
				mm_free(tmp);  /* fixed 09/Jun/2005 */
				if (boundary)
					mm_free(boundary);
				return -1;
			}
			start_of_bodies = *dest + offset_of_body;
			if (content_length_to_modify != NULL)
			{
				content_length_to_modify = *dest + offset_content_length_to_modify;
			}
			message = *dest + size;
		}

		memcpy(message, tmp, body_length);
		message[body_length] = '\0';
		mm_free(tmp);
		message = message + body_length;

		pos++;
	}

	if (boundary)
	{
		/* Needs at most 79 bytes,
		   last realloc allocate at least 100 bytes extra */
		message = osip_str_append(message, boundary);
		message = osip_strn_append(message, "--", 2);
		message = osip_strn_append(message, CRLF, 2);

		mm_free(boundary);
		boundary = NULL;
	}

	if (content_length_to_modify == NULL)
	{
		mm_free(*dest);
		*dest = NULL;
		return -1;
	}

	/* we NOW have the length of bodies: */
	{
		size_t size = message - start_of_bodies;
		char tmp2[15];

		total_length += size;
		sprintf(tmp2, "%i", size);
		/* do not use zstr_strncpy here! */
		strncpy(content_length_to_modify + 5 - strlen(tmp2), tmp2,
			strlen(tmp2));
	}

	/* same remark as at the beginning of the method */
	this->message_property = 1;
	this->message = (char*)mm_malloc(total_length + 1);
	if (this->message != NULL)
	{
		memcpy(this->message, *dest, total_length);
		this->message[total_length] = '\0';
		this->message_length = total_length;
		if (message_length != NULL)
			*message_length = total_length;
	}

	return 0;
}

int CSipMessage::GetMessageProperty()
{
	return message_property;	
}

/**
 * Fill the body of message.
 * @param sip The structure to store results.
 * @param buf The pointer to the start of body.
 * @param length The length of body;
 */
int CSipMessage::SetBody(const char* buf, size_t length)
{
	CBody* body;
	int i;

	body = new CBody();
	if (body == NULL)
		return -1;
	i = body->Parse(buf, length);
	if (i != 0)
	{
		delete(body);
		return -1;
	}
	message_property = 2;
	m_listBodys.Add(body, -1);
	return 0;
}