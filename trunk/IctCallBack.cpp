// IctCallBack.cpp: implementation of the CIctCallBack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IctCallBack.h"
#include "ZhDialog.h"
#include "ZhNotify.h"
#include "ZhSubscribe.h"
#include "ZhCall.h"
#include "ZhReg.h"
#include "TransactionInfo.h"
#include "SipTransactionManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIctCallBack::CIctCallBack()
{

}

CIctCallBack::~CIctCallBack()
{

}

void CIctCallBack::icb_SendInvite( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("icb_SendInvite");
}

void CIctCallBack::icb_SendInviteAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("icb_SendInviteAgain");
}

void CIctCallBack::icb_SendAck( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("icb_SendAck");
}

void CIctCallBack::icb_SendAckAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("icb_SendAckAgain");
}

void udp_tl_learn_port_from_via(CSipMessage* sip)
{
	/* EXOSIP_OPT_UDP_LEARN_PORT option set */ 
	CVia* via = NULL;
	CUrlParam* br;
	
	sip->GetVia(0,&via);
	if (via !=
		NULL &&
		via->m_pcProtocol !=
		NULL &&
		(zstr_strcasecmp(via->m_pcProtocol, "udp") == 0 ||
		zstr_strcasecmp(via->m_pcProtocol,
		"dtls-udp") == 0))
	{
		osip_via_param_get_byname(via, "rport", &br);
		if (br != NULL && br->gvalue != NULL)
		{
			//_snprintf(udp_firewall_port, 20, "%s", br->gvalue);
		}
	}
	
	return;
}

void CIctCallBack::icb_Rev1xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	CZhDialog* jd;
	CZhCall* jc;
#ifndef MINISIZE
	CZhSubscribe* js;
	CZhNotify* jn;
#endif
	CTransactionInfo* jinfo = (CTransactionInfo*) tr->GetInstance();

	udp_tl_learn_port_from_via(sip);

	if (jinfo == NULL)
		return;
	jd = jinfo->jd;
	jc = jinfo->jc;
#ifndef MINISIZE
	jn = jinfo->jn;
	js = jinfo->js;
#endif


	if ((MSG_IS_RESPONSE_FOR(sip, "INVITE")
#ifndef MINISIZE
 || MSG_IS_RESPONSE_FOR(sip, "SUBSCRIBE")
#endif
		) && !MSG_TEST_CODE(sip, 100))
	{
		int i;

		if (jd == NULL) 		  /* This transaction initiate a dialog in the case of
										   INVITE (else it would be attached to a "jd" element. */
		{
			/* allocate a jd */
			jd = new CZhDialog;
			i = jd->InitAsUac(sip);
			if (i != 0)
			{
				return;
			}
			if (jc != NULL)
			{
				jc->c_dialogs.Add(jd);
				jinfo->jd = jd;

				CSipTransactionManager *ptm = CSipTransactionManager::getInstance();
				ptm->eXosip_update();
			}
#ifndef MINISIZE
			else if (js != NULL)
			{
				js->s_dialogs.Add(jd);
				jinfo->jd = jd;
				CSipTransactionManager *ptm = CSipTransactionManager::getInstance();
				ptm->eXosip_update();
			}
			else if (jn != NULL)
			{
				jn->n_dialogs.Add(jd);
				jinfo->jd = jd;
				CSipTransactionManager *ptm = CSipTransactionManager::getInstance();
				ptm->eXosip_update();
			}
#endif
			else
			{
			}
			tr->SetInstance(jinfo);
		}
		else
		{
			if (jd->d_dialog == NULL)
			{
			}
			else if (jd->d_dialog->remote_tag == NULL)
			{
				jd->d_dialog->UpdateRouteSetAsUac(sip);
				jd->d_dialog->UpdateTagAsUac(sip);
			}
			else
			{
				CUrlParam* tag;
				int i;

				i = sip->m_to->GetTag(&tag);
				if (tag != NULL &&
					tag->gvalue != NULL &&
					0 == strcmp(jd->d_dialog->remote_tag,tag->gvalue))
				{
					/* Update only if it is the same dialog */
					jd->d_dialog->UpdateRouteSetAsUac(sip);
				}
			}
		}

		if (jd != NULL)
			jd->d_STATE = JD_TRYING;
		if (jd != NULL &&
			MSG_IS_RESPONSE_FOR(sip, "INVITE") &&
			sip->m_iStatusCode < 180)
		{
			//report_call_event(EXOSIP_CALL_PROCEEDING, jc, jd, tr);
		}
		else if (jd != NULL &&
			MSG_IS_RESPONSE_FOR(sip, "INVITE") &&
			sip->m_iStatusCode >= 180)
		{
			//report_call_event(EXOSIP_CALL_RINGING, jc, jd, tr);
		}
#ifndef MINISIZE
		else if (jd != NULL && MSG_IS_RESPONSE_FOR(sip, "SUBSCRIBE"))
		{
			
		}
#endif
		if (MSG_TEST_CODE(sip, 180) && jd != NULL)
		{
			jd->d_STATE = JD_RINGING;
		}
		else if (MSG_TEST_CODE(sip, 183) && jd != NULL)
		{
			jd->d_STATE = JD_QUEUED;
		}
	}
}

void CIctCallBack::icb_Rev2xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIctCallBack::icb_Rev3xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIctCallBack::icb_Rev4xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIctCallBack::icb_Rev5xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIctCallBack::icb_Rev6xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIctCallBack::icb_Rev3456xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}
