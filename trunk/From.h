// From.h: interface for the CFrom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FROM_H__5B30E441_FE53_4620_AC2E_EBC1869BB0ED__INCLUDED_)
#define AFX_FROM_H__5B30E441_FE53_4620_AC2E_EBC1869BB0ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Url.h"


class CSipMessage;

class CFrom : public CNodeRoot  
{
public:
	CFrom();
	virtual ~CFrom();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	/**
	 * Clone a From element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int Clone(CFrom** dest);
	/**
	 * Set the displayname in the From element.
	 * @param header The element to work on.
	 * @param value The value of the element.
	 */
	void	SetDisplayName(char* value);
	/**
	 * Get the displayname from a From header.
	 * @param header The element to work on.
	 */
	char*	GetDisplayName();
	/**
	 * Set the url in the From element.
	 * @param header The element to work on.
	 * @param url The value of the element.
	 */
	void	SetUrl(CUrl* value);
	/**
	 * Get the url from a From header.
	 * @param header The element to work on.
	 */
	CUrl*	GetUrl();
	/**
	 * Get a header parameter from a From element.
	 * @param header The element to work on.
	 * @param pos The index of the element to get.
	 * @param dest A pointer on the element found.
	 */
	int		GetParam(int pos,CUrlParam** dest);

	int		GetTag(CUrlParam** dest);

	int		Compare(CFrom *pFrom);
	
	char*	displayname;  	 /**< Display Name */ 
	CUrl*	url;		 /**< url */
	CZhList	gen_params;  /**< other From parameters */
};


int osip_from_clone(CFrom* from, CFrom** dest);

#define osip_contact_clone osip_from_clone

#endif // !defined(AFX_FROM_H__5B30E441_FE53_4620_AC2E_EBC1869BB0ED__INCLUDED_)
