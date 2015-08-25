// ProxyAuthorization.h: interface for the CProxyAuthorization class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYAUTHORIZATION_H__D6FBFA78_33DA_417C_806A_342380718507__INCLUDED_)
#define AFX_PROXYAUTHORIZATION_H__D6FBFA78_33DA_417C_806A_342380718507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Authorization.h"

typedef CAuthorization CProxyAuthorization;

int osip_proxy_authorization_clone(const CProxyAuthorization* auth, CProxyAuthorization** dest);

#endif // !defined(AFX_PROXYAUTHORIZATION_H__D6FBFA78_33DA_417C_806A_342380718507__INCLUDED_)
