// Url.h: interface for the CUrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_URL_H__E134F78A_3B0A_4EC5_9628_9F38384503FE__INCLUDED_)
#define AFX_URL_H__E134F78A_3B0A_4EC5_9628_9F38384503FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUrl : public CNodeRoot  
{
public:
	CUrl();
	virtual ~CUrl();	

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);


	void	UriUnescape(char* pcParam);
	int		UriParseHeaders(const char* headers);
	int		UriParseParams(const char* params);

	/**
	 * Clone a url element.
	 * @param url The element to work on.
	 * @param dest The resulting new allocated element.
	 */
	int Clone(CUrl** dest);
	/**
	 * Set the scheme of a url element.
	 * @param url The element to work on.
	 * @param value The token value.
	 */
	void	SetScheme(char* value);
	/**
	 * Get the scheme of a url element.
	 * @param url The element to work on.
	 */
	char*	GetScheme();
	/**
	 * Set the host of a url element.
	 * @param url The element to work on.
	 * @param value The token value.
	 */
	void	SetHost( char* value);
	/**
	 * Get the host of a url element.
	 * @param url The element to work on.
	 */
	char*	GetHost();
	/**
	 * Set the username of a url element.
	 * @param url The element to work on.
	 * @param value The token value.
	 */
	void	SetUsername( char* value);
	/**
	 * Get the username of a url element.
	 * @param url The element to work on.
	 */
	char*	GetUsername();
	/**
	 * Set the password of a url element.
	 * @param url The element to work on.
	 * @param value The token value.
	 */
	void	SetPassword( char* value);
	/**
	 * Get the password of a url element.
	 * @param url The element to work on.
	 */
	char*	GetPassword();
	/**
	 * Set the port of a url element.
	 * @param url The element to work on.
	 * @param value The token value.
	 */
	void	SetPort( char* value);
	/**
	 * Get the port of a url element.
	 * @param url The element to work on.
	 */
	char*	GetPort();	

	
	
	
	char*	scheme;   		   /**< Uri Scheme (sip or sips) */
	char*	username; 		   /**< Username */
	char*	password; 		   /**< Password */
	char*	host; 			   /**< Domain */
	char*	port; 			   /**< Port number */
	CZhList url_params;    /**< Uri parameters */
	CZhList url_headers;   /**< Uri headers */
	
	char* m_pcOther;  /**< Space for other url schemes. (http, mailto...) */
	char* m_pcOriginal;

};


	/**
	 * Set the transport parameter to UDP in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_transport_udp(url)   osip_uri_param_add((&(url)->url_params), zstr_strdup("transport"), zstr_strdup("udp"))
	/**
	 * Set the transport parameter to TCP in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_transport_tcp(url)   osip_uri_param_add((&(url)->url_params), zstr_strdup("transport"), zstr_strdup("tcp"))
	/**
	 * Set the transport parameter to SCTP in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_transport_sctp(url)  osip_uri_param_add((&(url)->url_params), zstr_strdup("transport"), zstr_strdup("sctp"))
	/**
	 * Set the transport parameter to TLS in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_transport_tls(url)   osip_uri_param_add((&(url)->url_params), zstr_strdup("transport"), zstr_strdup("tls"))
	/**
	 * Set the transport parameter to TLS in a url element.
	 * @param url The element to work on.
	 * @param value The value describing the transport protocol.
	 */
#define osip_uri_set_transport(url,value) osip_uri_param_add((&(url)->url_params), zstr_strdup("transport"), value)

	/**
	 * Set the user parameter to PHONE in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_user_phone(url)   osip_uri_param_add((&(url)->url_params), zstr_strdup("user"), zstr_strdup("phone"))
	/**
	 * Set the user parameter to IP in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_user_ip(url)      osip_uri_param_add((&(url)->url_params), zstr_strdup("user"), zstr_strdup("ip"))
	/**
	 * Set a method parameter to INVITE in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_invite(url)  osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), zstr_strdup("INVITE"))
	/**
	 * Set a method parameter to ACK in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_ack(url)	 osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), zstr_strdup("ACK"))
	/**
	 * Set a method parameter to OPTIONS in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_options(url) osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), zstr_strdup("OPTIONS"))
	/**
	 * Set a method parameter to BYE in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_bye(url)	 osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), zstr_strdup("BYE"))
	/**
	 * Set a method parameter to CANCEL in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_cancel(url)  osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), zstr_strdup("CANCEL"))
	/**
	 * Set a method parameter to REGISTER in a url element.
	 * @param url The element to work on.
	 */
#define osip_uri_set_method_register(url) osip_uri_param_add((&(url)->url_params),zstr_strdup("method"), zstr_strdup("REGISTER"))
	/**
	 * Set a method parameter in a url element.
	 * @param url The element to work on.
	 * @param value The value for the method parameter.
	 */
#define osip_uri_set_method(url, value) osip_uri_param_add((&(url)->url_params), zstr_strdup("method"), value)
	/**
	 * Set a ttl parameter in a url element.
	 * @param url The element to work on.
	 * @param value The value for the ttl parameter.
	 */
#define osip_uri_set_ttl(url, value)	osip_uri_param_add((&(url)->url_params), zstr_strdup("ttl"), value)
	/**
	 * Set a maddr parameter in a url element.
	 * @param url The element to work on.
	 * @param value The value for the maddr parameter.
	 */
#define osip_uri_set_maddr(url, value)  osip_uri_param_add((&(url)->url_params), zstr_strdup("maddr"), value)

	/**
	 * Allocate and add a url parameter element in a url element.
	 * @param url The element to work on.
	 * @param name The token name.
	 * @param value The token value.
	 */
#define osip_uri_uparam_add(url,name,value) osip_uri_param_add((&(url)->url_params),name,value)
	/**
	 * Find in a url parameter element in a url element.
	 * @param url The element to work on.
	 * @param name The name of the url parameter element to find.
	 * @param dest A pointer on the element found.
	 */
#define osip_uri_uparam_get_byname(url,name,dest)  osip_uri_param_get_byname((&(url)->url_params),name,dest)

	/**
	 * Allocate and add a url header element in a url element.
	 * @param url The element to work on.
	 * @param name The token name.
	 * @param value The token value.
	 */
#define osip_uri_uheader_add(url,name,value)	osip_uri_header_add((&(url)->url_headers),name,value)
	/**
	 * Find in a url header element in a url element.
	 * @param url The element to work on.
	 * @param name The name of the url header element to find.
	 * @param dest A pointer on the element found.
	 */
#define osip_uri_uheader_get_byname(url,name,dest) osip_uri_header_get_byname((&(url)->url_headers),name,dest)

#define osip_generic_param_set(GP, NAME, VALUE)  osip_uri_param_set(GP, NAME, VALUE)

#define osip_generic_param_get_byname(LIST,NAME,DEST) osip_uri_param_get_byname(LIST,NAME,DEST)

#endif // !defined(AFX_URL_H__E134F78A_3B0A_4EC5_9628_9F38384503FE__INCLUDED_)
