// keves/keves_vm.hpp - VM for Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
// 
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
// 
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
//  License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include <setjmp.h>
#include <QChar>
#include <QRunnable>
#include "keves_gc.hpp"
#include "keves_gc-inl.hpp"
#include "keves_value.hpp"
#include "kev/environment.hpp"
#include "kev/frame.hpp"

class CodeKev;
class ExactComplexNumberKev;
class KevesCommon;
class KevesTextualOutputPort;
class RationalNumberKev;
class RecordKev;
class StringKev;
class VectorKev;

class KevesVM : public QRunnable {
public:
  KevesVM() = default;
  KevesVM(const KevesVM&) = delete;
  KevesVM(const KevesVM&&) = delete;
  KevesVM& operator=(const KevesVM&) = delete;
  KevesVM& operator=(const KevesVM&&) = delete;
  ~KevesVM() = default;

  void run(); // for QRunnable
  
  int execute(const QString& arg);
  
  ////////////////////////////////////////////////////////////////
  // General                                                    //
  ////////////////////////////////////////////////////////////////
public:
  int execute_helper(const QString& arg);
  
  static KevesVM* make(KevesCommon* common);
  static KevesVM* make(KevesCommon* common, KevesTextualOutputPort* result_field);
  
  KevesCommon* common() {
    return common_;
  }
  
  KevesGC* gc() {
    return &gc_;
  }
  
  KevesTextualOutputPort* result_field() {
    return result_field_;
  }

  ////////////////////////////////////////////////////////////////
  // for Instruct and Procedures                                //
  ////////////////////////////////////////////////////////////////

  // Push and Return Values
  void pushToArgument(KevesValue);
  static void pushAccToArgument(KevesVM*, const_KevesIterator);
  static void pushAccToArgumentSafe(KevesVM*, const_KevesIterator);
  void pushAccToArgument();
  static void pushGr1ToArgument(KevesVM*, const_KevesIterator);
  static void pushGr1ToArgumentSafe(KevesVM*, const_KevesIterator);
  void pushGr1ToArgument();
  static void returnVoid(KevesVM*, const_KevesIterator);
  static void returnValueSafe(KevesVM*, const_KevesIterator);
  static void returnValue(KevesVM*, const_KevesIterator);
  static void returnTwoValues(KevesVM*, const_KevesIterator);

  // Make and Raise Exceptions
  static void makeLexicalException(KevesVM*, const_KevesIterator);
  static void raiseAssertFirstObjNotProc(KevesVM*, const_KevesIterator);
  static void raiseAssertLambdaReqLess(KevesVM*, const_KevesIterator);
  static void raiseAssertLambdaReqMore(KevesVM*, const_KevesIterator);
  static void raiseAssertReqRealNum(KevesVM*, const_KevesIterator);
  static void raiseAssertCondition(KevesVM*, const_KevesIterator);


  // for Numbers
  static void continueRationalNumberKev(KevesVM* vm,
					const_KevesIterator pc,
					const RationalNumberKev* real,
					vm_func for_fixnum,
					vm_func for_real);

  static void continueExactComplexNumberKev(KevesVM* vm,
					    const_KevesIterator pc,
					    const ExactComplexNumberKev* complex,
					    vm_func for_fixnum,
					    vm_func for_real,
					    vm_func for_complex);

  static void makeRectangular(KevesVM*, const_KevesIterator);
  static void testNumber(KevesVM*, const_KevesIterator);
  static void returnExactComplexNumberKev(KevesVM*, const_KevesIterator);

  // for list
  static int getListLength(KevesValue);

  // for string
  StringKev* makeString(int);
  const StringKev* makeString(int, QChar);

  // for vector
  VectorKev* makeVector(int);
  const VectorKev* makeVector(int, KevesValue);

  // for procedures
  static void applyProcedure(KevesVM*, const_KevesIterator);

  // for others
  void toString(KevesValue, QString*);
  static void displayCode(KevesVM*, const_KevesIterator);
  static void revertDynamicWind(KevesVM*, const_KevesIterator);


  static void cmd_NOP(KevesVM*, const_KevesIterator);
  static void cmd_HALT(KevesVM*, const_KevesIterator);
  static void cmd_JUMP(KevesVM*, const_KevesIterator);
  static void cmd_JUMP_R(KevesVM*, const_KevesIterator);
  static void cmd_SKIP(KevesVM*, const_KevesIterator);
  static void cmd_TEST(KevesVM*, const_KevesIterator);
  static void cmd_TEST_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST0_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_NOT_R(KevesVM*, const_KevesIterator);
  static void cmd_SWITCH_N_ARG(KevesVM*, const_KevesIterator);
  static void cmd_SWITCH_N_ARG_R(KevesVM*, const_KevesIterator);
  static void cmd_FRAME(KevesVM*, const_KevesIterator);
  static void cmd_FRAME_R(KevesVM*, const_KevesIterator);
  static void cmd_SFRAME(KevesVM*, const_KevesIterator);
  static void cmd_APPLY(KevesVM*, const_KevesIterator);
  static void cmd_APPLY_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_APPLY_MULT(KevesVM*, const_KevesIterator);
  static void cmd_RETURN(KevesVM*, const_KevesIterator);
  static void cmd_RETURN_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_RETURN0(KevesVM*, const_KevesIterator);
  static void cmd_RETURN_VOID(KevesVM*, const_KevesIterator);
  static void cmd_REFER_LOCAL(KevesVM*, const_KevesIterator);
  static void cmd_REFER_LOCAL0(KevesVM*, const_KevesIterator);
  static void cmd_REFER_FREE(KevesVM*, const_KevesIterator);
  static void cmd_REFER_FREE0(KevesVM*, const_KevesIterator);
  static void cmd_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_ASSIGN_LOCAL(KevesVM*, const_KevesIterator);
  static void cmd_ASSIGN_LOCAL0(KevesVM*, const_KevesIterator);
  static void cmd_CALL_LAMBDA(KevesVM*, const_KevesIterator);
  static void cmd_CALL_LAMBDA_helper(KevesVM*, const_KevesIterator);
  static void cmd_CALL_LAMBDA_VLA(KevesVM*, const_KevesIterator);
  static void cmd_CALL_LAMBDA_VLA_helper(KevesVM*, const_KevesIterator);
  static void cmd_ASSIGN_FREE(KevesVM*, const_KevesIterator);
  static void cmd_ASSIGN_FREE0(KevesVM*, const_KevesIterator);
  static void cmd_ASSIGN_MULT(KevesVM*, const_KevesIterator);
  static void cmd_CLOSE_R(KevesVM*, const_KevesIterator);
  static void cmd_NEW_BOX(KevesVM*, const_KevesIterator);
  static void cmd_BOX(KevesVM*, const_KevesIterator);
  static void cmd_RAISE(KevesVM*, const_KevesIterator);
  static void cmd_RAISE_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_CLEAR_VALS(KevesVM*, const_KevesIterator);
  static void cmd_JUMP_BOTTOM_FRAME(KevesVM*, const_KevesIterator);
  static void cmd_LAST_POP_TEST(KevesVM*, const_KevesIterator);
  static void cmd_LAST_POP_TEST_R(KevesVM*, const_KevesIterator);
  static void cmd_DEBUG_CODE(KevesVM*, const_KevesIterator);

  // argument
  static void cmd_LAST(KevesVM*, const_KevesIterator);
  static void cmd_ARGUMENT(KevesVM*, const_KevesIterator);
  static void cmd_PUSH(KevesVM*, const_KevesIterator);
  static void cmd_PUSH_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_POP(KevesVM*, const_KevesIterator);

  static void cmd_TEST_QEV_EQUAL_R(KevesVM*, const_KevesIterator);

  // for numbers
  static void cmd_TEST_NUMBER_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_NUMBER0(KevesVM*, const_KevesIterator);
  static void cmd_TEST_NUMBER0_R(KevesVM*, const_KevesIterator);

  static void cmd_TEST_NUMBER_EQUAL_R(KevesVM*, const_KevesIterator);

  // for char
  static void cmd_TEST_CHAR_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_CHAR0_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_CHAR_EQUAL_R(KevesVM*, const_KevesIterator);

  // for symbols
  static void cmd_TEST_SYMBOL(KevesVM*, const_KevesIterator);
  static void cmd_TEST_SYMBOL0(KevesVM*, const_KevesIterator);
  static void cmd_TEST_SYMBOL_EQUAL_R(KevesVM*, const_KevesIterator);

  // for strings
  static void cmd_STRING_CLONE(KevesVM*, const_KevesIterator);
  static void cmd_STRING_APPEND(KevesVM*, const_KevesIterator);
  static void cmd_TEST_STRING_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_STRING0_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_STRING_EQUAL_R(KevesVM*, const_KevesIterator);

  // for pairs
  static void cmd_CONS(KevesVM*, const_KevesIterator);
  static void cmd_CONS_CONSTANT(KevesVM*, const_KevesIterator);
  static void cmd_CAR(KevesVM*, const_KevesIterator);
  static void cmd_CAR0(KevesVM*, const_KevesIterator);
  static void cmd_CDR(KevesVM*, const_KevesIterator);
  static void cmd_CDR0(KevesVM*, const_KevesIterator);
  static void cmd_TEST_PAIR_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_PAIR0_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_NULL_R(KevesVM*, const_KevesIterator);
  static void cmd_TEST_NULL0_R(KevesVM*, const_KevesIterator);

  // for call/cc
  static void cmd_UNWIND_CONTINUATION(KevesVM*, const_KevesIterator);
  static void cmd_REVERT_DYNAMIC_WIND(KevesVM*, const_KevesIterator);

  // for dynamic-wind
  static void cmd_SET_DYNAMIC_WIND(KevesVM*, const_KevesIterator);
  static void cmd_REMOVE_DYNAMIC_WIND(KevesVM*, const_KevesIterator);

  // for vectors
  static void cmd_LIST_TO_VECTOR(KevesVM*, const_KevesIterator);

  // for conditions
  static void cmd_TEST_CONDITION(KevesVM*, const_KevesIterator);

  // for syntax
  static void cmd_SYNTAX(KevesVM*, const_KevesIterator);

  // expander
  static void cmd_BEGIN(KevesVM*, const_KevesIterator);
  static void cmd_BEGIN_helper(KevesVM*, const_KevesIterator);

  static void cmd_TERMINATE_EXPAND(KevesVM*, const_KevesIterator);
  static void cmd_ADD_GLOBAL_VARIABLE(KevesVM*, const_KevesIterator);

  // for library
  static void cmd_EXPORT_SYMBOL(KevesVM*, const_KevesIterator);
  static void cmd_TEST_ENVIRONMENT(KevesVM*, const_KevesIterator);


  ////////////////////////////////////////////////////////////////
  // for GC                                                     //
  ////////////////////////////////////////////////////////////////

  void checkStack(size_t*, vm_func, const_KevesIterator);
  void checkStack(const void*, vm_func, const_KevesIterator);
  void executeGC(vm_func, const_KevesIterator);
  KevesValue findConditionValue(KevesValue, RecordKev*);

  void* stack_lower_limit() {
    return &stack_lower_limit_;
  }

  void* stack_higher_limit() {
    return &stack_higher_limit_;
  }

  jmp_buf* ptr_jmp_exit() {
    return &jmp_exit_;
  }


private:
  KevesCommon* common_;
  vm_func* cmd_table_;
  KevesGC gc_;
  KevesTextualOutputPort* result_field_;

  EnvironmentKev curt_global_vars_;
  EnvironmentKev prev_global_vars_;

public:
  ArgumentFrameKev* keves_vals_;
  StackFrameKev registers_;
  KevesValue acc_;
  KevesValue gr1_;
  KevesValue gr2_;
  KevesValue gr3_;

  // for Cheney on the M.T.A.
  jmp_buf jmp_exit_;
  void* stack_lower_limit_;
  void* stack_higher_limit_;
  void* stack_safety_limit_;
  vm_func current_function_;
  const_KevesIterator current_pc_;
  const CodeKev* current_code_;
};
