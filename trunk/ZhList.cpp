// ZhList.cpp: implementation of the CZhList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "ZhList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZhList::CZhList()
{
	InitList();
}

CZhList::~CZhList()
{

}

int CZhList::InitList()
{
	nb_elt = 0;
	node = NULL;

	return 0;   				  /* ok */	
}
/* index starts from 0; */
int CZhList::Add(void* el, int pos)
{
	__node_t* ntmp;
	int i = 0;
	
	if (nb_elt == 0)
	{
		node = (__node_t *) MallocAndZero(sizeof(__node_t));
		if (node == NULL)
			return -1;
		node->element = el;
		node->next = NULL;
		nb_elt++;
		return nb_elt;
	}
	
	if (pos == -1 || pos >= nb_elt)
	{
		/* insert at the end  */
		pos = nb_elt;
	}
	
	ntmp = node;			  /* exist because nb_elt>0  */
	
	if (pos == 0)   			  /* pos = 0 insert before first elt  */
	{
		node = (__node_t *) MallocAndZero(sizeof(__node_t));
		if (node == NULL)
		{
			/* leave the list unchanged */
			node = ntmp;
			return -1;
		}
		node->element = el;
		node->next = ntmp;
		nb_elt++;
		return nb_elt;
	}
	
	
	while (pos > i + 1)
	{
		i++;
		/* when pos>i next node exist  */
		ntmp = (__node_t *) ntmp->next;
	}
	
	/* if pos==nb_elt next node does not exist  */
	if (pos == nb_elt)
	{
		ntmp->next = (__node_t *) MallocAndZero(sizeof(__node_t));
		if (ntmp->next == NULL)
			return -1;  			/* leave the list unchanged */
		ntmp = (__node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = NULL;
		nb_elt++;
		return nb_elt;
	}
	
	/* here pos==i so next node is where we want to insert new node */
	{
		__node_t* nextnode = (__node_t*) ntmp->next;
		
		ntmp->next = (__node_t *) MallocAndZero(sizeof(__node_t));
		if (ntmp->next == NULL)
		{
			/* leave the list unchanged */
			ntmp->next = nextnode;
			return -1;
		}
		ntmp = (__node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		nb_elt++;
	}
	return nb_elt;
}

void* CZhList::GetAt( int iIndex )
{
	__node_t* ntmp;
	int i = 0;
	
	if (iIndex < 0 || iIndex >= nb_elt)
		/* element does not exist */
		return NULL;
	
	
	ntmp = node;			  /* exist because nb_elt>0 */
	
	while (iIndex > i)
	{
		i++;
		ntmp = (__node_t *) ntmp->next;
	}
	return ntmp->element;	
}

/* return -1 if failed */
int CZhList::RemoveAt(int iIndex)
{
	__node_t* ntmp;
	int i = 0;
	
	if (iIndex < 0 || iIndex >= nb_elt)
		/* element does not exist */
		return -1;
	
	ntmp = node;			  /* exist because nb_elt>0 */
	
	if ((iIndex == 0))
	{
		/* special case  */
		node = (__node_t *) ntmp->next;
		nb_elt--;
		free(ntmp);
		return nb_elt;
	}
	
	while (iIndex > i + 1)
	{
		i++;
		ntmp = (__node_t *) ntmp->next;
	}
	
	/* insert new node */
	{
		__node_t* remnode;
		
		remnode = (__node_t *) ntmp->next;
		ntmp->next = ((__node_t *) ntmp->next)->next;
		free(remnode);
		nb_elt--;
	}
	return nb_elt;
}

void* CZhList::GetFirstPos( ListIterator* it )
{
	if (0 >= nb_elt)
	{
		it->actual = 0;
		return 0;
	}
	
	it->actual = node;
	it->prev = &node;
	it->li = this;
	it->pos = 0;
	
	return node->element;	
}

void* CZhList::GetNextPos( ListIterator* it )
{
	it->prev = (__node_t * *) &(it->actual->next);
	it->actual = (__node_t *)it->actual->next;
	++(it->pos);
	
	if (IsListIteratorHasElem(*it))
	{
		return it->actual->element;
	}
	
	it->actual = 0;
	return 0;	
}

void* CZhList::RemoveAtPos( ListIterator* it )
{
	if (IsListIteratorHasElem(*it))
	{
		--(it->li->nb_elt);
		
		*(it->prev) = (__node_t*)it->actual->next;
		
		free(it->actual);
		it->actual = *(it->prev);
	}
	
	if (IsListIteratorHasElem(*it))
	{
		return it->actual->element;
	}
	
	return 0;	
}

int CZhList::IsListEof( int pos )
{
	if (pos < nb_elt)
		return 0;   				/* not end of list */
	return 1;  	
}

int CZhList::GetSize()
{
	/* 
	   Robin Nayathodan <roooot@softhome.net> 
	   N.K Electronics INDIA
	   NULL Checks  
	 */

	return nb_elt;
}

void CZhList::OfCharFree()
{
	int pos = 0;
	char* chain;
	
	while (!IsListEof(pos))
	{
		chain = (char *) GetAt(pos);
		RemoveAt(pos);
		free(chain);
	}
}

int CZhList::Clone(CZhList* dst,int *(*clone_func) (void *, void*))
{
	void* data;
	void* data2;
	int pos = 0;
	while (!IsListEof(pos))
	{
		data = GetAt(pos);
		if (clone_func(data, &data2) != 0)
			return -1;
		dst->Add(data2, -1);
		pos++;
	}
	return 0;
}

void CZhList::Free(void *(*free_func) (void*))
{
	void* element;
	
	while (!IsListEof(0))
	{
		element = (void *) GetAt(0);
		RemoveAt(0);
		if (free_func != NULL)
			free_func(element);
	}
}
