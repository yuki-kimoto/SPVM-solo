#include "spvm_sv.h"

char* SPVM_COMPAT_SVpv(SPVM_SV* sv) {
  return sv->buffer;
}

void SPVM_COMPAT_sv_catpvn(SPVM_SV* sv, const char* pv, size_t length) {
  
}

SPVM_SV* SPVM_COMPAT_newSVsv(SPVM_SV* sv) {
  
  return sv;
}
