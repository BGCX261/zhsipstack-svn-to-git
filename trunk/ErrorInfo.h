// ErrorInfo.h: interface for the CErrorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORINFO_H__2DB4FADF_B194_41D6_B451_A99BAD919913__INCLUDED_)
#define AFX_ERRORINFO_H__2DB4FADF_B194_41D6_B451_A99BAD919913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CErrorInfo : public CNodeRoot  
{
public:
	CErrorInfo();
	virtual ~CErrorInfo();

	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr( char** dest );
	/**
	 * Clone a Error-Info element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int		Clone(CErrorInfo** dest);

	char*	GetUrl();
	void	SetUrl(char* uri);


	char* element;  			/**< Error-Info main value */
	CZhList gen_params; 	/**< Parameters for Call-Info header */
};

#endif // !defined(AFX_ERRORINFO_H__2DB4FADF_B194_41D6_B451_A99BAD919913__INCLUDED_)
