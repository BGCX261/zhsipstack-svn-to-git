// ZhDialog.cpp: implementation of the CZhDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhDialog::CZhDialog()
{
	Init();
}

CZhDialog::~CZhDialog()
{
	Free();
}

int CZhDialog::Init()
{
	
	return 0;
}

int CZhDialog::InitAsUac( CSipMessage *_200Ok )
{
	int i;
	
	
	d_id = -1;  			  /* not yet available to user */
	d_STATE = JD_EMPTY;
	
	if (MSG_IS_REQUEST(_200Ok))
	{
		d_dialog = new CSipDialog;
		i = d_dialog->InitAsUacWithRemoteRequest(d_200Ok,-1);
	}
	else
	{
		d_dialog = new CSipDialog;
		i = d_dialog->InitAsUac(d_200Ok);
	}
	if (i != 0)
	{
		return -1;
	}
	
	d_count = 0;
	d_timer = time(NULL);
	d_200Ok = NULL;
	d_ack = NULL;
	
	return 0;	
}

int CZhDialog::InitAsUas( CZhDialog** jd,CSipMessage* _invite, CSipMessage* _200Ok )
{
	int i;

	d_id = -1;  			  /* not yet available to user */
	d_STATE = JD_EMPTY;
	d_dialog = new CSipDialog;
	i = d_dialog->InitAsUas(_invite, _200Ok);
	if (i != 0)
	{
		return -1;
	}
	
	d_count = 0;
	d_timer = time(NULL);
	d_200Ok = NULL;
	d_ack = NULL;
	
#ifdef SUPPORT_MSN
	/* bugguy MSN */
	d_dialog->local_cseq = 1;
#endif

	return 0;	
}

void CZhDialog::Free()
{
	
}

void CZhDialog::SetState( int iState )
{
	d_STATE = iState;	
}

int CZhDialog::Set200Ok( CSipMessage *p200ok )
{
	int i;
	
	if (d_200Ok != NULL)
		delete d_200Ok;
	d_timer = time(NULL) + 2;
	d_count = 0;
	i = p200ok->Clone(&d_200Ok);
	if (i != 0)
	{
		return -1;
	}
	return 0;	
}