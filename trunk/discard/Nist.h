// Nist.h: interface for the CNist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NIST_H__94C00E1E_26F6_4A5C_AD54_DFE8160DD87C__INCLUDED_)
#define AFX_NIST_H__94C00E1E_26F6_4A5C_AD54_DFE8160DD87C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "siptypes.h"
#include "SipMessage.h"

class CNist  
{
public:
	CNist(CSipMessage* invite);
	virtual ~CNist();

	int		Init(CSipMessage* invite);


	int timer_j_length; 		   /**@internal J = 64*T1 (else 0) */
	struct timeval timer_j_start;  /**@internal */
};

#endif // !defined(AFX_NIST_H__94C00E1E_26F6_4A5C_AD54_DFE8160DD87C__INCLUDED_)
