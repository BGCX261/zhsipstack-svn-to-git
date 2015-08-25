// Via.cpp: implementation of the CVia class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Via.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVia::CVia()
{
	Init();
}

CVia::~CVia()
{
	Free();
}

int CVia::Parse( const char *hvalue )
{
	const char* version;
	const char* protocol;
	const char* host;
	const char* ipv6host;
	const char* port;
	const char* via_params;
	const char* comment;

	version = strchr(hvalue, '/');
	if (version == NULL)
		return -1;

	protocol = strchr(version + 1, '/');
	if (protocol == NULL)
		return -1;

	/* set the version */
	if (protocol - version < 2)
		return -1;
	m_pcVersion = (char *) MallocAndZero(protocol - version);
	if (m_pcVersion == NULL)
		return -1;
	zstr_clrncpy(m_pcVersion, version + 1, protocol - version - 1);

	/* Here: we avoid matching an additionnal space */
	host = strchr(protocol + 1, ' ');
	if (host == NULL)
		return -1;  				/* fixed in 0.8.4 */
	if (host == protocol + 1)     /* there are extra SPACE characters */
	{
		while (0 == strncmp(host, " ", 1))
		{
			host++;
			if (strlen(host) == 1)
				return -1;  		/* via is malformed */
		}
		/* here, we match the real space located after the protocol name */
		host = strchr(host + 1, ' ');
		if (host == NULL)
			return -1;  			/* fixed in 0.8.4 */
	}

	/* set the protocol */
	if (host - protocol < 2)
		return -1;
	m_pcProtocol = (char *) MallocAndZero(host - protocol);
	if (m_pcProtocol == NULL)
		return -1;
	zstr_clrncpy(m_pcProtocol, protocol + 1, host - protocol - 1);

	/* comments in Via are not allowed any more in the latest draft (09) */
	comment = strchr(host, '(');

	if (comment != NULL)
	{
		char* end_comment;

		end_comment = strchr(host, ')');
		if (end_comment == NULL)
			return -1;  			/* if '(' exist ')' MUST exist */
		if (end_comment - comment < 2)
			return -1;
		m_pcComment = (char *) MallocAndZero(end_comment - comment);
		if (m_pcComment == NULL)
			return -1;
		zstr_clrncpy(m_pcComment, comment + 1, end_comment - comment - 1);
		comment--;
	}
	else
		comment = host + strlen(host);

	via_params = strchr(host, ';');

	if ((via_params != NULL) && (via_params < comment))
		/* via params exist */
	{
		char* tmp;

		if (comment - via_params + 1 < 2)
			return -1;
		tmp = (char *) MallocAndZero(comment - via_params + 1);
		if (tmp == NULL)
			return -1;
		zstr_strncpy(tmp, via_params, comment - via_params);
		if (GenericParamParseAll(&(m_listVia_params), tmp))
		{
			free(tmp);
			return -1;
		}

		free(tmp);
	}

	if (via_params == NULL)
		via_params = comment;

	/* add ipv6 support (0.8.4) */
	/* Via: SIP/2.0/UDP [mlke::zeezf:ezfz:zef:zefzf]:port;.... */
	ipv6host = strchr(host, '[');
	if (ipv6host != NULL && ipv6host < via_params)
	{
		port = strchr(ipv6host, ']');
		if (port == NULL || port > via_params)
			return -1;

		if (port - ipv6host < 2)
			return -1;
		m_pcHost = (char *) MallocAndZero(port - ipv6host);
		if (m_pcHost == NULL)
			return -1;
		zstr_clrncpy(m_pcHost, ipv6host + 1, port - ipv6host - 1);

		port = strchr(port, ':');
	}
	else
	{
		port = strchr(host, ':');
		ipv6host = NULL;
	}

	if ((port != NULL) && (port < via_params))
	{
		if (via_params - port < 2)
			return -1;
		m_pcPort = (char *) MallocAndZero(via_params - port);
		if (m_pcPort == NULL)
			return -1;
		zstr_clrncpy(m_pcPort, port + 1, via_params - port - 1);
	}
	else
		port = via_params;

	/* host is already set in the case of ipv6 */
	if (ipv6host != NULL)
		return 0;

	if (port - host < 2)
		return -1;
	m_pcHost = (char *) MallocAndZero(port - host);
	if (m_pcHost == NULL)
		return -1;
	zstr_clrncpy(m_pcHost, host + 1, port - host - 1);

	return 0;		
}

/* returns the via header as a string. */
/* INPUT : osip_via_t via* | via header.	*/
/* returns null on error. */
int CVia::ToStr(char** dest)
{
	char* buf;
	size_t len;
	size_t plen;
	char* tmp;
	
	*dest = NULL;
	if ((this == NULL) ||
		(this->m_pcHost == NULL) ||
		(this->m_pcVersion == NULL) ||
		(this->m_pcProtocol == NULL))
		return -1;
	
	len = strlen(this->m_pcVersion) + 1 + strlen(this->m_pcProtocol) + 1 + 3 + 2; /* sip/xxx/xxx */
	len = len + strlen(this->m_pcHost) + 3 + 1;
	if (this->m_pcPort != NULL)
		len = len + strlen(this->m_pcPort) + 2;
	
	buf = (char *) mm_malloc(len);
	if (buf == NULL)
		return -1;
	
	if (strchr(this->m_pcHost, ':') != NULL)
	{
		if (this->m_pcPort == NULL)
			sprintf(buf, "SIP/%s/%s [%s]", this->m_pcVersion, this->m_pcProtocol,
			this->m_pcHost);
		else
			sprintf(buf, "SIP/%s/%s [%s]:%s", this->m_pcVersion, this->m_pcProtocol,
			this->m_pcHost, this->m_pcPort);
	}
	else
	{
		if (this->m_pcPort == NULL)
			sprintf(buf, "SIP/%s/%s %s", this->m_pcVersion, this->m_pcProtocol,
			this->m_pcHost);
		else
			sprintf(buf, "SIP/%s/%s %s:%s", this->m_pcVersion, this->m_pcProtocol,
			this->m_pcHost, this->m_pcPort);
	}
	{
		int pos = 0;
		CUrlParam* u_param;
		
		while (!this->m_listVia_params.IsListEof(pos))
		{
			u_param = (CUrlParam *) this->m_listVia_params.GetAt(pos);
			
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
	
	if (this->m_pcComment != NULL)
	{
		len = len + strlen(this->m_pcComment) + 4;
		buf = (char *) mm_realloc(buf, len);
		tmp = buf;
		tmp = tmp + strlen(tmp);
		sprintf(tmp, " (%s)", this->m_pcComment);
	}
	*dest = buf;
	return 0;
}

int CVia::Init()
{
	m_pcVersion = NULL;
	m_pcProtocol = NULL;
	m_pcPort = NULL;
	m_pcHost = NULL;
	m_pcComment = NULL;

	return 0;
}

void CVia::Free()
{
	mm_free(m_pcVersion);
	mm_free(m_pcProtocol);
	mm_free(m_pcHost);
	mm_free(m_pcPort);
	mm_free(m_pcComment);
	m_pcVersion = NULL;
	m_pcProtocol = NULL;
	m_pcPort = NULL;
	m_pcHost = NULL;
	m_pcComment = NULL;
}

void CVia::SetVersion(char* version)
{
	this->m_pcVersion = version;
}

char* CVia::GetVersion()
{
	if (this == NULL)
		return NULL;
	return this->m_pcVersion;
}

void CVia::SetProtocol(char* protocol)
{
	this->m_pcProtocol = protocol;
}

char* CVia::GetProtocol()
{
	if (this == NULL)
		return NULL;
	return this->m_pcProtocol;
}

void CVia::SetHost(char* host)
{
	this->m_pcHost = host;
}

char* CVia::GetHost()
{
	if (this == NULL)
		return NULL;
	return this->m_pcHost;
}

void CVia::SetPort(char* port)
{
	this->m_pcPort = port;
}

char* CVia::GetPort()
{
	if (this == NULL)
		return NULL;
	return this->m_pcPort;
}

void CVia::SetComment(char* comment)
{
	this->m_pcComment = comment;
}

char* CVia::GetComment()
{
	if (this == NULL)
		return NULL;
	return this->m_pcComment;
}

int CVia::Clone(CVia** dest)
{
	int i;
	CVia* vi;
	
	*dest = NULL;

	if (m_pcVersion == NULL)
		return -1;
	if (m_pcProtocol == NULL)
		return -1;
	if (m_pcHost == NULL)
		return -1;

	vi = new CVia;
	if (vi == NULL)
	{
		return -1;
	}

	vi->m_pcVersion = zstr_strdup(this->m_pcVersion);
	vi->m_pcProtocol = zstr_strdup(this->m_pcProtocol);
	vi->m_pcHost = zstr_strdup(this->m_pcHost);
	if (this->m_pcPort != NULL)
		vi->m_pcPort = zstr_strdup(this->m_pcPort);
	if (this->m_pcComment != NULL)
		vi->m_pcComment = zstr_strdup(this->m_pcComment);
	
	i = m_listVia_params.Clone(&vi->m_listVia_params,(int * (*) (void *, void *))&osip_generic_param_clone);
	if (i != 0)
	{
		delete vi;
		vi = NULL;
		return -1;
	}
	*dest = vi;
	return 0;
}

int osip_via_clone(CVia* via, CVia** dest)
{
	int i;
	CVia* vi;
	
	*dest = NULL;
	if (via == NULL)
		return -1;
	if (via->m_pcVersion == NULL)
		return -1;
	if (via->m_pcProtocol == NULL)
		return -1;
	if (via->m_pcHost == NULL)
		return -1;
	
	vi = new CVia();
	if (vi == NULL)
		return -1;
	vi->m_pcVersion = zstr_strdup(via->m_pcVersion);
	vi->m_pcProtocol = zstr_strdup(via->m_pcProtocol);
	vi->m_pcHost = zstr_strdup(via->m_pcHost);
	if (via->m_pcPort != NULL)
		vi->m_pcPort = zstr_strdup(via->m_pcPort);
	if (via->m_pcComment != NULL)
		vi->m_pcComment = zstr_strdup(via->m_pcComment);

	i = via->m_listVia_params.Clone(&vi->m_listVia_params,(int * (*) (void *, void *)) & osip_generic_param_clone);
	if (i != 0)
	{
		delete (vi);
		return -1;
	}
	*dest = vi;
	return 0;
}