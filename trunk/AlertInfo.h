// AlertInfo.h: interface for the CAlertInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALERTINFO_H__E320CB80_0EB6_477C_9B31_7AAA92D924BE__INCLUDED_)
#define AFX_ALERTINFO_H__E320CB80_0EB6_477C_9B31_7AAA92D924BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlertInfo : public CNodeRoot  
{
public:
	CAlertInfo();
	virtual ~CAlertInfo();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr( char** dest );

	int		Clone(CAlertInfo** dest);
	


	char*	element;  			/**< Call-Info main value */
	CZhList gen_params; 	/**< Parameters for Call-Info header */

};

#endif // !defined(AFX_ALERTINFO_H__E320CB80_0EB6_477C_9B31_7AAA92D924BE__INCLUDED_)
