// Ict.h: interface for the CIct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICT_H__5DD6140A_EBE5_477C_AFF4_42390ABE91E1__INCLUDED_)
#define AFX_ICT_H__5DD6140A_EBE5_477C_AFF4_42390ABE91E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipMessage.h"
#include "siptypes.h"

class CIct  
{
public:
	CIct(CSipMessage* invite);
	virtual ~CIct();

	int		Init(CSipMessage* invite);
	
	int		SetDestination(char* destination, int port);
	


	int timer_a_length; 		  /**@internal A=T1, A=2xT1... (unreliable only) */
	struct timeval timer_a_start; /**@internal */
	int timer_b_length; 		  /**@internal B = 64* T1 */
	struct timeval timer_b_start; /**@internal fire when transaction timeouts */
	int timer_d_length; 		  /**@internal D >= 32s for unreliable tr (or 0) */
	struct timeval timer_d_start; /**@internal should be equal to timer H */
	char* destination;  		  /**@internal url used to send requests */
	int port;   				  /**@internal port of next hop */

};

#endif // !defined(AFX_ICT_H__5DD6140A_EBE5_477C_AFF4_42390ABE91E1__INCLUDED_)
