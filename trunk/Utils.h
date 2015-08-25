// Utils.h: interface for the Utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__C12BDEF9_42F6_4414_BD6A_61E4F8C782F1__INCLUDED_)
#define AFX_UTILS_H__C12BDEF9_42F6_4414_BD6A_61E4F8C782F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32_USE_CRYPTO
	#include <Wincrypt.h>
#endif

#include "ZhList.h"
#include "UrlParam.h"



#ifndef SIP_MESSAGE_MAX_LENGTH
	/**
	 * You can re-define your own maximum length for SIP message.
	 */
#define SIP_MESSAGE_MAX_LENGTH 4000
#endif

#ifndef BODY_MESSAGE_MAX_SIZE
	/**
	 * You can define the maximum length for a body inside a SIP message.
	 */
#define BODY_MESSAGE_MAX_SIZE  4000
#endif

#define MIME_MAX_BOUNDARY_LEN 70


#define STR_LEN_6         6
#define STR_LEN_8         8
#define STR_LEN_16        32
#define STR_LEN_32        32
#define STR_LEN_64        64
#define STR_LEN_128       128
#define STR_LEN_196       196
#define STR_LEN_256       256
#define STR_LEN_512       512
#define STR_LEN_1024      1024


class CUtils  
{
public:
	CUtils();
	virtual ~CUtils();
	
	//static char* MallocAndZero(size_t len);
	//static char* zstr_clrncpy(char* dst, const char* src, size_t len);
	//static char* zstr_strncpy(char* dest, const char* src, size_t length);


	//static int GenericParamParseAll(CZhList* gen_params, const char* params);
	//static char* next_separator(const char* ch, int separator_osip_to_find, int before_separator);
	//static int zstr_clrspace(char* word);
	//static int zstr_strncasecmp(const char* s1, const char* s2, size_t len);
};

typedef void* mm_malloc_func_t(size_t size);
typedef void mm_free_func_t(void* ptr);
typedef void* mm_realloc_func_t(void* ptr, size_t size);

extern mm_malloc_func_t* mm_malloc_func;
extern mm_realloc_func_t* mm_realloc_func;
extern mm_free_func_t* mm_free_func;

#ifndef mm_malloc
	#define mm_malloc(S) (mm_malloc_func?mm_malloc_func(S):malloc(S))
#endif

#ifndef mm_realloc
	#define mm_realloc(P,S) (mm_realloc_func?mm_realloc_func(P,S):realloc(P,S))
#endif

#ifndef mm_free
	#define mm_free(P) { if (P!=NULL) { if (mm_free_func) mm_free_func(P); else delete(P);} }
#endif

int		GenericParamParseAll(CZhList* gen_params, const char* params);

char*	MallocAndZero(size_t len);
char*	zstr_clrncpy(char* dst, const char* src, size_t len);
char*	zstr_strncpy(char* dest, const char* src, size_t length);
int		zstr_strcasecmp(const char* s1, const char* s2);
int		zstr_strncasecmp(const char* s1, const char* s2, size_t len);
int		zstr_atoi(const char* number);
int		zstr_clrspace(char* word);

char*	next_separator(const char* ch, int separator_osip_to_find, int before_separator);
int		zstr_quoted_string_set(const char* name, const char* str, char** result, const char** next);
int		zstr_token_set(const char* name, const char* str, char** result, const char** next);
char*	zstr_quote_find(const char* qstring);
char*	zstr_strdup(const char* ch);
int		zstr_tolower(char* word);

int		zstr_find_next_occurence(const char* str, const char* buf, const char** index_of_str, const char* end_of_buf);
int		zstr_find_next_crlf(const char* start_of_header,const char** end_of_header);

unsigned int osip_build_random_number();
char* osip_call_id_new_random();
int eXosip_generate_random(char* buf, int buf_size);
char* osip_from_tag_new_random(void);
char* osip_to_tag_new_random(void);
unsigned int via_branch_new_random(void);


char* osip_strn_append(char* dst, const char* src, size_t len);
char* osip_str_append(char* dst, const char* src);
int __osip_set_next_token(char** dest, char* buf, int end_separator, char** next);
char* __osip_sdp_append_string(char* string, size_t size, char* cur, char* string_osip_to_append);


int osip_gettimeofday(struct timeval* tp, void* tz);
void add_gettimeofday(struct timeval* atv, int ms);
#define osip_timerisset(tvp)		 ((tvp)->tv_sec || (tvp)->tv_usec)
# define osip_timercmp(a, b, CMP)   					   \
	(((a)->tv_sec == (b)->tv_sec) ?   					   \
	((a)->tv_usec CMP (b)->tv_usec) :					   \
	((a)->tv_sec CMP (b)->tv_sec))
#define osip_timerclear(tvp)		 (tvp)->tv_sec = (tvp)->tv_usec = 0

class CZhTimer
{
public:
	CZhTimer()
	{
		ReSetTimer();
	}
	~CZhTimer()
	{
		ReSetTimer();
	}

	_inline void SetTimer(int iEndSpan,bool bIfStart)
	{
		m_iTimerSpan = iEndSpan;
		if (bIfStart)
		{
			osip_gettimeofday(&m_stTimerStart, NULL);
			add_gettimeofday(&m_stTimerStart,m_iTimerSpan);
		}
		else
		{
			m_stTimerStart.tv_sec = -1;		
		}
	};

	_inline void ReSetTimer()
	{
		m_stTimerStart.tv_sec = -1;
		m_iTimerSpan = 0;
	};

	_inline bool IsTimerTriger()
	{
		struct timeval now;
		osip_gettimeofday(&now, NULL);
		
		if (m_stTimerStart.tv_sec == -1)
			return false;
		if (osip_timercmp(&now, &m_stTimerStart, >))
			return true;
		else
			return false;
	};
protected:
private:
	int				m_iTimerSpan; 	 
	struct timeval	m_stTimerStart;  
};




#endif // !defined(AFX_UTILS_H__C12BDEF9_42F6_4414_BD6A_61E4F8C782F1__INCLUDED_)
