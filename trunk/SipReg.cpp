// SipReg.cpp: implementation of the CZhReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhReg.h"
#include "ZhMd5.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CZhReg::r_id;

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN + 1];

void CvtHex(HASH Bin, HASHHEX Hex);

void CvtHex(IN HASH Bin, OUT HASHHEX Hex)
{
	unsigned short i;
	unsigned char j;
	
	for (i = 0; i < HASHLEN; i++)
	{
		j = (Bin[i] >> 4) & 0xf;
		if (j <= 9)
			Hex[i * 2] = (j + '0');
		else
			Hex[i * 2] = (j + 'a' - 10);
		j = Bin[i] & 0xf;
		if (j <= 9)
			Hex[i * 2 + 1] = (j + '0');
		else
			Hex[i * 2 + 1] = (j + 'a' - 10);
	};
	Hex[HASHHEXLEN] = '\0';
}

CZhReg::CZhReg(const char* from,const char* proxy, const char* contact)
{
	memset(this,0,sizeof(*this));

	++r_id;
	r_reg_period = 3600;   /* delay between registration */
	r_aor = zstr_strdup(from);    /* sip identity */
	r_contact = zstr_strdup(contact);	 /* sip identity */
	r_registrar = zstr_strdup(proxy);	 /* registrar */
	{
		CZhMd5 Md5Ctx;
		HASH hval;
		HASHHEX key_line;
		
		Md5Ctx.MD5Init();
		Md5Ctx.MD5Update((unsigned char *) from, strlen(from));
		Md5Ctx.MD5Update((unsigned char *) ":", 1);
		Md5Ctx.MD5Update((unsigned char *) proxy, strlen(proxy));
		Md5Ctx.MD5Final((unsigned char *) hval);
		CvtHex(hval, key_line);
		
		zstr_strncpy(r_line, key_line, sizeof(r_line));
	}
}

CZhReg::~CZhReg()
{
	Free();
}

void CZhReg::Free()
{
	delete(r_aor);
	delete(r_contact);
	delete(r_registrar);
	
	if (r_last_tr != NULL)
	{
		if (r_last_tr != NULL &&
			r_last_tr->orig_request != NULL &&
			r_last_tr->orig_request->m_callid != NULL &&
			r_last_tr->orig_request->m_callid->number != NULL)
		{
			//_eXosip_delete_nonce(r_last_tr->orig_request->call_id->number);
		}
		
		if (r_last_tr->state == IST_TERMINATED ||
			r_last_tr->state == ICT_TERMINATED ||
			r_last_tr->state == NICT_TERMINATED ||
			r_last_tr->state == NIST_TERMINATED)
		{
			//__eXosip_delete_jinfo(r_last_tr);
			if (r_last_tr != NULL)
			{
				//osip_list_add(eXosip.j_transactions, r_last_tr, 0);
			}
		}
		else
		{
			//__eXosip_delete_jinfo(r_last_tr);
			if (r_last_tr != NULL)
			{
				//osip_list_add(eXosip.j_transactions, r_last_tr, 0);
			}
		}
	}
}
