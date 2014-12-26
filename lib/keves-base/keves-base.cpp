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
#include "keves_base.hpp"
#include "keves_base-inl.hpp"
#include "keves_builtin_values.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"


void LibKevesBase::init(KevesBase* base) {
  std::cout << "LibKevesBase::init()" << std::endl;

  // setID("keves", "base-bin");
  // setVerNum(0, 1);
					       
  sym_display_ = SymbolKev::make(base, "display");
  sym_newline_ = SymbolKev::make(base, "newline");

  proc_display_.set(procDisplay, sym_display_);
  proc_newline_.set(&Function::thunk<Newline>, sym_newline_);

  addBind("display", &proc_display_);
  addBind("newline", &proc_newline_);

  addBind("&syntax", base->builtin()->amp_syntax());
  addBind("&lexical", base->builtin()->amp_lexical());
  addBind("&assert", base->builtin()->amp_assert());
  addBind("&irritants", base->builtin()->amp_irritants());
  addBind("&who", base->builtin()->amp_who());
  addBind("&message", base->builtin()->amp_message());

  /*
  CodeKev* code(CodeKev::make(base, 16));
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
      vm->base()->getMesgText(KevesBuiltinValues::mesg_Req1GotMore) :
      vm->base()->getMesgText(KevesBuiltinValues::mesg_Req1Got0);

    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  vm->result_field()->append(vm->base()->toString(registers->lastArgument()));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibKevesBase::Newline::func(KevesVM* vm, const_KevesIterator pc) {
  vm->result_field()->append(QString("\n"));
  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValue(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesBase* base) {
    LibKevesBase* library(new LibKevesBase());
    library->init(base);
    return library;
  }
}
