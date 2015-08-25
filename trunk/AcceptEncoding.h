// AcceptEncoding.h: interface for the CAcceptEncoding class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCEPTENCODING_H__8C834731_1B14_4C9E_8E96_4DFADDAD52D9__INCLUDED_)
#define AFX_ACCEPTENCODING_H__8C834731_1B14_4C9E_8E96_4DFADDAD52D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAcceptEncoding : public CNodeRoot  
{
public:
	CAcceptEncoding();
	virtual ~CAcceptEncoding();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	
	int		ToStr(char** dest);
	/**
	 * Clone a Accept-Encoding element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int Clone(CAcceptEncoding** dest);

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

#endif // !defined(AFX_ACCEPTENCODING_H__8C834731_1B14_4C9E_8E96_4DFADDAD52D9__INCLUDED_)
