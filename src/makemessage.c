
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

char *make_message(const char *fmt, ...) {
 char *p = NULL;
 size_t size = 30;
 int n = 0; 
 va_list ap;

 if((p = (char*)malloc(size)) == NULL)
  return NULL;

 while(1) {
  va_start(ap, fmt);
  n = vsnprintf(p, size, fmt, ap);
  va_end(ap);

  if(n > -1 && (unsigned int)n < size)
   return p;

  /* failed: have to try again, alloc more mem. */
  if(n > -1)      /* glibc 2.1 */
   size = n + 1;
  else            /* glibc 2.0 */
   size *= 2;     /* twice the old size */

  if((p = (char*)realloc (p, size)) == NULL)
   return NULL;
 }
}

