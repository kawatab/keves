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

#include "kevc_generator.hpp"
#include "kev/number.hpp"
#include "kev/pair.hpp"
#include "kev/procedure.hpp"
#include "kev/reference.hpp"
#include "kev/string.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"
#include "value/char.hpp"
#include "value/instruct.hpp"


void TestCode::Code02::write(KevcGenerator* generator) {
  StringKev* str(generator->makeString("abdefgh"));
  SymbolKev* sym(generator->makeSymbol("sYmbOl"));

  PairKev* pair2(generator->makePair(KevesChar('a'), str)); 

  VectorKev* vector1(generator->makeVector(3));
  {
    KevesIterator iter(vector1->begin());
    *iter++ = EMB_FALSE;
    *iter++ = pair2;
    *iter++ = EMB_NULL;
  }

  PairKev* pair1(generator->makePair(EMB_TRUE, vector1)); 
  PairKev* pair3(generator->makePair(pair1, KevesFixnum(100)));
  PairKev* pair4(generator->makePair(pair2, pair3));
  PairKev* pair5(generator->makePair(sym, pair4));
  ReferenceKev* ref(ReferenceKev::make(generator->base(), pair5));
  pair1->set_car(pair5);
  
  Bignum* bignum(generator-> makeBignum("12334242342342431233424234234243"));
  StringKev* str_num(generator->makeString("123423423414127897/8907807843218742"));

  RationalNumberKev* rational(generator->makeRationalNumber(str_num));
  FlonumKev* flonum(generator->makeFlonum(1.2345678));
  ExactComplexNumberKev* exact_complex(generator->makeExactComplexNumber());
  exact_complex->set_real(*rational);
  exact_complex->set_imag(*rational);

  InexactComplexNumberKev*
    inexact_complex(generator->makeInexactComplexNumber(1.234, -3.234));

  CodeKev* code(generator->makeCode(12));
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
    
  LocalVarFrameKev* clsr(generator->makeLocalVarFrame(3, nullptr));
  LambdaKev* lambda(generator->makeLambda(clsr, code, 2));

  generator->exportBind("my-code", lambda);
  generator->writeToFile();
}

void TestCode::Code03::write(KevcGenerator* generator) {
  ArgumentFrameKev* argp(generator->makeArgumentFrame(5)); 
  LocalVarFrameKev* envp(generator->makeLocalVarFrame(5, nullptr)); 
  LocalVarFrameKev* clsr1(generator->makeLocalVarFrame(5, nullptr)); 
  LocalVarFrameKev* clsr2(generator->makeLocalVarFrame(5, clsr1)); 
  KevesIterator pc;

  StackFrameKev* fp(generator->makeStackFrameKev());
  fp->set_fp(nullptr);
  fp->set_pc(pc);
  fp->set_arg(argp, 3);
  fp->setEnvFrame(envp, 3);
  fp->setClosure(clsr2);
  fp->set_sfp(nullptr);

  generator->exportBind("my-code", fp);
  generator->writeToFile();
}
