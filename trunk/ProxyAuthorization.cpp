// ProxyAuthorization.cpp: implementation of the CProxyAuthorization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ProxyAuthorization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int osip_proxy_authorization_clone(const CProxyAuthorization* auth,
	CProxyAuthorization** dest)
{
	CProxyAuthorization* au;

	*dest = NULL;
	if (auth == NULL)
		return -1;
	/* to be removed?
	   if (auth->auth_type==NULL) return -1;
	   if (auth->username==NULL) return -1;
	   if (auth->realm==NULL) return -1;
	   if (auth->nonce==NULL) return -1;
	   if (auth->uri==NULL) return -1;
	   if (auth->response==NULL) return -1;
	   if (auth->opaque==NULL) return -1;
	 */

	au = new CProxyAuthorization();
	if (au == NULL)				  /* allocation failed */
		return -1;
	if (auth->auth_type != NULL)
	{
		au->auth_type = zstr_strdup(auth->auth_type);
		if (au->auth_type == NULL)
			goto ac_error;
	}
	if (auth->username != NULL)
	{
		au->username = zstr_strdup(auth->username);
		if (au->username == NULL)
			goto ac_error;
	}
	if (auth->realm != NULL)
	{
		au->realm = zstr_strdup(auth->realm);
		if (auth->realm == NULL)
			goto ac_error;
	}
	if (auth->nonce != NULL)
	{
		au->nonce = zstr_strdup(auth->nonce);
		if (auth->nonce == NULL)
			goto ac_error;
	}
	if (auth->uri != NULL)
	{
		au->uri = zstr_strdup(auth->uri);
		if (au->uri == NULL)
			goto ac_error;
	}
	if (auth->response != NULL)
	{
		au->response = zstr_strdup(auth->response);
		if (auth->response == NULL)
			goto ac_error;
	}
	if (auth->digest != NULL)
	{
		au->digest = zstr_strdup(auth->digest);
		if (au->digest == NULL)
			goto ac_error;
	}
	if (auth->algorithm != NULL)
	{
		au->algorithm = zstr_strdup(auth->algorithm);
		if (auth->algorithm == NULL)
			goto ac_error;
	}
	if (auth->cnonce != NULL)
	{
		au->cnonce = zstr_strdup(auth->cnonce);
		if (au->cnonce == NULL)
			goto ac_error;
	}
	if (auth->opaque != NULL)
	{
		au->opaque = zstr_strdup(auth->opaque);
		if (auth->opaque == NULL)
			goto ac_error;
	}
	if (auth->message_qop != NULL)
	{
		au->message_qop = zstr_strdup(auth->message_qop);
		if (auth->message_qop == NULL)
			goto ac_error;
	}
	if (auth->nonce_count != NULL)
	{
		au->nonce_count = zstr_strdup(auth->nonce_count);
		if (auth->nonce_count == NULL)
			goto ac_error;
	}

	*dest = au;
	return 0;

	ac_error : delete(au);
	return -1;
}