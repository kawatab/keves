// keves/lib/rnrs-exceptions/rnrs-exceptions.cpp - library of Keves base
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


#include "lib/rnrs-exceptions/rnrs-exceptions.hpp"

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
#include "value/char.hpp"


void LibRnrsExceptions::init(KevesBase* base) {
  std::cout << "LibRnrsExceptions::init()" << std::endl;

  // setID("rnrs", "exceptions-bin");
  // setVerNum(6);
					       
  sym_raise_ = SymbolKev::make(base, "raise");

  proc_raise_.set(&Function::make<Function::Anything, Raise>, sym_raise_);

  addBind("raise", &proc_raise_);
}


void LibRnrsExceptions::Raise::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());
  *registers = *registers->sfp();

  if (StackFrameKev::isBottom(registers)) {
    pc = vm->base()->builtin()->code_HALT();
    std::cout << "raise!!!!!!!!!!!!!" << std::endl; 
    vm->gr1_ = last;
    return KevesVM::pushGr1ToArgument(vm, pc);
  }

  std::cout << "catch exception!!!!!!!!!!!!!" << std::endl; 
  pc = registers->pc();
  vm->acc_ = last;
  return KevesVM::pushGr1ToArgument(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesBase* base) {
    LibRnrsExceptions* library(new LibRnrsExceptions());
    library->init(base);
    return library;
  }
}
