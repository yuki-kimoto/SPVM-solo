#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <inttypes.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_yacc_util.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_enumeration_value.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_enumeration.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_resolved_type.h"
#include "sperl_bytecode_builder.h"
#include "sperl_op_checker.h"
#include "sperl_switch_info.h"
#include "sperl_descriptor.h"
#include "sperl_func.h"
#include "sperl_allocator_parser.h"

const char* const SPerl_OP_C_CODE_NAMES[] = {
  "IF",
  "ELSIF",
  "ELSE",
  "FOR",
  "WHILE",
  "NULL",
  "LIST",
  "PUSHMARK",
  "GRAMMAR",
  "NAME",
  "PACKAGE",
  "MY_VAR",
  "MY_VAR_PARENT",
  "FIELD",
  "SUB",
  "DECL_ENUM",
  "DECL_DESCRIPTOR",
  "DECL_ENUMERATION_VALUE",
  "BLOCK",
  "ENUM_BLOCK",
  "CLASS_BLOCK",
  "TYPE",
  "DESCRIPTOR_ENUM",
  "CONSTANT",
  "PRE_INC",
  "POST_INC",
  "PRE_DEC",
  "POST_DEC",
  "BIT_NOT",
  "COMPLEMENT",
  "NEGATE",
  "PLUS",
  "EQ",
  "NE",
  "LT",
  "LE",
  "GT",
  "GE",
  "ADD",
  "SUBTRACT",
  "MULTIPLY",
  "DIVIDE",
  "BIT_AND",
  "BIT_OR",
  "REMAINDER",
  "BIT_XOR",
  "LEFT_SHIFT",
  "RIGHT_SHIFT",
  "RIGHT_SHIFT_UNSIGNED",
  "AND",
  "OR",
  "NOT",
  "ARRAY_ELEM",
  "ASSIGN",
  "CALL_SUB",
  "CALL_FIELD",
  "USE",
  "RETURN",
  "LAST",
  "NEXT",
  "LOOP",
  "VAR",
  "CONVERT",
  "POP",
  "NEW_ARRAY",
  "UNDEF",
  "MALLOC",
  "NEW_ARRAY_CONSTANT",
  "ARRAY_LENGTH",
  "CONDITION",
  "DIE",
  "SWITCH",
  "CASE",
  "DEFAULT",
  "SWITCH_CONDITION",
  "DESCRIPTOR",   // UNKNOWN
  "VOID",
  "TRY",
  "CATCH",
  "DECREFCOUNT",
  "INCREFCOUNT",
  "ARGS_MY_VARS",
  "BLOCK_END",
};

SPerl_OP* SPerl_OP_new_op_constant_int(SPerl* sperl, int32_t value, const char* file, int32_t line) {
  SPerl_OP* op_constant = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONSTANT, file, line);
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
  
  constant->code = SPerl_CONSTANT_C_CODE_INT;
  constant->uv.int_value = value;
  constant->resolved_type = SPerl_HASH_search(sperl, sperl->parser->resolved_type_symtable, "int", strlen("int"));
  
  op_constant->uv.constant = constant;
  
  return op_constant;
}

SPerl_OP* SPerl_OP_new_op_constant_long(SPerl* sperl, int64_t value, const char* file, int32_t line) {
  SPerl_OP* op_constant = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONSTANT, file, line);
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
  
  constant->code = SPerl_CONSTANT_C_CODE_LONG;
  constant->uv.long_value = value;
  constant->resolved_type = SPerl_HASH_search(sperl, sperl->parser->resolved_type_symtable, "long", strlen("long"));
  
  op_constant->uv.constant = constant;
  
  return op_constant;
}

SPerl_OP* SPerl_OP_new_op_constant_float(SPerl* sperl, float value, const char* file, int32_t line) {
  SPerl_OP* op_constant = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONSTANT, file, line);
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
  
  constant->code = SPerl_CONSTANT_C_CODE_FLOAT;
  constant->uv.float_value = value;
  constant->resolved_type = SPerl_HASH_search(sperl, sperl->parser->resolved_type_symtable, "float", strlen("float"));
  
  op_constant->uv.constant = constant;
  
  return op_constant;
}

SPerl_OP* SPerl_OP_new_op_constant_double(SPerl* sperl, double value, const char* file, int32_t line) {
  SPerl_OP* op_constant = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONSTANT, file, line);
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
  
  constant->code = SPerl_CONSTANT_C_CODE_DOUBLE;
  constant->uv.double_value = value;
  constant->resolved_type = SPerl_HASH_search(sperl, sperl->parser->resolved_type_symtable, "double", strlen("double"));
  
  op_constant->uv.constant = constant;
  
  return op_constant;
}

SPerl_OP* SPerl_OP_new_op_var_from_op_my_var(SPerl* sperl, SPerl_OP* op_my_var) {
  (void)sperl;
  
  SPerl_VAR* var = SPerl_VAR_new(sperl);
  SPerl_OP* op_var = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_VAR, op_my_var->file, op_my_var->line);
  
  SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
  
  SPerl_OP* op_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_my_var->file, op_my_var->line);
  op_name->uv.name = my_var->op_name->uv.name;
  var->op_name = op_name;
  var->op_my_var = op_my_var;
  op_var->uv.var = var;
  
  return op_var;
}

SPerl_OP* SPerl_OP_get_op_block_from_op_sub(SPerl* sperl, SPerl_OP* op_sub) {
  (void)sperl;
  
  SPerl_OP* op_block = op_sub->last;
  
  if (op_block->code == SPerl_OP_C_CODE_BLOCK) {
    return op_block;
  }
  else {
    return NULL;
  }
}

SPerl_OP* SPerl_OP_build_try_catch(SPerl* sperl, SPerl_OP* op_try, SPerl_OP* op_try_block, SPerl_OP* op_catch, SPerl_OP* op_my_var, SPerl_OP* op_catch_block) {
  
  // insert var declaration into catch block top
  SPerl_OP_sibling_splice(sperl, op_catch_block->first, op_catch_block->first->first, 0, op_my_var);
  
  // Add block
  SPerl_OP_sibling_splice(sperl, op_try, op_try->last, 0, op_try_block);
  SPerl_OP_sibling_splice(sperl, op_try, op_try->last, 0, op_catch);
  SPerl_OP_sibling_splice(sperl, op_try, op_try->last, 0, op_catch_block);
  
  return op_try;
}

SPerl_OP* SPerl_OP_build_switch_statement(SPerl* sperl, SPerl_OP* op_switch, SPerl_OP* op_term, SPerl_OP* op_block) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_OP* op_switch_condition = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_SWITCH_CONDITION, op_term->file, op_term->line);
  SPerl_OP_sibling_splice(sperl, op_switch_condition, op_switch_condition->last, 0, op_term);
  
  SPerl_OP_sibling_splice(sperl, op_switch, op_switch->last, 0, op_switch_condition);
  SPerl_OP_sibling_splice(sperl, op_switch, op_switch->last, 0, op_block);
  
  op_block->flag |= SPerl_OP_C_FLAG_BLOCK_SWITCH;
  
  op_switch->uv.switch_info = SPerl_SWITCH_INFO_new(sperl);
  op_switch->uv.switch_info->op_cases = parser->cur_op_cases;
  
  parser->cur_op_cases = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, sperl->parser->allocator, 0);
  
  return op_switch;
}

SPerl_OP* SPerl_OP_build_case_statement(SPerl* sperl, SPerl_OP* op_case, SPerl_OP* op_term) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_OP_sibling_splice(sperl, op_case, NULL, 0, op_term);
  
  op_term->flag = SPerl_OP_C_FLAG_CONSTANT_CASE;
  
  SPerl_ARRAY_push(sperl, parser->cur_op_cases, op_case);
  
  return op_case;
}

SPerl_OP* SPerl_OP_build_for_statement(SPerl* sperl, SPerl_OP* op_for, SPerl_OP* op_statement_init, SPerl_OP* op_term_condition, SPerl_OP* op_term_next_value, SPerl_OP* op_block) {

  SPerl_OP* op_condition = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONDITION, op_term_condition->file, op_term_condition->line);
  SPerl_OP_sibling_splice(sperl, op_condition, NULL, 0, op_term_condition);

  SPerl_OP* op_loop = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_LOOP, op_for->file, op_for->line);
  
  SPerl_OP* op_statements = op_block->first;
  
  op_block->flag |= SPerl_OP_C_FLAG_BLOCK_LOOP;
  op_condition->flag |= SPerl_OP_C_FLAG_CONDITION_LOOP;
  
  // Convert to while loop
  if (op_term_next_value->code != SPerl_OP_C_CODE_NULL) {
    if (op_statements->last) {
      SPerl_OP_sibling_splice(sperl, op_statements, op_statements->last, 0, op_term_next_value);
    }
    else {
      SPerl_OP_sibling_splice(sperl, op_statements, op_statements->first, 0, op_term_next_value);
    }
  }
  
  SPerl_OP_sibling_splice(sperl, op_loop, NULL, 0, op_statement_init);
  SPerl_OP_sibling_splice(sperl, op_loop, op_statement_init, 0, op_block);
  SPerl_OP_sibling_splice(sperl, op_loop, op_block, 0, op_condition);
  
  return op_loop;
}

SPerl_OP* SPerl_OP_build_while_statement(SPerl* sperl, SPerl_OP* op_while, SPerl_OP* op_term, SPerl_OP* op_block) {

  SPerl_OP* op_condition = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONDITION, op_term->file, op_term->line);
  SPerl_OP_sibling_splice(sperl, op_condition, NULL, 0, op_term);
  
  op_block->flag |= SPerl_OP_C_FLAG_BLOCK_LOOP;
  op_condition->flag |= SPerl_OP_C_FLAG_CONDITION_LOOP;

  SPerl_OP* op_loop = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_LOOP, op_while->file, op_while->line);
  
  SPerl_OP* op_null = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, op_while->file, op_while->line);
  
  SPerl_OP_sibling_splice(sperl, op_loop, NULL, 0, op_null);
  SPerl_OP_sibling_splice(sperl, op_loop, op_null, 0, op_block);
  SPerl_OP_sibling_splice(sperl, op_loop, op_block, 0, op_condition);
  
  return op_loop;
}

SPerl_OP* SPerl_OP_build_if_statement(SPerl* sperl, SPerl_OP* op_if, SPerl_OP* op_term, SPerl_OP* op_block_if, SPerl_OP* op_block_else) {
  
  if (op_if->code == SPerl_OP_C_CODE_ELSIF) {
    op_if->code = SPerl_OP_C_CODE_IF;
  }

  if (op_block_if->code != SPerl_OP_C_CODE_BLOCK) {
    SPerl_OP* op_term = op_block_if;
    op_block_if = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BLOCK, op_term->file, op_term->line);

    SPerl_OP* op_list = SPerl_OP_new_op_list(sperl, op_term->file, op_term->line);
    SPerl_OP_sibling_splice(sperl, op_list, op_list->first, 0, op_term);

    SPerl_OP_sibling_splice(sperl, op_block_if, NULL, 0, op_list);
  }
  
  if (op_block_else->code == SPerl_OP_C_CODE_IF) {
    SPerl_OP* op_if = op_block_else;
    op_block_else = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BLOCK, op_term->file, op_term->line);
    
    SPerl_OP* op_list = SPerl_OP_new_op_list(sperl, op_term->file, op_term->line);
    SPerl_OP_sibling_splice(sperl, op_list, op_list->first, 0, op_if);
    
    SPerl_OP_sibling_splice(sperl, op_block_else, NULL, 0, op_list);
  }
  else if (op_block_else->code != SPerl_OP_C_CODE_BLOCK && op_block_else->code != SPerl_OP_C_CODE_NULL) {
    SPerl_OP* op_term = op_block_else;
    op_block_else = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BLOCK, op_term->file, op_term->line);
    
    SPerl_OP* op_list = SPerl_OP_new_op_list(sperl, op_term->file, op_term->line);
    SPerl_OP_sibling_splice(sperl, op_list, op_list->first, 0, op_term);
    
    SPerl_OP_sibling_splice(sperl, op_block_else, NULL, 0, op_list);
  }
  
  SPerl_OP* op_condition = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONDITION, op_term->file, op_term->line);
  SPerl_OP_sibling_splice(sperl, op_condition, NULL, 0, op_term);
  
  op_block_if->flag |= SPerl_OP_C_FLAG_BLOCK_IF;
  op_condition->flag |= SPerl_OP_C_FLAG_CONDITION_IF;
  if (op_block_else->code == SPerl_OP_C_CODE_BLOCK) {
    op_block_else->flag |= SPerl_OP_C_FLAG_BLOCK_ELSE;
  }
  
  SPerl_OP_sibling_splice(sperl, op_if, NULL, 0, op_condition);
  SPerl_OP_sibling_splice(sperl, op_if, op_condition, 0, op_block_if);
  SPerl_OP_sibling_splice(sperl, op_if, op_block_if, 0, op_block_else);
  
  if (op_block_else->code == SPerl_OP_C_CODE_BLOCK) {
    op_block_if->flag |= SPerl_OP_C_FLAG_BLOCK_HAS_ELSE;
  }
  
  return op_if;
}

SPerl_OP* SPerl_OP_build_array_length(SPerl* sperl, SPerl_OP* op_array_length, SPerl_OP* op_term) {
  SPerl_OP_sibling_splice(sperl, op_array_length, NULL, 0, op_term);
  
  return op_array_length;
}

SPerl_OP* SPerl_OP_build_malloc_object(SPerl* sperl, SPerl_OP* op_malloc, SPerl_OP* op_type) {
  
  SPerl_OP_sibling_splice(sperl, op_malloc, NULL, 0, op_type);
  
  return op_malloc;
}

SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl* sperl, SPerl_OP* op) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_RESOLVED_TYPE*  resolved_type = NULL;
  
  switch (op->code) {
    case SPerl_OP_C_CODE_ARRAY_LENGTH:
      resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "long", strlen("long"));
    case SPerl_OP_C_CODE_ARRAY_ELEM: {
      SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op->first);
     resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, first_resolved_type->name, strlen(first_resolved_type->name) - 2);
      break;
    }
    case SPerl_OP_C_CODE_ADD:
    case SPerl_OP_C_CODE_SUBTRACT:
    case SPerl_OP_C_CODE_MULTIPLY:
    case SPerl_OP_C_CODE_DIVIDE:
    case SPerl_OP_C_CODE_REMAINDER:
    case SPerl_OP_C_CODE_PRE_INC:
    case SPerl_OP_C_CODE_POST_INC:
    case SPerl_OP_C_CODE_PRE_DEC:
    case SPerl_OP_C_CODE_POST_DEC:
    case SPerl_OP_C_CODE_LEFT_SHIFT:
    case SPerl_OP_C_CODE_RIGHT_SHIFT:
    case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED:
    case SPerl_OP_C_CODE_BIT_XOR:
    case SPerl_OP_C_CODE_BIT_OR:
    case SPerl_OP_C_CODE_BIT_AND:
    case SPerl_OP_C_CODE_PLUS:
    case SPerl_OP_C_CODE_NEGATE:
    case SPerl_OP_C_CODE_ASSIGN:
    case SPerl_OP_C_CODE_MALLOC:
    {
      resolved_type = SPerl_OP_get_resolved_type(sperl, op->first);
      break;
    }
    case SPerl_OP_C_CODE_RETURN: {
      if (op->first) {
        resolved_type = SPerl_OP_get_resolved_type(sperl, op->first);
      }
      break;
    }
    case SPerl_OP_C_CODE_CONVERT: {
      SPerl_OP* op_type = op->last;
      resolved_type = SPerl_OP_get_resolved_type(sperl, op_type);
      break;
    }
    case SPerl_OP_C_CODE_TYPE: {
      resolved_type = op->uv.type->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_SWITCH_CONDITION : {
      resolved_type = SPerl_OP_get_resolved_type(sperl, op->first);
      break;
    }
    case SPerl_OP_C_CODE_UNDEF : {
      resolved_type = NULL;
      break;
    }
    case SPerl_OP_C_CODE_CONSTANT: {
      SPerl_CONSTANT* constant = op->uv.constant;
      resolved_type = constant->resolved_type;
      break;
    }
    case SPerl_OP_C_CODE_VAR: {
      SPerl_VAR* var = op->uv.var;
      if (var->op_my_var->uv.my_var->op_type) {
        resolved_type = var->op_my_var->uv.my_var->op_type->uv.type->resolved_type;
      }
      break;
    }
    case SPerl_OP_C_CODE_MY_VAR: {
      SPerl_MY_VAR* my_var = op->uv.my_var;
      if ( my_var->op_type) {
        resolved_type = my_var->op_type->uv.type->resolved_type;
      }
      break;
    }
    case SPerl_OP_C_CODE_CALL_SUB: {
      SPerl_NAME_INFO* name_info = op->uv.name_info;
      const char* abs_name = name_info->resolved_name;
      SPerl_OP* op_sub = SPerl_HASH_search(sperl, parser->op_sub_symtable, abs_name, strlen(abs_name));
      SPerl_SUB* sub = op_sub->uv.sub;
      if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
        resolved_type = sub->op_return_type->uv.type->resolved_type;
      }
      break;
    }
    case SPerl_OP_C_CODE_CALL_FIELD: {
      SPerl_NAME_INFO* name_info = op->uv.name_info;
      const char* abs_name = name_info->resolved_name;
      SPerl_OP* op_field = SPerl_HASH_search(sperl, parser->op_field_symtable, abs_name, strlen(abs_name));
      SPerl_FIELD* field = op_field->uv.field;
      resolved_type = field->op_type->uv.type->resolved_type;
      break;
    }
  }
  
  return resolved_type;
}

void SPerl_OP_convert_and_to_if(SPerl* sperl, SPerl_OP* op) {
  
  /* before
    AND
      x
      y
  */
  
  /* after 
    IF
      x
      IF
        y
        1
        0
      0
  */
  
  SPerl_OP* op_first = op->first;
  SPerl_OP* op_last = op->last;
  
  // Constant false 1
  SPerl_OP* op_constant_false1 = SPerl_OP_new_op_constant_long(sperl, 0, op->file, op->line);
  
  // Constant false 2
  SPerl_OP* op_constant_false2 = SPerl_OP_new_op_constant_long(sperl, 0, op->file, op->line);
  
  // Constant true
  SPerl_OP* op_constant_true = SPerl_OP_new_op_constant_long(sperl, 1, op->file, op->line);
  
  // if
  SPerl_OP* op_if = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_IF, op->file, op->line);
  
  // and to if
  op->code = SPerl_OP_C_CODE_IF;
  op->first = NULL;
  
  op_if = SPerl_OP_build_if_statement(sperl, op_if, op_last, op_constant_true, op_constant_false2);
  
  op_first->sibparent = op_if;
  
  op_if->sibparent = op_if;
  
  SPerl_OP_sibling_splice(sperl, op, op_if, 0, op_constant_false1);
}

void SPerl_OP_convert_or_to_if(SPerl* sperl, SPerl_OP* op) {
  
  // before
  //  OR
  //    x
  //    y
  
  // after 
  //  IF
  //    x
  //    1
  //    IF
  //      y
  //      1
  //      0
  
  SPerl_OP* op_first = op->first;
  SPerl_OP* op_last = op->last;
  
  // Constant true 1
  SPerl_OP* op_constant_true1 = SPerl_OP_new_op_constant_long(sperl, 1, op->file, op->line);
  
  // Constant true 2
  SPerl_OP* op_constant_true2 = SPerl_OP_new_op_constant_long(sperl, 1, op->file, op->line);
  
  // Constant false
  SPerl_OP* op_constant_false = SPerl_OP_new_op_constant_long(sperl, 0, op->file, op->line);
  
  // if
  SPerl_OP* op_if = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_IF, op->file, op->line);
  
  // or to if
  op->code = SPerl_OP_C_CODE_IF;
  op->first = NULL;
  
  op_if = SPerl_OP_build_if_statement(sperl, op_if, op_last, op_constant_true2, op_constant_false);
  
  op_first->sibparent = op_constant_true1;
  
  SPerl_OP_sibling_splice(sperl, op, op_constant_true1, 0, op_if);
}

void SPerl_OP_convert_not_to_if(SPerl* sperl, SPerl_OP* op) {
  
  // before
  //  NOT
  //    x
  
  // after 
  //  IF
  //    x
  //    0
  //    1
  
  SPerl_OP* op_first = op->first;
  
  // Constant true 1
  SPerl_OP* op_constant_true = SPerl_OP_new_op_constant_long(sperl, 1, op->file, op->line);
  
  // Constant false
  SPerl_OP* op_constant_false = SPerl_OP_new_op_constant_long(sperl, 0, op->file, op->line);
  
  // If
  op->code = SPerl_OP_C_CODE_IF;
  op->first = NULL;
  op = SPerl_OP_build_if_statement(sperl, op, op_first, op_constant_false, op_constant_true);
}

void SPerl_OP_resolve_types(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_ARRAY* op_types = parser->op_types;
  
  for (int32_t i = 0, len = op_types->length; i < len; i++) {
    SPerl_OP* op_type = SPerl_ARRAY_fetch(sperl, op_types, i);
    _Bool success = SPerl_TYPE_resolve_type(sperl, op_type, 0);
    
    if (!success) {
      parser->fatal_error = 1;
      return;
    }
  }
}

void SPerl_OP_check(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  // Resolve types
  SPerl_OP_resolve_types(sperl);
  if (parser->fatal_error) {
    return;
  }
  
  // Resolve package
  SPerl_ARRAY* op_packages = sperl->parser->op_packages;
  for (int32_t package_pos = 0; package_pos < op_packages->length; package_pos++) {
    SPerl_OP* op_package = SPerl_ARRAY_fetch(sperl, op_packages, package_pos);
    SPerl_PACKAGE* package = op_package->uv.package;
    SPerl_ARRAY* op_fields = package->op_fields;
    
    // Alignment is max size of field
    int32_t alignment = 0;
    for (int32_t field_pos = 0; field_pos < op_fields->length; field_pos++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(sperl, op_fields, field_pos);
      SPerl_FIELD* field = op_field->uv.field;
      
      // Alignment
      int32_t field_byte_size = SPerl_FIELD_get_byte_size(sperl, field);
      if (field_byte_size > alignment) {
        alignment = field_byte_size;
      }
    }
    
    // Calculate package byte size
    int32_t package_byte_size = 0;
    for (int32_t field_pos = 0; field_pos < op_fields->length; field_pos++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(sperl, op_fields, field_pos);
      SPerl_FIELD* field = op_field->uv.field;
      
      // Current byte size
      
      int32_t field_byte_size = SPerl_FIELD_get_byte_size(sperl, field);
      int32_t next_alignment_base;
      if (package_byte_size % alignment == 0) {
        next_alignment_base = package_byte_size  + alignment;
      }
      else {
        next_alignment_base = ((package_byte_size / alignment) + 1) * alignment;
      }
      
      if (package_byte_size + field_byte_size > next_alignment_base) {
        int32_t padding = alignment - (package_byte_size % alignment);
        package_byte_size += padding;
      }
      field->package_byte_offset = package_byte_size;
      package_byte_size += field_byte_size;
    }
    package->byte_size = package_byte_size;
  }
  
  // Check types
  SPerl_OP_CHECKER_check(sperl);
  if (parser->fatal_error) {
    return;
  }
}

void SPerl_OP_resolve_sub_name(SPerl* sperl, SPerl_OP* op_package, SPerl_OP* op_name) {
  
  SPerl_NAME_INFO* name_info = op_name->uv.name_info;
  
  const char* sub_abs_name = NULL;
  if (name_info->code == SPerl_NAME_INFO_C_CODE_VARBASENAME) {
    const char* package_name = name_info->op_var->uv.var->op_my_var->uv.my_var->op_type->uv.type->resolved_type->name;
    const char* sub_name = name_info->op_name->uv.name;
    sub_abs_name = SPerl_OP_create_abs_name(sperl, package_name, sub_name);
  }
  else if (name_info->code == SPerl_NAME_INFO_C_CODE_ABSNAME) {
    sub_abs_name = name_info->op_name->uv.name;
  }
  else if (name_info->code == SPerl_NAME_INFO_C_CODE_BASENAME) {
    const char* package_name = op_package->uv.package->op_name->uv.name;
    const char* sub_name = name_info->op_name->uv.name;
    sub_abs_name = SPerl_OP_create_abs_name(sperl, package_name, sub_name);
  }
  
  name_info->resolved_name = sub_abs_name;
}

void SPerl_OP_resolve_field_name(SPerl* sperl, SPerl_OP* op_field) {
  
  SPerl_OP* op_term_invoker = op_field->first;
  SPerl_OP* op_name = op_field->last;
  
  SPerl_RESOLVED_TYPE* invoker_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term_invoker);
  const char* package_name = invoker_resolved_type->name;
  const char* field_name = op_name->uv.name;
  const char* field_abs_name = SPerl_OP_create_abs_name(sperl, package_name, field_name);
  
  op_field->uv.name_info->resolved_name = field_abs_name;
}

SPerl_OP* SPerl_OP_build_array_elem(SPerl* sperl, SPerl_OP* op_var, SPerl_OP* op_term) {
  
  SPerl_OP* op_array_elem = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ARRAY_ELEM, op_var->file, op_var->line);
  SPerl_OP_sibling_splice(sperl, op_array_elem, NULL, 0, op_var);
  SPerl_OP_sibling_splice(sperl, op_array_elem, op_array_elem->last, 0, op_term);
  
  return op_array_elem;
}

SPerl_OP* SPerl_OP_build_call_field(SPerl* sperl, SPerl_OP* op_var, SPerl_OP* op_name_field) {
  SPerl_OP* op_field = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CALL_FIELD, op_var->file, op_var->line);
  SPerl_OP_sibling_splice(sperl, op_field, NULL, 0, op_var);
  SPerl_OP_sibling_splice(sperl, op_field, op_var, 0, op_name_field);
  
  SPerl_NAME_INFO* name_info = SPerl_NAME_INFO_new(sperl);
  
  if (strstr(op_name_field->uv.name, ":")) {
    SPerl_yyerror_format(sperl, "field name \"%s\" can't contain :: at %s line %d\n",
      op_name_field, op_name_field->file, op_name_field->line);
  }
  
  name_info->code = SPerl_NAME_INFO_C_CODE_VARBASENAME;
  name_info->op_var = op_var;
  name_info->op_name = op_name_field;
  op_field->uv.name_info = name_info;
  
  return op_field;
}

SPerl_OP* SPerl_OP_build_array_init(SPerl* sperl, SPerl_OP* op_opt_terms) {
  SPerl_OP* op_new_array = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NEW_ARRAY_CONSTANT, op_opt_terms->file, op_opt_terms->line);
  SPerl_OP_sibling_splice(sperl, op_new_array, NULL, 0, op_opt_terms);
  
  return op_new_array;
}

SPerl_OP* SPerl_OP_build_convert_type(SPerl* sperl, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  SPerl_OP* op_convert_type = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONVERT, op_type->file, op_type->line);
  SPerl_OP_sibling_splice(sperl, op_convert_type, NULL, 0, op_term);
  SPerl_OP_sibling_splice(sperl, op_convert_type, op_term, 0, op_type);
  
  op_convert_type->file = op_type->file;
  op_convert_type->line = op_type->line;
  
  return op_convert_type;
}

SPerl_OP* SPerl_OP_build_grammar(SPerl* sperl, SPerl_OP* op_packages) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_OP* op_grammar = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_GRAMMAR, op_packages->file, op_packages->line);
  SPerl_OP_sibling_splice(sperl, op_grammar, NULL, 0, op_packages);
  
  parser->op_grammar = op_grammar;
  
  // Resovle types, check types, and names.
  SPerl_OP_check(sperl);
  if (parser->fatal_error) {
    return NULL;
  }
  
  // Create bytecodes
  if (parser->error_count > 0) {
    return NULL;
  }
  
  SPerl_BYTECODE_BUILDER_build_bytecode_array(sperl);
  
  return op_grammar;
}

const char* SPerl_OP_create_abs_name(SPerl* sperl, const char* package_name, const char* name) {
  int32_t length = (int32_t)(strlen(package_name) + 2 + strlen(name));
  
  char* abs_name = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, sperl->parser->allocator, length);
  
  sprintf(abs_name, "%s::%s", package_name, name);
  
  return abs_name;
}

SPerl_OP* SPerl_OP_build_package(SPerl* sperl, SPerl_OP* op_package, SPerl_OP* op_name_package, SPerl_OP* op_block) {
  
  SPerl_PARSER* parser = sperl->parser;

  SPerl_OP_sibling_splice(sperl, op_package, NULL, 0, op_name_package);
  SPerl_OP_sibling_splice(sperl, op_package, op_name_package, 0, op_block);
  
  const char* package_name = op_name_package->uv.name;
  SPerl_HASH* op_package_symtable = parser->op_package_symtable;
  
  // Redeclaration package error
  SPerl_OP* found_op_package = SPerl_HASH_search(sperl, op_package_symtable, package_name, strlen(package_name));
  if (found_op_package) {
    SPerl_yyerror_format(sperl, "redeclaration of package \"%s\" at %s line %d\n", package_name, op_package->file, op_package->line);
  }
  else {
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(sperl);
    package->op_name = op_name_package;
    
    // Type(type is same as package name)
    SPerl_TYPE* type = SPerl_TYPE_new(sperl);
    type->code = SPerl_TYPE_C_CODE_NAME;
    SPerl_TYPE_COMPONENT_NAME* type_component_name = SPerl_TYPE_COMPONENT_NAME_new(sperl);
    type_component_name->op_name = op_name_package;
    type->uv.type_component_name = type_component_name;
    
    // Type OP
    SPerl_OP* op_type = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_TYPE, op_name_package->file, op_name_package->line);
    op_type->uv.type = type;
    
    // Add type
    package->op_type = op_type;
    SPerl_ARRAY_push(sperl, parser->op_types, op_type);
    
    SPerl_ARRAY* op_fields = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, sperl->parser->allocator, 0);
    SPerl_ARRAY* op_subs = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, sperl->parser->allocator, 0);
    
    // Fields
    SPerl_OP* op_decls = op_block->first;
    SPerl_OP* op_decl = op_decls->first;
    while ((op_decl = SPerl_OP_sibling(sperl, op_decl))) {
      if (op_decl->code == SPerl_OP_C_CODE_FIELD) {
        SPerl_OP* op_field = op_decl;
        SPerl_FIELD* field = op_field->uv.field;
        const char* field_name = field->op_name->uv.name;
        SPerl_OP* found_op_field = SPerl_HASH_search(sperl, parser->op_field_symtable, field_name, strlen(field_name));
        
        if (found_op_field) {
          SPerl_yyerror_format(sperl, "redeclaration of has \"%s\" at %s line %d\n", field_name, op_field->file, op_field->line);
        }
        else if (op_fields->length >= SPerl_OP_LIMIT_FIELDS) {
          SPerl_yyerror_format(sperl, "too many fields, field \"%s\" ignored at %s line %d\n", field_name, op_field->file, op_field->line);
        }
        else {
          SPerl_ARRAY_push(sperl, op_fields, op_field);
          
          const char* field_abs_name = SPerl_OP_create_abs_name(sperl, package_name, field_name);
          field->abs_name = field_abs_name;
          
          // Add op field symtable
          SPerl_HASH_insert(sperl, parser->op_field_symtable, field_abs_name, strlen(field_abs_name), op_field);
        }
      }
      else if (op_decl->code == SPerl_OP_C_CODE_SUB) {
        SPerl_OP* op_sub = op_decl;
        SPerl_SUB* sub = op_sub->uv.sub;
        
        SPerl_OP* op_name_sub = sub->op_name;
        const char* sub_name = op_name_sub->uv.name;
        const char* sub_abs_name = SPerl_OP_create_abs_name(sperl, package_name, sub_name);
        
        SPerl_OP* found_op_sub = SPerl_HASH_search(sperl, parser->op_sub_symtable, sub_abs_name, strlen(sub_abs_name));
        
        if (found_op_sub) {
          SPerl_yyerror_format(sperl, "redeclaration of sub \"%s\" at %s line %d\n", sub_abs_name, op_sub->file, op_sub->line);
        }
        // Unknown sub
        else {
          // Bind standard functions
          if (sub->is_native) {
            if (strcmp(sub_abs_name, "std::printb") == 0) {
              sub->native_address = SPerl_FUNC_std_printb;
            }
            else if (strcmp(sub_abs_name, "std::prints") == 0) {
              sub->native_address = SPerl_FUNC_std_prints;
            }
            else if (strcmp(sub_abs_name, "std::printi") == 0) {
              sub->native_address = SPerl_FUNC_std_printi;
            }
            else if (strcmp(sub_abs_name, "std::printl") == 0) {
              sub->native_address = SPerl_FUNC_std_printl;
            }
            else if (strcmp(sub_abs_name, "std::printf") == 0) {
              sub->native_address = SPerl_FUNC_std_printf;
            }
            else if (strcmp(sub_abs_name, "std::printd") == 0) {
              sub->native_address = SPerl_FUNC_std_printd;
            }
            else if (strcmp(sub_abs_name, "std::println") == 0) {
              sub->native_address = SPerl_FUNC_std_println;
            }
            else if (strcmp(sub_abs_name, "std::suml") == 0) {
              sub->native_address = SPerl_FUNC_std_suml;
            }
            else if (strcmp(sub_abs_name, "std::test_call1") == 0) {
              sub->native_address = SPerl_FUNC_std_test_call1;
            }
            else if (strcmp(sub_abs_name, "std::test_call2") == 0) {
              sub->native_address = SPerl_FUNC_std_test_call2;
            }
          }
        }
        
        sub->abs_name = sub_abs_name;
        
        SPerl_HASH_insert(sperl, parser->op_sub_symtable, sub_abs_name, strlen(sub_abs_name), op_sub);
        SPerl_ARRAY_push(sperl, op_subs, op_sub);
      }
      else if (op_decl->code == SPerl_OP_C_CODE_DECL_ENUM) {
        SPerl_OP* op_enumeration = op_decl;
        SPerl_OP* op_enumeration_block = op_enumeration->first;
        
        // Starting value
        int32_t start_value = 0;
        SPerl_OP* op_enumeration_values = op_enumeration_block->first;
        SPerl_OP* op_enumeration_value = op_enumeration_values->first;
        while ((op_enumeration_value = SPerl_OP_sibling(sperl, op_enumeration_value))) {
          SPerl_ENUMERATION_VALUE* enumeration_value = SPerl_ENUMERATION_VALUE_new(sperl);
          enumeration_value->op_name = op_enumeration_value->first;
          if (op_enumeration_value->first != op_enumeration_value->last) {
            enumeration_value->op_constant = op_enumeration_value->last;
          }
          
          SPerl_OP* op_constant;
          if (enumeration_value->op_constant) {
            op_constant = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CONSTANT, op_enumeration_value->file, op_enumeration_value->line);
            op_constant->uv.constant = enumeration_value->op_constant->uv.constant;
            start_value = op_constant->uv.constant->uv.long_value + 1;
          }
          else {
            op_constant = SPerl_OP_new_op_constant_long(sperl, start_value, op_enumeration_value->file, op_enumeration_value->line);
            enumeration_value->op_constant = op_constant;
            start_value++;
          }
          
          // sub
          SPerl_OP* op_sub = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_SUB, op_enumeration_value->file, op_enumeration_value->line);
          op_sub->file = op_enumeration_value->file;
          op_sub->line = op_enumeration_value->line;
          
          // Type
          SPerl_OP* op_return_type = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_TYPE, op_enumeration_value->file, op_enumeration_value->line);
          SPerl_TYPE* return_type = SPerl_TYPE_new(sperl);
          return_type->resolved_type = op_constant->uv.constant->resolved_type;
          op_return_type->uv.type = return_type;

          // Name
          SPerl_OP* op_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_enumeration_value->file, op_enumeration_value->line);
          op_name->uv.name = enumeration_value->op_name->uv.name;
          
          // Return
          SPerl_OP* op_return = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_RETURN, op_enumeration_value->file, op_enumeration_value->line);
          SPerl_OP_sibling_splice(sperl, op_return, NULL, 0, op_constant);
          
          // Create sub information
          SPerl_SUB* sub = SPerl_SUB_new(sperl);
          sub->op_name = op_name;
          sub->op_return_type = op_return_type;
          sub->op_block = op_constant;
          sub->is_constant = 1;

          // Create absolute name
          const char* sub_abs_name = SPerl_OP_create_abs_name(sperl, package_name, op_name->uv.name);
          sub->abs_name = sub_abs_name;
          
          // Set sub
          op_sub->uv.sub = sub;
         
          SPerl_OP* found_op_sub = SPerl_HASH_search(sperl, parser->op_sub_symtable, sub_abs_name, strlen(sub_abs_name));
          
          if (found_op_sub) {
            SPerl_yyerror_format(sperl, "redeclaration of sub \"%s\" at %s line %d\n", sub_abs_name, op_sub->file, op_sub->line);
          }
          // Unknown sub
          else {
            SPerl_HASH_insert(sperl, parser->op_sub_symtable, sub_abs_name, strlen(sub_abs_name), op_sub);
            SPerl_ARRAY_push(sperl, op_subs, op_sub);
          }
        }
      }
    }
    package->op_fields = op_fields;
    package->op_subs = op_subs;
    
    // Add package
    op_package->uv.package = package;
    SPerl_ARRAY_push(sperl, parser->op_packages, op_package);
    SPerl_HASH_insert(sperl, parser->op_package_symtable, package_name, strlen(package_name), op_package);
  }
  
  return op_package;
}

SPerl_OP* SPerl_OP_build_use(SPerl* sperl, SPerl_OP* op_use, SPerl_OP* op_name_package) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_OP_sibling_splice(sperl, op_use, NULL, 0, op_name_package);
  
  const char* package_name = op_name_package->uv.name;
  SPerl_OP* found_op_use = SPerl_HASH_search(sperl, parser->op_use_symtable, package_name, strlen(package_name));
  
  if (!found_op_use) {
    SPerl_ARRAY_push(sperl, parser->op_use_stack, op_use);
    SPerl_HASH_insert(sperl, parser->op_use_symtable, package_name, strlen(package_name), op_use);
  }
  
  return op_use;
}

SPerl_OP* SPerl_OP_build_my_var(SPerl* sperl, SPerl_OP* op_my_var, SPerl_OP* op_var, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  // Stab
  SPerl_OP* op_my_var_parent = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_MY_VAR_PARENT, op_my_var->file, op_my_var->line);
  
  // Create my var information
  SPerl_MY_VAR* my_var = SPerl_MY_VAR_new(sperl);
  my_var->op_type = op_type;
  
  // Name OP
  SPerl_OP* op_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_var->file, op_var->line);
  op_name->uv.name = op_var->uv.var->op_name->uv.name;
  my_var->op_name = op_name;

  // Add my_var information to op
  op_my_var->uv.my_var = my_var;
  
  // Add my_var op
  SPerl_OP_sibling_splice(sperl, op_my_var_parent, NULL, 0, op_my_var);
  
  // Assign
  if (op_term) {
    SPerl_OP* op_assign = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ASSIGN, op_my_var->file, op_my_var->line);
    
    op_var->uv.var->op_my_var = op_my_var;
    
    SPerl_OP_sibling_splice(sperl, op_assign, NULL, 0, op_var);
    SPerl_OP_sibling_splice(sperl, op_assign, op_var, 0, op_term);
    
    SPerl_OP_sibling_splice(sperl, op_my_var_parent, op_my_var, 0, op_assign);
    
    // Type assumption
    my_var->op_term_assumption = op_term;
  }
  
  // Type is none
  if (!op_type && (!op_term || op_term->code == SPerl_OP_C_CODE_UNDEF)) {
    SPerl_yyerror_format(sperl, "\"my %s\" can't detect type at %s line %d\n", my_var->op_name->uv.name, op_my_var->file, op_my_var->line);
  }
  
  return op_my_var_parent;
}

SPerl_OP* SPerl_OP_build_field(SPerl* sperl, SPerl_OP* op_field, SPerl_OP* op_name_field, SPerl_OP* op_type) {
  
  // Build OP
  SPerl_OP_sibling_splice(sperl, op_field, NULL, 0, op_name_field);
  SPerl_OP_sibling_splice(sperl, op_field, op_name_field, 0, op_type);
  
  // Create field information
  SPerl_FIELD* field = SPerl_FIELD_new(sperl);
  
  // Name
  field->op_name = op_name_field;
  
  // Type
  field->op_type = op_type;
  
  // Set field informaiton
  op_field->uv.field = field;
  
  return op_field;
}

SPerl_OP* SPerl_OP_build_sub(SPerl* sperl, SPerl_OP* op_sub, SPerl_OP* op_name_sub, SPerl_OP* op_args, SPerl_OP* op_descriptors, SPerl_OP* op_type_or_void, SPerl_OP* op_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(sperl, op_sub, NULL, 0, op_name_sub);
  SPerl_OP_sibling_splice(sperl, op_sub, op_name_sub, 0, op_args);
  SPerl_OP_sibling_splice(sperl, op_sub, op_args, 0, op_descriptors);
  SPerl_OP_sibling_splice(sperl, op_sub, op_descriptors, 0, op_type_or_void);
  if (op_block) {
    op_block->flag = SPerl_OP_C_FLAG_BLOCK_SUB;
    SPerl_OP_sibling_splice(sperl, op_sub, op_type_or_void, 0, op_block);
  }
  
  // Create sub information
  SPerl_SUB* sub = SPerl_SUB_new(sperl);
  sub->op_name = op_name_sub;
  
  // Descriptors
  SPerl_OP* op_descriptor = op_descriptors->first;
  while ((op_descriptor = SPerl_OP_sibling(sperl, op_descriptor))) {
    if (op_descriptor->code == SPerl_DESCRIPTOR_C_CODE_NATIVE) {
      sub->is_native = 1;
    }
    else {
      SPerl_yyerror_format(sperl, "invalid descriptor %s", SPerl_DESCRIPTOR_C_CODE_NAMES[op_descriptor->code], op_descriptors->file, op_descriptors->line);
    }
  }
  
  // Native subroutine can't have block
  if (sub->is_native && op_block) {
    SPerl_yyerror_format(sperl, "Native subroutine can't have block", op_block->file, op_block->line);
  }
  
  // subargs
  {
    SPerl_OP* op_arg = op_args->first;
    while ((op_arg = SPerl_OP_sibling(sperl, op_arg))) {
      SPerl_ARRAY_push(sperl, sub->op_args, op_arg->first);
    }
  }
  
  // Add my declaration to top of block
  if (op_block) {
    SPerl_OP* op_list_statement = op_block->first;
    SPerl_OP* op_args_my_vars = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ARGS_MY_VARS, op_list_statement->file, op_list_statement->line);
    for (int32_t i = 0; i < sub->op_args->length; i++) {
      SPerl_OP* op_arg = SPerl_ARRAY_fetch(sperl, sub->op_args, i);
      SPerl_OP* op_my_var = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_MY_VAR, op_arg->file, op_arg->line);
      op_my_var->uv.my_var = op_arg->uv.my_var;
      SPerl_OP* op_my_var_parent = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_MY_VAR_PARENT, op_arg->file, op_arg->line);
      SPerl_OP_sibling_splice(sperl, op_my_var_parent, op_my_var_parent->last, 0, op_my_var);
      
      SPerl_OP_sibling_splice(sperl, op_args_my_vars, op_args_my_vars->first, 0, op_my_var_parent);
    }
    SPerl_OP_sibling_splice(sperl, op_list_statement, op_list_statement->first, 0, op_args_my_vars);
  }
  
  // return type
  sub->op_return_type = op_type_or_void;
  
  // Save block
  sub->op_block = op_block;

  op_sub->uv.sub = sub;
  
  return op_sub;
}

SPerl_OP* SPerl_OP_build_enumeration(SPerl* sperl, SPerl_OP* op_enumeration, SPerl_OP* op_enumeration_block) {
  
  // Build OP_SUB
  SPerl_OP_sibling_splice(sperl, op_enumeration, NULL, 0, op_enumeration_block);
  
  return op_enumeration;
}

SPerl_OP* SPerl_OP_build_call_sub(SPerl* sperl, SPerl_OP* op_invocant, SPerl_OP* op_name_sub, SPerl_OP* op_terms) {
  
  // Build OP_SUB
  SPerl_OP* op_call_sub = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_CALL_SUB, op_name_sub->file, op_name_sub->line);
  SPerl_OP_sibling_splice(sperl, op_call_sub, NULL, 0, op_name_sub);
  SPerl_OP_sibling_splice(sperl, op_call_sub, op_name_sub, 0, op_terms);
  
  SPerl_NAME_INFO* name_info = SPerl_NAME_INFO_new(sperl);
  
  const char* sub_name = op_name_sub->uv.name;
  SPerl_OP* op_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_invocant->file, op_invocant->line);
  
  // Normal call
  if (op_invocant->code == SPerl_OP_C_CODE_NULL) {
    // Absolute
    // P::m();
    if (strstr(sub_name, ":")) {
      name_info->code = SPerl_NAME_INFO_C_CODE_ABSNAME;
      op_name->uv.name = sub_name;
      name_info->op_name = op_name;
    }
    // Base name
    // m();
    else {
      name_info->code = SPerl_NAME_INFO_C_CODE_BASENAME;
      op_name->uv.name = sub_name;
      name_info->op_name = op_name;
    }
  }
  // Method call
  else if (op_invocant->code == SPerl_OP_C_CODE_VAR) {
    // Absolute
    // $var->P::m();
    if (strstr(sub_name, ":")) {
      name_info->code = SPerl_NAME_INFO_C_CODE_ABSNAME;
      op_name->uv.name = sub_name;
      name_info->op_name = op_name;
    }
    // Base name
    // $var->m();
    else {
      name_info->code = SPerl_NAME_INFO_C_CODE_VARBASENAME;
      name_info->op_var = op_invocant;
      name_info->op_name = op_name_sub;
    }
    SPerl_OP_sibling_splice(sperl, op_terms, op_terms->first, 0, op_invocant);
  }
  // Method call
  else if (op_invocant->code == SPerl_OP_C_CODE_NAME) {
    // Absolute
    // P->Q::m;
    if (strstr(sub_name, ":")) {
      SPerl_yyerror_format(sperl, "package name is ambiguas %s line %d\n", op_invocant->file, op_invocant->line);
    }
    // Base name
    else {
      const char* package_name = op_invocant->uv.name;
      const char* name = op_name_sub->uv.name;
      
      // Create abs name
      const char* abs_name = SPerl_OP_create_abs_name(sperl, package_name, name);
      
      // Create op abs name
      SPerl_OP* op_abs_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_invocant->file, op_invocant->line);
      op_abs_name->uv.name = abs_name;
      
      // Set abs name
      name_info->code = SPerl_NAME_INFO_C_CODE_ABSNAME;
      name_info->op_name = op_abs_name;
    }
  }
  
  op_call_sub->uv.name_info = name_info;
  
  return op_call_sub;
}

SPerl_OP* SPerl_OP_build_unop(SPerl* sperl, SPerl_OP* op_unary, SPerl_OP* op_first) {
  
  // Build op
  SPerl_OP_sibling_splice(sperl, op_unary, NULL, 0, op_first);
  
  return op_unary;
}

SPerl_OP* SPerl_OP_build_binop(SPerl* sperl, SPerl_OP* op_bin, SPerl_OP* op_first, SPerl_OP* op_last) {
  
  // Build op
  SPerl_OP_sibling_splice(sperl, op_bin, NULL, 0, op_first);
  if (op_last) {
    SPerl_OP_sibling_splice(sperl, op_bin, op_first, 0, op_last);
  }
  
  return op_bin;
}

SPerl_OP* SPerl_OP_build_type_name(SPerl* sperl, SPerl_OP* op_name) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  // Type component name
  SPerl_TYPE_COMPONENT_NAME* type_component_name = SPerl_TYPE_COMPONENT_NAME_new(sperl);
  type_component_name->op_name = op_name;
  
  // 
  SPerl_TYPE* type = SPerl_TYPE_new(sperl);
  type->code = SPerl_TYPE_C_CODE_NAME;
  type->uv.type_component_name = type_component_name;

  SPerl_OP* op_type_name = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_TYPE, op_name->file, op_name->line);
  SPerl_OP_sibling_splice(sperl, op_type_name, NULL, 0, op_name);
  
  op_type_name->uv.type = type;
  op_type_name->file = op_name->file;
  op_type_name->line = op_name->line;

  SPerl_ARRAY_push(sperl, parser->op_types, op_type_name);
  
  return op_type_name;
}

SPerl_OP* SPerl_OP_build_type_array(SPerl* sperl, SPerl_OP* op_type, SPerl_OP* op_term) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  // Type array
  SPerl_TYPE_COMPONENT_ARRAY* type_component_array = SPerl_TYPE_COMPONENT_ARRAY_new(sperl);
  type_component_array->type = op_type->uv.type;
  type_component_array->op_term = op_term;
  
  // Type
  SPerl_TYPE* type = SPerl_TYPE_new(sperl);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_component_array = type_component_array;
  
  // Type OP
  SPerl_OP* op_type_array = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_TYPE, op_type->file, op_type->line);
  SPerl_OP_sibling_splice(sperl, op_type_array, NULL, 0, op_type);
  
  if (op_term) {
    SPerl_OP_sibling_splice(sperl, op_type_array, op_type_array->last, 0, op_term);
  }
  else {
    SPerl_OP* op_null = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NULL, op_type->file, op_type->line);
    SPerl_OP_sibling_splice(sperl, op_type_array, op_type_array->last, 0, op_null);
  }
  
  op_type_array->uv.type = type;
  op_type_array->file = op_type->file;
  op_type_array->line = op_type->line;
  
  SPerl_ARRAY_push(sperl, parser->op_types, op_type_array);
  
  return op_type_array;
}

SPerl_OP* SPerl_OP_append_elem(SPerl* sperl, SPerl_OP *first, SPerl_OP *last, const char* file, int32_t line) {
  if (!first) {
    return last;
  }
  
  if (!last) {
    return first;
  }
  
  if (first->code == SPerl_OP_C_CODE_LIST) {
    SPerl_OP_sibling_splice(sperl, first, first->last, 0, last);
    return first;
  }
  else {
    SPerl_OP* op_list = SPerl_OP_new_op_list(sperl, file, line);
    SPerl_OP_sibling_splice(sperl, op_list, op_list->first, 0, first);
    SPerl_OP_sibling_splice(sperl, op_list, first, 0, last);
    
    return op_list;
  }
}

SPerl_OP* SPerl_OP_new_op_list(SPerl* sperl, const char* file, int32_t line) {
  
  SPerl_OP* op_pushmark = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_PUSHMARK, file, line);
  
  SPerl_OP* op_list = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_LIST, file, line);
  SPerl_OP_sibling_splice(sperl, op_list, NULL, 0, op_pushmark);
  
  return op_list;
}


SPerl_OP* SPerl_OP_new_op(SPerl* sperl, int32_t code, const char* file, int32_t line) {

  SPerl_OP *op = SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser->allocator, sizeof(SPerl_OP));
  
  memset(op, 0, sizeof(SPerl_OP));
  
  op->code = code;
  
  assert(!op->file);
  assert(!op->line);
  
  op->file = file;
  op->line = line;
  
  return op;
}

SPerl_OP* SPerl_OP_sibling_splice(SPerl* sperl, SPerl_OP* parent, SPerl_OP* start, int32_t del_count, SPerl_OP* insert) {
  SPerl_OP *first;
  SPerl_OP *rest;
  SPerl_OP *last_del = NULL;
  SPerl_OP *last_ins = NULL;

  if (start) {
    first = SPerl_OP_sibling(sperl, start);
  }
  else if (!parent) {
    goto no_parent;
  }
  else {
    first = parent->first;
  }
  
  if (del_count && first) {
    last_del = first;
    while (--del_count && last_del->moresib)
      last_del = SPerl_OP_sibling(sperl, last_del);
    rest = SPerl_OP_sibling(sperl, last_del);
    SPerl_OP_lastsib_set(sperl, last_del, NULL);
  }
  else {
    rest = first;
  }
  
  if (insert) {
    last_ins = insert;
    while (last_ins->moresib) {
      last_ins = SPerl_OP_sibling(sperl, last_ins);
    }
    SPerl_OP_maybesib_set(sperl, last_ins, rest, NULL);
  }
  else {
    insert = rest;
  }

  if (start) {
    SPerl_OP_maybesib_set(sperl, start, insert, NULL);
  }
  else {
    if (!parent) {
      goto no_parent;
    }
    parent->first = insert;
  }
  
  if (!rest) {
    /* update last etc */
    SPerl_OP *lastop;

    if (!parent) {
      goto no_parent;
    }
    lastop = last_ins ? last_ins : start ? start : NULL;
    parent->last = lastop;

    if (lastop) {
      SPerl_OP_lastsib_set(sperl, lastop, parent);
    }
  }
  return last_del ? first : NULL;

  no_parent:
    fprintf(stderr, "panic: op_sibling_splice(): NULL parent");
    exit(1);
}

SPerl_OP* SPerl_OP_sibling(SPerl* sperl, SPerl_OP* op) {
  (void)sperl;
  
  return op->moresib ? op->sibparent : NULL;
}

void SPerl_OP_moresib_set(SPerl* sperl, SPerl_OP* op, SPerl_OP* sib) {
  (void)sperl;
  
  op->moresib = 1;
  op->sibparent = sib;
}

void SPerl_OP_lastsib_set(SPerl* sperl, SPerl_OP* op, SPerl_OP* parent) {
  (void)sperl;
  op->moresib = 0;
  op->sibparent = parent;
}

void SPerl_OP_maybesib_set(SPerl* sperl, SPerl_OP* op, SPerl_OP* sib, SPerl_OP* parent) {
  (void)sperl;
  op->moresib = sib ? 1 : 0;
  op->sibparent = op->moresib ? sib : parent;
}
