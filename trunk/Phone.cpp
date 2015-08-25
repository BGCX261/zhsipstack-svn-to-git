// Phone.cpp: implementation of the CPhone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Phone.h"
#include "ThreadNet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhone::CPhone()
{
	m_pcLocalIp = NULL;
	m_usLocalPort = 5060;

	m_pcProxyIp = NULL;
	m_usProxyPort = 5060;

	m_pcUserName = NULL;
	m_pcPassword = NULL;
	m_pcDisplayName = NULL;
}

CPhone::~CPhone()
{
	delete m_pcLocalIp;
	delete m_pcProxyIp;
	delete m_pcUserName;
	delete m_pcPassword;
	delete m_pcDisplayName;

	m_pcLocalIp = NULL;
	m_usLocalPort = 5060;
	
	m_pcProxyIp = NULL;
	m_pcUserName = NULL;
	m_pcPassword = NULL;
	m_pcDisplayName = NULL;
}

void CPhone::SetListenAddr(int iTransFamily,int iProtocol, unsigned short iLocalPort,const char* pcLocalAddr,int iSecure)
{
	m_pcLocalIp = zstr_strdup(pcLocalAddr);
	m_usLocalPort = iLocalPort;
	m_iTransFamily = iTransFamily;
	m_iProtocol = iProtocol;

	CThreadNet *pThreadNet = CThreadNet::getInstance();
	pThreadNet->SetLocalSocketInfo(m_iTransFamily,iProtocol,iLocalPort,m_pcLocalIp);

}

void CPhone::SetPhoneIdentity( const char *pcUserName,const char *pcPassword,const char *pcDisplayName )
{
	m_pcUserName = zstr_strdup(pcUserName);
	m_pcPassword = zstr_strdup(pcPassword);
	m_pcDisplayName = zstr_strdup(pcDisplayName);
}

void CPhone::SetProxyServer( char *pcProxy ,unsigned short usProxyPort)
{
	m_pcProxyIp = pcProxy;
	m_usProxyPort = usProxyPort;

	CThreadNet *pThreadNet = CThreadNet::getInstance();
	pThreadNet->ConnectServer(m_pcProxyIp,m_usProxyPort);
	pThreadNet->CreateThread();

	CSipTransactionManager *psipManager = CSipTransactionManager::getInstance();
	psipManager->CreateThread();
}

int CPhone::Register()
{
	CSipMessage *pRegister;
	generating_request_out_of_dialog(&pRegister,"REGISTER",GetLocalUrl(),"UDP",GetLocalUrl(),GetProxyUrl());
	
	CSipTransactionManager *psipManager = CSipTransactionManager::getInstance();
	
	psipManager->OutComming(pRegister);

	

	return 0;
}

int CPhone::generating_request_out_of_dialog(CSipMessage** dest,
	const char* method, const char* to, const char* transport,
	const char* from, const char* proxy)
{
	/* Section 8.1:
	   A valid request contains at a minimum "To, From, Call-iD, Cseq,
	   Max-Forwards and Via
	 */
	int i;
	CSipMessage* request;
	char locip[65];
	int doing_register;
	char* register_callid_number = NULL;

	*dest = NULL;

	/*guess the local ip since req uri is known */
	memset(locip, '\0', sizeof(locip));
	_snprintf(locip,64,m_pcLocalIp);

	request = new CSipMessage();
	if (request == NULL)
		return -1;

	/* prepare the request-line */
	request->SetMethod(zstr_strdup(method));
	request->SetVersion(zstr_strdup("SIP/2.0"));
	request->SetStatusCode(0);
	request->SetReasonPhrase(NULL);

	doing_register = (0 == strcmp("REGISTER", method));

	if (doing_register)
	{
		request->m_pRequestUrl = new CUrl();
		i = request->m_pRequestUrl->Parse(proxy);
		if (i != 0)
		{
			goto brood_error_1;
		}
		request->SetTo(to);
	}
	else
	{
		/* in any cases except REGISTER: */
		i = request->SetTo(to);
		if (i != 0)
		{
			goto brood_error_1;
		}

		/* REMOVE ALL URL PARAMETERS from to->url headers and add them as headers */
		if (request->m_to != NULL && request->m_to->url != NULL)
		{
			CUrl* url = request->m_to->url;
			while (url->url_headers.GetSize() > 0)
			{
				CUrlHeader* u_header;
				u_header = (CUrlHeader*) url->url_headers.GetAt(0);
				if (u_header == NULL)
					break;

				if (zstr_strcasecmp(u_header->gname, "from") == 0)
				{
				}
				else if (zstr_strcasecmp(u_header->gname, "to") == 0)
				{
				}
				else if (zstr_strcasecmp(u_header->gname, "call-id") == 0)
				{
				}
				else if (zstr_strcasecmp(u_header->gname, "cseq") == 0)
				{
				}
				else if (zstr_strcasecmp(u_header->gname, "via") == 0)
				{
				}
				else if (zstr_strcasecmp(u_header->gname, "contact") == 0)
				{
				}
				else
				{
					request->SetHeader(u_header->gname,u_header->gvalue);
				}
				url->url_headers.RemoveAt(0);
				delete (u_header);
			}
		}

		if (proxy != NULL && proxy[0] != 0)
		{
			/* equal to a pre-existing route set */
			/* if the pre-existing route set contains a "lr" (compliance
					   with bis-08) then the req_uri should contains the remote target
					   URI */
			CUrlParam* lr_param;
			CRoute* o_proxy;

			o_proxy = new CRoute();
			i = o_proxy->Parse(proxy);
			if (i != 0)
			{
				delete (o_proxy);
				goto brood_error_1;
			}

			osip_uri_uparam_get_byname(o_proxy->url, "lr", &lr_param);
			if (lr_param != NULL) /* to is the remote target URI in this case! */
			{
				request->m_to->url, &(request->m_pRequestUrl);
				/* "[request] MUST includes a Route header field containing
						   the route set values in order." */
				request->m_listRoutes.Add(o_proxy, 0);
			}
			else
						/* if the first URI of route set does not contain "lr", the req_uri
						   is set to the first uri of route set */
			{
				request->m_pRequestUrl = o_proxy->url;
				o_proxy->url = NULL;
				delete (o_proxy);
				/* add the route set */
				/* "The UAC MUST add a route header field containing
						   the remainder of the route set values in order.
						   The UAC MUST then place the remote target URI into
						   the route header field as the last value
						 */
				request->SetRoute(to);
			}
		}
		else					/* No route set (outbound proxy) is used */
		{
			/* The UAC must put the remote target URI (to field) in the req_uri */
			i = request->m_to->url->Clone(&(request->m_pRequestUrl));
			if (i != 0)
				goto brood_error_1;
		}
	}

	if (locip[0] == '\0')
		goto brood_error_1;

	/* set To and From */
	request->SetFrom(from);
	if (request->m_from == NULL)
		goto brood_error_1;

	/* add a tag */
	osip_generic_param_add((&(request->m_from)->gen_params), zstr_strdup("tag"),osip_from_tag_new_random());

	/* set the cseq and call_id header */
	{
		CCallId* callid;
		CSeq* cseq;
		char* num;
		char* cidrand;

		/* call-id is always the same for REGISTRATIONS */
		callid = new CCallId();
		if (callid == NULL)
			goto brood_error_1;
		cidrand = osip_call_id_new_random();
		callid->SetNumber(cidrand);
		if (doing_register)
			register_callid_number = cidrand;

		request->m_callid = callid;

		cseq = new CSeq();
		if (cseq == NULL)
			goto brood_error_1;
		num = zstr_strdup(doing_register ? "1" : "1");
		cseq->SetNumber(zstr_strdup(num));
		cseq->SetMethod(zstr_strdup(method));
		request->m_seq = cseq;
	}

	i = request->AddVia(transport, locip);
	if (i != 0)
		goto brood_error_1;

	/* always add the Max-Forward header */
	request->SetHeader((const char *)"Max-Forwards","70");

	if (0 == strcmp("REGISTER", method))
	{
	}
	else if (0 == strcmp("INFO", method))
	{
	}
	else if (0 == strcmp("OPTIONS", method))
	{
		request->SetAccept("application/sdp");
	}

	request->SetHeader((const char *)"User-Agent","zhSipStack");

	request->SetHeader((const char *)"Contact","sip:7777@192.168.1.100:5067");
	/*  else if ... */
	*dest = request;
	return 0;

	brood_error_1 : 
	delete(request);
	*dest = NULL;

	return -1;
}

int CPhone::Invite(const char *to)
{
	if (to == NULL)
	{
		return -1;
	}

	int i;
	CTo* _to = NULL;
	
	CSipMessage *invite;
	invite = NULL;
	
	if (to != NULL && *to == '\0')
		return -1;
	
	_to = new CTo();
	if (_to == NULL)
		return -1;
	
	i = _to->Parse(to);
	if (i != 0)
	{
		delete (_to);
		return -1;
	}
	
	char *from = GetLocalUrl();
	i = generating_request_out_of_dialog(&invite, "INVITE", to,"5060", from, NULL);
	delete(_to);
	if (i != 0)
		return -1;
	
	invite->SetHeader((const char *)"Subject","huasipstack");
	
	/* after this delay, we should send a CANCEL */
	invite->SetHeader((const char *)"Expires","120");
	
	CSdpMessage sdp;
	
	sdp.sdp_message_v_version_set(zstr_strdup("0"));
	sdp.sdp_message_o_origin_set(zstr_strdup("Lion"),zstr_strdup("22022"),zstr_strdup("22022")
		,zstr_strdup("IN"),zstr_strdup("IP4"),zstr_strdup("192.168.1.100"));
	sdp.sdp_message_s_name_set(zstr_strdup("Sdp Of Lion"));
	
	sdp.sdp_message_m_media_add(zstr_strdup("audio"),zstr_strdup("19000"),zstr_strdup("2"),zstr_strdup("RTP/AVP"));
	sdp.sdp_message_m_payload_add(0,zstr_strdup("100"));
	
	sdp.sdp_message_c_connection_add(0,zstr_strdup("IN"),zstr_strdup("IP4"),zstr_strdup("192.168.1.100"),NULL,NULL);
	sdp.sdp_message_a_attribute_add(0,zstr_strdup("fmtp"),zstr_strdup("101 0-105"));
	sdp.sdp_message_a_attribute_add(0,zstr_strdup("rtpmap"),zstr_strdup("100 speex/16000"));	
	sdp.sdp_message_a_attribute_add(0,zstr_strdup("sendrecv"),NULL);	
	
	char *pcSdp;
	sdp.ToStr(&pcSdp);
	
	invite->SetBody(pcSdp,strlen(pcSdp));
	invite->SetContentType("application/sdp");
	invite->SetSupported("eventlist");
	invite->SetAllow("INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO");

	delete pcSdp;
	
	char *message;
	unsigned int iLenMessage = 0;
	invite->ToStr(&message,&iLenMessage);
	{
		FILE *pFile = fopen("e:\\myinvite.txt","wb");
		if (pFile != NULL)
		{
			fwrite(message,1,strlen(message),pFile);
			fclose(pFile);
		}
	}

	CSipTransactionManager *psipManager = CSipTransactionManager::getInstance();
	psipManager->OutComming(invite);

	return 0;
}

char* CPhone::GetLocalUrl()
{
	char ac[STR_LEN_256];	
	memset(ac,0,STR_LEN_256);

	_snprintf(ac,STR_LEN_256-1,"sip:%s@%s:%d",m_pcUserName,m_pcLocalIp,m_usLocalPort);

	return zstr_strdup(ac);
}

char* CPhone::GetProxyUrl()
{
	char ac[STR_LEN_256];	
	memset(ac,0,STR_LEN_256);
	
	_snprintf(ac,STR_LEN_256-1,"sip:%s:%d",m_pcProxyIp,m_usProxyPort);
	
	return zstr_strdup(ac);	
}
