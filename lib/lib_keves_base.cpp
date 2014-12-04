// Keves/lib_keves_base.cpp - library of Keves base
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



#include "lib/lib_keves_base.hpp"

#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>

#include "keves_base.hpp"
#include "keves_base-inl.hpp"
#include "keves_builtin_values.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/procedure.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"

LibKevesBase* LibKevesBase::Make(KevesBase* base) {
  LibKevesBase* library(new LibKevesBase());
  library->Init(base);
  return library;
}

void LibKevesBase::Init(KevesBase* base) {
  std::cout << "LibKevesBase::Init()" << std::endl;

  SetID("keves", "base");
  SetVerNum(0, 1);
					       
  sym_display_ = SymbolKev::Make(base, "display");
  sym_newline_ = SymbolKev::Make(base, "newline");

  proc_display_.set(procDisplay, sym_display_);
  proc_newline_.set(&Function::thunk<Newline>, sym_newline_);

  AddBind("display", &proc_display_);
  AddBind("newline", &proc_newline_);

  GetCode() = CodeKev::Make(base, 16);
  KevesIterator iter(base->ToMutable(GetCode().ToPtr<CodeKev>())->begin());
  *iter++ = KevesInstruct(CMD_FRAME_R);
  *iter++ = KevesFixnum(5);
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = FindBind("display");
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = KevesFixnum(5);
  *iter++ = KevesInstruct(CMD_APPLY);
  *iter++ = KevesInstruct(CMD_FRAME_R);
  *iter++ = KevesFixnum(3);
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = FindBind("newline");
  *iter++ = KevesInstruct(CMD_APPLY);
  *iter++ = KevesInstruct(CMD_HALT);
  Q_ASSERT(iter <= GetCode().ToPtr<CodeKev>()->end());
}

void LibKevesBase::procDisplay(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() != 2) {
    vm->acc_ = vm->gr2_;

    vm->gr1_ = registers->argn() > 2 ?
      vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req1GotMore) :
      vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req1Got0);

    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  vm->result_field()->Append(vm->base()->ToString(registers->lastArgument()));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibKevesBase::Newline::func(KevesVM* vm, const_KevesIterator pc) {
  vm->result_field()->Append(QString("\n"));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::ReturnValue(vm, pc);
}
