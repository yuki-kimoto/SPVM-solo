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
#include "sperl_allocator.h"

SPerl_OP* SPerl_OP_newOP_flag(SPerl_yy_parser* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last, SPerl_char flags, SPerl_char private) {
        
  SPerl_OP *op = (SPerl_OP*)SPerl_ALLOCATOR_alloc(parser->allocator_op);
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->type = type;
  op->first = first;
  op->flags = flags;
  op->private = private;
  
  if (last) {
    if (!first) {
      first = (SPerl_OP*)SPerl_ALLOCATOR_alloc(parser->allocator_op);
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

SPerl_OP* SPerl_OP_newOP_GRAMMER(SPerl_yy_parser* parser, SPerl_OP* op_packages) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_GRAMMER, op_packages, NULL);

  parser->main_root = op;

  // Set constant informations
  SPerl_ARRAY* const_infos = parser->const_infos;
  
  // Create constant pool
  SPerl_int i;
  for (i = 0; i < const_infos->length; i++) {
    SPerl_CONST_INFO* const_info = SPerl_ARRAY_fetch(const_infos, i);
    
    // Check same constant
    SPerl_int int_value;
    SPerl_char* key_ptr;
    SPerl_int key_len;
    switch(const_info->type) {
      
      case SPerl_CONST_INFO_BOOLEAN:
        int_value = (SPerl_int)const_info->uv.boolean_value;
        key_ptr = (SPerl_char*)&int_value;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_CHAR:
        int_value = (SPerl_int)const_info->uv.char_value;
        key_ptr = (SPerl_char*)&int_value;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_BYTE:
        int_value = (SPerl_int)const_info->uv.byte_value;
        key_ptr = (SPerl_char*)&int_value;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_SHORT:
        int_value = (SPerl_int)const_info->uv.short_value;
        key_ptr = (SPerl_char*)&int_value;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_INT:
        key_ptr = (SPerl_char*)&const_info->uv.int_value;;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_LONG:
        key_ptr = (SPerl_char*)&const_info->uv.long_value;
        key_len = 8;
        break;
      case SPerl_CONST_INFO_FLOAT:
        key_ptr = (SPerl_char*)&const_info->uv.float_value;
        key_len = 4;
        break;
      case SPerl_CONST_INFO_DOUBLE:
        key_ptr = (SPerl_char*)&const_info->uv.double_value;
        key_len = 8;
        break;
      case SPerl_CONST_INFO_STRING:
        key_ptr = const_info->uv.string_value;
        key_len = strlen(const_info->uv.string_value);
        break;
    }
    
    void* value = SPerl_HASH_search(parser->const_info_symtable, key_ptr, key_len);
    
    if (value) {
      SPerl_int pool_pos = *(SPerl_int*)value;
      const_info->pool_pos = pool_pos;
    }
    else {
      const_info->pool_pos = parser->const_pool_length;
      SPerl_int* pool_pos_ptr = (SPerl_int*)calloc(1, sizeof(SPerl_int));
      *pool_pos_ptr = parser->const_pool_length;
      SPerl_HASH_insert(parser->const_info_symtable, key_ptr, key_len, pool_pos_ptr);
      
      // Realloc
      if (parser->const_pool_length >= parser->const_pool_capacity) {
        SPerl_int new_const_pool_capacity = parser->const_pool_capacity * 2;
        SPerl_int* new_const_pool = calloc(new_const_pool_capacity, sizeof(SPerl_int));
        memcpy(new_const_pool, parser->const_pool, parser->const_pool_capacity);
        parser->const_pool = new_const_pool;
        parser->const_pool_capacity = new_const_pool_capacity;
      }

      SPerl_int* const_pool = parser->const_pool;
      switch(const_info->type) {
        case SPerl_CONST_INFO_BOOLEAN:
          *(const_pool + parser->const_pool_length) = (SPerl_int)const_info->uv.boolean_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_CHAR:
          *(const_pool + parser->const_pool_length) = (SPerl_int)const_info->uv.char_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_BYTE:
          *(const_pool + parser->const_pool_length) = (SPerl_int)const_info->uv.byte_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_SHORT:
          *(const_pool + parser->const_pool_length) = (SPerl_int)const_info->uv.short_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_INT:
          *(const_pool + parser->const_pool_length) = const_info->uv.int_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_LONG:
          *(SPerl_long*)(const_pool + parser->const_pool_length) = const_info->uv.long_value;
          parser->const_pool_length += 2;
          break;
        case SPerl_CONST_INFO_FLOAT:
          *(SPerl_float*)(const_pool + parser->const_pool_length) = const_info->uv.float_value;
          parser->const_pool_length += 1;
          break;
        case SPerl_CONST_INFO_DOUBLE:
          *(SPerl_double*)(const_pool + parser->const_pool_length) = const_info->uv.double_value;
          parser->const_pool_length += 2;
          break;
        case SPerl_CONST_INFO_STRING:
          strcpy((SPerl_char*)(const_pool + parser->const_pool_length), const_info->uv.string_value);
          parser->const_pool_length += (SPerl_int)(((strlen(const_info->uv.string_value) + 1) + 3) / sizeof(SPerl_int));
          break;
      }
    }
  }
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_MY(SPerl_yy_parser* parser, SPerl_OP* op_var, SPerl_OP* op_desctype) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_MY, op_var, op_desctype);
  
  // Create my var information
  SPerl_MY_VAR_INFO* my_var_info = SPerl_MY_VAR_INFO_new();
  SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op_var->uv.ptr_value;
  my_var_info->name = var_info->name;
  
  // type
  my_var_info->type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  my_var_info->desc_flags |= SPerl_OP_create_desc_flags(parser, op_descripters);
  
  // Add my_var information
  SPerl_MY_VAR_INFO* found_my_var_info = SPerl_HASH_search(
    parser->current_my_var_info_symtable,
    var_info->name,
    strlen(var_info->name)
  );
  if (found_my_var_info) {
    fprintf(stderr, "Warnings: Declare same name variable %s\n", var_info->name);
  }
  else {
    SPerl_HASH_insert(parser->current_my_var_info_symtable, var_info->name, strlen(var_info->name), my_var_info);
    SPerl_ARRAY_push(parser->current_my_var_infos, my_var_info);
  }
  
  // Add my_var information to op
  op->uv.ptr_value = my_var_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_HAS(SPerl_yy_parser* parser, SPerl_OP* op_field_name, SPerl_OP* op_desctype) {
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_HAS, op_field_name, op_desctype);
  
  // Create field information
  SPerl_FIELD_INFO* field_info = SPerl_FIELD_INFO_new();
  field_info->name = op_field_name->uv.string_value;
  
  // type
  field_info->type = op_desctype->first->uv.string_value;
  
  // descripters
  SPerl_OP* op_descripters = op_desctype->last;
  field_info->desc_flags |= SPerl_OP_create_desc_flags(parser, op_descripters);
  
  // Add field information
  SPerl_ARRAY_push(parser->current_field_infos, field_info);
  
  op->uv.ptr_value = field_info;
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_CONST(SPerl_yy_parser* parser, SPerl_OP* op) {
  
  SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op->uv.ptr_value;
  SPerl_ARRAY_push(parser->const_infos, const_info);
  
  switch(const_info->type) {
    case SPerl_CONST_INFO_BOOLEAN:
      printf("CONST(boolean %d) -> term\n", const_info->uv.boolean_value);
      break;
    case SPerl_CONST_INFO_CHAR:
      printf("CONST(char '%c') -> term\n", const_info->uv.char_value);
      break;
    case SPerl_CONST_INFO_BYTE:
      printf("CONST(byte %d) -> term\n", const_info->uv.byte_value);
      break;
    case SPerl_CONST_INFO_SHORT:
      printf("CONST(short %d) -> term\n", const_info->uv.short_value);
      break;
    case SPerl_CONST_INFO_INT:
      printf("CONST(int %d) -> term\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_LONG:
      printf("CONST(long %ld) -> term\n", const_info->uv.long_value);
      break;
    case SPerl_CONST_INFO_FLOAT:
      printf("CONST(float %f) -> term\n", const_info->uv.float_value);
      break;
    case SPerl_CONST_INFO_DOUBLE:
      printf("CONST(double %f) -> term\n", const_info->uv.double_value);
      break;
    case SPerl_CONST_INFO_STRING: {
      printf("CONST(string %s) -> term\n", const_info->uv.string_value);
      break;
    }
  }
  
  return op;
}

SPerl_OP* SPerl_OP_newOP_PACKAGE(SPerl_yy_parser* parser, SPerl_OP* op_pkgname, SPerl_OP* op_block, SPerl_OP* op_descripters) {
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
    SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new();
    class_info->name = name;
    class_info->op_block = op_block;
    
    // Set field information
    class_info->field_infos = parser->current_field_infos;
    parser->current_field_infos = SPerl_PARSER_new_array(parser, 0);

    // Set class information to field informations
    for (i = 0; i < class_info->field_infos->length; i++) {
      SPerl_FIELD_INFO* field_info = (SPerl_FIELD_INFO*)SPerl_ARRAY_fetch(class_info->field_infos, i);
      field_info->class_info = class_info;
    }
    
    // Set method informations
    class_info->method_infos = parser->current_method_infos;
    parser->current_method_infos = SPerl_PARSER_new_array(parser, 0);
    class_info->method_info_symtable = parser->current_method_info_symtable;
    parser->current_method_info_symtable = SPerl_HASH_new(0);
    
    // Set class information to method informations
    for (i = 0; i < class_info->method_infos->length; i++) {
      SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)SPerl_ARRAY_fetch(class_info->method_infos, i);
      method_info->class_info = class_info;
    }
    
    // Add class information
    SPerl_ARRAY_push(parser->class_infos, class_info);
    SPerl_HASH_insert(parser->class_info_symtable, name, strlen(name), class_info);
  }
  
  return op_package;
}

SPerl_char SPerl_OP_create_desc_flags(SPerl_yy_parser* parser, SPerl_OP* op_descripters) {
  
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

SPerl_OP* SPerl_OP_newOP_SUB(SPerl_yy_parser* parser, SPerl_OP* op_subname, SPerl_OP* op_optsubargs, SPerl_OP* op_desctype, SPerl_OP* op_block) {
  
  // Create OP_SUB
  if (!op_optsubargs) {
    op_optsubargs = SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
  }
  SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_SUB, op_subname, op_optsubargs);
  SPerl_OP_sibling_splice(parser, op, op_optsubargs, 0, op_desctype);
  SPerl_OP_sibling_splice(parser, op, op_desctype, 0, op_block);
  
  SPerl_char* name = op_subname->uv.string_value;
  SPerl_METHOD_INFO* found_method_info = SPerl_HASH_search(
    parser->current_method_info_symtable,
    name,
    strlen(name)
  );
  
  if (found_method_info) {
    fprintf(stderr, "Warnings: method %s is already defined\n", name);
  }
  else {
    // Create method infomation
    SPerl_METHOD_INFO* method_info = SPerl_METHOD_INFO_new(parser);
    method_info->name = name;
    
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
    
    // Add my var informations
    method_info->my_var_infos = parser->current_my_var_infos;
    parser->current_my_var_infos = SPerl_PARSER_new_array(parser, 0);
    method_info->my_var_info_symtable = parser->current_my_var_info_symtable;
    parser->current_my_var_info_symtable = SPerl_HASH_new(0);
    
    // Add method information to my_var
    SPerl_int i = 0;
    for (i = 0; i < method_info->my_var_infos->length; i++) {
      SPerl_MY_VAR_INFO* my_var_info = SPerl_ARRAY_fetch(method_info->my_var_infos, i);
      my_var_info->method_info = method_info;
    }
    
    // Add method information
    SPerl_ARRAY_push(parser->current_method_infos, method_info);
    SPerl_HASH_insert(parser->current_method_info_symtable, name, strlen(name), method_info);
  }
  
  return op;
}

SPerl_OP* SPerl_OP_sibling(SPerl_yy_parser* parser, SPerl_OP* op) {
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl_yy_parser* parser, SPerl_OP* op, SPerl_OP* sib) {
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl_yy_parser* parser, SPerl_OP* op, SPerl_OP* parent) {
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl_yy_parser* parser, SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl_yy_parser* parser, SPerl_OP* parent, SPerl_OP* start, SPerl_int del_count, SPerl_OP* insert) {
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

SPerl_OP* SPerl_OP_append_elem(SPerl_yy_parser* parser, SPerl_OP *first, SPerl_OP *last) {
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

SPerl_OP* SPerl_OP_newOP_LIST(SPerl_yy_parser* parser) {
  
  SPerl_OP* op_pushmark = SPerl_OP_newOP(parser, SPerl_OP_PUSHMARK, NULL, NULL);
  
  return SPerl_OP_newOP(parser, SPerl_OP_LIST, op_pushmark, NULL);
}

SPerl_OP* SPerl_OP_newOP(SPerl_yy_parser* parser, SPerl_char type, SPerl_OP* first, SPerl_OP* last) {
  return SPerl_OP_newOP_flag(parser, type, first, last, 0, 0);
}


