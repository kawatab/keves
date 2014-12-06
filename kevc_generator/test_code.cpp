// keves/kevc_generator/test_code.cpp - a test code for kevc_generator
// Keves will be an R6RS Scheme implementation.
//
// Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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


#include "test_code.hpp"

#include <iostream>
#include <QList>
#include "keves_base.hpp"
#include "keves_library.hpp"
#include "kev/bignum.hpp"
#include "kev/bignum-inl.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/pair.hpp"
#include "kev/pair-inl.hpp"
#include "kev/procedure.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/reference.hpp"
#include "kev/reference-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "kev/vector.hpp"
#include "kev/vector-inl.hpp"
#include "value/char.hpp"


void TestCode::Code02::Write(KevesBase* base, const char* file_name) {
  StringKev* str(StringKev::Make(base, "abdefgh"));
  SymbolKev* sym(SymbolKev::Make(base, "sYmbOl"));

  PairKev* pair2(PairKev::Make(base, KevesChar('a'), str)); 

  VectorKev* vector1(VectorKev::Make(base, 3));
  {
    KevesIterator iter(vector1->begin());
    *iter++ = EMB_FALSE;
    *iter++ = pair2;
    *iter++ = EMB_NULL;
  }

  PairKev* pair1(PairKev::Make(base, EMB_TRUE, vector1)); 
  PairKev* pair3(PairKev::Make(base, pair1, KevesFixnum(100)));
  PairKev* pair4(PairKev::Make(base, pair2, pair3));
  PairKev* pair5(PairKev::Make(base, sym, pair4));
  ReferenceKev* ref(ReferenceKev::Make(base, pair5));
  pair1->set_car(pair5);
  
  Bignum* bignum(Bignum::makeFromString(base,
					"12334242342342431233424234234243"));

  StringKev* str_num(StringKev::Make(base,
				     "123423423414127897/8907807843218742"));

  RationalNumberKev*
    rational(RationalNumberKev::makeFromString(base, *str_num));

  FlonumKev* flonum(FlonumKev::Make(base, 1.2345678));
  
  ExactComplexNumberKev*
    exact_complex(ExactComplexNumberKev::Make(base));

  exact_complex->set_real(*rational);
  exact_complex->set_imag(*rational);

  InexactComplexNumberKev*
    inexact_complex(InexactComplexNumberKev::makeFromFlonums(base,
							     FlonumKev(1.234),
							     FlonumKev(-3.234)));

  CodeKev* code(CodeKev::Make(base, 12));
  {
    KevesIterator iter(code->begin());
    *iter++ = pair1;
    *iter++ = vector1;
    *iter++ = str;
    *iter++ = sym;
    *iter++ = ref;
    *iter++ = bignum;
    *iter++ = rational;
    *iter++ = flonum;
    *iter++ = exact_complex;
    *iter++ = inexact_complex;
    *iter++ = KevesInstruct(CMD_HALT);
    Q_ASSERT(iter <= code->end());
  }
    
  FreeVarFrameKev* clsr(FreeVarFrameKev::Make(base, 3, nullptr));
  LambdaKev* lambda(LambdaKev::Make(base, clsr, code, 2));

  KevesLibrary this_lib;
  this_lib.SetID("main");
  this_lib.AddBind("my-code", lambda);

  KevesImportBinds required_lib;
  required_lib.SetID("rnrs", "io", "simple");
  required_lib.SetVerNum(6);
  required_lib.AddBind("display");
  required_lib.AddBind("newline");
  this_lib.AppendImportLib(required_lib);

  this_lib.WriteToFile(file_name, base);
}

void TestCode::Code02::Read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  
  KevesLibrary* lib(KevesLibrary::ReadFromFile(file_name, base));

  lib->Display(base);
  
  // KevesValue value(lib->GetCode());
  KevesValue value(lib->FindBind("my-code"));
  std::cout << qPrintable(base->ToString(value)) << std::endl;

  if (value.type() == LAMBDA) {
    std::cout << "  lambda :" << std::endl;
    const LambdaKev* lambda(value);
    std::cout << qPrintable(base->ToString(lambda->code())) << std::endl;
  }

  delete lib;
}

void TestCode::Code03::Write(KevesBase* base, const char* file_name) {
  ArgumentFrameKev* argp(ArgumentFrameKev::Make(base, 5)); 
  LocalVarFrameKev* envp(LocalVarFrameKev::Make(base, 5)); 
  FreeVarFrameKev* clsr1(FreeVarFrameKev::Make(base, 5, nullptr)); 
  FreeVarFrameKev* clsr2(FreeVarFrameKev::Make(base, 5, clsr1)); 
  KevesIterator pc;

  StackFrameKev* fp(StackFrameKev::Make(base));
  fp->set_fp(nullptr);
  fp->set_pc(pc);
  fp->set_arg(argp, 3);
  fp->setEnvFrame(envp, 3);
  fp->setClosure(clsr2);
  fp->set_sfp(nullptr);

  KevesLibrary this_lib;
  this_lib.SetID("main");
  this_lib.AddBind("my-code", fp);

  KevesImportBinds required_lib;
  required_lib.SetID("rnrs", "io", "simple");
  required_lib.SetVerNum(6);
  required_lib.AddBind("display");
  this_lib.AppendImportLib(required_lib);

  this_lib.WriteToFile(file_name, base);
}

void TestCode::Code03::Read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  KevesLibrary* lib(KevesLibrary::ReadFromFile(file_name, base));
  lib->Display(base);
  delete lib;
}
