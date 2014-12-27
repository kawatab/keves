// keves/lib/rnrs-mutable-strings/rnrs-mutable-strings.cpp - library of Keves base
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


#include "rnrs-mutable-strings.hpp"

// #include <QDir>
// #include <QFile>
// #include <QTextCodec>
// #include <QTextEdit>
// #include <QTextStream>
#include "keves_builtin_values.hpp"
// #include "keves_common.hpp"
// #include "keves_common-inl.hpp"
#include "keves_template.hpp"
// #include "keves_vm.hpp"
// #include "kev/code.hpp"
// #include "kev/code-inl.hpp"
#include "kev/procedure.hpp"
// #include "kev/procedure-inl.hpp"
#include "kev/string.hpp"
// #include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
// #include "kev/symbol-inl.hpp"
#include "value/char.hpp"


void LibRnrsMutableStrings::init(KevesCommon* common) {
  std::cout << "LibRnrsMutableStrings::init()" << std::endl;

  // setID("rnrs", "mutable-strings-bin");
  // setVerNum(6);
					       
  sym_string_fill_e_ = SymbolKev::make(common, "string-fill!");
  sym_string_set_e_ = SymbolKev::make(common, "string-set!");

  proc_string_set_e_.set(&Function::make<Function::IsString, Function::IsFixnum, Function::IsChar, StringSetE>, sym_string_set_e_);
  proc_string_fill_e_.set(&Function::make<Function::IsString, Function::IsChar, StringFillE>, sym_string_fill_e_);

  addBind("string-fill!", &proc_string_fill_e_);
  addBind("string-set!", &proc_string_set_e_);
}

void LibRnrsMutableStrings::StringSetE::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  StringKev* str(MutableKevesValue(registers->lastArgument(2)));
  KevesFixnum pos(registers->lastArgument(1));
  KevesChar chr(registers->lastArgument());

  if (pos >= 0 && pos < str->size()) {
    str->set(pos, chr);
    vm->acc_ = EMB_UNDEF;
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsMutableStrings::StringFillE::func(KevesVM* vm, const_KevesIterator pc) {
  StringKev* str(MutableKevesValue(vm->acc_));
  KevesChar chr(vm->gr1_);
  str->fill(chr);
  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValue(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesCommon* common) {
    LibRnrsMutableStrings* library(new LibRnrsMutableStrings());
    library->init(common);
    return library;
  }
}
