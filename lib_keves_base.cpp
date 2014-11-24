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



#include "lib_keves_base.hpp"

#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>

#include "code_kev.hpp"
// #include "condition_kev.hpp"
// #include "jump_kev.hpp"
#include "keves_base.hpp"
// #include "keves_char.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
// #include "number_kev.hpp"
#include "procedure_kev.hpp"
// #include "record_kev.hpp"
#include "symbol_kev.hpp"

LibKevesBase::LibKevesBase()
  : KevesLibrary(),
    sym_display_(), proc_display_(),
    sym_newline_(), proc_newline_() {
  id_ << "keves" << "base";
  ver_num_ << 0 << 1;
}

KevesLibrary* LibKevesBase::Init(KevesGC* gc) {
  std::cout << "LibKevesBase::Init()" << std::endl;

					       
  sym_display_ = SymbolKev::Make(gc, "display");
  sym_newline_ = SymbolKev::Make(gc, "newline");

  proc_display_.set(procDisplay, sym_display_);
  proc_newline_.set(&Function::thunk<Newline>, sym_newline_);

  AddBind("display", &proc_display_);
  AddBind("newline", &proc_newline_);

  code_ = CodeKev::make(gc, 16);
  KevesIterator iter(gc->ToMutable(code_.ToPtr<CodeKev>())->begin());
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
  Q_ASSERT(iter <= code_.ToPtr<CodeKev>()->end());

  return this;
}

void LibKevesBase::procDisplay(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() != 2) {
    vm->acc_ = vm->gr2_;

    vm->gr1_ = registers->argn() > 2 ?
      vm->base()->GetMesgText(KevesBase::mesg_Req1GotMore) :
      vm->base()->GetMesgText(KevesBase::mesg_Req1Got0);

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
