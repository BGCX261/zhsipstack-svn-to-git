// SipDialog.cpp: implementation of the CSipDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipDialog::CSipDialog()
{
	call_id = NULL;
	local_tag = NULL;
	remote_tag = NULL;

	remote_uri = NULL;
	local_uri = NULL;
	remote_contact_uri = NULL;
	your_instance = NULL;
}

CSipDialog::~CSipDialog()
{
	Free();
}

void CSipDialog::Free()
{
	delete remote_contact_uri;
	delete local_uri;
	delete remote_uri;
	delete remote_tag;
	delete local_tag;
	delete call_id;
}

void CSipDialog::SetInstance( void *pInstance )
{
	your_instance = pInstance;	
}

void * CSipDialog::GetInstance()
{
	return your_instance;	
}

int CSipDialog::InitAsUac( CSipMessage* response )
{
	int i;
	
	i = Init(NULL, response, response->m_from,
		response->m_to, response);
	
	if (i != 0)
	{
		return -1;
	}
	
	type = CALLER;
	remote_cseq = -1;
	
	return 0;
}

int CSipDialog::Init( CSipMessage* invite,CSipMessage* response, CFrom* local, CTo* remote,CSipMessage* remote_msg )
{
	int i;
	int pos;
	CUrlParam* tag;

	if (MSG_IS_STATUS_2XX(response))
		state = DIALOG_CONFIRMED;
	else						  /* 1XX */
		state = DIALOG_EARLY;

	i = response->m_callid->ToStr(&call_id);
	if (i != 0)
		goto diau_error_0;

	i = local->GetTag(&tag);
	if (i != 0)
		goto diau_error_1;
	local_tag = zstr_strdup(tag->gvalue);

	i = remote->GetTag(&tag);
	if (i == 0)
		remote_tag = zstr_strdup(tag->gvalue);

	pos = 0;
	while (!response->m_listRecordRoutes.IsListEof(pos))
	{
		CRecordRoute* rr;
		CRecordRoute* rr2;

		rr = (CRecordRoute *) response->m_listRecordRoutes.GetAt(pos);
		i = osip_record_route_clone(rr, &rr2);
		if (i != 0)
			goto diau_error_2;
		if (invite == NULL)
			route_set.Add(rr2,-1);
		else
			route_set.Add(rr2, 0);

		pos++;
	}

	/* local_cseq is set to response->cseq->number for better
	   handling of bad UA */
	local_cseq = zstr_atoi(response->m_seq->number);

	i = remote->Clone(&remote_uri);
	if (i != 0)
		goto diau_error_3;

	i = local->Clone(&local_uri);
	if (i != 0)
		goto diau_error_4;
	{
		CContact* contact;

		if (!remote_msg->m_contacts.IsListEof(0))
		{
			contact = (CContact*)remote_msg->m_contacts.GetAt(0);
			i = contact->Clone(&remote_contact_uri);
			if (i != 0)
				goto diau_error_5;
		}
		else
		{
			remote_contact_uri = NULL;
		}
	}
	secure = -1; 	  /* non secure */

	return 0;

	diau_error_5 : delete local_uri;
	diau_error_4 : delete remote_uri;
	diau_error_3:
	diau_error_2:
		delete remote_tag;
		delete local_tag;
	diau_error_1 : 
		delete call_id;
	diau_error_0:
		delete this;
	return -1;	
}

int CSipDialog::InitAsUacWithRemoteRequest( CSipMessage* next_request, int local_cseq )
{
	int i;
	
	i = Init(next_request, next_request,next_request->m_to, next_request->m_from, next_request);
	
	if (i != 0)
	{
		return -1;
	}
	
	type = CALLER;
	state = DIALOG_CONFIRMED;
	
	local_cseq = local_cseq;   /* -1 osip_atoi (xxx->cseq->number); */
	remote_cseq = zstr_atoi(next_request->m_seq->number);
	
	return 0;	
}

int CSipDialog::InitAsUas( CSipMessage* invite, CSipMessage* response )
{
	int i;
	
	i = Init(invite, response, response->m_to,response->m_from, invite);
	
	if (i != 0)
	{
		return -1;
	}
	
	type = CALLEE;
	remote_cseq = zstr_atoi(response->m_seq->number);
	
	return 0;	
}

void CSipDialog::SetState( eTransactionState type )
{
	state = type;	
}

int CSipDialog::UpdateRouteSetAsUas( CSipMessage* invite )
{
	CContact* contact;
	int i;
	
	if (invite == NULL)
		return -1;
	
	if (invite->m_contacts.IsListEof(0))
	{

	}
	else
	{
		if (remote_contact_uri != NULL)
		{
			delete remote_contact_uri;
		}
		remote_contact_uri = NULL;
		contact = (CContact*)invite->m_contacts.GetAt(0);
		i = contact->Clone(&remote_contact_uri);
		if (i != 0)
			return -1;
	}
	return 0;	
}

int CSipDialog::UpdateCseqAsUas( CSipMessage* request )
{
	if (request == NULL || request->m_seq == NULL || request->m_seq->number == NULL)
		return -1;
	
	remote_cseq = zstr_atoi(request->m_seq->number);
	return 0;	
}

int CSipDialog::MatchAsUac( CSipMessage* answer )
{
	CUrlParam* tag_param_local;
	CUrlParam* tag_param_remote;
	char* tmp;
	int i;

	if (answer == NULL ||
		answer->m_callid == NULL ||
		answer->m_from == NULL ||
		answer->m_to == NULL)
		return -1;

	/*
	   When starting a new transaction and when receiving several answers,
	   you must be prepared to receive several answers from different sources.
	   (because of forking).
	   Because some UAs are not compliant (a to tag is missing!), this method
	   may match the wrong dialog when a dialog has been created with an empty
	   tag in the To header.
	   Personnaly, I would recommend to discard 1xx>=101 answers without To tags!
	   Just my own feelings.
	 */
	answer->m_callid->ToStr(&tmp);
	if (0 != strcmp(call_id, tmp))
	{
		delete (tmp);
		return -1;
	}
	delete (tmp);

	/* for INCOMING RESPONSE:
	   To: remote_uri;remote_tag
	   From: local_uri;local_tag		   <- LOCAL TAG ALWAYS EXIST
	 */
	i = answer->m_from->GetTag(&tag_param_local);
	if (i != 0)
		return -1;
	if (local_tag == NULL)
		/* NOT POSSIBLE BECAUSE I MANAGE REMOTE_TAG AND I ALWAYS ADD IT! */
		return -1;
	if (0 != strcmp(tag_param_local->gvalue, local_tag))
		return -1;

	i = answer->m_to->GetTag(&tag_param_remote);
	if (i != 0 && remote_tag != NULL)  	  /* no tag in response but tag in dialog */
		return -1;  				/* impossible... */
	if (i != 0 && remote_tag == NULL)  	  /* no tag in response AND no tag in dialog */
	{
		if (0 == local_uri->Compare(answer->m_from) &&
			0 == remote_uri->Compare(answer->m_to))
			return 0;
		return -1;
	}

	if (remote_tag == NULL)  /* tag in response BUT no tag in dialog */
	{
		if (0 == local_uri->Compare(answer->m_from) &&
			0 == remote_uri->Compare(answer->m_to))
			return 0;
		return -1;
	}

	/* we don't have to compare
	   remote_uri with from
	   && local_uri with to.	----> we have both tag recognized, it's enough..
	 */
	if (0 == strcmp(tag_param_remote->gvalue, remote_tag))
		return 0;
	return -1;	
}

int CSipDialog::UpdateTagAsUac( CSipMessage* response )
{
	CUrlParam* tag;
	int i;
	
	if (response == NULL || response->m_to == NULL)
		return -1;
	
	if (remote_tag != NULL)
	{
		return -1;
	}
	
	i = response->m_to->GetTag(&tag);
	if (i != 0 || tag == NULL || tag->gvalue == NULL)
	{
		remote_tag = NULL;
	}
	else
		remote_tag = zstr_strdup(tag->gvalue);
	return 0;	
}

int CSipDialog::UpdateRouteSetAsUac( CSipMessage* response )
{
	/* only the remote target URI is updated here... */
	CContact* contact;
	int i;

	if (response == NULL)
		return -1;

	if (response->m_contacts.IsListEof(0))
	{
		/* no contact header in response? */
	}
	else
	{
		/* I personally think it's a bad idea to keep the old
		   value in case the new one is broken... */
		if (remote_contact_uri != NULL)
		{
			delete remote_contact_uri;
		}
		remote_contact_uri = NULL;
		contact = (CContact*)response->m_contacts.GetAt(0);
		i = contact->Clone(&remote_contact_uri);
		if (i != 0)
			return -1;
	}

	if (state == DIALOG_EARLY &&
		route_set.GetSize() > 0)
	{
	}

	if (state == DIALOG_EARLY &&
		route_set.GetSize() == 0)
	{
		/* update the route set */
		int pos = 0;

		while (!response->m_listRecordRoutes.IsListEof(pos))
		{
			CRecordRoute* rr;
			CRecordRoute* rr2;

			rr = (CRecordRoute *)response->m_listRecordRoutes.GetAt(pos);
			i = rr->Clone(&rr2);
			if (i != 0)
				return -1;
			route_set.Add(rr2, 0);
			pos++;
		}
	}

	if (MSG_IS_STATUS_2XX(response))
		state = DIALOG_CONFIRMED;
	return 0;	
}

int CSipDialog::MatchAsUas( CSipMessage* request )
{
	CUrlParam* tag_param_remote;
	int i;
	char* tmp;

	if (request == NULL ||
		request->m_callid == NULL ||
		request->m_from == NULL ||
		request->m_to == NULL)
		return -1;

	request->m_callid->ToStr(&tmp);
	if (0 != strcmp(call_id, tmp))
	{
		delete (tmp);
		return -1;
	}
	delete (tmp);

	/* for INCOMING REQUEST:
	   To: local_uri;local_tag  		 <- LOCAL TAG ALWAYS EXIST
	   From: remote_uri;remote_tag
	 */

	if (local_tag == NULL)
		/* NOT POSSIBLE BECAUSE I MANAGE REMOTE_TAG AND I ALWAYS ADD IT! */
		return -1;

	i = request->m_from->GetTag(&tag_param_remote);
	if (i != 0 && remote_tag != NULL)  	  /* no tag in request but tag in dialog */
		return -1;  				/* impossible... */
	if (i != 0 && remote_tag == NULL)  	  /* no tag in request AND no tag in dialog */
	{
		if (0 == remote_uri->Compare(request->m_from) &&
			0 == local_uri->Compare(request->m_to))
			return 0;
		return -1;
	}

	if (remote_tag == NULL)  /* tag in response BUT no tag in dialog */
	{
		if (0 == remote_uri->Compare(request->m_from) &&
			0 == local_uri->Compare(request->m_to))
			return 0;
		return -1;
	}
	/* we don't have to compare
	   remote_uri with from
	   && local_uri with to.	----> we have both tag recognized, it's enough..
	 */
	if (0 == strcmp(tag_param_remote->gvalue, remote_tag))
		return 0;

	return -1;	
}

int CSipDialog::IsOriginator()
{
	
	return 0;
}

int CSipDialog::IsCallee()
{
	return 0;
}
