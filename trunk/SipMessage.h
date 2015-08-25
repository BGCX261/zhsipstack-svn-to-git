// MessageSip.h: interface for the CSipMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGESIP_H__A338F7E4_661D_4F10_A55D_B42955133F74__INCLUDED_)
#define AFX_MESSAGESIP_H__A338F7E4_661D_4F10_A55D_B42955133F74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipParserHeader.h"

class CSipMessage;

typedef int (*ParseMethod)(CSipMessage* sip,const char *pcValue);

class CSipMessage  
{
public:
	CSipMessage();
	virtual ~CSipMessage();
	
	int		Init();
	void	Free();
	int		ToStr(char** dest, size_t* message_length, int sipfrag=0);
	int		StartlineToStrReq(char** dest);
	int		StartlineToStrResp(char** dest);
	int		StartlineToStr(char** dest);

	int		SetContentLength(const char* hvalue);

		/**
	 * Clone a osip_message_t element.
	 * @param sip The element to clone.
	 * @param dest The new allocated element cloned.
	 */
	int		Clone(CSipMessage** dest);

	/**
	 * Set the reason phrase. This is entirely free in SIP.
	 * @param sip The element to work on.
	 * @param reason The reason phrase.
	 */
	void	SetReasonPhrase(char* reason);
	/**
	 * Get the reason phrase. This is entirely free in SIP.
	 * @param sip The element to work on.
	 */
	char*	GetReasonPhrase();
	/**
	 * Set the status code. This is entirely free in SIP.
	 * @param sip The element to work on.
	 * @param statuscode The status code.
	 */
	void	SetStatusCode(int statuscode);
	/**
	 * Get the status code.
	 * @param sip The element to work on.
	 */
	int		GetStatusCode();
	/**
	 * Set the method. You can set any string here.
	 * @param sip The element to work on.
	 * @param method The method name.
	 */
	void	SetMethod(char* method);
	/**
	 * Get the method name.
	 * @param sip The element to work on.
	 */
	char*	GetMethod();
	/**
	 * Set the SIP version used. (default is "SIP/2.0")
	 * @param sip The element to work on.
	 * @param version The version of SIP.
	 */
	void	SetVersion(char* version);
	/**
	 * Get the SIP version.
	 * @param sip The element to work on.
	 */
	char*	GetVersion();
	/**
	 * Set the Request-URI.
	 * @param sip The element to work on.
	 * @param uri The uri to set.
	 */
	void	SetUri(CUrl* uri);
	/**
	 * Get the Request-URI.
	 * @param sip The element to work on.
	 */
	CUrl*	GetUri();

	int		SetFrom(const char* hvalue);
	CTo*	GetFrom();

	int		SetTo(const char* hvalue);
	CTo*	GetTo();

	int		SetHeader(const char* hname,const char* hvalue);
	
	int		GetRoute(int pos,CRoute** dest);
	int		SetRoute(const char* hvalue);

	int		SetVia(const char* hvalue);
	int		GetVia(int pos, CVia** dest);
	int		AddVia(const char* transport, const char* locip);

	int		SetAccept(const char* hvalue);

	int		GetMessageProperty();
	int		SetBody(const char* buf, size_t length);
	int		SetCallId(const char* hvalue);
	int		SetContentType(const char* hvalue);
	int		SetCseq(const char* hvalue);

	int		SetAllow(const char* hvalue);

	int		SetSupported(const char* hvalue);
	

	char			*m_strMethod;	
	CUrl			*m_pRequestUrl;
	char			*m_strVersion;

	int				m_iStatusCode;
	char			*m_strReasonPhrase;

	
	CCallId			*m_callid;
	CFrom			*m_from;
	CTo				*m_to;
	CSeq			*m_seq;
	CContentType	*m_contentType;
	CContentLength	*m_contentLength;
	CMimeVersion	*m_mime_version;/**< Mime-Version header */
		
	CZhList			m_listVias;
	CZhList			m_contacts;
	
	CZhList			m_listAllow;					/**< Allows headers */
	CZhList			m_listAccept;					/**< Accept headers */
	CZhList			m_listAcceptEncoding;			/**< Accept-Encoding headers */
	CZhList			m_listAcceptLanguages;			/**< Accept-Language headers */
	CZhList			m_listAlertInfos;				/**< Alert-Info headers */
	CZhList			m_listAuthenticationInfos;		/**< authentication_info headers */
	CZhList			m_listCallInfos; 				/**< Call-Infos header */
	CZhList			m_listContentEncodings;			/**< Content-Encodings headers */
	CZhList			m_listErrorInfos;				/**< Error-Info headers */
	CZhList			m_lisProxyAuthenticationInfos;	/**< P-Authentication-Info headers */
	CZhList			m_lisProxyAuthenticates;		/**< Proxy-Authenticate headers */
	CZhList			m_listAuthorizations; 			/**< Authorizations headers */
	CZhList			m_listProxyAuthorizations;		/**< Proxy-authorization headers */
	CZhList			m_listRecordRoutes;  			/**< Record-Route headers */
	CZhList			m_listRoutes; 					/**< Route headers */
	CZhList			m_listWwwAuthenticates;			/**< WWW-Authenticate headers */

	CZhList			m_headers;						/**< Other headers */
	CZhList			m_listBodys;

	/*
	   1: structure and buffer "message" are identical.
	   2: buffer "message" is not up to date with the structure info (call osip_message_to_str to update it).
	*/
	
	int message_property;   		  /**@internal */
	char* message;  				  /**@internal */
	size_t message_length;  		  /**@internal */

	void* application_data; 		  /**can be used by upper layer*/

};

	/*
	 *  These are helpfull MACROs to test messages type.
	 */
	/**
	 * Test if the message is a SIP RESPONSE
	 * @param msg the SIP message.
	 */
#define MSG_IS_RESPONSE(msg) ((msg)->m_iStatusCode!=0)
	/**
	 * Test if the message is a SIP REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_REQUEST(msg)  ((msg)->m_iStatusCode==0)

	/**
	 * Test if the message is an INVITE REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_INVITE(msg)   (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"INVITE"))
	/**
	 * Test if the message is an ACK REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_ACK(msg) 	 (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"ACK"))
	/**
	 * Test if the message is a REGISTER REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_REGISTER(msg) (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"REGISTER"))
	/**
	 * Test if the message is a BYE REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_BYE(msg) 	 (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"BYE"))
	/**
	 * Test if the message is an OPTIONS REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_OPTIONS(msg)  (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"OPTIONS"))
	/**
	 * Test if the message is an INFO REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_INFO(msg)	 (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"INFO"))
	/**
	 * Test if the message is a CANCEL REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_CANCEL(msg)   (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"CANCEL"))
	/**
	 * Test if the message is a REFER REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_REFER(msg)   (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"REFER"))
	/**
	 * Test if the message is a NOTIFY REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_NOTIFY(msg)   (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"NOTIFY"))
	/**
	 * Test if the message is a SUBSCRIBE REQUEST
	 * @def MSG_IS_SUBSCRIBE
	 * @param msg the SIP message.
	 */
#define MSG_IS_SUBSCRIBE(msg)  (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"SUBSCRIBE"))
	/**
	 * Test if the message is a MESSAGE REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_MESSAGE(msg)  (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"MESSAGE"))
	/**
	 * Test if the message is a PRACK REQUEST  (!! PRACK IS NOT SUPPORTED by the fsm!!)
	 * @param msg the SIP message.
	 */
#define MSG_IS_PRACK(msg)    (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"PRACK"))


	/**
	 * Test if the message is an UPDATE REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_UPDATE(msg)    (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"UPDATE"))

	/**
	 * Test if the message is an UPDATE REQUEST
	 * @param msg the SIP message.
	 */
#define MSG_IS_PUBLISH(msg)    (MSG_IS_REQUEST(msg) && \
				  0==strcmp((msg)->m_strMethod,"PUBLISH"))


	/**
	 * Test if the message is a response with status between 100 and 199
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_1XX(msg) ((msg)->m_iStatusCode >= 100 && \
								(msg)->m_iStatusCode < 200)
	/**
	 * Test if the message is a response with status between 200 and 299
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_2XX(msg) ((msg)->m_iStatusCode >= 200 && \
								(msg)->m_iStatusCode < 300)
	/**
	 * Test if the message is a response with status between 300 and 399
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_3XX(msg) ((msg)->m_iStatusCode >= 300 && \
								(msg)->m_iStatusCode < 400)
	/**
	 * Test if the message is a response with status between 400 and 499
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_4XX(msg) ((msg)->m_iStatusCode >= 400 && \
								(msg)->m_iStatusCode < 500)
	/**
	 * Test if the message is a response with status between 500 and 599
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_5XX(msg) ((msg)->m_iStatusCode >= 500 && \
								(msg)->m_iStatusCode < 600)
	/**
	 * Test if the message is a response with status between 600 and 699
	 * @param msg the SIP message.
	 */
#define MSG_IS_STATUS_6XX(msg) ((msg)->m_iStatusCode >= 600 && \
								(msg)->m_iStatusCode < 700)
	/**
	 * Test if the message is a response with a status set to the code value.
	 * @param msg the SIP message.
	 * @param code the status code.
	 */
#define MSG_TEST_CODE(msg,code) (MSG_IS_RESPONSE(msg) && \
				 (code)==(msg)->m_iStatusCode)
	/**
	 * Test if the message is a response for a REQUEST of certain type
	 * @param msg the SIP message.
	 * @param requestname the method name to match.
	 */
#define MSG_IS_RESPONSE_FOR(msg,requestname)  (MSG_IS_RESPONSE(msg) && \
				 0==strcmp((msg)->m_seq->method,(requestname)))

#endif // !defined(AFX_MESSAGESIP_H__A338F7E4_661D_4F10_A55D_B42955133F74__INCLUDED_)
