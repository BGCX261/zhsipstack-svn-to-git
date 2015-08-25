// Ist.h: interface for the CIst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IST_H__149CF89D_274F_4B68_8A38_CA8BC3427178__INCLUDED_)
#define AFX_IST_H__149CF89D_274F_4B68_8A38_CA8BC3427178__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "siptypes.h"
#include "SipMessage.h"

class CIst  
{
public:
	CIst(CSipMessage* invite);
	virtual ~CIst();

	int		Init(CSipMessage* invite);


	int timer_g_length; 	 /**@internal G=MIN(T1*2,T2) for unreliable trans. */
	struct timeval timer_g_start; /**@internal 0 when reliable transport is used */
	int timer_h_length; 		  /**@internal H = 64* T1 */
	struct timeval timer_h_start; /**@internal fire when if no ACK is received */
	int timer_i_length; 		  /**@internal I = T4 for unreliable (or 0) */
	struct timeval timer_i_start; /**@internal absorb all ACK */
};

#endif // !defined(AFX_IST_H__149CF89D_274F_4B68_8A38_CA8BC3427178__INCLUDED_)
