// UrlParam.h: interface for the CUrlParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_URLPARAM_H__E1576CDD_6503_4550_897C_EBE46AC50F4F__INCLUDED_)
#define AFX_URLPARAM_H__E1576CDD_6503_4550_897C_EBE46AC50F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUrlParam  
{
public:
	CUrlParam();
	virtual ~CUrlParam();

	int		InitParam();
	void	FreeParam();
	int		SetParam(char* pname, char* pvalue);
	
	char*	gname;	/**< uri parameter name */
	char*	gvalue;   /**< uri parameter value */	
};

int osip_uri_param_add(CZhList* url_params, char* name, char* value);
int osip_uri_param_get_byname(CZhList* url_params, char* name,CUrlParam** dest);
int osip_uri_param_set(CZhList* url_param, char* pname, char* pvalue);
int osip_uri_param_clone(const CUrlParam* uparam,CUrlParam** dest);
void osip_uri_param_freelist(CZhList* params);

#define osip_uri_header_add(url_headers,name,value) 	   osip_uri_param_add(url_headers,name,value)
#define osip_uri_header_get_byname(url_headers,name,dest)  osip_uri_param_get_byname(url_headers,name,dest)

#define osip_generic_param_add(LIST,NAME,VALUE) 	   osip_uri_param_add(LIST,NAME,VALUE)
#define osip_generic_param_clone	  osip_uri_param_clone
#define osip_generic_param_freelist(LIST)   	osip_uri_param_freelist(LIST)

#endif // !defined(AFX_URLPARAM_H__E1576CDD_6503_4550_897C_EBE46AC50F4F__INCLUDED_)
