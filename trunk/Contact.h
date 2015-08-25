// Contact.h: interface for the CContact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTACT_H__D16E172D_BFCD_4300_B0AE_832E7C30191B__INCLUDED_)
#define AFX_CONTACT_H__D16E172D_BFCD_4300_B0AE_832E7C30191B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Url.h"

class CSipMessage;


class CContact : public CNodeRoot  
{
public:
	CContact();
	virtual ~CContact();

	int		Init();
	void	Free();

	int		Clone(CContact** dest);
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);
	
	char*	displayname;  	 /**< Display Name */ 
	CUrl*	url;		 /**< url */
	CZhList gen_params;  /**< other From parameters */


	

};

#endif // !defined(AFX_CONTACT_H__D16E172D_BFCD_4300_B0AE_832E7C30191B__INCLUDED_)
