// keves/lib/rnrs-lists/rnrs-lists.cpp - library of Keves base
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


#include "lib/rnrs-lists/rnrs-lists.hpp"

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
#include "value/char.hpp"


void LibRnrsLists::init(KevesCommon* common) {
  std::cout << "LibRnrsLists::init()" << std::endl;

  // setID("rnrs", "lists-bin");
  // setVerNum(6);
					       
  sym_cons_star_ = SymbolKev::make(common, "cons*");

  proc_cons_star_.set(&procConsStar, sym_cons_star_);

  addBind("cons*", &proc_cons_star_);
}

void LibRnrsLists::procConsStar(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());

  if (argn > 2) {
    vm->acc_ = registers->lastArgument();
    registers->popArgument();
    return procConsStar_helper(vm, pc);
  }

  if (argn == 2) {
    vm->acc_ = registers->lastArgument();
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req1OrMoreGot0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsLists::procConsStar_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  PairKev pair(registers->lastArgument(), vm->acc_);
  
  vm->checkStack(&pair, &procConsStar_helper, pc);
  
  vm->acc_ = &pair;

  if (registers->argn() < 3)
    return KevesVM::returnValue(vm, pc);

  registers->popArgument();
  return procConsStar_helper(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesCommon* common) {
    LibRnrsLists* library(new LibRnrsLists());
    library->init(common);
    return library;
  }
}
