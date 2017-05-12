#include "spvm_sv.h"

char* SPVM_COMPAT_SVpv(SPVM_SV* sv) {
  return sv->buffer;
}
