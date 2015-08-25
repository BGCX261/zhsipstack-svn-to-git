// AcceptLanguage.h: interface for the CAcceptLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCEPTLANGUAGE_H__F2DC67FA_1BFF_49D6_A007_519035F5EC19__INCLUDED_)
#define AFX_ACCEPTLANGUAGE_H__F2DC67FA_1BFF_49D6_A007_519035F5EC19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAcceptLanguage : public CNodeRoot  
{
public:
	CAcceptLanguage();
	virtual ~CAcceptLanguage();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);
	/**
	 * Clone a Accept-Encoding element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int Clone(CAcceptLanguage** dest);

	/**
	 * Set the value of an Accept-Encoding element.
	 * @param header The element to work on.
	 * @param value The token value to set.
	 */
	void SetElement(char* value);
	/**
	 * Get the value of an Accept-Encoding element.
	 * @param header The element to work on.
	 */
	char* GetElement();

	char*	element;  		 /**< accept encoding main value */
	CZhList gen_params;  /**< accept encoding parameters */

};

#endif // !defined(AFX_ACCEPTLANGUAGE_H__F2DC67FA_1BFF_49D6_A007_519035F5EC19__INCLUDED_)
