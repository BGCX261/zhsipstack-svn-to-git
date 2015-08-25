// SdpMessage.h: interface for the CSdpMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDPMESSAGE_H__6DC637FF_0A1F_47B9_BCD5_1A947E037D0D__INCLUDED_)
#define AFX_SDPMESSAGE_H__6DC637FF_0A1F_47B9_BCD5_1A947E037D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Utils.h"

/**
* SDP bandwidth definition.
* @struct sdp_bandwidth
*/
struct sdp_bandwidth
{
	sdp_bandwidth();
	~sdp_bandwidth();

	int		Init();
	void	Free();
	int		sdp_append_bandwidth(char* stringParam, int size, char* tmp, char** next_tmp);
	char* b_bwtype; 			/**< bandwidth type */
	char* b_bandwidth;  		/**< bandwidth value */
};


/**
* SDP Time description definition.
* @struct sdp_time_descr
*/
struct sdp_time_descr
{
	sdp_time_descr();
	~sdp_time_descr();

	int		Init();
	void	Free();
	int		sdp_append_time_descr(char* stringParam, int size, char* tmp, char** next_tmp);
	char* t_start_time; 		   /**< start time */
	char* t_stop_time;  		   /**< stop time */
	CZhList r_repeats;  	  /**< repeat headers */
};

/**
* SDP key definition.
* @struct sdp_key
*/
struct sdp_key
{
	sdp_key();
	~sdp_key();

	int		Init();
	void	Free();
	int		sdp_append_key(char* stringParam, int size, char* tmp, char** next_tmp);
	char* k_keytype;	/**< Key Type (prompt, clear, base64, uri) */
	char* k_keydata;	/**< key data */
};

/**
* SDP attribute definition.
* @struct sdp_attribute
*/
struct sdp_attribute
{
	sdp_attribute();
	~sdp_attribute();

	int		Init();
	void	Free();
	int		sdp_append_attribute(char* stringParam, int size, char* tmp, char** next_tmp);
	char* a_att_field;  		/**< attribute field */
	char* a_att_value;  		/**< attribute value (optional) */
};

/**
* SDP connection definition.
* @struct sdp_connection
*/
struct sdp_connection
{
	sdp_connection();
	~sdp_connection();

	int		Init();
	void	Free();
	int		sdp_append_connection(char* stringParam, int size, char* tmp,char** next_tmp);
	char* c_nettype;			 /**< Network Type */
	char* c_addrtype;   		 /**< Network Address Type */
	char* c_addr;   			 /**< Address */
	char* c_addr_multicast_ttl;  /**< TTL value for multicast address */
	char* c_addr_multicast_int;  /**< Number of multicast address */
};

/**
* SDP media definition.
* @struct sdp_media
*/
struct sdp_media
{
	sdp_media();
	~sdp_media();

	int		Init();
	void	Free();
	int		sdp_append_media(char* stringParam, int size, char* tmp, char** next_tmp);
	char* m_media;  			/**< media type */
	char* m_port;   			/**< port number */
	char* m_number_of_port; 	/**< number of port */
	char* m_proto;  			/**< protocol to be used */
	CZhList m_payloads;    /**< list of payloads (as strings) */
	
	char* i_info;   			/**< information header */
	CZhList c_connections; /**< list of sdp_connection_t * */
	CZhList b_bandwidths;  /**< list of sdp_bandwidth_t * */
	CZhList a_attributes;  /**< list of sdp_attribute_t * */
	sdp_key* k_key;   		/**< key informations */
};

class CSdpMessage  
{
public:
	CSdpMessage();
	virtual ~CSdpMessage();

	int		Init();
	void	Free();
	int		Parse(const char* buf);
	
	int		ToStr(char** dest);

	int		Clone(CSdpMessage** dest);

protected:
	int sdp_message_parse_v(char* buf, char** next);
	int sdp_message_parse_o(char* buf, char** next);
	int sdp_message_parse_s(char* buf, char** next);
	int sdp_message_parse_i(char* buf, char** next);
	int sdp_message_parse_u(char* buf, char** next);
	int sdp_message_parse_e(char* buf, char** next);
	int sdp_message_parse_p(char* buf, char** next);
	int sdp_message_parse_c(char* buf, char** next);
	int sdp_message_parse_b(char* buf, char** next);
	int sdp_message_parse_t(char* buf, char** next);
	int sdp_message_parse_r(char* buf, char** next);
	int sdp_message_parse_z(char* buf, char** next);
	int sdp_message_parse_k(char* buf, char** next);
	int sdp_message_parse_a(char* buf, char** next);
	int sdp_message_parse_m(char* buf, char** next);
public:
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_v_version_set(char* value);
	/**
	 * Get the version ('v' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_v_version_get();
	/**
	 * Set the origin field in a SDP packet.
	 * @param sdp The element to work on.
	 * @param username The token value.
	 * @param sess_id The token value.
	 * @param sess_version The token value.
	 * @param nettype The token value.
	 * @param addrtype The token value.
	 * @param addr The token value.
	 */
	int sdp_message_o_origin_set(char* username,
		char* sess_id, char* sess_version, char* nettype, char* addrtype,
		char* addr);
	/**
	 * Get the username ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_username_get();
	/**
	 * Get the session id ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_sess_id_get();
	/**
	 * Get the session version ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_sess_version_get();
	/**
	 * Get the nettype ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_nettype_get();
	/**
	 * Get the addrtype ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_addrtype_get();
	/**
	 * Get the addr ('o' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_o_addr_get();
	/**
	 * Set the session name in a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_s_name_set(char* value);
	/**
	 * Get the session name ('s' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_s_name_get();
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param value The token value.
	 */
	int sdp_message_i_info_set(int pos_media, char* value);
	/**
	 * Get the session info ('i' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 */
	char* sdp_message_i_info_get(int pos_media);
	/**
	 * Set the session info in a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_u_uri_set(char* value);
	/**
	 * Get the uri ('u' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_u_uri_get();
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_e_email_add(char* value);
	/**
	 * OBSOLETE: see sdp_message_e_email_get
	 * @def sdp_e_email_get
	 */
#define sdp_e_email_get sdp_message_e_email_get
	/**
	 * Get one of the email ('e' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos the index of the email line.
	 */
	char* sdp_message_e_email_get(int pos);
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_p_phone_add(char* value);
	/**
	 * Get one of the phone ('p' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos the index of the phone line.
	 */
	char* sdp_message_p_phone_get(int pos);
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param nettype The token value.
	 * @param addrtype The token value.
	 * @param addr The token value.
	 * @param addr_multicast_ttl The token value.
	 * @param addr_multicast_int The token value.
	 */
	int sdp_message_c_connection_add(int pos_media,
		char* nettype, char* addrtype, char* addr, char* addr_multicast_ttl,
		char* addr_multicast_int);
#ifndef DOXYGEN
	/* this method should be internal only... */
	sdp_connection* sdp_message_connection_get(int pos_media, int pos);
#endif
	/**
	 * Get the network type ('c' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the connection element list..
	 */
	char* sdp_message_c_nettype_get(int pos_media, int pos);
	/**
	 * Get the address type ('c' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the connection element list..
	 */
	char* sdp_message_c_addrtype_get(int pos_media,
		int pos);
	/**
	 * Get the address ('c' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the connection element list..
	 */
	char* sdp_message_c_addr_get(int pos_media, int pos);
	/**
	 * Get the multicast ttl ('c' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the connection element list..
	 */
	char* sdp_message_c_addr_multicast_ttl_get(int pos_media, int pos);
	/**
	 * Get the multicast int info ('c' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the connection element list..
	 */
	char* sdp_message_c_addr_multicast_int_get(int pos_media, int pos);
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param bwtype The token value.
	 * @param bandwidth The token value.
	 */
	int sdp_message_b_bandwidth_add(int pos_media,
		char* bwtype, char* bandwidth);
	/**
	 * Get the bandwidth ('b' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the bandwidth element list..
	 */
	sdp_bandwidth* sdp_message_bandwidth_get(int pos_media, int pos);
	/**
	 * Get the bandwidth type ('b' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the bandwidth element list..
	 */
	char* sdp_message_b_bwtype_get(int pos_media, int pos);
	/**
	 * Get the bandwidth value ('b' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The index in the bandwidth element list..
	 */
	char* sdp_message_b_bandwidth_get(int pos_media,
		int pos);
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param start The token value.
	 * @param stop The token value.
	 */
	int sdp_message_t_time_descr_add(char* start,
		char* stop);
	/**
	 * Get the start time value ('t' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_td The time description line number.
	 */
	char* sdp_message_t_start_time_get(int pos_td);
	/**
	 * Get the stop time value ('t' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_td The time description line number.
	 */
	char* sdp_message_t_stop_time_get(int pos_td);
	/**
	 * Set the repeat information ('r' field) in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_time_descr index of t field.
	 * @param value The token value.
	 */
	int sdp_message_r_repeat_add(int pos_time_descr,
		char* value);
	/**
	 * Get the repeat information ('r' field) in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_time_descr index of t field.
	 * @param pos_repeat index of element in the 'r' field.
	 */
	char* sdp_message_r_repeat_get(int pos_time_descr,
		int pos_repeat);
	/**
	 * Set the adjustments ('z' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param value The token value.
	 */
	int sdp_message_z_adjustments_set(char* value);
	/**
	 * Get the adjustments ('z' field) of a SDP packet.
	 * @param sdp The element to work on.
	 */
	char* sdp_message_z_adjustments_get();
	/**
	 * Add a key in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media index of m field.
	 * @param keytype The token value.
	 * @param keydata The token value.
	 */
	int sdp_message_k_key_set(int pos_media,
		char* keytype, char* keydata);
	/**
	 * Get the key type ('k' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 */
	char* sdp_message_k_keytype_get(int pos_media);
	/**
	 * Get the key value ('k' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 */
	char* sdp_message_k_keydata_get(int pos_media);
	/**
	 * Set the version in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param att_field The token value.
	 * @param att_value The token value.
	 */
	int sdp_message_a_attribute_add(int pos_media,
		char* att_field, char* att_value);
	/**
	 * delete all attribute fields specified by att_field.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param att_field The value to remove.
	 */
	int sdp_message_a_attribute_del(int pos_media,
		char* att_field);
	/**
	 * delete one specific attribute fields specified by att_field.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param att_field The value to remove.
	 * @param pos_attr The index of attribute to remove.
	 */
	int sdp_message_a_attribute_del_at_index(int pos_media, char* att_field, int pos_attr);
	/**
	* Get one of the attribute ('a' field) of a SDP packet.
	* @param sdp The element to work on.
	* @param pos_media The media line number.
	* @param pos The attribute line number.
	*/
	sdp_attribute* sdp_message_attribute_get(int pos_media, int pos);
	/**
	 * Get the attribute name ('a' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The attribute line number.
	 */
	char* sdp_message_a_att_field_get(int pos_media,
		int pos);
	/**
	 * Get the attribute value ('a' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The media line number.
	 * @param pos The attribute line number.
	 */
	char* sdp_message_a_att_value_get(int pos_media,
		int pos);
	/**
	 * Check if there is more media lines a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos The attribute line number.
	 */
	int sdp_message_endof_media(int pos);
	/**
	 * Add a media line in a SDP packet.
	 * @param sdp The element to work on.
	 * @param media The token value.
	 * @param port The token value.
	 * @param number_of_port The token value.
	 * @param proto The token value.
	 */
	int sdp_message_m_media_add(char* media, char* port,
		char* number_of_port, char* proto);
	/**
	 * Get the media type ('m' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 */
	char* sdp_message_m_media_get(int pos_media);
	/**
	 * Get the port number ('m' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 */
	char* sdp_message_m_port_get(int pos_media);
	/**
	 * Set the port number ('m' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param port The new port to set (must be allocated with osip_malloc)
	 */
	int sdp_message_m_port_set(int pos_media, char* port);
	/**
	* Get the number of port ('m' field) of a SDP packet.
	* @param sdp The element to work on.
	* @param pos_media The line number.
	*/
	char* sdp_message_m_number_of_port_get(int pos_media);
	/**
	 * Get the protocol ('m' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 */
	char* sdp_message_m_proto_get(int pos_media);
	/**
	 * Set the payload in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param payload The token value.
	 */
	int sdp_message_m_payload_add(int pos_media,
		char* payload);
	/**
	 * Get one of the payload number ('m' field) of a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param pos The i th payload element.
	 */
	char* sdp_message_m_payload_get(int pos_media, int pos);
	/**
	 * Remove a payload in a SDP packet.
	 * @param sdp The element to work on.
	 * @param pos_media The line number.
	 * @param pos The position of the payload in the media line.
	 */
	int sdp_message_m_payload_del(int pos_media, int pos);

	char* v_version;			/**< version header */
	char* o_username;   		/**< Username */
	char* o_sess_id;			/**< Identifier for session */
	char* o_sess_version;   	/**< Version of session */
	char* o_nettype;			/**< Network type */
	char* o_addrtype;   		/**< Address type */
	char* o_addr;   			/**< Address */
	char* s_name;   			/**< Subject header */
	char* i_info;   			/**< Information header */
	char* u_uri;				/**< Uri header */
	CZhList e_emails;      /**< list of mail address */
	CZhList p_phones;      /**< list of phone numbers * */
	sdp_connection* c_connection;   /**< Connection information */
	CZhList b_bandwidths;  /**< list of bandwidth info (sdp_bandwidth_t) */
	CZhList t_descrs;      /**< list of time description (sdp_time_descr_t) */
	char* z_adjustments;		/**< Time adjustment header */
	sdp_key* k_key;   		/**< Key information header */
	CZhList a_attributes;  /**< list of global attributes (sdp_attribute_t) */
	CZhList m_medias;      /**< list of supported media (sdp_media_t) */

};

#endif // !defined(AFX_SDPMESSAGE_H__6DC637FF_0A1F_47B9_BCD5_1A947E037D0D__INCLUDED_)
