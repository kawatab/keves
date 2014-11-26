/* keves_generator/test_code.cpp - a test code for keves_generator
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test_code.hpp"

#include <iostream>
#include <QDataStream>
#include <QFile>
#include <QList>
#include "keves_file_io.hpp"
#include "keves_base.hpp"
#include "kev/bignum.hpp"
#include "kev/code.hpp"
#include "kev/frame.hpp"
#include "kev/number.hpp"
#include "kev/pair.hpp"
#include "kev/reference.hpp"
#include "kev/string.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"
#include "lib/library.hpp"
#include "value/char.hpp"


// void TestCode::Code01::Write(KevesBase* /*base*/, const char* file_name) {
  /*
  QFile file(file_name);

  if (!file.open(QIODevice::WriteOnly)) {
    std::cerr << "io error in " << file_name;
    return;
  }

  QDataStream out(&file);
  KevesLibrary this_lib;
  this_lib.id_ << "keves" << "base" << "simple";
  this_lib.ver_num_ << 6 << 1;

  KevesLibrary import_lib1;
  import_lib1.id_ << "rnrs" << "base";
  import_lib1.ver_num_ << 6 << 1;

  KevesLibrary import_lib2;
  import_lib2.id_ << "rnrs" << "io" << "simple";
  import_lib2.ver_num_ << 6 << 1;

  KevesLibrary import_lib3;
  import_lib3.id_ << "rnrs" << "unicode";
  import_lib3.ver_num_ << 6 << 2;

  QList<KevesLibrary> libs;
  libs << this_lib << import_lib1 << import_lib2 << import_lib3;
  out << libs;

  file.close();
}
  */

// void TestCode::Code01::Read(KevesBase* /*gc*/, const char* file_name) {
/*
  QFile file(file_name);

  if (!file.open(QIODevice::ReadOnly)) {
    std::cout << "io error " << file_name << std::endl;
    return;
  }

  std::cout << file_name << ":" << std::endl;

  QDataStream in(&file);
  QList<KevesLibrary> libs;

  in >> libs;
  KevesBase::DisplayLibraryName(libs.at(0));

  for (int i(1); i < libs.size(); ++i)
    KevesBase::DisplayLibraryName(libs.at(i));
}
*/

void TestCode::Code02::Write(KevesBase* base, const char* file_name) {
  StringKev* str(StringKev::Make(base->gc(), "abdefgh"));
  SymbolKev* sym(SymbolKev::Make(base->gc(), "sYmbOl"));

  PairKev* pair2(PairKev::Make(base->gc(), KevesChar('a'), str)); 

  VectorKev* vector1(VectorKev::Make(base->gc(), 3));
  {
    KevesIterator iter(vector1->begin());
    *iter++ = EMB_FALSE;
    *iter++ = pair2;
    *iter++ = EMB_NULL;
  }

  PairKev* pair1(PairKev::Make(base->gc(), EMB_TRUE, vector1)); 
  PairKev* pair3(PairKev::Make(base->gc(), pair1, KevesFixnum(100)));
  PairKev* pair4(PairKev::Make(base->gc(), pair2, pair3));
  PairKev* pair5(PairKev::Make(base->gc(), sym, pair4));
  ReferenceKev* ref(ReferenceKev::make(base->gc(), pair5));
  pair1->set_car(pair5);
  
  Bignum* bignum(Bignum::makeFromString(base->gc(),
					"12334242342342431233424234234243"));

  StringKev* str_num(StringKev::Make(base->gc(),
				     "123423423414127897/8907807843218742"));

  RationalNumberKev*
    rational(RationalNumberKev::makeFromString(base->gc(), *str_num));

  FlonumKev* flonum(FlonumKev::make(base->gc(), 1.2345678));
  
  ExactComplexNumberKev*
    exact_complex(ExactComplexNumberKev::make(base->gc()));

  exact_complex->set_real(*rational);
  exact_complex->set_imag(*rational);

  InexactComplexNumberKev*
    inexact_complex(InexactComplexNumberKev::makeFromFlonums(base->gc(),
							     FlonumKev(1.234),
							     FlonumKev(-3.234)));

  CodeKev* code(CodeKev::make(base->gc(), 12));
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
    
  FreeVarFrameKev* clsr(FreeVarFrameKev::make(base->gc(), 3, nullptr));
  LambdaKev* lambda(LambdaKev::make(base->gc(), clsr, code, 2));

  KevesLibrary this_lib;
  this_lib.id_ << "main";

  KevesLibrary required_lib;
  required_lib.id_ << "rnrs" << "io" << "simple";
  required_lib.ver_num_ << 6;
  required_lib.AddBind("display", EMB_NULL);
  required_lib.AddBind("newline", EMB_NULL);

  QList<KevesLibrary> libs;
  libs << this_lib << required_lib;

  KevesFileIO file(file_name);
  file.Write(base, libs, lambda);
}

void TestCode::Code02::Read(KevesBase* base, const char* file_name) {
  KevesFileIO file(file_name);

  std::cout << file_name << ":" << std::endl;
  
  QList<KevesLibrary> libs(file.Read(base));

  libs.at(0).DisplayLibraryName();
  
  for (int i(1); i < libs.size(); ++i)
    libs.at(i).DisplayLibraryName();

  KevesValue value(libs.at(0).code_);
  std::cout << qPrintable(base->ToString(value)) << std::endl;

  if (value.type() == LAMBDA) {
    std::cout << "  lambda :" << std::endl;
    const LambdaKev* lambda(value);
    std::cout << qPrintable(base->ToString(lambda->code())) << std::endl;
  }
}

void TestCode::Code03::Write(KevesBase* base, const char* file_name) {
  ArgumentFrameKev* argp(ArgumentFrameKev::make(base->gc(), 5)); 
  LocalVarFrameKev* envp(LocalVarFrameKev::make(base->gc(), 5)); 
  FreeVarFrameKev* clsr1(FreeVarFrameKev::make(base->gc(), 5, nullptr)); 
  FreeVarFrameKev* clsr2(FreeVarFrameKev::make(base->gc(), 5, clsr1)); 
  KevesIterator pc;

  StackFrameKev* fp(StackFrameKev::make(base->gc()));
  fp->set_fp(nullptr);
  fp->set_pc(pc);
  fp->set_arg(argp, 3);
  fp->setEnvFrame(envp, 3);
  fp->setClosure(clsr2);
  fp->set_sfp(nullptr);

  KevesLibrary this_lib;
  this_lib.id_ << "main";

  KevesLibrary required_lib;
  required_lib.id_ << "rnrs" << "io" << "simple";
  required_lib.ver_num_ << 6;
  required_lib.AddBind("display", EMB_NULL);

  QList<KevesLibrary> libs;
  libs << this_lib << required_lib;

  KevesFileIO file(file_name);
  file.Write(base, libs, fp);
}

void TestCode::Code03::Read(KevesBase* base, const char* file_name) {
  KevesFileIO file(file_name);

  std::cout << file_name << ":" << std::endl;
  
  QList<KevesLibrary> libs(file.Read(base));
  libs.at(0).DisplayLibraryName();
  
  for (int i(1); i < libs.size(); ++i)
    libs.at(i).DisplayLibraryName();

  std::cout << qPrintable(base->ToString(libs.at(0).code_)) << std::endl;
}
