// keves/keves_template.cpp - templates for Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
// 
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "keves_template.hpp"

#include "keves_builtin_values.hpp"
#include "kev/string.hpp"

void Function::raiseGetAnything(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void Function::raiseNotGet1Arg(KevesVM* vm, const_KevesIterator pc) {
  KevesCommon* common(vm->common());
  int argn(vm->registers_.argn());
  vm->acc_ = vm->gr2_;
  vm->gr1_ = common->getMesgText(argn > 2 ?
				 KevesBuiltinValues::mesg_Req1GotMore :
				 KevesBuiltinValues::mesg_Req1Got0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void Function::raiseNotGet2Args(KevesVM* vm, const_KevesIterator pc) {
  KevesCommon* common(vm->common());
  int argn(vm->registers_.argn());
  vm->acc_ = vm->gr2_;

  vm->gr1_ =
    argn > 3 ? common->getMesgText(KevesBuiltinValues::mesg_Req2GotMore) :
    argn == 2 ? common->getMesgText(KevesBuiltinValues::mesg_Req2Got1) :
    common->getMesgText(KevesBuiltinValues::mesg_Req2Got0);

  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void Function::raiseNotGet2OrMoreArgs(KevesVM* vm, const_KevesIterator pc) {
  KevesCommon* common(vm->common());
  int argn(vm->registers_.argn());
  vm->acc_ = vm->gr2_;

  vm->gr1_ = common->getMesgText(argn == 2 ?
				 KevesBuiltinValues::mesg_Req2OrMoreGot1 :
				 KevesBuiltinValues::mesg_Req2OrMoreGot0);

  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void Function::raiseNotGet3Args(KevesVM* vm, const_KevesIterator pc) {
  KevesCommon* common(vm->common());
  int argn(vm->registers_.argn());
  vm->acc_ = vm->gr2_;

  vm->gr1_ = common->getMesgText(argn > 4 ? KevesBuiltinValues::mesg_Req3GotMore :
				 argn == 3 ? KevesBuiltinValues::mesg_Req3Got2 :
				 argn == 2 ? KevesBuiltinValues::mesg_Req3Got1 :
				 KevesBuiltinValues::mesg_Req3Got0);
  
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

bool Function::IsFixnum::func(KevesValue kev) {
  return kev.isFixnum();
}

KevesValue Function::IsFixnum::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqIntNum);
}

KevesValue Function::IsFixnum::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqIntNumAs1st);
}

KevesValue Function::IsFixnum::message2(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqIntNumAs2nd);
}

KevesValue Function::IsFixnum::message3(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqIntNumAs3rd);
}

bool Function::IsChar::func(KevesValue kev) {
  return kev.isChar();
}

KevesValue Function::IsChar::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqChar);
}

KevesValue Function::IsChar::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqCharAs1st);
}

KevesValue Function::IsChar::message2(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqCharAs2nd);
}

bool Function::IsString::func(KevesValue kev) {
  return kev.isString();
}

KevesValue Function::IsString::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqStr);
}

KevesValue Function::IsString::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqStrAs1st);
}

bool Function::IsSymbol::func(KevesValue kev) {
  return kev.isSymbol();
}

KevesValue Function::IsSymbol::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqSym);
}

bool Function::IsPair::func(KevesValue kev) {
  return kev.isPair();
}

KevesValue Function::IsPair::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqPair);
}
