// RecordRoute.h: interface for the CRecordRoute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDROUTE_H__AB78B1BE_D626_4504_8E9F_1C100552848F__INCLUDED_)
#define AFX_RECORDROUTE_H__AB78B1BE_D626_4504_8E9F_1C100552848F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "From.h"

typedef CFrom CRecordRoute;

int osip_record_route_clone(CRecordRoute* from, CRecordRoute** dest);


#endif // !defined(AFX_RECORDROUTE_H__AB78B1BE_D626_4504_8E9F_1C100552848F__INCLUDED_)
