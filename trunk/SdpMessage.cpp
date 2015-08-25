// SdpMessage.cpp: implementation of the CSdpMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "SdpMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ERR_ERROR   -1  		/* bad header */
#define ERR_DISCARD  0  		/* wrong header */
#define WF  		 1  		/* well formed header */



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CSdpMessage::sdp_message_parse_v(char* buf, char** next)
{
	char* equal;
	char* crlf;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	if (equal == buf)
		return ERR_DISCARD;

	/* check if header is "v" */
	if (equal[-1] != 'v')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/*v=\r ?? bad header */
	this->v_version = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(this->v_version, equal + 1, crlf - (equal + 1));

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_o(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	int i;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "o" */
	if (equal[-1] != 'o')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* o=\r ?? bad header */

	tmp = equal + 1;
	/* o=username sess-id sess-version nettype addrtype addr */

	/* useranme can contain any char (ascii) except "space" and CRLF */
#ifdef FIREFLY_BUG_SUPPORT
	if (tmp[0] == ' ')
	{
		this->o_username = zstr_strdup("firefly");
		tmp++;
	}
	else
	{
		i = __osip_set_next_token(&(this->o_username), tmp, ' ', &tmp_next);
		if (i != 0)
			return -1;
		tmp = tmp_next;
	}
#else
	i = __osip_set_next_token(&(this->o_username), tmp, ' ', &tmp_next);
	if (i != 0)
		return -1;
	tmp = tmp_next;
#endif

	/* sess_id contains only numeric characters */
	i = __osip_set_next_token(&(this->o_sess_id), tmp, ' ', &tmp_next);
	if (i != 0)
		return -1;
	tmp = tmp_next;

	/* sess_id contains only numeric characters */
	i = __osip_set_next_token(&(this->o_sess_version), tmp, ' ', &tmp_next);
	if (i != 0)
		return -1;
	tmp = tmp_next;

	/* nettype is "IN" but will surely be extented!!! assume it's some alpha-char */
	i = __osip_set_next_token(&(this->o_nettype), tmp, ' ', &tmp_next);
	if (i != 0)
		return -1;
	tmp = tmp_next;

	/* addrtype  is "IP4" or "IP6" but will surely be extented!!! */
	i = __osip_set_next_token(&(this->o_addrtype), tmp, ' ', &tmp_next);
	if (i != 0)
		return -1;
	tmp = tmp_next;

	/* addr  is "IP4" or "IP6" but will surely be extented!!! */
	i = __osip_set_next_token(&(this->o_addr), tmp, '\r', &tmp_next);
	if (i != 0)
	{
		/* could it be "\n" only??? rfc says to accept CR or LF instead of CRLF */
		i = __osip_set_next_token(&(this->o_addr), tmp, '\n', &tmp_next);
		if (i != 0)
			return -1;
	}

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_s(char* buf, char** next)
{
	char* equal;
	char* crlf;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "s" */
	if (equal[-1] != 's')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
#ifdef FIREFLY_BUG_SUPPORT
	if (crlf == equal + 1)
	{
		this->s_name = zstr_strdup(" ");
		if (crlf[1] == '\n')
			*next = crlf + 2;
		else
			*next = crlf + 1;
		return WF;  			  /* o=\r ?? bad header */
	}
#else
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* o=\r ?? bad header */
#endif

	/* s=text */

	/* text is interpreted as ISO-10646 UTF8! */
	/* using ISO 8859-1 requires "a=charset:ISO-8859-1 */
	this->s_name = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(this->s_name, equal + 1, crlf - (equal + 1));

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_i(char* buf, char** next)
{
	char* equal;
	char* crlf;
	int i;
	char* i_info;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "i" */
	if (equal[-1] != 'i')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* o=\r ?? bad header */

	/* s=text */

	/* text is interpreted as ISO-10646 UTF8! */
	/* using ISO 8859-1 requires "a=charset:ISO-8859-1 */
	i_info = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(i_info, equal + 1, crlf - (equal + 1));

	/* add the bandwidth at the correct place:
	   if there is no media line yet, then the "b=" is the
	   global one.
	 */
	i = this->m_medias.GetSize();
	if (i == 0)
		this->i_info = i_info;
	else
	{
		sdp_media* last_sdp_media = (sdp_media*) this->m_medias.GetAt(i - 1);
		last_sdp_media->i_info = i_info;
	}

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_u(char* buf, char** next)
{
	char* equal;
	char* crlf;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "u" */
	if (equal[-1] != 'u')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* u=\r ?? bad header */

	/* u=uri */
	/* we assume this is a URI */
	this->u_uri = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(this->u_uri, equal + 1, crlf - (equal + 1));

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_e(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* e_email;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "e" */
	if (equal[-1] != 'e')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* e=\r ?? bad header */

	/* e=email */
	/* we assume this is an EMAIL-ADDRESS */
	e_email = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(e_email, equal + 1, crlf - (equal + 1));

	this->e_emails.Add(e_email, -1);

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_p(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* p_phone;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "p" */
	if (equal[-1] != 'p')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* p=\r ?? bad header */

	/* e=email */
	/* we assume this is an EMAIL-ADDRESS */
	p_phone = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(p_phone, equal + 1, crlf - (equal + 1));

	this->p_phones.Add(p_phone, -1);

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_c(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	sdp_connection* c_header;
	int i;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "c" */
	if (equal[-1] != 'c')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* c=\r ?? bad header */

	tmp = equal + 1;
	c_header = new sdp_connection();
	if (NULL == c_header)
		return ERR_ERROR;
	/* c=nettype addrtype (multicastaddr | addr) */

	/* nettype is "IN" and will be extended */
	i = __osip_set_next_token(&(c_header->c_nettype), tmp, ' ', &tmp_next);
	if (i != 0)
	{
		delete (c_header);
		return -1;
	}
	tmp = tmp_next;

	/* nettype is "IP4" or "IP6" and will be extended */
	i = __osip_set_next_token(&(c_header->c_addrtype), tmp, ' ', &tmp_next);
	if (i != 0)
	{
		delete (c_header);
		return -1;
	}
	tmp = tmp_next;

	/* there we have a multicast or unicast address */
	/* multicast can be ip/ttl [/integer] */
	/* unicast is FQDN or ip (no ttl, no integer) */

	/* is MULTICAST? */
	{
		char* slash = strchr(tmp, '/');

		if (slash != NULL && slash < crlf)  /* it's a multicast address! */
		{
			i = __osip_set_next_token(&(c_header->c_addr), tmp, '/', &tmp_next);
			if (i != 0)
			{
				delete (c_header);
				return -1;
			}
			tmp = tmp_next;
			slash = strchr(slash + 1, '/');
			if (slash != NULL && slash < crlf)  	/* optionnal integer is there! */
			{
				i = __osip_set_next_token(&(c_header->c_addr_multicast_ttl),
						tmp, '/', &tmp_next);
				if (i != 0)
				{
					delete(c_header);
					return -1;
				}
				tmp = tmp_next;
				i = __osip_set_next_token(&(c_header->c_addr_multicast_int),
						tmp, '\r', &tmp_next);
				if (i != 0)
				{
					i = __osip_set_next_token(&(c_header->c_addr_multicast_int),
							tmp, '\n', &tmp_next);
					if (i != 0)
					{
						delete(c_header);
						return -1;
					}
				}
			}
			else
			{
				i = __osip_set_next_token(&(c_header->c_addr_multicast_ttl),
						tmp, '\r', &tmp_next);
				if (i != 0)
				{
					i = __osip_set_next_token(&(c_header->c_addr_multicast_ttl),
							tmp, '\n', &tmp_next);
					if (i != 0)
					{
						delete (c_header);
						return -1;
					}
				}
			}
		}
		else
		{
			/* in this case, we have a unicast address */
			i = __osip_set_next_token(&(c_header->c_addr), tmp, '\r',
					&tmp_next);
			if (i != 0)
			{
				i = __osip_set_next_token(&(c_header->c_addr), tmp, '\n',
						&tmp_next);
				if (i != 0)
				{
					delete (c_header);
					return -1;
				}
			}
		}
	}

	/* add the connection at the correct place:
	   if there is no media line yet, then the "c=" is the
	   global one.
	 */
	i = this->m_medias.GetSize();
	if (i == 0)
		this->c_connection = c_header;
	else
	{
		sdp_media* last_sdp_media = (sdp_media*) this->m_medias.GetAt( i - 1);
		last_sdp_media->c_connections.Add(c_header, -1);
	}
	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_b(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	int i;
	sdp_bandwidth* b_header;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "b" */
	if (equal[-1] != 'b')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* b=\r ?? bad header */

	tmp = equal + 1;
	/* b = bwtype: bandwidth */
	b_header = new sdp_bandwidth();
	if (b_header == NULL)
		return ERR_ERROR;

	/* bwtype is alpha-numeric */
	i = __osip_set_next_token(&(b_header->b_bwtype), tmp, ':', &tmp_next);
	if (i != 0)
	{
		delete (b_header);
		return -1;
	}
	tmp = tmp_next;

	i = __osip_set_next_token(&(b_header->b_bandwidth), tmp, '\r', &tmp_next);
	if (i != 0)
	{
		i = __osip_set_next_token(&(b_header->b_bandwidth), tmp, '\n',&tmp_next);
		if (i != 0)
		{
			delete (b_header);
			return -1;
		}
	}

	/* add the bandwidth at the correct place:
	   if there is no media line yet, then the "b=" is the
	   global one.
	 */
	i = this->m_medias.GetSize();
	if (i == 0)
		this->b_bandwidths.Add(b_header, -1);
	else
	{
		sdp_media* last_sdp_media = (sdp_media*) this->m_medias.GetAt(i - 1);
		last_sdp_media->b_bandwidths.Add(b_header, -1);
	}

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_t(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	int i;
	sdp_time_descr* t_header;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "t" */
	if (equal[-1] != 't')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* t=\r ?? bad header */

	tmp = equal + 1;
	/* t = start_time stop_time */
	t_header = new sdp_time_descr();
	if (NULL == t_header)
		return ERR_ERROR;

	i = __osip_set_next_token(&(t_header->t_start_time), tmp, ' ', &tmp_next);
	if (i != 0)
	{
		delete(t_header);
		return -1;
	}
	tmp = tmp_next;

	i = __osip_set_next_token(&(t_header->t_stop_time), tmp, '\r', &tmp_next);
	if (i != 0)
	{
		i = __osip_set_next_token(&(t_header->t_stop_time), tmp, '\n',
				&tmp_next);
		if (i != 0)
		{
			delete(t_header);
			return -1;
		}
	}

	/* add the new time_description header */
	this->t_descrs.Add(t_header, -1);

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_r(char* buf, char** next)
{
	char* equal;
	char* crlf;
	int index;
	char* r_header;
	sdp_time_descr* t_descr;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "r" */
	if (equal[-1] != 'r')
		return ERR_DISCARD;

	index = this->t_descrs.GetSize();
	if (index == 0)
		return ERR_ERROR;   		/* r field can't come alone! */

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* r=\r ?? bad header */

	/* r=far too complexe and somewhat useless... I don't parse it! */
	r_header = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(r_header, equal + 1, crlf - (equal + 1));

	/* r field carry information for the last "t" field */
	t_descr = (sdp_time_descr *) this->t_descrs.GetAt(index - 1);
	t_descr->r_repeats.Add(r_header, -1);

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_z(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* z_header;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "z" */
	if (equal[-1] != 'z')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* z=\r ?? bad header */

	/* z=somewhat useless... I don't parse it! */
	z_header = (char*)mm_malloc(crlf - (equal + 1) + 1);
	zstr_strncpy(z_header, equal + 1, crlf - (equal + 1));

	this->z_adjustments = z_header;

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_k(char* buf, char** next)
{
	char* equal;
	char* crlf;
	int i;
	char* colon;
	sdp_key* k_header;
	char* tmp;
	char* tmp_next;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "k" */
	if (equal[-1] != 'k')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* k=\r ?? bad header */

	tmp = equal + 1;

	k_header = new sdp_key();
	if (NULL == k_header)
		return ERR_ERROR;
	/* k=key-type[:key-data] */

	/* is there any key-data? */
	colon = strchr(equal + 1, ':');
	if ((colon != NULL) && (colon < crlf))
	{
		/* att-field is alpha-numeric */
		i = __osip_set_next_token(&(k_header->k_keytype), tmp, ':', &tmp_next);
		if (i != 0)
		{
			delete(k_header);
			return -1;
		}
		tmp = tmp_next;

		i = __osip_set_next_token(&(k_header->k_keydata), tmp, '\r', &tmp_next);
		if (i != 0)
		{
			i = __osip_set_next_token(&(k_header->k_keydata), tmp, '\n',
					&tmp_next);
			if (i != 0)
			{
				delete(k_header);
				return -1;
			}
		}
	}
	else
	{
		i = __osip_set_next_token(&(k_header->k_keytype), tmp, '\r', &tmp_next);
		if (i != 0)
		{
			i = __osip_set_next_token(&(k_header->k_keytype), tmp, '\n',
					&tmp_next);
			if (i != 0)
			{
				delete(k_header);
				return -1;
			}
		}
	}

	/* add the key at the correct place:
	   if there is no media line yet, then the "k=" is the
	   global one.
	 */
	i = this->m_medias.GetSize();
	if (i == 0)
		this->k_key = k_header;
	else
	{
		sdp_media* last_sdp_media = (sdp_media*)this->m_medias.GetAt(i - 1);
		last_sdp_media->k_key = k_header;
	}

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_a(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	int i;
	sdp_attribute* a_attribute;
	char* colon;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "a" */
	if (equal[-1] != 'a')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* a=\r ?? bad header */

	tmp = equal + 1;

	a_attribute = new sdp_attribute();
	if (a_attribute == NULL)
		return ERR_ERROR;

	/* a=att-field[:att-value] */

	/* is there any att-value? */
	colon = strchr(equal + 1, ':');
	if ((colon != NULL) && (colon < crlf))
	{
		/* att-field is alpha-numeric */
		i = __osip_set_next_token(&(a_attribute->a_att_field), tmp, ':',
				&tmp_next);
		if (i != 0)
		{
			delete(a_attribute);
			return -1;
		}
		tmp = tmp_next;

		i = __osip_set_next_token(&(a_attribute->a_att_value), tmp, '\r',
				&tmp_next);
		if (i != 0)
		{
			i = __osip_set_next_token(&(a_attribute->a_att_value), tmp, '\n',
					&tmp_next);
			if (i != 0)
			{
				delete(a_attribute);
				return -1;
			}
		}
	}
	else
	{
		i = __osip_set_next_token(&(a_attribute->a_att_field), tmp, '\r',
				&tmp_next);
		if (i != 0)
		{
			i = __osip_set_next_token(&(a_attribute->a_att_field), tmp, '\n',
					&tmp_next);
			if (i != 0)
			{
				delete(a_attribute);
				return -1;
			}
		}
	}

	/* add the attribute at the correct place:
	   if there is no media line yet, then the "a=" is the
	   global one.
	 */
	i = this->m_medias.GetSize();
	if (i == 0)
		this->a_attributes.Add(a_attribute, -1);
	else
	{
		sdp_media* last_sdp_media = (sdp_media*) this->m_medias.GetAt(i - 1);
		last_sdp_media->a_attributes.Add(a_attribute, -1);
	}

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

int CSdpMessage::sdp_message_parse_m(char* buf, char** next)
{
	char* equal;
	char* crlf;
	char* tmp;
	char* tmp_next;
	int i;
	sdp_media* m_header;
	char* slash;
	char* space;

	*next = buf;

	equal = buf;
	while ((*equal != '=') && (*equal != '\0'))
		equal++;
	if (*equal == '\0')
		return ERR_ERROR;

	/* check if header is "m" */
	if (equal[-1] != 'm')
		return ERR_DISCARD;

	crlf = equal + 1;

	while ((*crlf != '\r') && (*crlf != '\n') && (*crlf != '\0'))
		crlf++;
	if (*crlf == '\0')
		return ERR_ERROR;
	if (crlf == equal + 1)
		return ERR_ERROR;   		/* a=\r ?? bad header */

	tmp = equal + 1;

	m_header = new sdp_media();
	if (m_header == NULL)
		return ERR_ERROR;

	/* m=media port ["/"integer] proto *(payload_number) */

	/* media is "audio" "video" "application" "data" or other... */
	i = __osip_set_next_token(&(m_header->m_media), tmp, ' ', &tmp_next);
	if (i != 0)
	{
		delete(m_header);
		return -1;
	}
	tmp = tmp_next;

	slash = strchr(tmp, '/');
	space = strchr(tmp, ' ');
	if (space == NULL)  		  /* not possible! */
	{
		delete(m_header);
		return ERR_ERROR;
	}
	if ((slash != NULL) && (slash < space))
	{
		/* a number of port is specified! */
		i = __osip_set_next_token(&(m_header->m_port), tmp, '/', &tmp_next);
		if (i != 0)
		{
			delete(m_header);
			return -1;
		}
		tmp = tmp_next;

		i = __osip_set_next_token(&(m_header->m_number_of_port), tmp, ' ',
				&tmp_next);
		if (i != 0)
		{
			delete(m_header);
			return -1;
		}
		tmp = tmp_next;
	}
	else
	{
		i = __osip_set_next_token(&(m_header->m_port), tmp, ' ', &tmp_next);
		if (i != 0)
		{
			delete(m_header);
			return -1;
		}
		tmp = tmp_next;
	}

	i = __osip_set_next_token(&(m_header->m_proto), tmp, ' ', &tmp_next);
	if (i != 0)
	{
		/* a few stack don't add SPACE after m_proto when rejecting all payloads */
		i = __osip_set_next_token(&(m_header->m_proto), tmp, '\r', &tmp_next);
		if (i != 0)
		{
			i = __osip_set_next_token(&(m_header->m_proto), tmp, '\n',
					&tmp_next);
			if (i != 0)
			{
				delete(m_header);
				return -1;
			}
		}
	}
	tmp = tmp_next;
	{
		char* str;
		int more_space_before_crlf;

		space = strchr(tmp + 1, ' ');
		if (space == NULL)
			more_space_before_crlf = 1;
		else if ((space != NULL) && (space > crlf))
			more_space_before_crlf = 1;
		else
			more_space_before_crlf = 0;
		while (more_space_before_crlf == 0)
		{
			i = __osip_set_next_token(&str, tmp, ' ', &tmp_next);
			if (i != 0)
			{
				delete(m_header);
				return -1;
			}
			tmp = tmp_next;
			m_header->m_payloads.Add(str, -1);

			space = strchr(tmp + 1, ' ');
			if (space == NULL)
				more_space_before_crlf = 1;
			else if ((space != NULL) && (space > crlf))
				more_space_before_crlf = 1;
			else
				more_space_before_crlf = 0;
		}
		if (tmp_next < crlf)
		{
			/* tmp_next is still less than clrf: no space */
			i = __osip_set_next_token(&str, tmp, '\r', &tmp_next);
			if (i != 0)
			{
				i = __osip_set_next_token(&str, tmp, '\n', &tmp_next);
				if (i != 0)
				{
					delete(m_header);
					return -1;
				}
			}
			m_header->m_payloads.Add(str, -1);
		}
	}

	this->m_medias.Add(m_header, -1);

	if (crlf[1] == '\n')
		*next = crlf + 2;
	else
		*next = crlf + 1;
	return WF;
}

CSdpMessage::CSdpMessage()
{
	Init();
}

CSdpMessage::~CSdpMessage()
{
	Free();
}

int CSdpMessage::Init()
{
	(this)->v_version = NULL;
	(this)->o_username = NULL;
	(this)->o_sess_id = NULL;
	(this)->o_sess_version = NULL;
	(this)->o_nettype = NULL;
	(this)->o_addrtype = NULL;
	(this)->o_addr = NULL;
	(this)->s_name = NULL;
	(this)->i_info = NULL;
	(this)->u_uri = NULL;
	
	(this)->c_connection = NULL;
	
	(this)->z_adjustments = NULL;
	(this)->k_key = NULL;
	
	return 0;
}

void CSdpMessage::Free()
{
	mm_free(this->v_version);
	mm_free(this->o_username);
	mm_free(this->o_sess_id);
	mm_free(this->o_sess_version);
	mm_free(this->o_nettype);
	mm_free(this->o_addrtype);
	mm_free(this->o_addr);
	mm_free(this->s_name);
	mm_free(this->i_info);
	mm_free(this->u_uri);
	
	
	mm_free(this->z_adjustments);	
}

int CSdpMessage::Parse( const char* buf )
{
		/* In SDP, headers must be in the right order */
	/* This is a simple example
	   v=0
	   o=user1 53655765 2353687637 IN IP4 128.3.4.5
	   s=Mbone Audio
	   i=Discussion of Mbone Engineering Issues
	   e=mbone@somewhere.com
	   c=IN IP4 224.2.0.1/127
	   t=0 0
	   m=audio 3456 RTP/AVP 0
	   a=rtpmap:0 PCMU/8000
	 */

	char* next_buf;
	char* ptr;
	int i;

	ptr = (char *) buf;
	/* mandatory */
	i = sdp_message_parse_v(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	else if (0 == i)			  /* header is not "v" */
		return -1;
	ptr = next_buf;

	/* adtech phone use the wrong ordering and place "s" before "o" */
	i = sdp_message_parse_s(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	/* else if (0==i) header is not "s" */
	/* else ADTECH PHONE DETECTED */

	ptr = next_buf;



	i = sdp_message_parse_o(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	else if (0 == i)			  /* header is not "o" */
		return -1;
	ptr = next_buf;

	i = sdp_message_parse_s(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	else if (0 == i)			  /* header is not "s" */
		/* return -1; */
	{
	
	}
	ptr = next_buf;

	i = sdp_message_parse_i(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	ptr = next_buf;

	i = sdp_message_parse_u(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	ptr = next_buf;

	i = 1;
	while (i == 1)
	{
		i = sdp_message_parse_e(ptr, &next_buf);
		if (i == -1)			  /* header is bad */
			return -1;
		ptr = next_buf;
	}
	i = 1;
	while (i == 1)
	{
		i = sdp_message_parse_p(ptr, &next_buf);
		if (i == -1)			  /* header is bad */
			return -1;
		ptr = next_buf;
	}

	/* rfc2327: there should be at least of email or phone number! */
	if (this->e_emails.GetSize() == 0 &&
		this->p_phones.GetSize() == 0)
	{
		
	}

	i = sdp_message_parse_c(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	ptr = next_buf;

	i = 1;
	while (i == 1)
	{
		i = sdp_message_parse_b(ptr, &next_buf);
		if (i == -1)			  /* header is bad */
			return -1;
		ptr = next_buf;
	}

	/* 1 or more "t" header + 0 or more "r" header for each "t" header */
	i = sdp_message_parse_t(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	else if (i == ERR_DISCARD)
		return -1;  				/* t is mandatory */
	ptr = next_buf;

	if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
		return 0;

	i = 1;
	while (i == 1)  			  /* is a "r" header */
	{
		i = sdp_message_parse_r(ptr, &next_buf);
		if (i == -1)			  /* header is bad */
			return -1;
		ptr = next_buf;
		if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
			return 0;
	}
	{
		int more_t_header = 1;

		i = sdp_message_parse_t(ptr, &next_buf);
		if (i == -1)				/* header is bad */
			return -1;
		ptr = next_buf;

		if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
			return 0;

		while (more_t_header == 1)
		{
			i = 1;
			while (i == 1)  		/* is a "r" header */
			{
				i = sdp_message_parse_r(ptr, &next_buf);
				if (i == -1)		/* header is bad */
					return -1;
				ptr = next_buf;
				if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
					return 0;
			}

			i = sdp_message_parse_t(ptr, &next_buf);
			if (i == -1)			/* header is bad */
				return -1;
			else if (i == ERR_DISCARD)
				more_t_header = 0;
			else
				more_t_header = 1;    /* no more "t" headers */
			ptr = next_buf;
			if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
				return 0;
		}
	}

	i = sdp_message_parse_z(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	ptr = next_buf;
	if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
		return 0;

	i = sdp_message_parse_k(ptr, &next_buf);
	if (i == -1)				  /* header is bad */
		return -1;
	ptr = next_buf;
	if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
		return 0;

	/* 0 or more "a" header */
	i = 1;
	while (i == 1)  			  /* no more "a" header */
	{
		i = sdp_message_parse_a(ptr, &next_buf);
		if (i == -1)			  /* header is bad */
			return -1;
		ptr = next_buf;
		if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
			return 0;
	}
	/* 0 or more media headers */
	{
		int more_m_header = 1;

		while (more_m_header == 1)
		{
			more_m_header = sdp_message_parse_m(ptr, &next_buf);
			if (more_m_header == -1)		/* header is bad */
				return -1;
			ptr = next_buf;
			if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
				return 0;

			i = sdp_message_parse_i(ptr, &next_buf);
			if (i == -1)			/* header is bad */
				return -1;
			ptr = next_buf;
			if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
				return 0;

			i = 1;
			while (i == 1)
			{
				i = sdp_message_parse_c(ptr, &next_buf);
				if (i == -1)		/* header is bad */
					return -1;
				ptr = next_buf;
				if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
					return 0;
			}

			i = 1;
			while (i == 1)
			{
				i = sdp_message_parse_b(ptr, &next_buf);
				if (i == -1)		/* header is bad */
					return -1;
				ptr = next_buf;
				if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
					return 0;
			}
			i = sdp_message_parse_k(ptr, &next_buf);
			if (i == -1)			/* header is bad */
				return -1;
			ptr = next_buf;
			if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
				return 0;
			/* 0 or more a headers */
			i = 1;
			while (i == 1)
			{
				i = sdp_message_parse_a(ptr, &next_buf);
				if (i == -1)		/* header is bad */
					return -1;
				ptr = next_buf;
				if (*ptr == '\0' || (*ptr == '\r') || (*ptr == '\n'))
					return 0;
			}
		}
	}

	return 0;	
}

int CSdpMessage::Clone( CSdpMessage** dest )
{
	int i;
	char* body;
	
	*dest = new CSdpMessage();
	if (dest == NULL)
		return -1;
	
	i = this->ToStr(&body);
	if (i != 0)
		goto error_sc1;
	
	i = (*dest)->Parse(body);
	delete(body);
	if (i != 0)
		goto error_sc1;
	
	return 0;
	
error_sc1:
	delete(*dest);
	return -1;	
}

int CSdpMessage::ToStr( char** dest )
{
	int size;
	int pos;
	char* tmp;
	char* stringTemp;

	*dest = NULL;
	if (!this || this->v_version == NULL)
		return -1;
	if (this->o_username == NULL ||
		this->o_sess_id == NULL ||
		this->o_sess_version == NULL ||
		this->o_nettype == NULL ||
		this->o_addrtype == NULL ||
		this->o_addr == NULL)
		return -1;

	/* RFC says "s=" is mandatory... rfc2543 (SIP) recommends to
	   accept SDP datas without s_name... as some buggy implementations
	   often forget it...
	 */
	/* if (this->s_name == NULL)
	   return -1; */

	size = BODY_MESSAGE_MAX_SIZE;
	tmp = (char *) mm_malloc(size);
	stringTemp = tmp;

	tmp = __osip_sdp_append_string(stringTemp, size, tmp, "v=");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->v_version);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, "o=");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_username);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_sess_id);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_sess_version);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_nettype);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_addrtype);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->o_addr);
	tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	if (this->s_name != NULL)
	{
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "s=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->s_name);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	}
	if (this->i_info != NULL)
	{
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "i=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->i_info);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	}
	if (this->u_uri != NULL)
	{
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "u=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->u_uri);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	}
	pos = 0;
	while (!this->e_emails.IsListEof(pos))
	{
		char* email = (char*) this->e_emails.GetAt(pos);

		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "e=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, email);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
		pos++;
	}
	pos = 0;
	while (!this->p_phones.IsListEof(pos))
	{
		char* phone = (char*) this->p_phones.GetAt(pos);

		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "p=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, phone);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
		pos++;
	}
	if (this->c_connection != NULL)
	{
		char* next_tmp;
		int i;

		i = this->c_connection->sdp_append_connection(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete(stringTemp);
			return -1;
		}
		tmp = next_tmp;
	}
	pos = 0;
	while (!this->b_bandwidths.IsListEof(pos))
	{
		sdp_bandwidth* header = (sdp_bandwidth*) this->b_bandwidths.GetAt(pos);
		char* next_tmp;
		int i;

		i = header->sdp_append_bandwidth(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete(stringTemp);
			return -1;
		}
		tmp = next_tmp;
		pos++;
	}

	pos = 0;
	while (!this->t_descrs.IsListEof(pos))
	{
		sdp_time_descr* header = (sdp_time_descr*)this->t_descrs.GetAt(pos);
		char* next_tmp;
		int i;

		i = header->sdp_append_time_descr(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete(stringTemp);
			return -1;
		}
		tmp = next_tmp;
		pos++;
	}

	if (this->z_adjustments != NULL)
	{
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, "z=");
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, this->z_adjustments);
		tmp = __osip_sdp_append_string(stringTemp, size, tmp, CRLF);
	}

	if (this->k_key != NULL)
	{
		char* next_tmp;
		int i;

		i = this->k_key->sdp_append_key(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete (stringTemp);
			return -1;
		}
		tmp = next_tmp;
	}

	pos = 0;
	while (!this->a_attributes.IsListEof(pos))
	{
		sdp_attribute* header = (sdp_attribute*) this->a_attributes.GetAt(pos);
		char* next_tmp;
		int i;

		i = header->sdp_append_attribute(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete(stringTemp);
			return -1;
		}
		tmp = next_tmp;
		pos++;
	}

	pos = 0;
	while (!this->m_medias.IsListEof(pos))
	{
		sdp_media* header = (sdp_media*) this->m_medias.GetAt(pos);
		char* next_tmp;
		int i;

		i = header->sdp_append_media(stringTemp, size, tmp, &next_tmp);
		if (i != 0)
		{
			delete(stringTemp);
			return -1;
		}
		tmp = next_tmp;
		pos++;
	}
	*dest = stringTemp;
	return 0;	
}

sdp_bandwidth::sdp_bandwidth()
{
	Init();	
}

sdp_bandwidth::~sdp_bandwidth()
{
	Free();	
}

int sdp_bandwidth::Init()
{
	(this)->b_bwtype = NULL;
	(this)->b_bandwidth = NULL;
	return 0;
}

void sdp_bandwidth::Free()
{
	mm_free(this->b_bwtype);
	mm_free(this->b_bandwidth);
}

int sdp_bandwidth::sdp_append_bandwidth(char* stringParam, int size, char* tmp, char** next_tmp)
{
	if (this->b_bwtype == NULL)
		return -1;
	if (this->b_bandwidth == NULL)
		return -1;
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, "b=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->b_bwtype);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, ":");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->b_bandwidth);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	
	*next_tmp = tmp;
	return 0;
}

sdp_time_descr::sdp_time_descr()
{
	Init();	
}

sdp_time_descr::~sdp_time_descr()
{
	Free();	
}

int sdp_time_descr::Init()
{
	(this)->t_start_time = NULL;
	(this)->t_stop_time = NULL;
	return 0;
}

void sdp_time_descr::Free()
{
	mm_free(this->t_start_time);
	mm_free(this->t_stop_time);
}

int sdp_time_descr::sdp_append_time_descr(char* stringParam, int size, char* tmp, char** next_tmp)
{
	int pos;
	
	if (this->t_start_time == NULL)
		return -1;
	if (this->t_stop_time == NULL)
		return -1;
	
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, "t=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->t_start_time);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->t_stop_time);
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	
	pos = 0;
	while (!this->r_repeats.IsListEof(pos))
	{
		char* str = (char*) this->r_repeats.GetAt(pos);
		
		tmp = __osip_sdp_append_string(stringParam, size, tmp, "r=");
		tmp = __osip_sdp_append_string(stringParam, size, tmp, str);
		tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
		pos++;
	}
	
	*next_tmp = tmp;
	return 0;
}

sdp_key::sdp_key()
{
	Init();	
}

sdp_key::~sdp_key()
{
	Free();
}

int sdp_key::Init()
{
	(this)->k_keytype = NULL;
	(this)->k_keydata = NULL;
	return 0;
}

void sdp_key::Free()
{
	mm_free(this->k_keytype);
	mm_free(this->k_keydata);
}

int sdp_key::sdp_append_key(char* stringParam, int size, char* tmp, char** next_tmp)
{
	if (this->k_keytype == NULL)
		return -1;
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, "k=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->k_keytype);
	if (this->k_keydata != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, ":");
		tmp = __osip_sdp_append_string(stringParam, size, tmp, this->k_keydata);
	}
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	*next_tmp = tmp;
	return 0;
}

sdp_attribute::sdp_attribute()
{
	Init();	
}

sdp_attribute::~sdp_attribute()
{
	Free();
}

int sdp_attribute::Init()
{
	(this)->a_att_field = NULL;
	(this)->a_att_value = NULL;
	return 0;
}

void sdp_attribute::Free()
{
	mm_free(this->a_att_field);
	mm_free(this->a_att_value);	
}

int sdp_attribute::sdp_append_attribute(char* stringParam, int size, char* tmp, char** next_tmp)
{
	if (this->a_att_field == NULL)
		return -1;
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, "a=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->a_att_field);
	if (this->a_att_value != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, ":");
		tmp = __osip_sdp_append_string(stringParam, size, tmp, this->a_att_value);
	}
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	
	*next_tmp = tmp;
	return 0;
}

sdp_connection::sdp_connection()
{
	Init();
}

sdp_connection::~sdp_connection()
{
	Free();
}

int sdp_connection::Init()
{
	(this)->c_nettype = NULL;
	(this)->c_addrtype = NULL;
	(this)->c_addr = NULL;
	(this)->c_addr_multicast_ttl = NULL;
	(this)->c_addr_multicast_int = NULL;	
	return 0;
}

void sdp_connection::Free()
{
	mm_free(this->c_nettype);
	mm_free(this->c_addrtype);
	mm_free(this->c_addr);
	mm_free(this->c_addr_multicast_ttl);
	mm_free(this->c_addr_multicast_int);	
}

int sdp_connection::sdp_append_connection(char* stringParam, int size, char* tmp,char** next_tmp)
{
	if (this->c_nettype == NULL)
		return -1;
	if (this->c_addrtype == NULL)
		return -1;
	if (this->c_addr == NULL)
		return -1;
	
	tmp = __osip_sdp_append_string(stringParam, size, tmp, "c=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->c_nettype);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->c_addrtype);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->c_addr);
	if (this->c_addr_multicast_ttl != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, "/");
		tmp = __osip_sdp_append_string(stringParam, size, tmp,this->c_addr_multicast_ttl);
	}
	if (this->c_addr_multicast_int != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, "/");
		tmp = __osip_sdp_append_string(stringParam, size, tmp,this->c_addr_multicast_int);
	}
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	*next_tmp = tmp;
	return 0;
}

sdp_media::sdp_media()
{
	Init();
}

sdp_media::~sdp_media()
{
	Free();	
}


int sdp_media::Init()
{
	(this)->m_media = NULL;
	(this)->m_port = NULL;
	(this)->m_number_of_port = NULL;
	(this)->m_proto = NULL;
	(this)->i_info = NULL;
	(this)->k_key = NULL;
	return 0;
}

void sdp_media::Free()
{
	mm_free(this->m_media);
	mm_free(this->m_port);
	mm_free(this->m_number_of_port);
	mm_free(this->m_proto);
	mm_free(this->i_info);
}
/* internal facility */
int sdp_media::sdp_append_media(char* stringParam, int size, char* tmp, char** next_tmp)
{
	int pos;

	if (this->m_media == NULL)
		return -1;
	if (this->m_port == NULL)
		return -1;
	if (this->m_proto == NULL)
		return -1;

	tmp = __osip_sdp_append_string(stringParam, size, tmp, "m=");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->m_media);
	tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->m_port);
	if (this->m_number_of_port != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, "/");
		tmp = __osip_sdp_append_string(stringParam, size, tmp,this->m_number_of_port);
	}
	tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
	tmp = __osip_sdp_append_string(stringParam, size, tmp, this->m_proto);
	pos = 0;
	while (!this->m_payloads.IsListEof(pos))
	{
		char* str = (char*) this->m_payloads.GetAt(pos);

		tmp = __osip_sdp_append_string(stringParam, size, tmp, " ");
		tmp = __osip_sdp_append_string(stringParam, size, tmp, str);
		pos++;
	}
	tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);

	if (this->i_info != NULL)
	{
		tmp = __osip_sdp_append_string(stringParam, size, tmp, "i=");
		tmp = __osip_sdp_append_string(stringParam, size, tmp, this->i_info);
		tmp = __osip_sdp_append_string(stringParam, size, tmp, CRLF);
	}

	pos = 0;
	while (!this->c_connections.IsListEof(pos))
	{
		sdp_connection* conn = (sdp_connection*) this->c_connections.GetAt(pos);
		char* next_tmp2;
		int i;

		i = conn->sdp_append_connection(stringParam, size, tmp, &next_tmp2);
		if (i != 0)
			return -1;
		tmp = next_tmp2;
		pos++;
	}

	pos = 0;
	while (!this->b_bandwidths.IsListEof(pos))
	{
		sdp_bandwidth* band = (sdp_bandwidth*) this->b_bandwidths.GetAt(pos);
		char* next_tmp2;
		int i;

		i = band->sdp_append_bandwidth(stringParam, size, tmp, &next_tmp2);
		if (i != 0)
			return -1;
		tmp = next_tmp2;
		pos++;
	}

	if (this->k_key != NULL)
	{
		char* next_tmp2;
		int i;

		i = this->k_key->sdp_append_key(stringParam, size, tmp, &next_tmp2);
		if (i != 0)
			return -1;
		tmp = next_tmp2;
	}

	pos = 0;
	while (!this->a_attributes.IsListEof(pos))
	{
		sdp_attribute* attr = (sdp_attribute*) this->a_attributes.GetAt(pos);
		char* next_tmp2;
		int i;

		i = attr->sdp_append_attribute(stringParam, size, tmp, &next_tmp2);
		if (i != 0)
			return -1;
		tmp = next_tmp2;
		pos++;
	}

	*next_tmp = tmp;
	return 0;
}

int CSdpMessage::sdp_message_v_version_set(char* v_version)
{
	if (this == NULL)
		return -1;
	this->v_version = v_version;
	return 0;
}

char* CSdpMessage::sdp_message_v_version_get()
{
	if (this == NULL)
		return NULL;
	return this->v_version;
}

int CSdpMessage::sdp_message_o_origin_set(char* username,
	char* sess_id, char* sess_version, char* nettype, char* addrtype,
	char* addr)
{
	if (this == NULL)
		return -1;
	this->o_username = username;
	this->o_sess_id = sess_id;
	this->o_sess_version = sess_version;
	this->o_nettype = nettype;
	this->o_addrtype = addrtype;
	this->o_addr = addr;
	return 0;
}

char* CSdpMessage::sdp_message_o_username_get()
{
	if (this == NULL)
		return NULL;
	return this->o_username;
}

char* CSdpMessage::sdp_message_o_sess_id_get()
{
	if (this == NULL)
		return NULL;
	return this->o_sess_id;
}

char* CSdpMessage::sdp_message_o_sess_version_get()
{
	if (this == NULL)
		return NULL;
	return this->o_sess_version;
}

char* CSdpMessage::sdp_message_o_nettype_get()
{
	if (this == NULL)
		return NULL;
	return this->o_nettype;
}

char* CSdpMessage::sdp_message_o_addrtype_get()
{
	if (this == NULL)
		return NULL;
	return this->o_addrtype;
}

char* CSdpMessage::sdp_message_o_addr_get()
{
	if (this == NULL)
		return NULL;
	return this->o_addr;
}

int CSdpMessage::sdp_message_s_name_set(char* name)
{
	if (this == NULL)
		return -1;
	this->s_name = name;
	return 0;
}

char* CSdpMessage::sdp_message_s_name_get()
{
	if (this == NULL)
		return NULL;
	return this->s_name;
}

int CSdpMessage::sdp_message_i_info_set(int pos_media, char* info)
{
	sdp_media* med;

	if (this == NULL)
		return -1;
	if (pos_media == -1)
	{
		this->i_info = info;
		return 0;
	}
	med = (sdp_media *)this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return -1;
	med->i_info = info;
	return 0;
}

char* CSdpMessage::sdp_message_i_info_get(int pos_media)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)
	{
		return this->i_info;
	}
	med = (sdp_media *)this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return NULL;
	return med->i_info;
}

int CSdpMessage::sdp_message_u_uri_set(char* uri)
{
	if (this == NULL)
		return -1;
	this->u_uri = uri;
	return 0;
}

char* CSdpMessage::sdp_message_u_uri_get()
{
	if (this == NULL)
		return NULL;
	return this->u_uri;
}

int CSdpMessage::sdp_message_e_email_add(char* email)
{
	if (this == NULL)
		return -1;
	this->e_emails.Add(email, -1);
	return 0;
}

char* CSdpMessage::sdp_message_e_email_get(int pos)
{
	if (this == NULL)
		return NULL;
	if (this->e_emails.GetSize() > pos)
		return (char *) this->e_emails.GetAt(pos);
	return NULL;
}

int CSdpMessage::sdp_message_p_phone_add(char* phone)
{
	if (this == NULL)
		return -1;
	this->p_phones.Add(phone, -1);
	return 0;
}

char* CSdpMessage::sdp_message_p_phone_get(int pos)
{
	if (this == NULL)
		return NULL;
	if (this->p_phones.GetSize() > pos)
		return (char *) this->p_phones.GetAt(pos);
	return NULL;
}

int CSdpMessage::sdp_message_c_connection_add(int pos_media,
	char* nettype, char* addrtype, char* addr, char* addr_multicast_ttl,
	char* addr_multicast_int)
{
	sdp_media* med;
	sdp_connection* conn;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	conn = new sdp_connection();
	if (conn == NULL)
		return -1;
	conn->c_nettype = nettype;
	conn->c_addrtype = addrtype;
	conn->c_addr = addr;
	conn->c_addr_multicast_ttl = addr_multicast_ttl;
	conn->c_addr_multicast_int = addr_multicast_int;
	if (pos_media == -1)
	{
		this->c_connection = conn;
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	med->c_connections.Add(conn, -1);
	return 0;
}

/* this method should be internal only... */
sdp_connection* CSdpMessage::sdp_message_connection_get(int pos_media, int pos)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)		  /* pos is useless in this case: 1 global "c=" is allowed */
		return this->c_connection;
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return NULL;
	return (sdp_connection *) med->c_connections.GetAt(pos);
}

char* CSdpMessage::sdp_message_c_nettype_get(int pos_media, int pos)
{
	sdp_connection* conn = sdp_message_connection_get(pos_media, pos);

	if (conn == NULL)
		return NULL;
	return conn->c_nettype;
}

char* CSdpMessage::sdp_message_c_addrtype_get(int pos_media, int pos)
{
	sdp_connection* conn = sdp_message_connection_get(pos_media, pos);

	if (conn == NULL)
		return NULL;
	return conn->c_addrtype;
}

char* CSdpMessage::sdp_message_c_addr_get(int pos_media, int pos)
{
	sdp_connection* conn = sdp_message_connection_get(pos_media, pos);

	if (conn == NULL)
		return NULL;
	return conn->c_addr;
}

char* CSdpMessage::sdp_message_c_addr_multicast_ttl_get(int pos_media,
	int pos)
{
	sdp_connection* conn = sdp_message_connection_get(pos_media, pos);

	if (conn == NULL)
		return NULL;
	return conn->c_addr_multicast_ttl;
}

char* CSdpMessage::sdp_message_c_addr_multicast_int_get(int pos_media,
	int pos)
{
	sdp_connection* conn = sdp_message_connection_get(pos_media, pos);

	if (conn == NULL)
		return NULL;
	return conn->c_addr_multicast_int;
}

int CSdpMessage::sdp_message_b_bandwidth_add(int pos_media,	char* bwtype, char* bandwidth)
{
	sdp_media* med;
	sdp_bandwidth* band;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	band = new sdp_bandwidth();
	if (band != NULL)
		return -1;
	band->b_bwtype = bwtype;
	band->b_bandwidth = bandwidth;
	if (pos_media == -1)
	{
		this->b_bandwidths.Add(band, -1);
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	med->b_bandwidths.Add(band, -1);
	return 0;
}

sdp_bandwidth* CSdpMessage::sdp_message_bandwidth_get(int pos_media,
	int pos)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)
		return (sdp_bandwidth *) this->b_bandwidths.GetAt(pos);
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return NULL;
	return (sdp_bandwidth *) med->b_bandwidths.GetAt(pos);
}

char* CSdpMessage::sdp_message_b_bwtype_get(int pos_media, int pos)
{
	sdp_bandwidth* band = sdp_message_bandwidth_get(pos_media, pos);

	if (band == NULL)
		return NULL;
	return band->b_bwtype;
}

char* CSdpMessage::sdp_message_b_bandwidth_get(int pos_media, int pos)
{
	sdp_bandwidth* band = sdp_message_bandwidth_get(pos_media, pos);

	if (band == NULL)
		return NULL;
	return band->b_bandwidth;
}

int CSdpMessage::sdp_message_t_time_descr_add(char* start, char* stop)
{
	sdp_time_descr* td;

	if (this == NULL)
		return -1;
	td = new sdp_time_descr();
	if (td == NULL)
		return -1;
	td->t_start_time = start;
	td->t_stop_time = stop;
	this->t_descrs.Add(td, -1);
	return 0;
}

char* CSdpMessage::sdp_message_t_start_time_get(int pos_td)
{
	sdp_time_descr* td;

	if (this == NULL)
		return NULL;
	td = (sdp_time_descr *) this->t_descrs.GetAt(pos_td);
	if (td == NULL)
		return NULL;
	return td->t_start_time;
}

char* CSdpMessage::sdp_message_t_stop_time_get(int pos_td)
{
	sdp_time_descr* td;

	if (this == NULL)
		return NULL;
	td = (sdp_time_descr *) this->t_descrs.GetAt(pos_td);
	if (td == NULL)
		return NULL;
	return td->t_stop_time;
}

int CSdpMessage::sdp_message_r_repeat_add(int pos_time_descr,
	char* field)
{
	sdp_time_descr* td;

	if (this == NULL)
		return -1;
	td = (sdp_time_descr *) this->t_descrs.GetAt(pos_time_descr);
	if (td == NULL)
		return -1;
	td->r_repeats.Add(field, -1);
	return 0;
}

char* CSdpMessage::sdp_message_r_repeat_get(int pos_time_descr,
	int pos_repeat)
{
	sdp_time_descr* td;

	if (this == NULL)
		return NULL;
	td = (sdp_time_descr *) this->t_descrs.GetAt(pos_time_descr);
	if (td == NULL)
		return NULL;
	return (char *) td->r_repeats.GetAt(pos_repeat);
}

int CSdpMessage::sdp_message_z_adjustments_set(char* field)
{
	if (this == NULL)
		return -1;
	this->z_adjustments = field;
	return 0;
}

char* CSdpMessage::sdp_message_z_adjustments_get()
{
	if (this == NULL)
		return NULL;
	return this->z_adjustments;
}

int CSdpMessage::sdp_message_k_key_set(int pos_media, char* keytype,
	char* keydata)
{
	sdp_key* key;
	sdp_media* med;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	key = new sdp_key();
	if (key == NULL)
	{
		return -1;
	}
	key->k_keytype = keytype;
	key->k_keydata = keydata;
	if (pos_media == -1)
	{
		this->k_key = key;
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	med->k_key = key;
	return 0;
}

char* CSdpMessage::sdp_message_k_keytype_get(int pos_media)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)
	{
		if (this->k_key == NULL)
			return NULL;
		return this->k_key->k_keytype;
	}
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return NULL;
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med->k_key == NULL)
		return NULL;
	return med->k_key->k_keytype;
}

char* CSdpMessage::sdp_message_k_keydata_get(int pos_media)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)
	{
		if (this->k_key == NULL)
			return NULL;
		return this->k_key->k_keydata;
	}
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return NULL;
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med->k_key == NULL)
		return NULL;
	return med->k_key->k_keydata;
}

int CSdpMessage::sdp_message_a_attribute_add(int pos_media,
	char* att_field, char* att_value)
{
	sdp_media* med;
	sdp_attribute* attr;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	attr = new sdp_attribute();
	if (attr == NULL)
		return -1;
	attr->a_att_field = att_field;
	attr->a_att_value = att_value;
	if (pos_media == -1)
	{
		this->a_attributes.Add(attr, -1);
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	med->a_attributes.Add(attr, -1);
	return 0;
}

int CSdpMessage::sdp_message_a_attribute_del(int pos_media,
	char* att_field)
{
	int i;
	sdp_media* med;
	sdp_attribute* attr;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	if (pos_media == -1)
	{
		for (i = 0; i < this->a_attributes.GetSize();)
		{
			attr = (sdp_attribute*)this->a_attributes.GetAt(i);
			if (strcmp(attr->a_att_field, att_field) == 0)
			{
				this->a_attributes.RemoveAt(i);
				delete (attr);
			}
			else
				i++;
		}
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return -1;
	for (i = 0; i < med->a_attributes.GetSize();)
	{
		attr = (sdp_attribute*)med->a_attributes.GetAt(i);
		if (strcmp(attr->a_att_field, att_field) == 0)
		{
			med->a_attributes.RemoveAt(i);
			delete (attr);
		}
		else
			i++;
	}
	return 0;
}

int CSdpMessage::sdp_message_a_attribute_del_at_index(int pos_media,
	char* att_field, int pos_attr)
{
	int i;
	sdp_media* med;
	sdp_attribute* attr;

	if (this == NULL)
		return -1;
	if ((pos_media != -1) && (this->m_medias.GetSize() < pos_media + 1))
		return -1;
	if (pos_media == -1)
	{
		if (pos_attr == -1)
		{
			for (i = 0; i < this->a_attributes.GetSize();)
			{
				attr = (sdp_attribute*)this->a_attributes.GetAt(i);
				if (strcmp(attr->a_att_field, att_field) == 0)
				{
					this->a_attributes.RemoveAt(i);
					delete (attr);
				}
				else
					i++;
			}
		}
		else if ((attr = (sdp_attribute*)this->a_attributes.GetAt(pos_attr)) != NULL)
		{
			this->a_attributes.RemoveAt(pos_attr);
			delete (attr);
		}
		return 0;
	}
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return -1;
	for (i = 0; i < med->a_attributes.GetSize();)
	{
		if (pos_attr == -1)
		{
			attr = (sdp_attribute*)med->a_attributes.GetAt(i);
			if (strcmp(attr->a_att_field, att_field) == 0)
			{
				med->a_attributes.RemoveAt(i);
				delete (attr);
			}
			else
				i++;
		}
		else if ((attr = (sdp_attribute*)med->a_attributes.GetAt(pos_attr)) != NULL)
		{
			med->a_attributes.RemoveAt(pos_attr);
			delete (attr);
		}
	}
	return 0;
}


sdp_attribute* CSdpMessage::sdp_message_attribute_get(int pos_media,
	int pos)
{
	sdp_media* med;

	if (this == NULL)
		return NULL;
	if (pos_media == -1)
		return (sdp_attribute *) this->a_attributes.GetAt(pos);
	med = (sdp_media *) this->m_medias.GetAt(pos_media);
	if (med == NULL)
		return NULL;
	return (sdp_attribute *) med->a_attributes.GetAt(pos);
}

char* CSdpMessage::sdp_message_a_att_field_get(int pos_media, int pos)
{
	sdp_attribute* attr = sdp_message_attribute_get(pos_media, pos);

	if (attr == NULL)
		return NULL;
	return attr->a_att_field;
}

char* CSdpMessage::sdp_message_a_att_value_get(int pos_media, int pos)
{
	sdp_attribute* attr = sdp_message_attribute_get(pos_media, pos);

	if (attr == NULL)
		return NULL;
	return attr->a_att_value;
}

int CSdpMessage::sdp_message_endof_media(int i)
{
	if (this == NULL)
		return -1;
	if (i == -1)
		return 0;
	if (!this->m_medias.IsListEof(i))
		return 0;   				/* not end of list */
	return -1;  				  /* end of list */
}

int CSdpMessage::sdp_message_m_media_add(char* media, char* port,
	char* number_of_port, char* proto)
{
	sdp_media* med;

	med = new sdp_media();
	if (med == NULL)
		return -1;
	med->m_media = media;
	med->m_port = port;
	med->m_number_of_port = number_of_port;
	med->m_proto = proto;
	this->m_medias.Add(med, -1);
	return 0;
}

char* CSdpMessage::sdp_message_m_media_get(int pos_media)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return NULL;
	return med->m_media;
}

char* CSdpMessage::sdp_message_m_port_get(int pos_media)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return NULL;
	return med->m_port;
}

char* CSdpMessage::sdp_message_m_number_of_port_get(int pos_media)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return NULL;
	return med->m_number_of_port;
}

int CSdpMessage::sdp_message_m_port_set(int pos_media, char* port)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return -1;
	if (med->m_port)
		delete(med->m_port);
	med->m_port = port;
	return 0;
}

char* CSdpMessage::sdp_message_m_proto_get(int pos_media)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return NULL;
	return med->m_proto;
}

int CSdpMessage::sdp_message_m_payload_add(int pos_media, char* payload)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return -1;
	med->m_payloads.Add(payload, -1);
	return 0;
}

char* CSdpMessage::sdp_message_m_payload_get(int pos_media, int pos)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);

	if (med == NULL)
		return NULL;
	return (char *) med->m_payloads.GetAt(pos);
}

int CSdpMessage::sdp_message_m_payload_del(int pos_media, int pos)
{
	sdp_media* med = (sdp_media*)this->m_medias.GetAt(pos_media);
	char* payload;

	if (med == NULL)
		return -1;
	if ((payload = (char*)med->m_payloads.GetAt(pos)) == NULL)
		return -1;
	med->m_payloads.RemoveAt(pos);
	mm_free(payload);
	return 0;
}
