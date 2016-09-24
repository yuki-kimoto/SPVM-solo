#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_method_info.h"
#include "sperl_descripter.h"

SPerl_OP* SPerl_OP_newOP_SUB(SPerl_yy_parser* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {

  if (!op_optsubargs) {
    op_optsubargs = SPerl_OP_newOP(SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP* op = SPerl_OP_newOP(SPerl_OP_SUB, op_subname, op_optsubargs);
  SPerl_OP_sibling_splice(op, op_optsubargs, 0, op_desctype);
  SPerl_OP_sibling_splice(op, op_desctype, 0, op_block);
  
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new();
  method_info->name = op_subname->uv.string_value;
  
  if (op_optsubargs->type == SPerl_OP_NULL) {
    method_info->argument_count = 0;
  }
  else if (op_optsubargs->type == SPerl_OP_SUBARG) {
    method_info->argument_count = 1;
  }
  else if (op_optsubargs->type == SPerl_OP_LIST) {
    SPerl_long argument_count = 0;
    SPerl_OP* op_next = op_optsubargs->first;
    while (op_next = SPerl_OP_sibling(op_next)) {
      argument_count++;
    }
    method_info->argument_count = argument_count;
  }
  
  // type, descripters
  if (op_desctype->type == SPerl_OP_LIST) {
    method_info->return_type = op_desctype->first->uv.string_value;
    SPerl_OP* op_descripters = op_desctype->last;
    if (op_descripters->type == SPerl_OP_LIST) {
      SPerl_OP* op_next = op_descripters->first;
      while (op_next = SPerl_OP_sibling(op_next)) {
        method_info->desc_flags |= SPerl_DESCRIPTER_get_flag(op_next->uv.string_value);
      }
    }
    else if (op_descripters->type == SPerl_OP_CONST) {
      method_info->desc_flags |= SPerl_DESCRIPTER_get_flag(op_descripters->uv.string_value);
    }
  }
  // type
  else if (op_desctype->type == SPerl_OP_CONST) {
    method_info->return_type = op_desctype->uv.string_value;
  }
  
  method_info->op_block = op_block;
  
  SPerl_ARRAY_push(parser->method_infos, method_info);
  
  
  return op;
}

void SPerl_OP_dump_ast(SPerl_OP* op, SPerl_long depth) {
  
  SPerl_long i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_long type = op->type;
  printf("%s", SPerl_op_name[type]);
  if (type == SPerl_OP_CONST) {
    switch(op->private) {
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
  
  if (op->first) {
    depth++;
    SPerl_OP_dump_ast(op->first, depth);
    depth--;
  }
  
  if (op->moresib) {
    SPerl_OP_dump_ast(SPerl_OP_sibling(op), depth);
  }
}

SPerl_OP* SPerl_OP_sibling(SPerl_OP* op) {
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl_OP* op, SPerl_OP* sib) {
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl_OP* op, SPerl_OP* parent) {
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl_OP* parent, SPerl_OP* start, SPerl_long del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OP_sibling(start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->moresib)
      last_del = SPerl_OP_sibling(last_del);
    rest = SPerl_OP_sibling(last_del);
    SPerl_OP_lastsib_set(last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (last_ins->moresib) {
      last_ins = SPerl_OP_sibling(last_ins);
    }
    SPerl_OP_maybesib_set(last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OP_maybesib_set(start, insert, NULL);
  }
  else {
    if (!parent)
      goto no_parent;
    parent->first = insert;
  }
  
  if (!rest) {
    /* update last etc */
    SPerl_OP *lastop;

    if (!parent)
      goto no_parent;

    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->last = lastop;

    if (lastop)
      SPerl_OP_lastsib_set(lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_OP_append_elem(SPerl_OP *first, SPerl_OP *last)
{
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->type == SPerl_OP_LIST) {
    SPerl_OP_sibling_splice(first, first->last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_OP_newOP_LIST();
    SPerl_OP_sibling_splice(op_list, op_list->first, 0, first);
    SPerl_OP_sibling_splice(op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_OP_newOP_LIST() {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP(SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op;

  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  memset(op, 0, sizeof(SPerl_OP));
  
  op->type = type;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1 );
      first->type = SPerl_OP_NULL;
    }
    
    op->last = last;
    SPerl_OP_moresib_set(first, last);
    if (op->last)
      SPerl_OP_lastsib_set(op->last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OP_lastsib_set(op->first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}


