// CallInfo.h: interface for the CCallInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLINFO_H__3A7D8740_72D9_40E1_B38A_4DB741AE4460__INCLUDED_)
#define AFX_CALLINFO_H__3A7D8740_72D9_40E1_B38A_4DB741AE4460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCallInfo : public CNodeRoot  
{
public:
	CCallInfo();
	virtual ~CCallInfo();

	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);

	/**
	 * Clone a Call-Info element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int		Clone(CCallInfo** dest);

	char*	GetUrl();
	void	SetUrl(char* uri);

	char* element;  			/**< Call-Info main value */
	CZhList gen_params; 	/**< Parameters for Call-Info header */

};

#endif // !defined(AFX_CALLINFO_H__3A7D8740_72D9_40E1_B38A_4DB741AE4460__INCLUDED_)
