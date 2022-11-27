#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *src) {
  int len = 0;
  assert(src);
  while(*src++ != '\0') len++;
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *p = NULL;
  if(dst == NULL || src == NULL) {
      return NULL;
  }
  p = dst;
  while((*dst++ = *src++) != '\0');
  return p;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dest, const char *src) {
  char *p = dest;
	assert((dest != NULL) && (src != NULL));

	while (*dest != '\0') {
		dest++;
	}
	while ((*dest++ = *src++) != '\0') { }
	return p;
}

int strcmp(const char *str1, const char *str2) {
  while ((*str1) && (*str1 == *str2)) {
		str1++;
		str2++;
	}

	if (*(unsigned char*)str1 > *(unsigned char*)str2) {
		return 1;
	}
	else if (*(unsigned char*)str1 < *(unsigned char*)str2) {
		return -1;
	}
	else {
		return 0;
	}
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *dest, int set, size_t len) {
  if (dest == NULL || len < 0) {
		return NULL;
	}
	char *pdest = (char *)dest;
	while (len-- > 0) {
		*pdest++ = set;
	}
	return dest;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *dest, const void *src, size_t count) {
  if ((src == NULL) || (dest == NULL)) return 0;
  while (count--) {
    *(char*)dest++ = *(char*)src++;
  }
  return dest;
}

int memcmp(const void *dest, const void *src, size_t n) {
  assert(dest != NULL && src != NULL && n > 0);
  const char *pdest = (char*)dest;
  const char *psrc = (char*)src;
  while(*pdest == *psrc && --n > 0) {
    pdest++;
    psrc++;
  }
  int a = *pdest - *psrc;
  if(a > 0) { return 1; }
  else if(a < 0) { return -1; }
  else { return 0; }
  return a;
}

#endif
