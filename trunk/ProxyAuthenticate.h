// ProxyAuthenticate.h: interface for the CProxyAuthenticate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYAUTHENTICATE_H__51386750_78B3_46BD_93C6_7165B13AA828__INCLUDED_)
#define AFX_PROXYAUTHENTICATE_H__51386750_78B3_46BD_93C6_7165B13AA828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WwwAuthenticate.h"


typedef CWwwAuthenticate CProxyAuthenticate;

int osip_proxy_authenticate_clone(const CProxyAuthenticate* wwwa, CProxyAuthenticate** dest);

#endif // !defined(AFX_PROXYAUTHENTICATE_H__51386750_78B3_46BD_93C6_7165B13AA828__INCLUDED_)
