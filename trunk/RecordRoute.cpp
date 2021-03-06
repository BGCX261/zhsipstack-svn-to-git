// RecordRoute.cpp: implementation of the CRecordRoute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "RecordRoute.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int osip_record_route_clone(CRecordRoute* from, CRecordRoute** dest)
{
	int i;
	CRecordRoute* fr;
	
	*dest = NULL;
	if (from == NULL)
		return -1;
	
	fr = new CRecordRoute();
	if (fr == NULL) 				  /* allocation failed */
		return -1;
	if (from->displayname != NULL)
		fr->displayname = zstr_strdup(from->displayname);
	
	if (from->url != NULL)
	{
		i = from->url->Clone(&(fr->url));
		if (i != 0)
		{
			delete (fr);
			return -1;
		}
	}
	
	i = from->gen_params.Clone(&(fr->gen_params),(int * (*) (void *, void *)) & osip_generic_param_clone);
	if (i != 0)
	{
		delete (fr);
		return -1;
	}
	*dest = fr;
	return 0;
}