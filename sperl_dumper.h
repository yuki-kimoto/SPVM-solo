#ifndef SPERL_DUMPER_H
#define SPERL_DUMPER_H

#include "sperl_base.h"

void SPerl_DUMPER_dump_parser(SPerl_PARSER* parser);
void SPerl_DUMPER_dump_classs(SPerl_PARSER* parser, SPerl_ARRAY* classs);
void SPerl_DUMPER_dump_constants(SPerl_PARSER* parser, SPerl_ARRAY* constants);
void SPerl_DUMPER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int size);
void SPerl_DUMPER_dump_constant(SPerl_PARSER* parser, SPerl_CONSTANT* constant);
void SPerl_DUMPER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field);
void SPerl_DUMPER_dump_sub(SPerl_PARSER* parser, SPerl_SUB* sub);
void SPerl_DUMPER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var);
void SPerl_DUMPER_dump_enumeration_value(SPerl_PARSER* parser, SPerl_ENUMERATION_VALUE* enumeration_value);
void SPerl_DUMPER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* types);
void SPerl_DUMPER_dump_bodys(SPerl_PARSER* parser, SPerl_ARRAY* bodys);
void SPerl_DUMPER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* packages);
void SPerl_DUMPER_dump_resolved_types(SPerl_PARSER* parser, SPerl_ARRAY* resolved_types);
void SPerl_DUMPER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op);

#endif
