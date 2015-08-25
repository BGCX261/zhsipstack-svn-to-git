// Accept.h: interface for the CAccept class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCEPT_H__277F60C6_8F87_4432_AB8F_6ADD5B2A1C89__INCLUDED_)
#define AFX_ACCEPT_H__277F60C6_8F87_4432_AB8F_6ADD5B2A1C89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAccept : public CNodeRoot
{
public:
	CAccept();
	virtual ~CAccept();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);

	int		Clone(CAccept** dest);
	int		ToStr(char** dest);
	
	char*	type; 				/**< Type of attachement */
	char*	subtype;  			/**< Sub-Type of attachement */
	CZhList gen_params; 		/**< Content-Type parameters */

};

#endif // !defined(AFX_ACCEPT_H__277F60C6_8F87_4432_AB8F_6ADD5B2A1C89__INCLUDED_)
