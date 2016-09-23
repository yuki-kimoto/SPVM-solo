#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_class.h"

void SPerl_dump_ast(SPerl_OP* op, SPerl_long depth) {
  
  SPerl_long i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_long type = op->op_type;
  printf("%s", SPerl_op_name[type]);
  if (type == SPerl_OP_CONST) {
    switch(op->op_private) {
      case SPerl_OPp_CONST_BOOLEAN:
        printf(" boolean %d", op->uv.boolean_value);
        break;
      case SPerl_OPp_CONST_CHAR:
        printf(" char %c", op->uv.char_value);
        break;
      case SPerl_OPp_CONST_BYTE:
        printf(" byte %d", op->uv.byte_value);
        break;
      case SPerl_OPp_CONST_SHORT:
        printf(" short %d", op->uv.short_value);
        break;
      case SPerl_OPp_CONST_INT:
        printf(" int %d", op->uv.int_value);
        break;
      case SPerl_OPp_CONST_LONG:
        printf(" long %ld", op->uv.long_value);
        break;
      case SPerl_OPp_CONST_FLOAT:
        printf(" float %f", op->uv.float_value);
        break;
      case SPerl_OPp_CONST_DOUBLE:
        printf(" double %f", op->uv.double_value);
        break;
      case SPerl_OPp_CONST_STRING:
        printf(" string \"%s\"", op->uv.string_value);
        break;
    }
  }
  else if (type == SPerl_OP_VAR) {
    printf(" \"%s\"", op->uv.string_value);
  }
  printf("\n");
  
  if (op->op_first) {
    depth++;
    SPerl_dump_ast(op->op_first, depth);
    depth--;
  }
  
  if (op->op_moresib) {
    SPerl_dump_ast(SPerl_OpSIBLING(op), depth);
  }
}

SPerl_OP* SPerl_OpSIBLING(SPerl_OP* o) {
  return o->op_moresib ? o->op_sibparent : NULL;
}

void SPerl_OpMORESIB_set(SPerl_OP* o, SPerl_OP* sib) {
  o->op_moresib = 1;
  o->op_sibparent = sib;
}

void SPerl_OpLASTSIB_set(SPerl_OP* o, SPerl_OP* parent) {
  o->op_moresib = 0;
  o->op_sibparent = parent;
}

void SPerl_OpMAYBESIB_set(SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent) {
  o->op_moresib = sib ? 1 : 0;
  o->op_sibparent = o->op_moresib ? sib : parent;
}

SPerl_OP* SPerl_op_sibling_splice(SPerl_OP* parent, SPerl_OP* start, SPerl_long del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OpSIBLING(start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->op_first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->op_moresib)
      last_del = SPerl_OpSIBLING(last_del);
    rest = SPerl_OpSIBLING(last_del);
    SPerl_OpLASTSIB_set(last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (last_ins->op_moresib) {
      last_ins = SPerl_OpSIBLING(last_ins);
    }
    SPerl_OpMAYBESIB_set(last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OpMAYBESIB_set(start, insert, NULL);
  }
  else {
    if (!parent)
      goto no_parent;
    parent->op_first = insert;
  }
  
  if (!rest) {
    /* update op_last etc */
    SPerl_OP *lastop;

    if (!parent)
      goto no_parent;

    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->op_last = lastop;

    if (lastop)
      SPerl_OpLASTSIB_set(lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_op_append_elem(SPerl_OP *first, SPerl_OP *last)
{
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->op_type == SPerl_OP_LIST) {
    SPerl_op_sibling_splice(first, first->op_last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_newOP_LIST();
    SPerl_op_sibling_splice(op_list, op_list->op_first, 0, first);
    SPerl_op_sibling_splice(op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_newOP_LIST() {
  
  SPerl_OP* op_pushmark = SPerl_newOP(SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_newOP(SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_newOP(SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_newOP_flag(type, first, last, 0, 0);
}

SPerl_OP* SPerl_newOP_flag(SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op;

  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  memset(op, 0, sizeof(SPerl_OP));
  
  op->op_type = type;
  op->op_first = first;
  op->op_flags = flags;
  op->op_private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1 );
      first->op_type = SPerl_OP_NULL;
    }
    
    op->op_last = last;
    SPerl_OpMORESIB_set(first, last);
    if (op->op_last)
      SPerl_OpLASTSIB_set(op->op_last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OpLASTSIB_set(op->op_first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}

SPerl_OP* SPerl_newOP_SUB(SPerl_yy_parser* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {

  if (!op_optsubargs) {
    op_optsubargs = SPerl_newOP(SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP* op = SPerl_newOP(SPerl_OP_SUB, op_subname, op_optsubargs);
  SPerl_op_sibling_splice(op, op_optsubargs, 0, op_desctype);
  SPerl_op_sibling_splice(op, op_desctype, 0, op_block);
  
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new();
  method_info->name = op_subname->uv.string_value;
  
  if (op_optsubargs->op_type == SPerl_OP_NULL) {
    method_info->argument_count = 0;
  }
  else if (op_optsubargs->op_type == SPerl_OP_SUBARG) {
    method_info->argument_count = 1;
  }
  else if (op_optsubargs->op_type == SPerl_OP_LIST) {
    method_info->argument_count = 2;
  }
  
  // type, descripters
  if (op_desctype->op_type == SPerl_OP_LIST) {
    method_info->return_type = op_desctype->op_first->uv.string_value;
    SPerl_OP* op_descripters = op_desctype->op_last;
    if (op_descripters->op_type == SPerl_OP_LIST) {
      
    }
    else if (op_descripters->op_type == SPerl_OP_CONST) {
      method_info->desc_flag |= SPerl_get_desc_flag(op_descripters->uv.string_value);
    }
  }
  // type
  else if (op_desctype->op_type == SPerl_OP_CONST) {
    method_info->return_type = op_desctype->uv.string_value;
  }
  
  method_info->op_block = op_block;
  
  SPerl_ARRAY_push(parser->method_infos, method_info);
  
  
  return op;
}
