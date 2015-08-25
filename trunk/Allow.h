// Allow.h: interface for the CAllow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLOW_H__6B799466_D329_4E3A_845E_4BF2D5E86F5B__INCLUDED_)
#define AFX_ALLOW_H__6B799466_D329_4E3A_845E_4BF2D5E86F5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAllow : public CNodeRoot  
{
public:
	CAllow();
	virtual ~CAllow();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CAllow** dest);

	char*	value;	/**< value for Content-Length (size of attachments) */

};

#endif // !defined(AFX_ALLOW_H__6B799466_D329_4E3A_845E_4BF2D5E86F5B__INCLUDED_)
