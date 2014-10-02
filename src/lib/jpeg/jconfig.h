#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
#define HAVE_STDDEF_H
#define HAVE_STDLIB_H
#undef CHAR_IS_UNSIGNED
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef NEED_FAR_POINTERS
#undef NEED_SHORT_EXTERNAL_NAMES
#undef INCOMPLETE_TYPES_BROKEN

/* Define "boolean" as unsigned char, not enum, on Windows systems.
 */
#ifdef _WIN32
#ifndef __RPCNDR_H__ /* don't conflict if rpcndr.h already read */
typedef unsigned char boolean;
#endif
#ifndef FALSE /* in case these macros already exist */
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#define HAVE_BOOLEAN /* prevent jmorecfg.h from redefining it */
#endif

#ifdef JPEG_INTERNALS
#undef RIGHT_SHIFT_IS_UNSIGNED
#endif
