// ContentLength.h: interface for the CContentLength class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENTLENGTH_H__4178B31F_10AE_409A_B2EB_84B217532E31__INCLUDED_)
#define AFX_CONTENTLENGTH_H__4178B31F_10AE_409A_B2EB_84B217532E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CContentLength : public CNodeRoot  
{
public:
	CContentLength();
	virtual ~CContentLength();

	int		Init();
	void	Free();
	
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CContentLength** dest);

	char*	value;	/**< value for Content-Length (size of attachments) */
	
};

#endif // !defined(AFX_CONTENTLENGTH_H__4178B31F_10AE_409A_B2EB_84B217532E31__INCLUDED_)
