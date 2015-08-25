// ProxyAuthenticate.cpp: implementation of the CProxyAuthenticate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ProxyAuthenticate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int osip_proxy_authenticate_clone(const CProxyAuthenticate* wwwa,
								CProxyAuthenticate** dest)
{
	CProxyAuthenticate* wa;
	
	*dest = NULL;
	if (wwwa == NULL)
		return -1;
	if (wwwa->auth_type == NULL)
		return -1;
	
	wa = new CProxyAuthenticate();
	if (wa == NULL)				  /* allocation failed */
		return -1;
	wa->auth_type = zstr_strdup(wwwa->auth_type);
	if (wwwa->realm != NULL)
		wa->realm = zstr_strdup(wwwa->realm);
	if (wwwa->domain != NULL)
		wa->domain = zstr_strdup(wwwa->domain);
	if (wwwa->nonce != NULL)
		wa->nonce = zstr_strdup(wwwa->nonce);
	if (wwwa->opaque != NULL)
		wa->opaque = zstr_strdup(wwwa->opaque);
	if (wwwa->stale != NULL)
		wa->stale = zstr_strdup(wwwa->stale);
	if (wwwa->algorithm != NULL)
		wa->algorithm = zstr_strdup(wwwa->algorithm);
	if (wwwa->qop_options != NULL)
		wa->qop_options = zstr_strdup(wwwa->qop_options);
	
	*dest = wa;
	return 0;
}
