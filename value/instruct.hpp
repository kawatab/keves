// keves/keves_instruct.hpp - instructs for Keves
// Keves will be an R6RS Scheme implementation.
//
// Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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

#include <QString>
#include "keves_value.hpp"

enum keves_instruct {
  // essential
  CMD_NOP,
  CMD_HALT,
  CMD_JUMP,
  CMD_JUMP_R,
  CMD_SKIP,
  CMD_TEST,
  CMD_TEST_R,
  CMD_TEST0,
  CMD_TEST_NOT_R,
  CMD_SWITCH_N_ARG,
  CMD_SWITCH_N_ARG_R,
  CMD_FRAME,
  CMD_FRAME_R,
  CMD_SFRAME,
  CMD_APPLY,
  CMD_APPLY_CONSTANT,
  CMD_APPLY_MULT,
  CMD_RETURN,
  CMD_RETURN_CONSTANT,
  CMD_RETURN0,
  CMD_RETURN_VOID,
  CMD_REFER_LOCAL,
  CMD_REFER_LOCAL0,
  CMD_REFER_FREE,
  CMD_REFER_FREE0,
  CMD_CONSTANT,
  CMD_ASSIGN_LOCAL,
  CMD_ASSIGN_LOCAL0,
  CMD_CALL_LAMBDA,
  CMD_CALL_LAMBDA_VLA,
  CMD_ASSIGN_FREE,
  CMD_ASSIGN_FREE0,
  CMD_ASSIGN_MULT,
  CMD_CLOSE_R,
  CMD_NEW_BOX,
  CMD_BOX,
  CMD_RAISE,
  CMD_RAISE_CONSTANT,
  CMD_CLEAR_VALS,
  CMD_LAST_POP_TEST,
  CMD_LAST_POP_TEST_R,
  CMD_DEBUG_CODE,

  // argument
  CMD_LAST,
  CMD_ARGUMENT,
  CMD_PUSH,
  CMD_PUSH_CONSTANT,
  CMD_POP,

  // for Kev
  CMD_TEST_QEV_EQUAL,

  // for numbers
  CMD_TEST_NUMBER,
  CMD_TEST_NUMBER0,
  CMD_TEST_NUMBER0_R,

  CMD_TEST_NUMBER_EQUAL,

  // for char
  CMD_TEST_CHAR,
  CMD_TEST_CHAR0,
  CMD_TEST_CHAR_EQUAL,

  // for symbols
  CMD_TEST_SYMBOL,
  CMD_TEST_SYMBOL0,
  CMD_TEST_SYMBOL_EQUAL,

  // for strings
  CMD_STRING_CLONE,
  CMD_STRING_APPEND,
  CMD_TEST_STRING,
  CMD_TEST_STRING0,
  CMD_TEST_STRING_EQUAL,

  // for pairs
  CMD_CONS,
  CMD_CONS_CONSTANT,
  CMD_CAR,
  CMD_CAR0,
  CMD_CDR,
  CMD_CDR0,
  CMD_TEST_PAIR_R,
  CMD_TEST_PAIR0_R,
  CMD_TEST_NULL_R,
  CMD_TEST_NULL0_R,

  // for call/cc
  CMD_UNWIND_CONTINUATION,
  CMD_REVERT_DYNAMIC_WIND,

  // for dynamic-wind
  CMD_SET_DYNAMIC_WIND,
  CMD_REMOVE_DYNAMIC_WIND,

  // for vectors
  // CMD_VECTOR,
  CMD_LIST_TO_VECTOR,

  // for conditions
  CMD_TEST_CONDITION,

  // for syntax
  CMD_SYNTAX,

  // expander
  CMD_BEGIN,
  CMD_TERMINATE_EXPAND,
  CMD_ADD_GLOBAL_VARIABLE,

  // for library
  CMD_EXPORT_SYMBOL,
  CMD_TEST_ENVIRONMENT,

  END_OF_LIST
};

class KevesInstruct {
public:
  KevesInstruct() = delete;
  KevesInstruct(const KevesInstruct&) = default;
  KevesInstruct(KevesInstruct&&) = default;
  KevesInstruct& operator=(const KevesInstruct&) = default;
  KevesInstruct& operator=(KevesInstruct&&) = default;
  ~KevesInstruct() = default;

  explicit constexpr KevesInstruct(keves_instruct a) : val(a) {
  }

  explicit KevesInstruct(KevesValue kev)
  : val(static_cast<keves_instruct>(kev.toUIntPtr() >> 8)) {
    Q_ASSERT(kev.IsInstruct());
  }

  operator KevesValue() const {
    return KevesValue(reinterpret_cast<Kev*>(static_cast<quintptr>(val) << 8 | 5));
  }

  operator keves_instruct() const {
    return val;
  }

private:
  keves_instruct val;
};

class KevesInstructTable {
public:
  KevesInstructTable();
  KevesInstructTable(const KevesInstructTable&) = default;
  KevesInstructTable(KevesInstructTable&&) = delete;
  KevesInstructTable& operator=(const KevesInstructTable&) = default;
  KevesInstructTable& operator=(KevesInstructTable&&) = delete;
  ~KevesInstructTable() = default;

  const char* GetLabel(KevesInstruct n) const {
    return array[n];
  }

  /*
  static const KevesInstructTable* make() {
    static KevesInstructTable obj;
    return &obj;
  }
  */

private:
  const char* array[END_OF_LIST];
};
