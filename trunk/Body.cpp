// Body.cpp: implementation of the CBody class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Body.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBody::CBody()
{
	Init();
}

CBody::~CBody()
{
	Free();
}

int CBody::Init()
{
	body = NULL;
	content_type = NULL;
	length = 0;

	return 0;
}

void CBody::Free()
{
	mm_free(body);
	mm_free(content_type);

	body = NULL;
	content_type = NULL;
}

int CBody::Parse( const char* start_of_body ,int iLength)
{
	if (start_of_body == NULL)
		return -1;
	
	body = (char *) MallocAndZero(iLength + 1);
	if (body == NULL)
		return -1;
	memcpy(body, start_of_body, iLength);
	body[iLength] = '\0';
	length = iLength;

	return 0;		
}
/* returns the body as a string.		  */
/* INPUT : osip_body_t *body | body.  */
/* returns null on error. */
int CBody::ToStr(char** dest, size_t* str_length)
{
	char* tmp_body;
	char* tmp;
	char* ptr;
	int pos;
	int i;
	size_t length;

	*dest = NULL;
	*str_length = 0;

	if (this->body == NULL)
		return -1;

	if (this->length <= 0)
		return -1;

	length = 15 + this->length + ((this->headers.GetSize()) * 40);
	tmp_body = (char *) mm_malloc(length);
	if (tmp_body == NULL)
		return -1;
	ptr = tmp_body; 			  /* save the initial address of the string */

	if (this->content_type != NULL)
	{
		tmp_body = osip_strn_append(tmp_body, "content-type: ", 14);
		i = this->content_type->ToStr(&tmp);
		if (i == -1)
		{
			mm_free(ptr);
			return -1;
		}
		if (length < tmp_body - ptr + strlen(tmp) + 4)
		{
			size_t len;

			len = tmp_body - ptr;
			length = length + strlen(tmp) + 4;
			ptr = (char*)mm_realloc(ptr, length);
			tmp_body = ptr + len;
		}

		tmp_body = osip_str_append(tmp_body, tmp);
		mm_free(tmp);
		tmp_body = osip_strn_append(tmp_body, CRLF, 2);
	}

	pos = 0;
	while (!this->headers.IsListEof(pos))
	{
		CHeader* header;

		header = (CHeader *) this->headers.GetAt(pos);
		i = header->ToStr(&tmp);
		if (i == -1)
		{
			mm_free(ptr);
			return -1;
		}
		if (length < tmp_body - ptr + strlen(tmp) + 4)
		{
			size_t len;

			len = tmp_body - ptr;
			length = length + strlen(tmp) + 4;
			ptr = (char*)mm_realloc(ptr, length);
			tmp_body = ptr + len;
		}
		tmp_body = osip_str_append(tmp_body, tmp);
		mm_free(tmp);
		tmp_body = osip_strn_append(tmp_body, CRLF, 2);
		pos++;
	}

	if (((this->headers.GetSize()) > 0) || (this->content_type != NULL))
	{
		tmp_body = osip_strn_append(tmp_body, CRLF, 2);
	}
	if (length < tmp_body - ptr + this->length + 4)
	{
		size_t len;

		len = tmp_body - ptr;
		length = length + this->length + 4;
		ptr = (char*)mm_realloc(ptr, length);
		tmp_body = ptr + len;
	}
	memcpy(tmp_body, this->body, this->length);
	tmp_body = tmp_body + this->length;

	/* end of this body */
	if (str_length != NULL)
		*str_length = tmp_body - ptr;
	*dest = ptr;
	return 0;
}

int CBody::ToStr( char** dest )
{
	return 0;
}

int CBody::ParseHeader(const char* start_of_osip_body_header, const char** next_body)
{
	const char* start_of_line;
	const char* end_of_line;
	const char* colon_index;
	char* hname;
	char* hvalue;
	int i;
	
	*next_body = NULL;
	start_of_line = start_of_osip_body_header;
	for (; ;)
	{
		i = zstr_find_next_crlf(start_of_line, &end_of_line);
		if (i == -1)
			return -1;  			/* error case: no end of body found */
		
		/* find the headere name */
		colon_index = strchr(start_of_line, ':');
		if (colon_index == NULL)
			return -1;  			/* this is also an error case */
		
		if (colon_index - start_of_line + 1 < 2)
			return -1;
		hname = (char *) mm_malloc(colon_index - start_of_line + 1);
		if (hname == NULL)
			return -1;
		zstr_clrncpy(hname, start_of_line, colon_index - start_of_line);
		
		if ((end_of_line - 2) - colon_index < 2)
		{
			mm_free(hname);
			return -1;
		}
		hvalue = (char *) mm_malloc((end_of_line - 2) - colon_index);
		if (hvalue == NULL)
		{
			mm_free(hname);
			return -1;
		}
		zstr_clrncpy(hvalue, colon_index + 1,
			(end_of_line - 2) - colon_index - 1);
		
		/* really store the header in the sip structure */
		if (zstr_strncasecmp(hname, "content-type", 12) == 0)
			i = SetContentType(hvalue);
		else
			i = SetHeader(hname, hvalue);
		mm_free(hname);
		mm_free(hvalue);
		if (i == -1)
			return -1;
		
		if (strncmp(end_of_line, CRLF, 2) == 0 ||
			strncmp(end_of_line, "\n", 1) == 0 ||
			strncmp(end_of_line,"\r",1) == 0)
		{
			*next_body = end_of_line;
			return 0;
		}
		start_of_line = end_of_line;
	}

	return 0;
}

int CBody::ParseMime( const char* start_of_body,int iLength )
{
	const char* end_of_osip_body_header;
	const char* start_of_osip_body_header;
	int i;
	
	if (start_of_body == NULL)
		return -1;
	
	start_of_osip_body_header = start_of_body;
	
	i = ParseHeader(start_of_osip_body_header,
		&end_of_osip_body_header);
	if (i == -1)
		return -1;
	
	start_of_osip_body_header = end_of_osip_body_header;
	/* set the real start of body */
	if (strncmp(start_of_osip_body_header, CRLF, 2) == 0)
		start_of_osip_body_header = start_of_osip_body_header + 2;
	else
	{
		if ((strncmp(start_of_osip_body_header, "\n", 1) == 0) ||
			(strncmp(start_of_osip_body_header, "\r", 1) == 0))
			start_of_osip_body_header = start_of_osip_body_header + 1;
		else
			return -1;  			/* message does not end with CRLFCRLF, CRCR or LFLF */
	}
	
	end_of_osip_body_header = start_of_body + length;
	if (end_of_osip_body_header - start_of_osip_body_header <= 0)
		return -1;
	this->body = (char *) mm_malloc(end_of_osip_body_header -
		start_of_osip_body_header +
		1);
	if (this->body == NULL)
		return -1;
	memcpy(this->body, start_of_osip_body_header,
		end_of_osip_body_header - start_of_osip_body_header);
	this->length = end_of_osip_body_header - start_of_osip_body_header;
	
	return 0;	
}

int CBody::SetContentType(const char* hvalue)
{
	int i;
	
	if (hvalue == NULL)
		return -1;
	
	content_type = new CContentType;
	if (content_type == NULL)
		return -1;
	i = content_type->Parse(hvalue);
	if (i != 0)
	{
		delete this->content_type;
		this->content_type = NULL;
		return -1;
	}
	
	return 0;
}

int CBody::SetHeader(const char* hname,const char* hvalue)
{
	CHeader* h;
	
	if (body == NULL)
		return -1;
	if (hname == NULL)
		return -1;
	if (hvalue == NULL)
		return -1;
	
	h = new CHeader;
	if (h != NULL)
		return -1;
	
	h->hname = zstr_strdup(hname);
	h->hvalue = zstr_strdup(hvalue);
	
	this->headers.Add(h, -1);
	return 0;
}

int osip_body_clone(CBody* body, CBody** dest)
{
	int i;
	CBody* copy;
	
	if (body == NULL || body->length <= 0)
		return -1;
	
	copy = new CBody();
	if (copy == NULL)
		return -1;
	
	
	copy->body = (char *) mm_malloc(body->length + 2);
	copy->length = body->length;
	memcpy(copy->body, body->body, body->length);
	copy->body[body->length] = '\0';
	
	if (body->content_type != NULL)
	{
		i = body->content_type->Clone(&(copy->content_type));
		if (i != 0)
			goto bc_error1;
	}
	
	i = body->headers.Clone(&copy->headers,(int * (*) (void *, void *)) & osip_header_clone);
	if (i != 0)
		goto bc_error1;
	
	*dest = copy;
	return 0;
bc_error1 : delete(copy);
			return -1;
}