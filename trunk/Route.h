// Route.h: interface for the CRoute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTE_H__0276C5A9_0E3A_40B8_AB7A_117B184026AD__INCLUDED_)
#define AFX_ROUTE_H__0276C5A9_0E3A_40B8_AB7A_117B184026AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "From.h"

typedef CFrom CRoute;

int osip_route_clone(CRoute* from, CRoute** dest);

#endif // !defined(AFX_ROUTE_H__0276C5A9_0E3A_40B8_AB7A_117B184026AD__INCLUDED_)
