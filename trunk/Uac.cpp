// Uac.cpp: implementation of the CUac class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Uac.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUac::CUac()
{

}

CUac::~CUac()
{

}

/* prepare a minimal request (outside of a dialog) with required headers */
/* 
   method is the type of request. ("INVITE", "REGISTER"...)
   to is the remote target URI
   transport is either "TCP" or "UDP" (by now, only UDP is implemented!)
*/
int generating_request_out_of_dialog(CSipMessage** dest,
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
	_snprintf(locip,64,"127.0.0.1");

	request = new CSipMessage();
	if (request == NULL)
		return -1;

	/* prepare the request-line */
	request->SetMethod(zstr_strdup(method));
	request->SetVersion(zstr_strdup("SIP/2.0"));
	request->SetStatusCode(0);
	request->SetReasonPhrase(NULL);

	doing_register = 0 == strcmp("REGISTER", method);

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

#if 0
	osip_call_id_set_host (callid, zstr_strdup (locip));
#endif
		request->m_callid = callid;

		cseq = new CSeq();
		if (cseq == NULL)
			goto brood_error_1;
		num = zstr_strdup(doing_register ? "1" : "20");
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

	/*  else if ... */
	*dest = request;
	return 0;

	brood_error_1 : delete(request);
	*dest = NULL;

	return -1;
}

int CUac::Invite( const char *to,const char* from, const char* route, const char* subject )
{
	int i;
	CTo* _to = NULL;
	
	CSipMessage *invite;
	invite = NULL;
	
	if (to != NULL && *to == '\0')
		return -1;
	if (route != NULL && *route == '\0')
		route = NULL;
	if (subject != NULL && *subject == '\0')
		subject = NULL;
	
	_to = new CTo();
	if (_to == NULL)
		return -1;
	
	i = _to->Parse(to);
	if (i != 0)
	{
		delete (_to);
		return -1;
	}
	
	i = generating_request_out_of_dialog(&invite, "INVITE", to,"5060", from, route);
	delete(_to);
	if (i != 0)
		return -1;
	
	if (subject != NULL)
	{
		invite->SetHeader((const char *)"Subject",subject);

	}
	
	/* after this delay, we should send a CANCEL */
	invite->SetHeader((const char *)"Expires","120");
	
	{
		FILE *pFile = fopen("e:\\sdp.txt","rb");
		if (pFile != NULL)
		{
			char buff[2048];
			memset(buff,0,2048);
			
			int iLen = 0;
			iLen = fread(buff,1,2048,pFile);
			if (iLen <= 0)
			{
				return -1;
			}
			
			// 		CThreadNet *pNet = CThreadNet::getInstance();
			// 		pNet->ParseMessage(buff,iLen);
			
			CSdpMessage sdp;
//			
//			sdp.Parse(buff);
			
			sdp.sdp_message_v_version_set(zstr_strdup("0"));
			sdp.sdp_message_o_origin_set(zstr_strdup("Lion"),zstr_strdup("22022"),zstr_strdup("22022")
				,zstr_strdup("IN"),zstr_strdup("IP4"),zstr_strdup("192.168.1.8"));
			sdp.sdp_message_s_name_set(zstr_strdup("Sdp Of Lion"));
			
			sdp.sdp_message_m_media_add(zstr_strdup("video"),zstr_strdup("19000"),zstr_strdup("2"),zstr_strdup("RTP/AVP"));
			sdp.sdp_message_m_payload_add(0,zstr_strdup("34"));

			sdp.sdp_message_c_connection_add(0,zstr_strdup("IN"),zstr_strdup("IP4"),zstr_strdup("192.168.1.8"),NULL,NULL);
			sdp.sdp_message_a_attribute_add(0,zstr_strdup("rtpmap"),zstr_strdup("34 H263/90000"));	
			sdp.sdp_message_a_attribute_add(0,zstr_strdup("fmtp"),zstr_strdup("34 CIF=1 QCIF=1 MaxBR=7680"));	

			char *pcSdp;
			sdp.ToStr(&pcSdp);

			invite->SetBody(pcSdp,strlen(pcSdp));
			
			delete pcSdp;
		}
	}


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

	return 0;	
}
