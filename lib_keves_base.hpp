/* Keves/lib_keves_base.hpp - library of Keves base
 * Keves will be an R6RS Scheme implementation.
 *
 *  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "keves_library.hpp"

#include "environment_kev.hpp"
#include "generator_kev.hpp"
// #include "macro_kev.hpp"
#include "symbol_kev.hpp"

class CodeKev;
class KevesBuiltInValues;
class LibKevesParse;
class LibRnrsBase;
class LibRnrsEval;
class LibRnrsExceptions;
class LibRnrsLists;
class LibRnrsMutableStrings;
class LibRnrsSyntaxCase;
class LibRnrsUnicode;

class LibKevesBase : public KevesLibrary {
public:
  static constexpr int NUM_OF_GLOBAL_VARS = 50;
  static constexpr int SIZE_OF_BUILTIN_CODE = 0x10000;
  static constexpr int SIZE_OF_PROC_TABLE = 512;

  LibKevesBase();
  LibKevesBase(const LibKevesBase&) = delete;
  LibKevesBase(LibKevesBase&&) = delete;
  LibKevesBase& operator=(const LibKevesBase&) = delete;
  LibKevesBase& operator=(LibKevesBase&&) = delete;
  ~LibKevesBase() = default;

  KevesLibrary* Init(KevesGC* gc);
  KevesValue name();
  EnvironmentKev* environment_base();
  const KevesValue* table() const;

  CodeKev* test_code_;

  /*
  LibKevesParse* lib_keves_parse_;
  LibRnrsBase* lib_rnrs_base_;
  LibRnrsEval* lib_rnrs_eval_;
  LibRnrsExceptions* lib_rnrs_exceptions_;
  LibRnrsLists* lib_rnrs_lists_;
  LibRnrsMutableStrings* lib_rnrs_mutable_strings_;
  LibRnrsSyntaxCase* lib_rnrs_syntax_case_;
  LibRnrsUnicode* lib_rnrs_unicode_;

  SymbolKev sym_display_;
  CPSKev proc_display_;
  SymbolKev sym_newline_;
  CPSKev proc_newline_;
  SymbolKev sym_u8_list_to_vector_;
  CPSKev proc_u8_list_to_vector_;

  SymbolKev sym_keves_make_macro_;
  LambdaKev proc_keves_make_macro_;
  SymbolKev sym_keves_make_macro_helper_;
  CPSKev proc_keves_make_macro_helper_;
  SymbolKev sym_and_;
  GeneratorKev generator_and_;
  SymbolKev sym_begin_;
  GeneratorKev generator_begin_;
  SymbolKev sym_begin0_;
  GeneratorKev generator_begin0_;
  SymbolKev sym_cond_;
  MacroKev syntax_cond_;
  SymbolKev sym_define_;
  GeneratorKev generator_define_;
  SymbolKev sym_define_syntax_;
  GeneratorKev generator_define_syntax_;
  SymbolKev sym_if_;
  GeneratorKev generator_if_;
  SymbolKev sym_lambda_;
  GeneratorKev generator_lambda_;
  SymbolKev sym_let_;
  MacroKev syntax_let_;
  SymbolKev sym_let_star_;
  MacroKev syntax_let_star_;
  SymbolKev sym_letrec_;
  MacroKev syntax_letrec_;
  SymbolKev sym_letrec_star_;
  MacroKev syntax_letrec_star_;
  SymbolKev sym_or_;
  GeneratorKev generator_or_;
  SymbolKev sym_quasiquote_;
  MacroKev syntax_quasiquote_;
  SymbolKev sym_quote_;
  GeneratorKev generator_quote_;
  SymbolKev sym_set_e_;
  GeneratorKev generator_set_e_;

  SymbolKev sym_else_;
  SymbolKev sym_export_;
  SymbolKev sym_import_;
  SymbolKev sym_library_;
  SymbolKev sym_syntax_rules_;
  SymbolKev sym_underscore_;
  SymbolKev sym_ellipsis_;
  SymbolKev sym_keves_add_library_;
  LambdaKev proc_keves_add_library_;
  SymbolKev sym_keves_asm_instruction_;
  CPSKev proc_keves_asm_instruction_;
  SymbolKev sym_keves_condition_;
  CPSKev proc_keves_condition_;
  SymbolKev sym_keves_current_environment_;
  CPSKev proc_keves_current_environment_;
  SymbolKev sym_keves_eval_;
  LambdaKev proc_keves_eval_;
  SymbolKev sym_keves_expand_;
  LambdaKev proc_keves_expand_;
  SymbolKev sym_keves_expand_helper_;
  LambdaKev proc_keves_expand_helper_;
  SymbolKev sym_keves_expand_for_define_;
  LambdaKev proc_keves_expand_for_define_;
  SymbolKev sym_keves_transform_;
  LambdaKev proc_keves_transform_;
  SymbolKev sym_keves_expand_library_;
  LambdaKev proc_keves_expand_library_;
  SymbolKev sym_keves_expand_toplevel_;
  LambdaKev proc_keves_expand_toplevel_;
  SymbolKev sym_keves_export_;
  LambdaKev proc_keves_export_;
  SymbolKev sym_keves_eval_code_;
  LambdaKev proc_keves_eval_code_;
  SymbolKev sym_keves_find_global_variable_;
  CPSKev proc_keves_find_global_variable_;
  SymbolKev sym_keves_find_syntax_id_;
  CPSKev proc_keves_find_syntax_id_;
  SymbolKev sym_keves_find_internal_variable_;
  CPSKev proc_keves_find_internal_variable_;
  SymbolKev sym_keves_find_library_;
  LambdaKev proc_keves_find_library_;
  SymbolKev sym_keves_find_macro_;
  LambdaKev proc_keves_find_macro_;
  SymbolKev sym_keves_get_free_variable_index_;
  LambdaKev proc_keves_get_free_variable_index_;
  // SymbolKev sym_keves_get_local_variable_index_;
  // LambdaKev proc_keves_get_local_variable_index_;
  SymbolKev sym_keves_get_variable_index_;
  LambdaKev proc_keves_get_variable_index_;
  SymbolKev sym_keves_get_macro_body_;
  CPSKev proc_keves_get_macro_body_;
  SymbolKev sym_keves_get_generator_body_;
  CPSKev proc_keves_get_generator_body_;
  SymbolKev sym_keves_load_library_;
  CPSKev proc_keves_load_library_;
  SymbolKev sym_keves_macro_q_;
  CPSKev proc_keves_macro_q_;
  SymbolKev sym_keves_make_asm_code_;
  LambdaKev proc_keves_make_asm_code_;
  SymbolKev sym_keves_make_assertion_violation_;
  CPSKev proc_keves_make_assertion_violation_;
  SymbolKev sym_keves_make_irritants_condition_;
  CPSKev proc_keves_make_irritants_condition_;
  SymbolKev sym_keves_make_label_;
  CPSKev proc_keves_make_label_;
  SymbolKev sym_keves_make_label_setter_;
  CPSKev proc_keves_make_label_setter_;
  SymbolKev sym_keves_make_local_variable_;
  CPSKev proc_keves_make_local_variable_;
  SymbolKev sym_keves_make_free_variable_;
  CPSKev proc_keves_make_free_variable_;
  SymbolKev sym_keves_make_message_condition_;
  CPSKev proc_keves_make_message_condition_;
  SymbolKev sym_keves_make_who_condition_;
  CPSKev proc_keves_make_who_condition_;
  SymbolKev sym_keves_read_library_;
  LambdaKev proc_keves_read_library_;
  SymbolKev sym_keves_refer_builtin_table_;
  CPSKev proc_keves_refer_builtin_table_;
  SymbolKev sym_keves_separate_vars_init_;
  LambdaKev proc_keves_separate_vars_init_;
  SymbolKev sym_keves_generator_q_;
  CPSKev proc_keves_generator_q_;
  SymbolKev sym_keves_syntax_rules_;
  LambdaKev proc_keves_syntax_rules_;
  SymbolKev sym_keves_export_symbol_;
  GeneratorKev generator_keves_export_symbol_;
  */

  static KevesValue makeLexicalCondition(KevesIterator*, KevesValue, KevesValue, KevesValue);

private:
  PairKev pair_for_name1_;
  PairKev pair_for_name2_;
  SymbolKev symbol_for_name1_;
  SymbolKev symbol_for_name2_;

  KevesValue table_[SIZE_OF_PROC_TABLE];
  EnvironmentKev environment_base_;
  KevesBuiltInValues* builtin_;
  char builtin_code_array_[SIZE_OF_BUILTIN_CODE];
  CodeKev* builtin_code_;

  /*
  KevesIterator appendCode_proc_keves_add_library(KevesIterator);
  KevesIterator appendCode_proc_keves_eval(KevesIterator);
  KevesIterator appendCode_proc_keves_expand(KevesIterator);
  KevesIterator appendCode_proc_keves_expand_helper(KevesIterator);
  KevesIterator appendCode_proc_keves_expand_for_define(KevesIterator);
  KevesIterator appendCode_proc_keves_transform(KevesIterator);
  KevesIterator appendCode_proc_keves_expand_library(KevesIterator);
  KevesIterator appendCode_proc_keves_expand_toplevel(KevesIterator);
  KevesIterator appendCode_proc_keves_export(KevesIterator);
  KevesIterator appendCode_proc_keves_eval_code(KevesIterator);
  KevesIterator appendCode_proc_keves_find_library(KevesIterator);
  KevesIterator appendCode_proc_keves_find_macro(KevesIterator);
  KevesIterator appendCode_proc_keves_get_free_variable_index(KevesIterator);
  // KevesIterator appendCode_proc_keves_get_local_variable_index(KevesIterator);
  KevesIterator appendCode_proc_keves_get_variable_index(KevesIterator);
  KevesIterator appendCode_proc_keves_make_asm_code(KevesIterator);
  KevesIterator appendCode_proc_keves_read_library(KevesIterator);
  KevesIterator appendCode_proc_keves_separate_vars_init(KevesIterator);
  KevesIterator appendCode_proc_keves_syntax_rules(KevesIterator);
  KevesIterator appendCode_generator_keves_export_symbol(KevesIterator);

  KevesIterator appendCode_proc_keves_make_macro(KevesIterator);
  KevesIterator appendCode_generator_and(KevesIterator);
  KevesIterator appendCode_generator_begin(KevesIterator);
  KevesIterator appendCode_generator_begin0(KevesIterator);
  KevesIterator appendCode_syntax_cond(KevesIterator);
  KevesIterator appendCode_generator_define(KevesIterator);
  KevesIterator appendCode_generator_define_syntax(KevesIterator);
  KevesIterator appendCode_generator_if(KevesIterator);
  KevesIterator appendCode_generator_lambda(KevesIterator);
  KevesIterator appendCode_syntax_let(KevesIterator);
  KevesIterator appendCode_syntax_let_star(KevesIterator);
  KevesIterator appendCode_syntax_letrec(KevesIterator);
  KevesIterator appendCode_syntax_letrec_star(KevesIterator);
  KevesIterator appendCode_generator_or(KevesIterator);
  KevesIterator appendCode_generator_quote(KevesIterator);
  KevesIterator appendCode_syntax_quasiquote(KevesIterator);
  KevesIterator appendCode_generator_set_e(KevesIterator);

  static void procScmCondition(KevesVM*, const_KevesIterator);
  static void procKevesLoadLibrary(KevesVM*, const_KevesIterator);
  static void procU8ListToVector(KevesVM*, const_KevesIterator);
  static void procU8ListToVector_helper(KevesVM*, const_KevesIterator);

  static void procDisplay(KevesVM*, const_KevesIterator);

  struct Newline {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct CurrentEnvironment {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmAsmInstruction {
    static void func(KevesVM*, const_KevesIterator);
  };


  struct ScmFindGlobalVariable {
    static void func(KevesVM*, const_KevesIterator);
    static void func_helper1(KevesVM*, const_KevesIterator);
    static void func_helper2(KevesVM*, const_KevesIterator);
  };
  */

  /*
  struct KevesFindSyntaxId {
    static void func(KevesVM*, const_KevesIterator);
  };
  */

  /*
  struct ScmFindInternalVariable {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmGetMacroBody {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmGetGeneratorBody {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeAssertionViolation {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeLabel {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeLabelSetter {
    static void func(KevesVM*, const_KevesIterator);
  };

  static void procMakeLocalVariable(KevesVM*, const_KevesIterator);
  static void procMakeFreeVariable(KevesVM*, const_KevesIterator);

  struct ScmMakeIrritantsCondition {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeMacroHelper {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeMessageCondition {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmMakeWhoCondition {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ScmReferBuiltinTable {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IsJump {
    static bool func(KevesValue);
    static KevesValue message(KevesBuiltInValues*);
  };

  struct IsMacro {
    static bool func(KevesValue);
    static KevesValue message(KevesBuiltInValues*);
  };

  struct IsGenerator {
    static bool func(KevesValue);
    static KevesValue message(KevesBuiltInValues*);
  };
  */
};
