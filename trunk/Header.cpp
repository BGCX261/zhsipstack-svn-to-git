// Header.cpp: implementation of the CHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Header.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeader::CHeader()
{
	Init();
}

CHeader::~CHeader()
{
	Free();
}

int CHeader::Init()
{
	hname = NULL;
	hvalue = NULL;

	return 0;
}

void CHeader::Parse( const char *pcValue )
{
	
}

/* returns the header as a string.    */
/* INPUT : osip_header_t *header | header. */
/* returns null on error. */
int CHeader::ToStr(char** dest)
{
	size_t len;
	
	*dest = NULL;
	if ((this->hname == NULL))
		return -1;
	
	len = 0;
	if (this->hvalue != NULL)
		len = strlen(this->hvalue);
	
	*dest = (char *) mm_malloc(strlen(this->hname) + len + 3);
	if (*dest == NULL)
		return -1;
	
	if (this->hvalue != NULL)
		sprintf(*dest, "%s: %s", this->hname, this->hvalue);
	else
		sprintf(*dest, "%s: ", this->hname);
	
	if (*dest[0] > 'a' && *dest[0] < 'z')
		*dest[0] = (*dest[0] - 32);

	return 0;
}

void CHeader::Free()
{
	if (hname != NULL)
	{
		free(hname);
	}
	if (hvalue != NULL)
	{
		free(hvalue);
	}
	hname = NULL;
	hvalue = NULL;
}

char* CHeader::GetName()
{
	return hname;
}

char* CHeader::GetValue()
{
	return hvalue;
}

void CHeader::SetName(char* name)
{
	hname = name;	
}

void CHeader::SetValue(char* value)
{
	
}

int osip_header_clone(CHeader* header, CHeader** dest)
{
	CHeader* he;
	
	*dest = NULL;
	if (header == NULL)
		return -1;
	if (header->hname == NULL)
		return -1;
	
	he = new CHeader();
	if (he == NULL)
		return -1;
	he->hname = zstr_strdup(header->hname);
	if (header->hvalue != NULL)
		he->hvalue = zstr_strdup(header->hvalue);
	
	*dest = he;
	return 0;
}
