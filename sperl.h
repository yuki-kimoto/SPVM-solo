#ifndef SPERL_H
#define SPERL_H

#include <stdint.h>

typedef unsigned short U16;
typedef long I32;

typedef U16 SPerl_OPCODE;

#define STMT_START do
#define STMT_END while(0)

/* bool is built-in for g++-2.6.3 and later, which might be used
   for extensions.  <_G_config.h> defines _G_HAVE_BOOL, but we can't
   be sure _G_config.h will be included before this file.  _G_config.h
   also defines _G_HAVE_BOOL for both gcc and g++, but only g++
   actually has bool.  Hence, _G_HAVE_BOOL is pretty useless for us.
   g++ can be identified by __GNUG__.
   Andy Dougherty	February 2000
*/
#ifdef __GNUG__		/* GNU g++ has bool built-in */
# ifndef PERL_BOOL_AS_CHAR
#  ifndef HAS_BOOL
#    define HAS_BOOL 1
#  endif
# endif
#endif

#ifndef HAS_BOOL
# ifdef bool
#  undef bool
# endif
# define bool char
# define HAS_BOOL 1
#endif

/* cast-to-bool.  A simple (bool) cast may not do the right thing: if bool is
 * defined as char for example, then the cast from int is
 * implementation-defined (bool)!!(cbool) in a ternary triggers a bug in xlc on
 * AIX */
#define cBOOL(cbool) ((cbool) ? (bool)1 : (bool)0)

#endif
