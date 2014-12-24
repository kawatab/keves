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


void TestCode::Code02::write(KevesBase* base, const char* file_name) {
  StringKev* str(StringKev::make(base, "abdefgh"));
  SymbolKev* sym(SymbolKev::make(base, "sYmbOl"));

  PairKev* pair2(PairKev::make(base, KevesChar('a'), str)); 

  VectorKev* vector1(VectorKev::make(base, 3));
  {
    KevesIterator iter(vector1->begin());
    *iter++ = EMB_FALSE;
    *iter++ = pair2;
    *iter++ = EMB_NULL;
  }

  PairKev* pair1(PairKev::make(base, EMB_TRUE, vector1)); 
  PairKev* pair3(PairKev::make(base, pair1, KevesFixnum(100)));
  PairKev* pair4(PairKev::make(base, pair2, pair3));
  PairKev* pair5(PairKev::make(base, sym, pair4));
  ReferenceKev* ref(ReferenceKev::make(base, pair5));
  pair1->set_car(pair5);
  
  Bignum* bignum(Bignum::makeFromString(base,
					"12334242342342431233424234234243"));

  StringKev* str_num(StringKev::make(base,
				     "123423423414127897/8907807843218742"));

  RationalNumberKev*
    rational(RationalNumberKev::makeFromString(base, *str_num));

  FlonumKev* flonum(FlonumKev::make(base, 1.2345678));
  
  ExactComplexNumberKev*
    exact_complex(ExactComplexNumberKev::make(base));

  exact_complex->set_real(*rational);
  exact_complex->set_imag(*rational);

  InexactComplexNumberKev*
    inexact_complex(InexactComplexNumberKev::makeFromFlonums(base,
							     FlonumKev(1.234),
							     FlonumKev(-3.234)));

  CodeKev* code(CodeKev::make(base, 12));
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
    
  LocalVarFrameKev* clsr(LocalVarFrameKev::make(base, 3, nullptr));
  LambdaKev* lambda(LambdaKev::make(base, clsr, code, 2));

  QStringList id;
  QList<ver_num_t> ver_num; // empty list
  id << "main";
  KevesLibrary this_lib(id, ver_num);
  this_lib.addBind("my-code", lambda);
  KevesImportLibraryList import_libs(base); // empty list
  this_lib.writeToFile(base, file_name, import_libs);
}

void TestCode::Code02::read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  
  KevesLibrary* lib(KevesLibrary::readFromFile(file_name, base));

  lib->displayProperty(base);
  
  KevesValue value(lib->findBind("my-code"));
  std::cout << qPrintable(base->toString(value)) << std::endl;

  if (value.type() == LAMBDA) {
    std::cout << "  lambda :" << std::endl;
    const LambdaKev* lambda(value);
    std::cout << qPrintable(base->toString(lambda->code())) << std::endl;
  }

  delete lib;
}

void TestCode::Code03::write(KevesBase* base, const char* file_name) {
  ArgumentFrameKev* argp(ArgumentFrameKev::make(base, 5)); 
  LocalVarFrameKev* envp(LocalVarFrameKev::make(base, 5, nullptr)); 
  LocalVarFrameKev* clsr1(LocalVarFrameKev::make(base, 5, nullptr)); 
  LocalVarFrameKev* clsr2(LocalVarFrameKev::make(base, 5, clsr1)); 
  KevesIterator pc;

  StackFrameKev* fp(StackFrameKev::make(base));
  fp->set_fp(nullptr);
  fp->set_pc(pc);
  fp->set_arg(argp, 3);
  fp->setEnvFrame(envp, 3);
  fp->setClosure(clsr2);
  fp->set_sfp(nullptr);

  QStringList id;
  QList<ver_num_t> ver_num;
  id << "main";
  KevesLibrary this_lib(id, ver_num);
  this_lib.addBind("my-code", fp);
  KevesImportLibraryList import_libs(base);
  this_lib.writeToFile(base, file_name, import_libs);
}

void TestCode::Code03::read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  KevesLibrary* lib(KevesLibrary::readFromFile(file_name, base));
  lib->displayProperty(base);
  delete lib;
}

void TestCode::KevesBaseCode::write(KevesBase* base, const char* file_name) {
  // Library Header
  QStringList id;
  QList<ver_num_t> ver_num;
  id << "keves" << "base";
  ver_num << 0 << 1;
  KevesLibrary this_lib(id, ver_num);

  // import binds
  KevesImportLibraryList import_libs(base);

  // from (keves base-bin)
  QStringList id_keves_base_bin;
  id_keves_base_bin << "keves" << "base-bin";
  QList<ver_num_t> ver_keves_base_bin;
  ver_keves_base_bin << 6;

  if (!import_libs.setLibrary(id_keves_base_bin, ver_keves_base_bin)) {
    std::cerr << "Import is failed\n";
    return;
  }

  KevesValue proc_display(import_libs.nominateBind("display"));
  KevesValue proc_newline(import_libs.nominateBind("newline"));
 
  // from (rnrs base)
  QStringList id_rnrs_base;
  id_rnrs_base << "rnrs" << "base";
  QList<ver_num_t> ver_rnrs_base;
  ver_rnrs_base << 6;

  if (!import_libs.setLibrary(id_rnrs_base, ver_rnrs_base)) {
    std::cerr << "Aborted writing the file: " << file_name << ".\n";
    return;
  }

  KevesValue proc_car(import_libs.nominateBind("car"));
  KevesValue proc_cadr(import_libs.nominateBind("cadr"));
  KevesValue proc_append(import_libs.nominateBind("append"));
  KevesValue proc_cons(import_libs.nominateBind("cons"));
  KevesValue proc_eqv_q(import_libs.nominateBind("eqv?"));
  KevesValue proc_eq_q(import_libs.nominateBind("eq?"));
  KevesValue proc_list(import_libs.nominateBind("list"));
  KevesValue proc_map(import_libs.nominateBind("map"));
  KevesValue proc_string_append(import_libs.nominateBind("string-append"));
 
  // from (rnrs unicode)
  QStringList id_rnrs_unicode;
  id_rnrs_unicode << "rnrs" << "unicode";
  QList<ver_num_t> ver_rnrs_unicode;
  ver_rnrs_unicode << 6;

  if (!import_libs.setLibrary(id_rnrs_unicode, ver_rnrs_unicode)) {
    std::cerr << "Aborted writing the file: " << file_name << ".\n";
    return;
  }

  KevesValue proc_char_upcase(import_libs.nominateBind("char-upcase"));
  KevesValue proc_char_upper_case_q(import_libs.nominateBind("char-upper-case?"));
  KevesValue proc_char_downcase(import_libs.nominateBind("char-downcase"));
 
  // values
  KevesValue list1(PairKev::make(base, KevesChar('B'), EMB_NULL));
  KevesValue list2(PairKev::make(base, KevesChar('a'), list1));
  KevesValue str1(StringKev::make(base, "ABC"));
  KevesValue str2(StringKev::make(base, "def"));
  KevesValue str3(StringKev::make(base, "xyz"));

  CodeKev* code(CodeKev::make(base, 125));
  {
    KevesIterator iter(code->begin());
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(15); // jump01
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(10); // jump03
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_char_downcase;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(5); // jump04
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_cadr;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list2;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_APPLY); // jump04
      }
      *iter++ = KevesInstruct(CMD_APPLY); // jump03
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump05
      *iter++ = KevesFixnum(3); // jump02
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump01
      *iter++ = KevesFixnum(17); // jump05
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(12); // jump06
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_append;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(7); // jump07
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_append;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list2;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list2;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_APPLY); // jump07
      }
      *iter++ = KevesInstruct(CMD_APPLY); // jump06
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump05
      *iter++ = KevesFixnum(3); // jump02
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(14);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(9);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_string_append;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str2;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str3;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_eqv_q;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	// *iter++ = proc_eq_q;
	*iter++ = proc_cons;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str2;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_map;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_char_upper_case_q;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = list2;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    *iter++ = KevesInstruct(CMD_HALT); // jump02

    Q_ASSERT(iter <= code->end());
  }

  // export binds
  this_lib.addBind("my-code", code);

  this_lib.writeToFile(base, file_name, import_libs);
}

void TestCode::KevesBaseCode::read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  
  KevesLibrary* lib(KevesLibrary::readFromFile(file_name, base));

  lib->displayProperty(base);
  
  KevesValue value(lib->findBind("my-code"));
  std::cout << qPrintable(base->toString(value)) << std::endl;

  delete lib;
}
