// ContentType.h: interface for the CContentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENTTYPE_H__1B86BA6B_5824_430A_BEC4_02DCBD8E72A2__INCLUDED_)
#define AFX_CONTENTTYPE_H__1B86BA6B_5824_430A_BEC4_02DCBD8E72A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CContentType : public CNodeRoot  
{
public:
	CContentType();
	virtual ~CContentType();

	int		Init();
	void	Free();
	
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CContentType** dest);

	
	char*	type; 				/**< Type of attachement */
	char*	subtype;  			/**< Sub-Type of attachement */
	CZhList gen_params; 	/**< Content-Type parameters */

};

#endif // !defined(AFX_CONTENTTYPE_H__1B86BA6B_5824_430A_BEC4_02DCBD8E72A2__INCLUDED_)
