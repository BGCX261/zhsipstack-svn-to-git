// ZhNotify.h: interface for the CZhNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHNOTIFY_H__15C9B077_72E3_4515_9E26_B6599A154C55__INCLUDED_)
#define AFX_ZHNOTIFY_H__15C9B077_72E3_4515_9E26_B6599A154C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZhDialog.h"
#include "SipTransaction.h"
#include "ZhList.h"
#include "SipMessage.h"

class CZhNotify  
{
public:
	CZhNotify(CSipMessage* inc_subscribe);
	virtual ~CZhNotify();

	int Init(CSipMessage* inc_subscribe);
	void Free();
	
	int n_id;
	int n_online_status;
	
	int n_ss_status;
	int n_ss_reason;
	time_t n_ss_expires;
	CZhList n_dialogs;
	
	CSipTransaction* n_inc_tr;
	CSipTransaction* n_out_tr;

};

#endif // !defined(AFX_ZHNOTIFY_H__15C9B077_72E3_4515_9E26_B6599A154C55__INCLUDED_)
