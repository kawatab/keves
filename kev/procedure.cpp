// keves/kev/procedure.cpp - procedures for Keves
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


#include "kev/procedure.hpp"


// class CPSKev ----------------------------------------
CPSKev::CPSKev()
  : CPSKev(nullptr) {}

CPSKev::CPSKev(vm_func func)
  : CPSKev(func, EMB_UNDEF) {}

CPSKev::CPSKev(vm_func func, KevesValue who)
  : MutableKev(TYPE), func_(func), who_(who) {}



// class LambdaKev ----------------------------------------
LambdaKev::LambdaKev()
  : LambdaKev(nullptr, nullptr, 0) {}

LambdaKev::LambdaKev(CodeKev* code, int index)
  : LambdaKev(nullptr, code, index) {}

LambdaKev::LambdaKev(const LocalVarFrameKev* free_vars, CodeKev* code, int index)
  : MutableKev(TYPE), free_vars_(free_vars), code_(code), index_(index) {}

LambdaKev::LambdaKev(const LocalVarFrameKev* free_vars, CodeKev* code,
		     const_KevesIterator pc)
  : MutableKev(TYPE),
    free_vars_(free_vars),
    code_(code),
    index_(pc - code_->begin()) {}



// class ContinuationKev ----------------------------------------
ContinuationKev::ContinuationKev(StackFrameKev registers)
  : MutableKev(TYPE), registers_(registers) {}

const StackFrameKev* ContinuationKev::btmp() const {
  const StackFrameKev* frame(registers_.sfp());
  while (!StackFrameKev::isBottom(frame)) frame = frame->sfp();
  return frame;
}
