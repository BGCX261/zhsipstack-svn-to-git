// ZhDialog.h: interface for the CZhDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHDIALOG_H__3D3787AF_C651_41F8_9AEC_4CBEFA7092E6__INCLUDED_)
#define AFX_ZHDIALOG_H__3D3787AF_C651_41F8_9AEC_4CBEFA7092E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipDialog.h"
#include "SipMessage.h"
#include "ZhList.h"

#ifndef JD_EMPTY

#define JD_EMPTY		  0
#define JD_INITIALIZED    1
#define JD_TRYING   	  2
#define JD_QUEUED   	  3
#define JD_RINGING  	  4
#define JD_ESTABLISHED    5
#define JD_REDIRECTED     6
#define JD_AUTH_REQUIRED  7
#define JD_CLIENTERROR    8
#define JD_SERVERERROR    9
#define JD_GLOBALFAILURE  10
#define JD_TERMINATED     11

#define JD_MAX  		  11

#endif

class CZhDialog  
{
public:
	CZhDialog();
	virtual ~CZhDialog();

	int Init();
	int InitAsUac(CSipMessage *_200Ok);
	int InitAsUas(CZhDialog** jd,CSipMessage* _invite, CSipMessage* _200Ok);
	void Free();
	void SetState(int iState);
	int Set200Ok(CSipMessage *p200ok);
	
	int d_id;
	int d_STATE;
	CSipDialog* d_dialog;	/* active dialog */
	
	time_t d_timer;
	int d_count;
	CSipMessage* d_200Ok;
	CSipMessage* d_ack;
	
	CZhList d_inc_trs;
	CZhList d_out_trs;
	int d_retry;				/* avoid too many unsuccessfull retry */
};

#endif // !defined(AFX_ZHDIALOG_H__3D3787AF_C651_41F8_9AEC_4CBEFA7092E6__INCLUDED_)
