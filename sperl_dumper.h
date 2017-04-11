#ifndef SPERL_DUMPER_H
#define SPERL_DUMPER_H

#include "sperl_base.h"

void SPerl_DUMPER_dump_sperl(SPerl* sperl);
void SPerl_DUMPER_dump_classs(SPerl* sperl, SPerl_ARRAY* classs);
void SPerl_DUMPER_dump_constants(SPerl* sperl, SPerl_ARRAY* constants);
void SPerl_DUMPER_dump_constant(SPerl* sperl, SPerl_CONSTANT* constant);
void SPerl_DUMPER_dump_field(SPerl* sperl, SPerl_FIELD* field);
void SPerl_DUMPER_dump_sub(SPerl* sperl, SPerl_SUB* sub);
void SPerl_DUMPER_dump_my_var(SPerl* sperl, SPerl_MY_VAR* my_var);
void SPerl_DUMPER_dump_enumeration_value(SPerl* sperl, SPerl_ENUMERATION_VALUE* enumeration_value);
void SPerl_DUMPER_dump_packages(SPerl* sperl, SPerl_ARRAY* packages);
void SPerl_DUMPER_dump_resolved_types(SPerl* sperl, SPerl_ARRAY* resolved_types);
void SPerl_DUMPER_dump_ast(SPerl* sperl, SPerl_OP* op);
void SPerl_DUMPER_dump_constant_pool(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool);
void SPerl_DUMPER_dump_bytecode_array(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, int32_t base, int32_t length);

#endif
