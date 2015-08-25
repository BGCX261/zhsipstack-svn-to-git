// Utils.cpp: implementation of the Utils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUtils::CUtils()
{

}

CUtils::~CUtils()
{

}

mm_malloc_func_t* mm_malloc_func;
mm_realloc_func_t* mm_realloc_func;
mm_free_func_t* mm_free_func;

char* MallocAndZero( size_t len )
{
	char *temp = NULL;
	
	temp = (char *)malloc(len);
	if (temp == NULL)
	{
		return NULL;
	}
	
	memset(temp,0,len);
	
	return temp;	
}

char* zstr_clrncpy(char* dst, const char* src, size_t len)
{
	const char* pbeg;
	const char* pend;
	char* p;
	size_t spaceless_length;
	
	if (src == NULL)
		return NULL;
	
	/* find the start of relevant text */
	pbeg = src;
	while ((' ' == *pbeg) ||
		('\r' == *pbeg) ||
		('\n' == *pbeg) ||
		('\t' == *pbeg))
		pbeg++;
	
	
	/* find the end of relevant text */
	pend = src + len - 1;
	while ((' ' == *pend) ||
		('\r' == *pend) ||
		('\n' == *pend) ||
		('\t' == *pend))
	{
		pend--;
		if (pend < pbeg)
		{
			*dst = '\0';
			return dst;
		}
	}
	
	/* if pend == pbeg there is only one char to copy */
	spaceless_length = pend - pbeg + 1;   /* excluding any '\0' */
	memmove(dst, pbeg, spaceless_length);
	p = dst + spaceless_length;
	
	/* terminate the string and pad dest with zeros until len */
	do
	{
		*p = '\0';
		p++;
		spaceless_length++;
	}
	while (spaceless_length < len);
	
	return dst;
}

char* zstr_strncpy(char* dest, const char* src, size_t length)
{
	strncpy(dest, src, length);
	dest[length] = '\0';
	return dest;
}

char* next_separator(const char* ch, int separator_osip_to_find,
					 int before_separator)
{
	char* ind;
	char* tmp;
	
	ind = strchr(ch, separator_osip_to_find);
	if (ind == NULL)
		return NULL;
	
	tmp = NULL;
	if (before_separator != 0)
		tmp = strchr(ch, before_separator);
	
	if (tmp != NULL)
	{
		if (ind < tmp)
			return ind;
	}
	else
		return ind;
	
	return NULL;
}

int GenericParamParseAll( CZhList* gen_params, const char* params )
{
	if (gen_params == NULL || params == NULL)
	{
		return -1;
	}
		char* pname;
	char* pvalue;

	const char* comma;
	const char* equal;

	/* find '=' wich is the separator for one param */
	/* find ';' wich is the separator for multiple params */

	equal = next_separator(params + 1, '=', ';');
	comma = strchr(params + 1, ';');

	while (comma != NULL)
	{
		if (equal == NULL)
		{
			equal = comma;
			pvalue = NULL;
		}
		else
		{
			const char* tmp;

			/* check for NULL param with an '=' character */
			tmp = equal + 1;
			for (; *tmp == '\t' || *tmp == ' '; tmp++)
			{
			}
			pvalue = NULL;
			if (*tmp != ',' && *tmp != '\0')
			{
				if (comma - equal < 2)
					return -1;
				pvalue = (char *) MallocAndZero(comma - equal);
				if (pvalue == NULL)
					return -1;
				zstr_strncpy(pvalue, equal + 1, comma - equal - 1);
			}
		}

		if (equal - params < 2)
		{
			free(pvalue);
			return -1;
		}
		pname = (char *) MallocAndZero(equal - params);
		if (pname == NULL)
		{
			free(pvalue);
			return -1;
		}
		zstr_strncpy(pname, params + 1, equal - params - 1);

		osip_generic_param_add(gen_params, pname, pvalue);

		params = comma;
		equal = next_separator(params + 1, '=', ';');
		comma = strchr(params + 1, ';');
	}

	/* this is the last header (comma==NULL) */
	comma = params + strlen(params);

	if (equal == NULL)
	{
		equal = comma;  		  /* at the end */
		pvalue = NULL;
	}
	else
	{
		const char* tmp;

		/* check for NULL param with an '=' character */
		tmp = equal + 1;
		for (; *tmp == '\t' || *tmp == ' '; tmp++)
		{
		}
		pvalue = NULL;
		if (*tmp != ',' && *tmp != '\0')
		{
			if (comma - equal < 2)
				return -1;
			pvalue = (char *) MallocAndZero(comma - equal);
			if (pvalue == NULL)
				return -1;
			zstr_strncpy(pvalue, equal + 1, comma - equal - 1);
		}
	}

	if (equal - params < 2)
	{
		free(pvalue);
		return -1;
	}
	pname = (char *) MallocAndZero(equal - params);
	if (pname == NULL)
	{
		free(pvalue);
		return -1;
	}
	zstr_strncpy(pname, params + 1, equal - params - 1);

	osip_generic_param_add(gen_params, pname, pvalue);

	return 0;
}

int zstr_clrspace(char* word)
{
	char* pbeg;
	char* pend;
	size_t len;
	
	if (word == NULL)
		return -1;
	if (*word == '\0')
		return 0;
	len = strlen(word);
	
	pbeg = word;
	while ((' ' == *pbeg) ||
		('\r' == *pbeg) ||
		('\n' == *pbeg) ||
		('\t' == *pbeg))
		pbeg++;
	
	pend = word + len - 1;
	while ((' ' == *pend) ||
		('\r' == *pend) ||
		('\n' == *pend) ||
		('\t' == *pend))
	{
		pend--;
		if (pend < pbeg)
		{
			*word = '\0';
			return 0;
		}
	}
	
	/* Add terminating NULL only if we've cleared room for it */
	if (pend + 1 <= word + (len - 1))
		pend[1] = '\0';
	
	if (pbeg != word)
		memmove(word, pbeg, pend - pbeg + 2);
	
	return 0;
}

int zstr_quoted_string_set(const char* name, const char* str, char** result,
	const char** next)
{
	*next = str;
	if (*result != NULL)
		return 0;   				/* already parsed */
	*next = NULL;
	while ((' ' == *str) || ('\t' == *str) || (',' == *str))
		if (*str)
			str++;
		else
			return -1;  			  /* bad header format */

	if (strlen(str) <= strlen(name))
		return -1;  				/* bad header format... */
	if (zstr_strncasecmp(name, str, strlen(name)) == 0)
	{
		const char* quote1;
		const char* quote2;
		const char* tmp;
		const char* hack = strchr(str, '=');

		if (hack == NULL)
			return -1;

		while (' ' == *(hack - 1))  	  /* get rid of extra spaces */
			hack--;
		if ((size_t) (hack - str) != strlen(name))
		{
			*next = str;
			return 0;
		}

		quote1 = zstr_quote_find(str);
		if (quote1 == NULL)
			return -1;  			/* bad header format... */
		quote2 = zstr_quote_find(quote1 + 1);
		if (quote2 == NULL)
			return -1;  			/* bad header format... */
		if (quote2 - quote1 == 1)
		{
			/* this is a special case! The quote contains nothing! */
			/* example:   Digest opaque="",cnonce=""			   */
			/* in this case, we just forget the parameter... this  */
			/* this should prevent from user manipulating empty    */
			/* strings */
			tmp = quote2 + 1;     /* next element start here */
			for (; *tmp == ' ' || *tmp == '\t'; tmp++)
			{
			}
			for (; *tmp == '\n' || *tmp == '\r'; tmp++)
			{
			}   				/* skip LWS */
			*next = NULL;
			if (*tmp == '\0')     /* end of header detected */
				return 0;
			if (*tmp != '\t' && *tmp != ' ')
						/* LWS here ? */
				*next = tmp;
			else
			{
				/* it is: skip it... */
				for (; *tmp == ' ' || *tmp == '\t'; tmp++)
				{
				}
				if (*tmp == '\0') /* end of header detected */
					return 0;
				*next = tmp;
			}
			return 0;
		}
		*result = (char *) mm_malloc(quote2 - quote1 + 3);
		if (*result == NULL)
			return -1;
		zstr_strncpy(*result, quote1, quote2 - quote1 + 1);
		tmp = quote2 + 1;   	  /* next element start here */
		for (; *tmp == ' ' || *tmp == '\t'; tmp++)
		{
		}
		for (; *tmp == '\n' || *tmp == '\r'; tmp++)
		{
		}   					/* skip LWS */
		*next = NULL;
		if (*tmp == '\0')   	  /* end of header detected */
			return 0;
		if (*tmp != '\t' && *tmp != ' ')
				/* LWS here ? */
			*next = tmp;
		else
		{
			/* it is: skip it... */
			for (; *tmp == ' ' || *tmp == '\t'; tmp++)
			{
			}
			if (*tmp == '\0')     /* end of header detected */
				return 0;
			*next = tmp;
		}
	}
	else
		*next = str;				/* wrong header asked! */
	return 0;
}

int zstr_token_set(const char* name, const char* str, char** result,
					 const char** next)
{
	const char* beg;
	const char* tmp;
	
	*next = str;
	if (*result != NULL)
		return 0;   				/* already parsed */
	*next = NULL;
	
	beg = strchr(str, '=');
	if (beg == NULL)
		return -1;  				/* bad header format... */
	
	if (strlen(str) < 6)
		return 0;   				/* end of header... */
	
	while ((' ' == *str) || ('\t' == *str) || (',' == *str))
		if (*str)
			str++;
		else
			return -1;  			  /* bad header format */
		
		if (zstr_strncasecmp(name, str, strlen(name)) == 0)
		{
			const char* end;
			
			end = strchr(str, ',');
			if (end == NULL)
				end = str + strlen(str);	   /* This is the end of the header */
			
			if (end - beg < 2)
				return -1;
			*result = (char *) mm_malloc(end - beg);
			if (*result == NULL)
				return -1;
			zstr_clrncpy(*result, beg + 1, end - beg - 1);
			
			/* make sure the element does not contain more parameter */
			tmp = (*end) ? (end + 1) : end;
			for (; *tmp == ' ' || *tmp == '\t'; tmp++)
			{
			}
			for (; *tmp == '\n' || *tmp == '\r'; tmp++)
			{
			}   					/* skip LWS */
			*next = NULL;
			if (*tmp == '\0')   	  /* end of header detected */
				return 0;
			if (*tmp != '\t' && *tmp != ' ')
				/* LWS here ? */
				*next = tmp;
			else
			{
				/* it is: skip it... */
				for (; *tmp == ' ' || *tmp == '\t'; tmp++)
				{
				}
				if (*tmp == '\0')     /* end of header detected */
					return 0;
				*next = tmp;
			}
		}
		else
			*next = str;				/* next element start here */
		return 0;
}

/* in quoted-string, many characters can be escaped...   */
/* zstr_quote_find returns the next quote that is not escaped */
char* zstr_quote_find(const char* qstring)
{
	char* quote;

	quote = strchr(qstring, '"');
	if (quote == qstring)   	  /* the first char matches and is not escaped... */
		return quote;

	if (quote == NULL)
		return NULL;				/* no quote at all... */

	/* this is now the nasty cases where '"' is escaped
	   '" jonathan ros \\\""'
	   |				  |
	   '" jonathan ros \\"'
	   |				|
	   '" jonathan ros \""'
	   |				|
	   we must count the number of preceeding '\' */
	{
		int i = 1;

		for (; ;)
		{
			if (0 == strncmp(quote - i, "\\", 1))
				i++;
			else
			{
				if (i % 2 == 1) 	/* the '"' was not escaped */
					return quote;

				/* else continue with the next '"' */
				quote = strchr(quote + 1, '"');
				if (quote == NULL)
					return NULL;
				i = 1;
			}
			if (quote - i == qstring - 1)
					  /* example: "\"john"  */
					  /* example: "\\"jack" */
			{
				/* special case where the string start with '\' */
				if (*qstring == '\\')
					i++;			  /* an escape char was not counted */
				if (i % 2 == 0) 	/* the '"' was not escaped */
					return quote;
				else
				{
					/* else continue with the next '"' */
					qstring = quote + 1;	/* reset qstring because
															   (*quote+1) may be also == to '\\' */
					quote = strchr(quote + 1, '"');
					if (quote == NULL)
						return NULL;
					i = 1;
				}
			}
		}
		return NULL;
	}
}

char* zstr_strdup(const char* ch)
{
	char* copy;
	size_t length;
	
	if (ch == NULL)
		return NULL;
	length = strlen(ch);
	copy = (char *) mm_malloc(length + 1);
	zstr_strncpy(copy, ch, length);
	return copy;
}


int zstr_tolower(char* word)
{
#if defined(HAVE_CTYPE_H) && !defined (_WIN32_WCE)
	
	for (; *word; word++)
		*word = (char) tolower(*word);
#else
	size_t i;
	size_t len = strlen(word);
	
	for (i = 0; i <= len - 1; i++)
	{
		if ('A' <= word[i] && word[i] <= 'Z')
			word[i] = word[i] + 32;
	}
#endif
	return 0;
}

int zstr_strcasecmp(const char* s1, const char* s2)
{
#if defined(__VXWORKS_OS__) || defined( __PSOS__)
	while ((*s1 != '\0') && (tolower(*s1) == tolower(*s2)))
	{
		s1++;
		s2++;
	}
	return (tolower(*s1) - tolower(*s2));
#elif defined(__PALMOS__) && (__PALMOS__ < 0x06000000)
	return StrCaselessCompare(s1, s2);
#elif defined(__PALMOS__) || (!defined WIN32 && !defined _WIN32_WCE)
	return strcasecmp(s1, s2);
#else
	return _stricmp(s1, s2);
#endif
}

int zstr_strncasecmp(const char* s1, const char* s2, size_t len)
{
#if defined(__VXWORKS_OS__) || defined( __PSOS__)
	if (len == 0)
		return 0;
	while ((len > 0) && (tolower(*s1) == tolower(*s2)))
	{
		len--;
		if ((len == 0) || (*s1 == '\0') || (*s2 == '\0'))
			break;
		s1++;
		s2++;
	}
	return tolower(*s1) - tolower(*s2);
#elif defined(__PALMOS__) && (__PALMOS__ < 0x06000000)
	return StrNCaselessCompare(s1, s2, len);
#elif defined(__PALMOS__) || (!defined WIN32 && !defined _WIN32_WCE)
	return strncasecmp(s1, s2, len);
#else
	return _strnicmp(s1, s2, len);
#endif
}

int zstr_atoi(const char* number)
{
#if defined(__linux) || defined(HAVE_STRTOL)
	int i;
	
	if (number == NULL)
		return -1;
	i = strtol(number, (char * *) NULL, 10);
	if (i == LONG_MIN || i == LONG_MAX)
		return -1;
	return i;
#endif
	
	return atoi(number);
}

int zstr_find_next_occurence(const char* str, const char* buf,
	const char** index_of_str, const char* end_of_buf)
{
	int i;

	*index_of_str = NULL;   	  /* AMD fix */
	if ((NULL == str) || (NULL == buf))
		return -1;
	/* TODO? we may prefer strcasestr instead of strstr? */
	for (i = 0; i < 1000; i++)
	{
		*index_of_str = strstr(buf, str);
		if (NULL == (*index_of_str))
		{
			/* if '\0' (when binary data is used) is located before the separator,
					   then we have to continue searching */
			const char* ptr = buf + strlen(buf);

			if (end_of_buf - ptr > 0)
			{
				buf = ptr + 1;
				continue;
			}
			return -1;
		}
		return 0;
	}

	return -1;
}

int zstr_find_next_crlf(const char* start_of_header,const char** end_of_header)
{
	const char* soh = start_of_header;

	*end_of_header = NULL;  	  /* AMD fix */

	while (('\r' != *soh) && ('\n' != *soh))
	{
		if (*soh)
			soh++;
		else
		{
			return -1;
		}
	}

	if (('\r' == soh[0]) && ('\n' == soh[1]))
		/* case 1: CRLF is the separator
		   case 2 or 3: CR or LF is the separator */
		soh = soh + 1;


	/* VERIFY if TMP is the end of header or LWS.   		 */
	/* LWS are extra SP, HT, CR and LF contained in headers. */
	if ((' ' == soh[1]) || ('\t' == soh[1]))
	{
		/* From now on, incoming message that potentially
			   contains LWS must be processed with
			   -> void osip_util_replace_all_lws(char *)
			   This is because the parser methods does not
			   support detection of LWS inside. */
		return -1;
	}

	*end_of_header = soh + 1;
	return 0;
}

static unsigned int random_seed_set = 0;

#ifndef WIN32_USE_CRYPTO
unsigned int osip_build_random_number()
#else
static unsigned int osip_fallback_random_number()
#endif
{
	if (!random_seed_set)
	{
		unsigned int ticks;
		
#ifdef __PALMOS__
#	if __PALMOS__ < 0x06000000
		SysRandom((Int32) TimGetTicks());
#	else
		struct timeval tv;
		
		gettimeofday(&tv, NULL);
		srand(tv.tv_usec);
		ticks = tv.tv_sec + tv.tv_usec;
#	endif
#elif defined(WIN32)
		LARGE_INTEGER lCount;
		
		QueryPerformanceCounter(&lCount);
		ticks = lCount.LowPart + lCount.HighPart;
#elif defined(_WIN32_WCE)
		ticks = GetTickCount();
#elif defined(__PSOS__)
#elif defined(__rtems__)
		rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &ticks);
#elif defined(__VXWORKS_OS__)
		struct timespec tp;
		
		clock_gettime(CLOCK_REALTIME, &tp);
		ticks = tp.tv_sec + tp.tv_nsec;
#else
		struct timeval tv;
		int fd;
		
		gettimeofday(&tv, NULL);
		ticks = tv.tv_sec + tv.tv_usec;
		fd = open("/dev/urandom", O_RDONLY);
		if (fd > 0)
		{
			unsigned int r;
			int i;
			
			for (i = 0; i < 512; i++)
			{
				read(fd, &r, sizeof(r));
				ticks += r;
			}
			close(fd);
		}
#endif
		
#ifdef HAVE_LRAND48
		srand48(ticks);
#else
		srand(ticks);
#endif
		random_seed_set = 1;
	}
#ifdef HAVE_LRAND48
	{
		int val = lrand48();
		if (val == 0)
		{
			unsigned int ticks;
			struct timeval tv;
			gettimeofday(&tv, NULL);
			ticks = tv.tv_sec + tv.tv_usec;
			srand48(ticks);
			return lrand48();
		}
		
		return val;
	}
#else
	return rand();
#endif
}

#ifdef WIN32_USE_CRYPTO

unsigned int osip_build_random_number()
{
	HCRYPTPROV crypto;
	BOOL err;
	unsigned int num;
	
	err = CryptAcquireContext(&crypto, NULL, NULL, PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT);
	if (err)
	{
		err = CryptGenRandom(crypto, sizeof(num), (BYTE *) &num);
		CryptReleaseContext(crypto, 0);
	}
	if (!err)
	{
		num = osip_fallback_random_number();
	}
	return num;
}

#endif

/* should use cryptographically random identifier is RECOMMENDED.... */
/* by now this should lead to identical call-id when application are
   started at the same time...   */
char* osip_call_id_new_random()
{
	char* tmp = (char*) mm_malloc(33);
	unsigned int number = osip_build_random_number();

	sprintf(tmp, "%u", number);
	return tmp;
}

int eXosip_generate_random(char* buf, int buf_size)
{
	unsigned int number = osip_build_random_number();

	_snprintf(buf, buf_size, "%u", number);
	return 0;
}

char* osip_from_tag_new_random(void)
{
	return osip_call_id_new_random();
}

char* osip_to_tag_new_random(void)
{
	return osip_call_id_new_random();
}

unsigned int via_branch_new_random(void)
{
	return osip_build_random_number();
}

/* ---For better performance---
   Same as above, only this time we know the length */
char* osip_strn_append(char* dst, const char* src, size_t len)
{
	memmove((void *) dst, (void *) src, len);
	dst += len;
	*dst = '\0';
	return dst;
}
/* ---For better performance---
   Appends src-string to dst-string.
   
   This was introduced to replace the 
   inefficient constructions like:
   
   osip_strncpy (tmp, src, strlen(src) );
   tmp = tmp + strlen (src);
   
   This function returns a pointer to the
   end of the destination string
   
   Pre: src is null terminated */
char* osip_str_append(char* dst, const char* src)
{
	while (*src != '\0')
	{
		*dst = *src;
		src++;
		dst++;
	}
	*dst = '\0';
	return dst;
}

/* __osip_set_next_token:
   dest is the place where the value will be allocated
   buf is the string where the value is searched
   end_separator is the character that MUST be found at the end of the value
   next is the final location of the separator + 1

   the element MUST be found before any "\r" "\n" "\0" and
   end_separator

   return -1 on error
   return 1 on success
*/
int __osip_set_next_token(char** dest, char* buf, int end_separator,
	char** next)
{
	char* sep;  				  /* separator */

	*next = NULL;

	sep = buf;
	while ((*sep != end_separator) &&
		(*sep != '\0') &&
		(*sep != '\r') &&
		(*sep != '\n'))
		sep++;
	if ((*sep == '\r') || (*sep == '\n'))
	{
		/* we should continue normally only if this is the separator asked! */
		if (*sep != end_separator)
			return -1;
	}
	if (*sep == '\0')
		return -1;  				/* value must not end with this separator! */
	if (sep == buf)
		return -1;  				/* empty value (or several space!) */

	*dest = (char*)mm_malloc(sep - (buf) + 1);
	zstr_strncpy(*dest, buf, sep - buf);

	*next = sep + 1;			  /* return the position right after the separator */
	return 0;
}

/* append string_osip_to_append to string at position cur
   size is the current allocated size of the element
*/
char* __osip_sdp_append_string(char* stringParam, size_t size, char* cur,
	char* string_osip_to_append)
{
	size_t length = strlen(string_osip_to_append);

	if (cur - stringParam + length > size)
	{
		size_t length2;

		length2 = cur - stringParam;
		stringParam = (char*)mm_realloc(stringParam, size + length + 10);
		cur = stringParam + length2;   /* the initial allocation may have changed! */
	}
	zstr_strncpy(cur, string_osip_to_append, length);
	return cur + strlen(cur);
}

#include <time.h>
#include <sys/timeb.h>

int osip_gettimeofday(struct timeval* tp, void* tz)
{
	struct _timeb timebuffer;
	
	_ftime(&timebuffer);
	tp->tv_sec = (long) timebuffer.time;
	tp->tv_usec = timebuffer.millitm * 1000;
	return 0;
}

void add_gettimeofday(struct timeval* atv, int ms)
{
	int m;
	
	if (ms >= 1000000)
	{
		atv->tv_usec = 0;
		m = ms / 1000;
	}
	else
	{
		atv->tv_usec += ms * 1000;
		m = atv->tv_usec / 1000000;
		atv->tv_usec = atv->tv_usec % 1000000;
	}
	atv->tv_sec += m;
}