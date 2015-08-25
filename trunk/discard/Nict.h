// Nict.h: interface for the CNict class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NICT_H__C57A3117_9371_428A_860A_17B167D99C71__INCLUDED_)
#define AFX_NICT_H__C57A3117_9371_428A_860A_17B167D99C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "siptypes.h"
#include "SipMessage.h"

class CNict  
{
public:
	CNict(CSipMessage* request);
	virtual ~CNict();

	int		Init(CSipMessage* request);

	int		SetDestination(char* destination, int port);
	
	int timer_e_length; 		  /**@internal A=T1, A=2xT1... (unreliable only) */
	struct timeval timer_e_start; /**@internal */
	int timer_f_length; 		  /**@internal B = 64* T1 */
	struct timeval timer_f_start; /**@internal fire when transaction timeouts */
	int timer_k_length; 		  /**@internal K = T4 (else = 0) */
	struct timeval timer_k_start; /**@internal */
	char* destination;  		  /**@internal url used to send requests */
	int port;   				  /**@internal port of next hop */
};

#endif // !defined(AFX_NICT_H__C57A3117_9371_428A_860A_17B167D99C71__INCLUDED_)
