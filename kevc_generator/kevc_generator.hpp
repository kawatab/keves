// keves/kevc_generator/kevc_generator.hpp - a test code for kevc_generator
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

#pragma once

#include "keves_library.hpp"


class ArgumentFrameKev;
class Bignum;
class CodeKev;
class ExactComplexNumberKev;
class FlonumKev;
class InexactComplexNumberKev;
class LambdaKev;
class LocalVarFrameKev;
class PairKev;
class RationalNumberKev;
class ReferenceKev;
class StackFrameKev;
class StringKev;
class SymbolKev;
class VectorKev;


class KevcGenerator {
public:
  KevcGenerator() = delete;
  KevcGenerator(const KevcGenerator&) = delete;
  KevcGenerator(const KevcGenerator&&) = delete;
  KevcGenerator& operator=(const KevcGenerator&) = delete;
  KevcGenerator& operator=(const KevcGenerator&&) = delete;
  ~KevcGenerator() = default;

  KevcGenerator(KevesCommon* common, const char* file_name,
		const char* id);

  KevcGenerator(KevesCommon* common, const char* file_name,
		const char* id1, const char* id2, ver_num_t ver_num);

  KevcGenerator(KevesCommon* common, const char* file_name,
		const char* id1, const char* id2,
		ver_num_t ver_num1, ver_num_t ver_num2);

  /*
  KevesCommon* common() {
    return common_;
  }
  */
  
  void exportBind(const char* id, KevesValue value);
  KevesValue findBind(const char* id) const;
  KevesValue importBind(const char* id);
  ArgumentFrameKev* makeArgumentFrame(int size);

  KevesValue makeAssertCondition(const SymbolKev* who,
				 const char* message,
				 KevesValue irritants);

  Bignum* makeBignum(const char* str);
  CodeKev* makeCode(int size);
  ExactComplexNumberKev* makeExactComplexNumber();
  FlonumKev* makeFlonum(double value);
  InexactComplexNumberKev* makeInexactComplexNumber(double real, double imag);
  LambdaKev* makeLambda(LocalVarFrameKev* free_vars, CodeKev* code, int index);
  LocalVarFrameKev* makeLocalVarFrame(int size, LocalVarFrameKev* next);
  PairKev* makePair(KevesValue car, KevesValue cdr);
  RationalNumberKev* makeRationalNumber(const StringKev* str_num);
  ReferenceKev* makeReference(KevesValue value);
  StackFrameKev* makeStackFrameKev();
  StringKev* makeString(const char* str);
  SymbolKev* makeSymbol(const char* sym);
  VectorKev* makeVector(int size);
  bool setImportLibrary(const char* id1, const char* id2, ver_num_t ver_num1);

  bool setImportLibrary(const char* id1, const char* id2,
			ver_num_t ver_num1, ver_num_t ver_num2);

  void writeToFile();

  static void testRead(KevesCommon* common, const char* file_name);

private:
  static KevesLibrary* readFromFile(KevesCommon* common, const char* file_name);

  KevesCommon* common_;
  const char* file_name_;
  KevesLibrary this_lib_;
  KevesImportLibraryList import_libs_;
};
