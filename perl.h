#ifndef SPVM_COMPAT_PERL_H
#define SPVM_COMPAT_PERL_H

#include <stdint.h>

// This header file is only contained in SPVM-solo

/* spvm_env.h */
struct SPVM_sv;
typedef struct SPVM_sv SPVM_SV;

struct SPVM_sv {
  char* buffer;
  int64_t ref_count;
  int64_t length;
};

char* SPVM_COMPAT_SVpv(SPVM_SV* sv);
void SPVM_COMPAT_sv_catpvn(SPVM_SV*, const char*, size_t);
SPVM_SV* SPVM_COMPAT_newSVsv(SPVM_SV* sv);
SPVM_SV* SPVM_COMPAT_newSVpvn(const char* pv, size_t length);
void SPVM_SvREFCNT_dec(SPVM_SV* sv);

#endif
