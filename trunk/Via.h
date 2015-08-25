// Via.h: interface for the CVia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIA_H__A397071F_ED66_4340_A09E_CB01EF61B53F__INCLUDED_)
#define AFX_VIA_H__A397071F_ED66_4340_A09E_CB01EF61B53F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVia : public CNodeRoot  
{
public:
	CVia();
	virtual ~CVia();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CVia** dest);

	void	SetVersion(char* version);
	char*	GetVersion();
	void	SetProtocol(char* protocol);
	char*	GetProtocol();
	void	SetHost(char* host);
	char*	GetHost();
	void	SetPort(char* port);
	char*	GetPort();
	void	SetComment(char* comment);
	char*	GetComment();
	
	
	char*	m_pcVersion;  			/**< SIP Version */
	char*	m_pcProtocol; 			/**< Protocol used by SIP Agent */
	char*	m_pcHost; 				/**< Host where to send answers */
	char*	m_pcPort; 				/**< Port where to send answers */
	char*	m_pcComment;  			/**< Comments about SIP Agent */
	CZhList m_listVia_params; 	/**< Via parameters */

	string m_strOriginal;

};

int osip_via_clone(CVia* via, CVia** dest);

#define osip_via_param_get_byname(header,name,dest) osip_generic_param_get_byname((&(header)->m_listVia_params),name,dest)

#endif // !defined(AFX_VIA_H__A397071F_ED66_4340_A09E_CB01EF61B53F__INCLUDED_)
