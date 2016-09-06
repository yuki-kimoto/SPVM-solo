#include "sperl.h"

#include "sperl_op.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

char* const SPerl_op_name[] = {
  "null",
  "const_int",
  "const_float",
  "const_string",
  "lt",
  "le",
  "gt",
  "ge",
  "add",
  "subtract",
  "multiply",
  "divide",
  "and",
  "or",
  "bit_and",
  "bit_or",
  "modulo",
  "bit_xor",
  "not",
  "left_shift",
  "right_shift",
  "inc",
  "dec",
  "bit_not",
  "const_bool",
  "preinc",
  "postinc",
  "predec",
  "postdec",
  "complement",
  "negate",
  "list",
  "pushmark",
  "aelem",
  "attr",
  "assign",
  "func",
  "subarg",
  "anonsub",
  "block",
  "my",
  "our",
  "has",
  "cond",
  "use",
  "return",
  "last",
  "next",
  "package",
  "loop",
  "sub"
};

void SPerl_dump_abstract_tree(SPerl_OP* op, I32 depth) {
  
  I32 i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }
  printf("%s\n", SPerl_op_name[op->op_type]);
  
  if (op->op_first) {
    depth++;
    SPerl_dump_abstract_tree(op->op_first, depth);
    depth--;
  }
  else if (op->op_moresib) {
    SPerl_dump_abstract_tree(SPerl_OpSIBLING(op), depth);
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

SPerl_OP* SPerl_op_sibling_splice(SPerl_OP* parent, SPerl_OP* start, int del_count, SPerl_OP* insert) {
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
    while (last_ins->op_moresib)
      last_ins = SPerl_OpSIBLING(last_ins);
    SPerl_OpMAYBESIB_set(last_ins, rest, NULL);
  }
  else
    insert = rest;

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
  
  if (first->op_type != SPerl_OP_LIST) {
    return SPerl_newOP(SPerl_OP_LIST, 0, first, last);
  }
  
  SPerl_op_sibling_splice(first, first->op_last, 0, last);
  return first;
}

SPerl_OP* SPerl_newOP(I32 type, I32 flags, SPerl_OP* first, SPerl_OP* last) {
        
  SPerl_OP *op;

  op = (SPerl_OP*)malloc(sizeof(SPerl_OP) * 1);
  op->op_type = type;
  op->op_first = first;
  op->op_flags = (U8)flags;
  op->op_private = (U8)(1 | (flags >> 8));
  
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
