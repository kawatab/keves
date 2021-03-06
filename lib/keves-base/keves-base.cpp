// keves/lib/keves-base/keves-base.cpp - library of Keves base
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


#include "lib/keves-base/keves-base.hpp"

#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>
#include "keves_builtin_values.hpp"
#include "keves_common.hpp"
#include "keves_common-inl.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/pair.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "kev/vector.hpp"


void LibKevesBase::init(KevesCommon* common) {
  std::cout << "LibKevesBase::init()" << std::endl;

  // setID("keves", "base-bin");
  // setVerNum(0, 1);
					       
  sym_display_ = SymbolKev::make(common, "display");
  sym_newline_ = SymbolKev::make(common, "newline");
  sym_u8_list_to_vector_ = SymbolKev::make(common, "u8-list->vector");

  proc_display_.set(procDisplay, sym_display_);
  proc_newline_.set(&Function::thunk<Newline>, sym_newline_);
  proc_u8_list_to_vector_.set(&procU8ListToVector, sym_u8_list_to_vector_);

  addBind("display", &proc_display_);
  addBind("newline", &proc_newline_);
  addBind("u8-list->vector", &proc_u8_list_to_vector_);

  addBind("&syntax", common->builtin()->amp_syntax());
  addBind("&lexical", common->builtin()->amp_lexical());
  addBind("&assert", common->builtin()->amp_assert());
  addBind("&irritants", common->builtin()->amp_irritants());
  addBind("&who", common->builtin()->amp_who());
  addBind("&message", common->builtin()->amp_message());

  /*
  CodeKev* code(CodeKev::make(common, 16));
  KevesIterator iter(code->begin());
  *iter++ = KevesInstruct(CMD_FRAME_R);
  *iter++ = KevesFixnum(5);
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = findBind("display");
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = KevesFixnum(5);
  *iter++ = KevesInstruct(CMD_APPLY);
  *iter++ = KevesInstruct(CMD_FRAME_R);
  *iter++ = KevesFixnum(3);
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = findBind("newline");
  *iter++ = KevesInstruct(CMD_APPLY);
  *iter++ = KevesInstruct(CMD_HALT);
  Q_ASSERT(iter <= code->end());

  addBind("my-code", code);
  */
}

void LibKevesBase::procDisplay(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() != 2) {
    vm->acc_ = vm->gr2_;

    vm->gr1_ = registers->argn() > 2 ?
      vm->common()->getMesgText(KevesBuiltinValues::mesg_Req1GotMore) :
      vm->common()->getMesgText(KevesBuiltinValues::mesg_Req1Got0);

    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  vm->result_field()->append(vm->common()->toString(registers->lastArgument()));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibKevesBase::Newline::func(KevesVM* vm, const_KevesIterator pc) {
  vm->result_field()->append(QString("\n"));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValue(vm, pc);
}

// u8-list->vector
void LibKevesBase::procU8ListToVector(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  
  if (registers->argn() != 2) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(registers->argn() > 2 ?
					 KevesBuiltinValues::mesg_Req1GotMore :
					 KevesBuiltinValues::mesg_Req1Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  return procU8ListToVector_helper(vm, pc);
}

void LibKevesBase::procU8ListToVector_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue temp(registers->lastArgument());
  int length(KevesVM::getListLength(temp));

  if (length < 0) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProperList);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  VectorKev* vector(vm->makeVector(length));
  KevesIterator iter(vector->begin());
  
  while (temp != EMB_NULL) {
    const PairKev* pair(temp);
    *iter++ = pair->car();
    temp = pair->cdr();
  }
  
  vm->acc_ = vector;
  return KevesVM::returnValue(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesCommon* common) {
    LibKevesBase* library(new LibKevesBase());
    library->init(common);
    return library;
  }
}
