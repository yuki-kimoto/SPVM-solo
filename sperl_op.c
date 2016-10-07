#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_op.h"
#include "sperl_method_info.h"
#include "sperl_descripter.h"
#include "sperl_class_info.h"
#include "sperl_parser.h"
#include "sperl_const_info.h"
#include "sperl_field_info.h"
#include "sperl_my_var_info.h"
#include "sperl_var_info.h"
#include "sperl_memory_pool.h"
#include "sperl_use_info.h"

/* sperl_op.h */
SPerl_char* const SPerl_OP_names[] = {
  "null",
  "const",
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
  "preinc",
  "postinc",
  "predec",
  "postdec",
  "complement",
  "negate",
  "list",
  "pushmark",
  "aelem",
  "field",
  "assign",
  "callsub",
  "subarg",
  "block",
  "my",
  "has",
  "cond",
  "use",
  "return",
  "last",
  "next",
  "package",
  "loop",
  "sub",
  "var",
  "eq",
  "ne",
  "statement",
  "grammer",
  "desctype",
  "word"
};

SPerl_OP* SPerl_OP_newOP_GRAMMER(SPerl_PARSER* parser, SPerl_OP* op_packages) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_GRAMMER, op_packages, NULL);

  parser->op_root = op;

  // Set constant informations
  SPerl_ARRAY* const_infos = parser->const_infos;
  
  // Create constant pool
  SPerl_int i;
  for (i = 0; i < const_infos->length; i++) {
    SPerl_CONST_INFO* const_info = SPerl_ARRAY_fetch(const_infos, i);
    
    const_info->pool_pos = parser->const_pool_pos;
    
    // Realloc
    if (parser->const_pool_pos - 1 >= parser->const_pool_capacity) {
      
      SPerl_int new_const_pool_capacity = parser->const_pool_capacity * 2;
      parser->const_pool = realloc(parser->const_pool, new_const_pool_capacity * sizeof(SPerl_int));
      memset(
        parser->const_pool + parser->const_pool_capacity,
        0,
        (new_const_pool_capacity - parser->const_pool_capacity) * sizeof(SPerl_int)
      );
      parser->const_pool_capacity = new_const_pool_capacity;
    }
    
    SPerl_int* const_pool = parser->const_pool;
    switch(const_info->type) {
      case SPerl_CONST_INFO_BOOLEAN:
      case SPerl_CONST_INFO_CHAR:
      case SPerl_CONST_INFO_BYTE:
      case SPerl_CONST_INFO_SHORT:
      case SPerl_CONST_INFO_INT:
        const_info->pool_pos = parser->const_pool_pos;
        *(const_pool + parser->const_pool_pos) = const_info->uv.int_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_LONG:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_long*)(const_pool + parser->const_pool_pos) = const_info->uv.long_value;
        parser->const_pool_pos += 2;
        break;
      case SPerl_CONST_INFO_FLOAT:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_float*)(const_pool + parser->const_pool_pos) = const_info->uv.float_value;
        parser->const_pool_pos += 1;
        break;
      case SPerl_CONST_INFO_DOUBLE:
        const_info->pool_pos = parser->const_pool_pos;
        *(SPerl_double*)(const_pool + parser->const_pool_pos) = const_info->uv.double_value;
        parser->const_pool_pos += 2;
        break;
    }
  }
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_PACKAGE(SPerl_PARSER* parser, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters) {
  SPerl_int i;

  SPerl_OP* op_package = SPerl_OP_newOP(parser, SPerl_OP_PACKAGE, op_pkgname, op_block);
  if (op_descripters) {
    SPerl_OP_sibling_splice(parser, op_package, op_package->first, 0, op_descripters);
  }
  
  SPerl_char* name = op_pkgname->uv.string_value;
  SPerl_CLASS_INFO* found_class_info = SPerl_HASH_search(
    parser->class_info_symtable,
    name,
    strlen(name)
  );
  
  if (found_class_info) {
    fprintf(stderr, "Warnings: class %s is already defined\n", name);
  }
  else {
    SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new(parser);
    class_info->name = name;
    class_info->op_block = op_block;
    class_info->alias = SPerl_PARSER_new_hash(parser, 0);
    
    // Search field and methods
    SPerl_ARRAY* field_infos = SPerl_PARSER_new_array(parser, 0);;
    SPerl_HASH* field_info_symtable = SPerl_PARSER_new_hash(parser, 0);
    SPerl_ARRAY* method_infos = SPerl_PARSER_new_array(parser, 0);;
    SPerl_HASH* method_info_symtable = SPerl_PARSER_new_hash(parser, 0);
    SPerl_OP* op_fs = op_block->first;
    if (op_fs && op_fs->type == SPerl_OP_LIST) {
      op_fs = SPerl_OP_sibling(parser, op_fs->first);
    }
    while (op_fs) {
      // Search field
      if (op_fs->type == SPerl_OP_HAS) {
        SPerl_FIELD_INFO* field_info = (SPerl_FIELD_INFO*)op_fs->uv.ptr_value;
        SPerl_char* field_name = field_info->name;
        SPerl_CLASS_INFO* found_field_info
          = SPerl_HASH_search(field_info_symtable, field_name, strlen(field_name));
        if (found_field_info) {
          fprintf(stderr, "Warnings: field %s::%s is already defined\n", class_info->name, field_name);
        }
        else {
          field_info->class_info = class_info;
          SPerl_ARRAY_push(field_infos, field_info);
          SPerl_HASH_insert(field_info_symtable, field_name, strlen(field_name), field_info);
        }
      }
      // Search method
      else if (op_fs->type == SPerl_OP_SUB) {
        SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)op_fs->uv.ptr_value;
        SPerl_char* method_name = method_info->name;
        SPerl_CLASS_INFO* found_method_info
          = SPerl_HASH_search(method_info_symtable, method_name, strlen(method_name));
        if (found_method_info) {
          fprintf(stderr, "Warnings: method %s::%s is already defined\n", class_info->name, method_name);
        }
        else {
          method_info->class_info = class_info;
          SPerl_ARRAY_push(method_infos, method_info);
          SPerl_HASH_insert(method_info_symtable, method_name, strlen(method_name), method_info);
        }
      }
      op_fs = SPerl_OP_sibling(parser, op_fs);
    }
    
    // Set filed and method information
    class_info->field_infos = field_infos;
    class_info->field_info_symtable = field_info_symtable;
    class_info->method_infos = method_infos;
    class_info->method_info_symtable = method_info_symtable;
   
    // Add class information
    SPerl_ARRAY_push(parser->class_infos, class_info);
    SPerl_HASH_insert(parser->class_info_symtable, name, strlen(name), class_info);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_newOP_USE(SPerl_PARSER* parser, SPerl_OP* op_pkgname, SPerl_OP* op_pkgalias) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_USE, op_pkgname, op_pkgalias);
  
  SPerl_USE_INFO* use_info = SPerl_USE_INFO_new(parser);
  use_info->class_name = op_pkgname->uv.string_value;
  if (op_pkgalias) {
    use_info->alias = op_pkgalias->uv.string_value;
  }
  op->uv.ptr_value = use_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_MY(SPerl_PARSER* parser, SPerl_OP* op_var, SPerl_OP* op_desctype) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_MY, op_var, op_desctype);
  
  // Create my var information
  SPerl_MY_VAR_INFO* my_var_info = SPerl_MY_VAR_INFO_new(parser);
  SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_var->uv.ptr_value;
  my_var_info->name = var_info->name;
  
  // type
  my_var_info->type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  my_var_info->desc_flags |= SPerl_OP_create_desc_flags(parser, op_descripters);
  
  // Add my_var information to op
  op->uv.ptr_value = my_var_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_HAS(SPerl_PARSER* parser, SPerl_OP* op_field_name, SPerl_OP* op_desctype) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_HAS, op_field_name, op_desctype);
  
  // Create field information
  SPerl_FIELD_INFO* field_info = SPerl_FIELD_INFO_new(parser);
  field_info->name = op_field_name->uv.string_value;
  
  // type
  field_info->type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  field_info->desc_flags |= SPerl_OP_create_desc_flags(parser, op_descripters);
  
  op->uv.ptr_value = field_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_CONST(SPerl_PARSER* parser, SPerl_OP* op) {
  
  SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op->uv.ptr_value;
  SPerl_ARRAY_push(parser->const_infos, const_info);
  
  return op;
}

SPerl_char SPerl_OP_create_desc_flags(SPerl_PARSER* parser, SPerl_OP* op_descripters) {
  
  if (!op_descripters) {
    return 0;
  }
  
  SPerl_char desc_flags = 0;
  
  // descripters
  // descripters is list of descripter
  if (op_descripters->type == SPerl_OP_LIST) {
    SPerl_OP* op_next = op_descripters->first;
    while (op_next = SPerl_OP_sibling(parser, op_next)) {
      desc_flags |= SPerl_DESCRIPTER_get_flag(op_next->uv.string_value);
    }
  }
  // descripters is descripter
  else if (op_descripters->type == SPerl_OP_CONST) {
    desc_flags |= SPerl_DESCRIPTER_get_flag(op_descripters->uv.string_value);
  }
  
  return desc_flags;
}

SPerl_OP* SPerl_OP_newOP_SUB(SPerl_PARSER* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {
  
  // Create OP_SUB
  if (!op_optsubargs) {
    op_optsubargs = SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_SUB, op_subname, op_optsubargs);
  SPerl_OP_sibling_splice(parser, op, op_optsubargs, 0, op_desctype);
  SPerl_OP_sibling_splice(parser, op, op_desctype, 0, op_block);
  
  // Create method infomation
  SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new(parser);
  method_info->name = op_subname->uv.string_value;
  
  // subargs
  // subargs is NULL
  if (op_optsubargs->type == SPerl_OP_NULL) {
    method_info->argument_count = 0;
  }
  // subargs is subarg
  else if (op_optsubargs->type == SPerl_OP_MY) {
    
    // Argument count
    method_info->argument_count = 1;
  }
  // subargs is list of subarg
  else if (op_optsubargs->type == SPerl_OP_LIST) {
    SPerl_int argument_count = 0;
    SPerl_OP* op_subarg = op_optsubargs->first;
    while (op_subarg = SPerl_OP_sibling(parser, op_subarg)) {
      // Increment argument count
      argument_count++;
    }
    method_info->argument_count = argument_count;
  }
  
  // return type
  method_info->return_type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  method_info->desc_flags |= SPerl_OP_create_desc_flags(parser, op_descripters);
  
  // Save block
  method_info->op_block = op_block;
  
  // my var informations
  SPerl_ARRAY* my_var_infos = SPerl_PARSER_new_array(parser, 0);
  SPerl_HASH* my_var_info_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  SPerl_ARRAY* op_stack = SPerl_ARRAY_new(0);
  SPerl_OP* op_cur = op;
  while (op_cur) {
    SPerl_OP* first;
    
    // Add my var
    if (op_cur->type == SPerl_OP_MY) {
      SPerl_MY_VAR_INFO* my_var_info = (SPerl_MY_VAR_INFO*)op_cur->uv.ptr_value;
      SPerl_MY_VAR_INFO* found_my_var_info
        = SPerl_HASH_search(my_var_info_symtable, my_var_info->name, strlen(my_var_info->name));
      if (found_my_var_info) {
        fprintf(stderr, "Warnings: Declare same name variable %s\n", my_var_info->name);
      }
      else {
        SPerl_HASH_insert(my_var_info_symtable, my_var_info->name, strlen(my_var_info->name), my_var_info);
        SPerl_ARRAY_push(my_var_infos, my_var_info);
        my_var_info->method_info = method_info;
      }
    }
    else {
      first = op_cur->first;
    }
    
    if (first) {
      SPerl_ARRAY_push(op_stack, op_cur);
      op_cur = op_cur->first;
    }
    else {
      SPerl_OP* op_sib = SPerl_OP_sibling(parser, op_cur);
      if (op_sib) {
        op_cur = op_sib;
      }
      else {
        op_cur = (SPerl_OP*)SPerl_ARRAY_pop(op_stack);
        op_cur = SPerl_OP_sibling(parser, op_cur);
      }
    }
  }
  SPerl_ARRAY_free(op_stack);
  
  // Set my var information
  method_info->my_var_infos = my_var_infos;
  method_info->my_var_info_symtable = my_var_info_symtable;
  
  op->uv.ptr_value = method_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_LIST(SPerl_PARSER* parser) {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}

SPerl_OP* SPerl_OP_newOP_flag(SPerl_PARSER* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->type = type;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_OP));
      first->type = SPerl_OP_NULL;
    }
    
    op->last = last;
    SPerl_OP_moresib_set(parser, first, last);
    if (op->last)
      SPerl_OP_lastsib_set(parser, op->last, (SPerl_OP*)op);
  }
  else if (first) {
    SPerl_OP_lastsib_set(parser, op->first, (SPerl_OP*)op);
  }

  return (SPerl_OP *)op;
}

SPerl_OP* SPerl_OP_append_elem(SPerl_PARSER* parser, SPerl_OP *first, SPerl_OP *last) {
  if (!first)
    return last;

  if (!last)
    return first;
  
  if (first->type == SPerl_OP_LIST) {
    SPerl_OP_sibling_splice(parser, first, first->last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_OP_newOP_LIST(parser);
    SPerl_OP_sibling_splice(parser, op_list, op_list->first, 0, first);
    SPerl_OP_sibling_splice(parser, op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl_PARSER* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start)
    first = SPerl_OP_sibling(parser, start);
  else if (!parent)
    goto no_parent;
  else
    first = parent->first;

  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->moresib)
      last_del = SPerl_OP_sibling(parser, last_del);
    rest = SPerl_OP_sibling(parser, last_del);
    SPerl_OP_lastsib_set(parser, last_del, NULL);
  }
  else
    rest = first;

  if (insert) {
    last_ins = insert;
    while (last_ins->moresib) {
      last_ins = SPerl_OP_sibling(parser, last_ins);
    }
    SPerl_OP_maybesib_set(parser, last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OP_maybesib_set(parser, start, insert, NULL);
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
      SPerl_OP_lastsib_set(parser, lastop, parent);
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_OP_sibling(SPerl_PARSER* parser, SPerl_OP* op) {
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* sib) {
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* parent) {
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl_PARSER* parser, SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}
