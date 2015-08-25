// SipEvent.h: interface for the CSipEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPEVENT_H__F5FD9359_DDF0_4E5A_B1E9_63FCE5EAB1C1__INCLUDED_)
#define AFX_SIPEVENT_H__F5FD9359_DDF0_4E5A_B1E9_63FCE5EAB1C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipMessage.h"
#include "siptypes.h"

class CSipEvent  
{
public:
	CSipEvent();
	virtual ~CSipEvent();
	
	int		Init();
	void	Free();
	
	static CSipEvent* OutGoingEventNew(CSipMessage *sip);
	static CSipEvent* InComingEventNew(CSipMessage *sip);
	static CSipEvent* EventNew(eSipEventType type, int transactionid);
	static CSipEvent* TimerXTriger(CZhTimer *timer, int cond_state, int transactionid, int TIMER_VAL);


	eSipEventType	type;			 /**< Event Type */
	int				transactionid;  	 /**< identifier of the related osip transaction */
	CSipMessage*	sip;	 /**< SIP message (optional) */
};

	/* FOR INCOMING TRANSACTION */
	/**
	 * Check if the sipevent is of type RCV_REQINVITE.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_INVITE(event)	   (event->type==RCV_REQINVITE)
	/**
	 * Check if the sipevent is of type RCV_REQACK.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_ACK(event)   	   (event->type==RCV_REQACK)
	/**
	 * Check if the sipevent is of type RCV_REQUEST.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_REQUEST(event)      (event->type==RCV_REQUEST)
	/**
	 * Check if the sipevent is of type RCV_STATUS_1XX.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_STATUS_1XX(event)   (event->type==RCV_STATUS_1XX)
	/**
	 * Check if the sipevent is of type RCV_STATUS_2XX.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_STATUS_2XX(event)   (event->type==RCV_STATUS_2XX)
	/**
	 * Check if the sipevent is of type RCV_STATUS_3456XX.
	 * @param event the event to check.
	 */
#define EVT_IS_RCV_STATUS_3456XX(event)   (event->type==RCV_STATUS_3456XX)


	/* FOR OUTGOING TRANSACTION */
	/**
	 * Check if the sipevent is of type SND_REQINVITE.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_INVITE(event)	   (event->type==SND_REQINVITE)
	/**
	 * Check if the sipevent is of type SND_REQACK.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_ACK(event)   	   (event->type==SND_REQACK)
	/**
	 * Check if the sipevent is of type SND_REQUEST.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_REQUEST(event)      (event->type==SND_REQUEST)
	/**
	 * Check if the sipevent is of type SND_STATUS_1XX.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_STATUS_1XX(event)   (event->type==SND_STATUS_1XX)
	/**
	 * Check if the sipevent is of type SND_STATUS_2XX.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_STATUS_2XX(event)   (event->type==SND_STATUS_2XX)
	/**
	 * Check if the sipevent is of type SND_STATUS_3456XX.
	 * @param event the event to check.
	 */
#define EVT_IS_SND_STATUS_3456XX(event)   (event->type==SND_STATUS_3456XX)
	/**
	 * Check if the sipevent is of an incoming SIP MESSAGE.
	 * @param event the event to check.
	 */
#define EVT_IS_INCOMINGMSG(event)      (event->type>=RCV_REQINVITE \
								   &&event->type<=RCV_STATUS_3456XX)
	/**
	 * Check if the sipevent is of an incoming SIP REQUEST.
	 * @param event the event to check.
	 */
#define EVT_IS_INCOMINGREQ(event)      (EVT_IS_RCV_INVITE(event) \
									   ||EVT_IS_RCV_ACK(event) \
									   ||EVT_IS_RCV_REQUEST(event))
	/**
	 * Check if the sipevent is of an incoming SIP RESPONSE.
	 * @param event the event to check.
	 */
#define EVT_IS_INCOMINGRESP(event)     (EVT_IS_RCV_STATUS_1XX(event) \
									   ||EVT_IS_RCV_STATUS_2XX(event) \
					   ||EVT_IS_RCV_STATUS_3456XX(event))
	/**
	 * Check if the sipevent is of an outgoing SIP MESSAGE.
	 * @param event the event to check.
	 */
#define EVT_IS_OUTGOINGMSG(event)      (event->type>=SND_REQINVITE \
								   &&event->type<=SND_STATUS_3456XX)
	/**
	 * Check if the sipevent is of an outgoing SIP REQUEST.
	 * @param event the event to check.
	 */
#define EVT_IS_OUTGOINGREQ(event)      (EVT_IS_SND_INVITE(event) \
									   ||EVT_IS_SND_ACK(event) \
									   ||EVT_IS_SND_REQUEST(event))
	/**
	 * Check if the sipevent is of an outgoing SIP RESPONSE.
	 * @param event the event to check.
	 */
#define EVT_IS_OUTGOINGRESP(event)     (EVT_IS_SND_STATUS_1XX(event) \
									   ||EVT_IS_SND_STATUS_2XX(event) \
					   ||EVT_IS_SND_STATUS_3456XX(event))

	/**
	 * Check if the sipevent is a SIP MESSAGE.
	 * @param event the event to check.
	 */
#define EVT_IS_MSG(event)   		   (event->type>=RCV_REQINVITE \
								   &&event->type<=SND_STATUS_3456XX)
	/**
	 * Check if the sipevent is of type KILL_TRANSACTION.
	 * NOTE: THIS IS AN INTERNAL METHOD ONLY
	 * @param event the event to check.
	 */
#define EVT_IS_KILL_TRANSACTION(event) (event->type==KILL_TRANSACTION)

#endif // !defined(AFX_SIPEVENT_H__F5FD9359_DDF0_4E5A_B1E9_63FCE5EAB1C1__INCLUDED_)
