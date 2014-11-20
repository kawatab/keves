/* Keves/keves_base.cpp - base of Keves
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


#include "keves_base.hpp"

#include <QDataStream>
#include <QFile>

#define DEFINE_MESG_KEY(mesg) const char KevesBase::mesg_ ## mesg[] = #mesg
DEFINE_MESG_KEY(ReqRealNum);
DEFINE_MESG_KEY(1stObjNotProcOrSyn);
DEFINE_MESG_KEY(ReqLessGotMore);
DEFINE_MESG_KEY(ReqMoreGotLess);
#undef DEFINE_MESG_KEY

KevesBase::KevesBase()
  : gc_(10),
    keves_instruct_table_(),
    mesg_text_(),
    sym_eval_(),
    builtin_code_(),
    code_APPLY_(),
    code_HALT_(),
    code_REMOVE_DYNAMIC_WIND_(),
    code_REVERT_DYNAMIC_WIND_and_APPLY_(),
    code_POP_APPLY_(),
    code_POP_RETURN_(),
    lib_keves_base_() {

  InitCMDTable();
  KevesVM::keves_instruct_table_ = &keves_instruct_table_;
  InitMesgList("mesg_text.csv");

  std::cout << qPrintable(GetMesgText(mesg_ReqRealNum)->ToQString()) << std::endl;
			    
  sym_eval_ = SymbolKev::Make(&gc_, "eval");

  constexpr int CODE_SIZE(8);
  builtin_code_ = CodeKev::make(&gc_, CODE_SIZE);
  KevesIterator iter(builtin_code_->begin());
  code_HALT_ = iter;
  *iter++ = KevesInstruct(CMD_HALT);
  code_POP_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_POP);
  code_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_APPLY);
  code_POP_RETURN_ = iter;
  *iter++ = KevesInstruct(CMD_POP);
  *iter++ = KevesInstruct(CMD_RETURN);
  code_REVERT_DYNAMIC_WIND_and_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_REVERT_DYNAMIC_WIND);
  *iter++ = KevesInstruct(CMD_APPLY);
  code_REMOVE_DYNAMIC_WIND_ = iter;
  *iter++ = KevesInstruct(CMD_REMOVE_DYNAMIC_WIND);
  Q_ASSERT(iter <= builtin_code_->begin() + CODE_SIZE);

  lib_keves_base_.Init(&gc_);
}

KevesBase::~KevesBase() {
  gc_.Reset();
}

KevesVM* KevesBase::MakeVM() {
  KevesVM* vm(new KevesVM(this));
  gc_.Append(vm);
  return vm;
}

void KevesBase::InitCMDTable() {
#define INIT_CHECK_SUM() int check_sum(0);
#define CHECK_SUM(x) Q_ASSERT(check_sum == (CMD_##x + 1) * CMD_##x / 2);

#define SET_FUNCTION_TO_TABLE(x)			\
  KevesVM::cmd_table_[CMD_##x] = &KevesVM::cmd_##x;	\
  check_sum += CMD_##x;
  
  INIT_CHECK_SUM();

  SET_FUNCTION_TO_TABLE(NOP);
  SET_FUNCTION_TO_TABLE(HALT);
  SET_FUNCTION_TO_TABLE(JUMP);
  SET_FUNCTION_TO_TABLE(JUMP_R);
  SET_FUNCTION_TO_TABLE(SKIP);
  SET_FUNCTION_TO_TABLE(TEST);
  SET_FUNCTION_TO_TABLE(TEST_R);
  SET_FUNCTION_TO_TABLE(TEST0);
  SET_FUNCTION_TO_TABLE(TEST_NOT_R);
  SET_FUNCTION_TO_TABLE(SWITCH_N_ARG);
  SET_FUNCTION_TO_TABLE(SWITCH_N_ARG_R);
  SET_FUNCTION_TO_TABLE(FRAME);
  SET_FUNCTION_TO_TABLE(FRAME_R);
  SET_FUNCTION_TO_TABLE(SFRAME);
  SET_FUNCTION_TO_TABLE(APPLY);
  SET_FUNCTION_TO_TABLE(APPLY_CONSTANT);
  SET_FUNCTION_TO_TABLE(APPLY_MULT);
  SET_FUNCTION_TO_TABLE(RETURN);
  SET_FUNCTION_TO_TABLE(RETURN_CONSTANT);
  SET_FUNCTION_TO_TABLE(RETURN0);
  SET_FUNCTION_TO_TABLE(RETURN_VOID);
  SET_FUNCTION_TO_TABLE(REFER_LOCAL);
  SET_FUNCTION_TO_TABLE(REFER_LOCAL0);
  SET_FUNCTION_TO_TABLE(REFER_FREE);
  SET_FUNCTION_TO_TABLE(REFER_FREE0);
  SET_FUNCTION_TO_TABLE(CONSTANT);
  SET_FUNCTION_TO_TABLE(ASSIGN_LOCAL);
  SET_FUNCTION_TO_TABLE(ASSIGN_LOCAL0);
  SET_FUNCTION_TO_TABLE(CALL_LAMBDA);
  SET_FUNCTION_TO_TABLE(CALL_LAMBDA_VLA);
  SET_FUNCTION_TO_TABLE(ASSIGN_FREE);
  SET_FUNCTION_TO_TABLE(ASSIGN_FREE0);
  SET_FUNCTION_TO_TABLE(ASSIGN_MULT);
  SET_FUNCTION_TO_TABLE(CLOSE_R);
  SET_FUNCTION_TO_TABLE(NEW_BOX);
  SET_FUNCTION_TO_TABLE(BOX);
  SET_FUNCTION_TO_TABLE(RAISE);
  SET_FUNCTION_TO_TABLE(RAISE_CONSTANT);
  SET_FUNCTION_TO_TABLE(CLEAR_VALS);
  SET_FUNCTION_TO_TABLE(LAST_POP_TEST);
  SET_FUNCTION_TO_TABLE(LAST_POP_TEST_R);
  SET_FUNCTION_TO_TABLE(DEBUG_CODE);
  CHECK_SUM(DEBUG_CODE);

  // argument
  SET_FUNCTION_TO_TABLE(LAST);
  SET_FUNCTION_TO_TABLE(ARGUMENT);
  SET_FUNCTION_TO_TABLE(PUSH);
  SET_FUNCTION_TO_TABLE(PUSH_CONSTANT);
  SET_FUNCTION_TO_TABLE(POP);
  CHECK_SUM(POP);

  // for Qev
  SET_FUNCTION_TO_TABLE(TEST_QEV_EQUAL);

  // for numbers
  SET_FUNCTION_TO_TABLE(TEST_NUMBER);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0_R);

  SET_FUNCTION_TO_TABLE(TEST_NUMBER_EQUAL);

  // for char
  SET_FUNCTION_TO_TABLE(TEST_CHAR);
  SET_FUNCTION_TO_TABLE(TEST_CHAR0);
  SET_FUNCTION_TO_TABLE(TEST_CHAR_EQUAL);

  // for symbols
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL0);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL_EQUAL);

  // for strings
  SET_FUNCTION_TO_TABLE(STRING_CLONE);
  SET_FUNCTION_TO_TABLE(STRING_APPEND);
  SET_FUNCTION_TO_TABLE(TEST_STRING);
  SET_FUNCTION_TO_TABLE(TEST_STRING0);
  SET_FUNCTION_TO_TABLE(TEST_STRING_EQUAL);
  CHECK_SUM(TEST_STRING_EQUAL);

  // for pairs
  SET_FUNCTION_TO_TABLE(CONS);
  SET_FUNCTION_TO_TABLE(CONS_CONSTANT);
  SET_FUNCTION_TO_TABLE(CAR);
  SET_FUNCTION_TO_TABLE(CAR0);
  SET_FUNCTION_TO_TABLE(CDR);
  SET_FUNCTION_TO_TABLE(CDR0);
  SET_FUNCTION_TO_TABLE(TEST_PAIR);
  SET_FUNCTION_TO_TABLE(TEST_PAIR0);
  SET_FUNCTION_TO_TABLE(TEST_NULL);
  SET_FUNCTION_TO_TABLE(TEST_NULL0);

  // for call/cc
  SET_FUNCTION_TO_TABLE(UNWIND_CONTINUATION);
  SET_FUNCTION_TO_TABLE(REVERT_DYNAMIC_WIND);

  // for dynamic-wind
  SET_FUNCTION_TO_TABLE(SET_DYNAMIC_WIND);
  SET_FUNCTION_TO_TABLE(REMOVE_DYNAMIC_WIND);

  // for vectors
  SET_FUNCTION_TO_TABLE(LIST_TO_VECTOR);
  CHECK_SUM(LIST_TO_VECTOR);

  // for conditions
  SET_FUNCTION_TO_TABLE(TEST_CONDITION);
  
  // for syntax
  SET_FUNCTION_TO_TABLE(SYNTAX);
  
  // expander
  SET_FUNCTION_TO_TABLE(BEGIN);
  SET_FUNCTION_TO_TABLE(TERMINATE_EXPAND);
  SET_FUNCTION_TO_TABLE(ADD_GLOBAL_VARIABLE);
  CHECK_SUM(ADD_GLOBAL_VARIABLE);

  // for library
  SET_FUNCTION_TO_TABLE(EXPORT_SYMBOL);
  SET_FUNCTION_TO_TABLE(TEST_ENVIRONMENT);
  CHECK_SUM(TEST_ENVIRONMENT);

#undef SET_FUNCTION_TO_TABLE
#undef CHECK_SUM
#undef INIT_CHECK_SUM
}

void KevesBase::InitMesgList(const QString& file_name) {
  QFile mesg_file(file_name);

  if (!mesg_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cerr << "Not found `mesg_text.csv'\n";
    return;
  }
  
  QString temp, key, value;
  int line_num(0);

  while (!mesg_file.atEnd()) {
    temp = mesg_file.readLine();
    ++line_num;
    int pos(temp.indexOf('\t'));
    int size(temp.length());

    if (size == 0) continue;

    if (pos < 0 || pos == size - 1) {
      std::cerr << "Error: There is no value in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    key = temp.left(pos).trimmed();
    value = temp.mid(pos + 1).trimmed();

    if (key.isEmpty()) {
      std::cerr << "Error: There is no key in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    if (value.isEmpty()) {
      std::cerr << "Error: There is no value in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    mesg_text_.insert(key, StringKev::Make(&gc_, value));
  }

  mesg_file.close();
}

const StringKev* KevesBase::GetMesgText(const QString& key) const {
  return mesg_text_.value(key);
}
