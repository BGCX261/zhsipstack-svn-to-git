// SipEvent.cpp: implementation of the CSipEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SipEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSipEvent::CSipEvent()
{
	Init();
}

CSipEvent::~CSipEvent()
{
	Free();
}

int CSipEvent::Init()
{
	type = UNKNOWN_EVT;
	transactionid = 0;
	sip = NULL;

	return 0;
}

void CSipEvent::Free()
{
	delete sip;
	sip = NULL;
}

CSipEvent* CSipEvent::TimerXTriger(CZhTimer *timer, int cond_state, int transactionid, int TIMER_VAL)
{
	if (cond_state)
	{
		if (timer->IsTimerTriger())
			return EventNew((eSipEventType)TIMER_VAL, transactionid);
	}
	return NULL;
}

CSipEvent* CSipEvent::EventNew(eSipEventType type, int transactionid)
{
	CSipEvent* sipevent;
	
	sipevent = new CSipEvent();
	if (sipevent == NULL)
		return NULL;
	sipevent->type = type;
	sipevent->sip = NULL;
	sipevent->transactionid = transactionid;
	return sipevent;
}

CSipEvent* CSipEvent::OutGoingEventNew( CSipMessage *sip )
{
	if (sip == NULL)
	{
		return NULL;
	}
	
	CSipEvent* pEvent;
	pEvent = new CSipEvent();
	if (pEvent == NULL)
	{
		return NULL;
	}

	_eSipEventType eSipEvent;
	if (MSG_IS_REQUEST(sip))
	{
		if (MSG_IS_INVITE(sip))
		{
			eSipEvent = SND_REQINVITE;
		}
		else if (MSG_IS_ACK(sip))
		{
			eSipEvent = SND_REQACK;
		}
		else
		{
			eSipEvent = SND_REQUEST;
		}
	}
	else
	{
		if (MSG_IS_STATUS_1XX(sip))
		{
			eSipEvent = SND_STATUS_1XX;
		}
		else if (MSG_IS_STATUS_2XX(sip))
		{
			eSipEvent = SND_STATUS_2XX;
		}
		else
		{
			eSipEvent = SND_STATUS_3456XX;
		}
	}

	pEvent->sip = sip;
	pEvent->type = eSipEvent;

	return pEvent;
}

CSipEvent* CSipEvent::InComingEventNew( CSipMessage *sip )
{
	if (sip == NULL)
	{
		return NULL;
	}
	
	CSipEvent* pEvent;
	pEvent = new CSipEvent();
	if (pEvent == NULL)
	{
		return NULL;
	}
	
	_eSipEventType eSipEvent;
	if (MSG_IS_REQUEST(sip))
	{
		if (MSG_IS_INVITE(sip))
		{
			eSipEvent = RCV_REQINVITE;
		}
		else if (MSG_IS_ACK(sip))
		{
			eSipEvent = RCV_REQACK;
		}
		else
		{
			eSipEvent = RCV_REQUEST;
		}
	}
	else
	{
		if (MSG_IS_STATUS_1XX(sip))
		{
			eSipEvent = RCV_STATUS_1XX;
		}
		else if (MSG_IS_STATUS_2XX(sip))
		{
			eSipEvent = RCV_STATUS_2XX;
		}
		else
		{
			eSipEvent = RCV_STATUS_3456XX;
		}
	}
	
	pEvent->sip = sip;
	pEvent->type = eSipEvent;
	
	return pEvent;
}
