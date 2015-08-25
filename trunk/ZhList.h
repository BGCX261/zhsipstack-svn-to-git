// ZhList.h: interface for the CZhList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHLIST_H__AF3B9553_FCCA_4F66_B3C2_58CA34181098__INCLUDED_)
#define AFX_ZHLIST_H__AF3B9553_FCCA_4F66_B3C2_58CA34181098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CZhList;

typedef struct __node __node_t;

/**
* Structure for referencing a node in a osip_list_t element.
* @struct __node
*/
struct __node
{
	void* next; 				/**< next __node_t containing element */
	void* element;  			/**< element in Current node */
};


typedef struct
{
	__node_t* actual;
	__node_t** prev;
	CZhList* li;
	int pos;
} ListIterator;

class CZhList  
{
public:
	CZhList();
	virtual ~CZhList();

	int InitList();
	void* GetAt(int pos);
	int RemoveAt(int pos);
	int Add(void* el, int pos=0);
	int GetSize();
	int IsListEof(int pos);
	int Clone(CZhList* dst,int *(*clone_func) (void *, void*));
	void OfCharFree();
	void Free(void *(*free_func) (void*));

	void* GetFirstPos(ListIterator* it);
	void* GetNextPos(ListIterator* it);
	void* RemoveAtPos(ListIterator* it);

	int nb_elt; 		/**< Number of element in the list */
	__node_t* node; 	/**< Next node containing element  */
};

#define IsListIteratorHasElem( it ) ( 0 != (it).actual && (it).pos < (it).li->nb_elt )

#endif // !defined(AFX_ZHLIST_H__AF3B9553_FCCA_4F66_B3C2_58CA34181098__INCLUDED_)

