// keves/kevc_generator/kevc_generator.cpp - a test code for kevc_generator
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


#include "kevc_generator.hpp"

#include <iostream>
#include "keves_common.hpp"
#include "kev/bignum.hpp"
#include "kev/bignum-inl.hpp"
#include "kev/code.hpp"
#include "kev/environment.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/pair.hpp"
#include "kev/procedure.hpp"
#include "kev/reference.hpp"
#include "kev/reference-inl.hpp"
#include "kev/string.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"


KevcGenerator::KevcGenerator(KevesCommon* common, const char* file_name,
			     const char* id)
  : common_(common),
    file_name_(file_name),
    this_lib_(id),
    import_libs_(common) {}

KevcGenerator::KevcGenerator(KevesCommon* common, const char* file_name,
			     const char* id1, const char* id2, ver_num_t ver_num)
  : common_(common),
    file_name_(file_name),
    this_lib_(id1, id2, ver_num),
    import_libs_(common) {}

KevcGenerator::KevcGenerator(KevesCommon* common, const char* file_name,
			     const char* id1, const char* id2,
			     ver_num_t ver_num1, ver_num_t ver_num2)
  : common_(common),
    file_name_(file_name),
    this_lib_(id1, id2, ver_num1, ver_num2),
    import_libs_(common) {}

bool KevcGenerator::setImportLibrary(const char* id1,
				     const char* id2,
				     ver_num_t ver_num1) {
  QStringList id;
  id << id1 << id2;
  QList<ver_num_t> ver_num;
  ver_num << ver_num1;
    
  if (!import_libs_.setLibrary(id, ver_num)) {
    std::cerr << "Aborted writing the file: " << file_name_ << ".\n";
    return false;
  }

  return true;
}

bool KevcGenerator::setImportLibrary(const char* id1, const char* id2,
				     ver_num_t ver_num1, ver_num_t ver_num2) {
  QStringList id;
  id << id1 << id2;
  QList<ver_num_t> ver_num;
  ver_num << ver_num1 <<ver_num2;
    
  if (!import_libs_.setLibrary(id, ver_num)) {
    std::cerr << "Aborted writing the file: " << file_name_ << ".\n";
    return false;
  }

  return true;
}

KevesValue KevcGenerator::findBind(const char* id) const {
  return this_lib_.findBind(id);
}

void KevcGenerator::exportBind(const char* id, KevesValue value) {
  this_lib_.addBind(id, value);
}

KevesValue KevcGenerator::importBind(const char* id) {
  return import_libs_.nominateBind(id);
}

void KevcGenerator::writeToFile() {
  this_lib_.writeToFile(common_, file_name_, import_libs_);
}

KevesLibrary* KevcGenerator::readFromFile(KevesCommon* common,
					  const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  return KevesLibrary::readFromFile(file_name, common);
}

void KevcGenerator::testRead(KevesCommon* common, const char* file_name) {
  KevesLibrary* lib(readFromFile(common, file_name));
  lib->displayProperty(common);
  delete lib;
}

ArgumentFrameKev* KevcGenerator::makeArgumentFrame(int size) {
  return ArgumentFrameKev::make(common_, size);
}

KevesValue KevcGenerator::makeAssertCondition(const SymbolKev* who,
					      const char* message,
					      KevesValue irritants) {
  return common_->makeAssertCondition(who,
				      common_->getMesgText(message),
				      irritants);
}
      
Bignum* KevcGenerator::makeBignum(const char* str) {
  return Bignum::makeFromString(common_, str);
}

CodeKev* KevcGenerator::makeCode(int size) {
  return CodeKev::make(common_, size);
}

ExactComplexNumberKev* KevcGenerator::makeExactComplexNumber() {
  return ExactComplexNumberKev::make(common_);
}

EnvironmentKev* KevcGenerator::makeEnvironment(KevesValue values) {
  return EnvironmentKev::make(common_, values);
}

FlonumKev* KevcGenerator::makeFlonum(double value) {
  return FlonumKev::make(common_, value);
}

InexactComplexNumberKev* KevcGenerator::makeInexactComplexNumber(double real,
								 double imag) {
  return InexactComplexNumberKev::makeFromFlonums(common_,
						  FlonumKev(real),
						  FlonumKev(imag));
}

LambdaKev* KevcGenerator::makeLambda(LocalVarFrameKev* free_vars,
				     CodeKev* code,
				     int index) {
  return LambdaKev::make(common_, free_vars, code, index);
}

LocalVarFrameKev* KevcGenerator::makeLocalVarFrame(int size,
						   LocalVarFrameKev* next) {
  return LocalVarFrameKev::make(common_, size, next);
}

PairKev* KevcGenerator::makePair(KevesValue car, KevesValue cdr) {
  return PairKev::make(common_, car, cdr);
}

RationalNumberKev* KevcGenerator::makeRationalNumber(const StringKev* str_num) {
  return RationalNumberKev::makeFromString(common_, *str_num);
}

ReferenceKev* KevcGenerator::makeReference(KevesValue value) {
  return ReferenceKev::make(common_, value);
}

StackFrameKev* KevcGenerator::makeStackFrameKev() {
  return StackFrameKev::make(common_);
}

StringKev* KevcGenerator::makeString(const char* str) {
  return StringKev::make(common_, str);
}

SymbolKev* KevcGenerator::makeSymbol(const char* sym) {
  return SymbolKev::make(common_, sym);
}

VectorKev* KevcGenerator::makeVector(int size) {
  return VectorKev::make(common_, size);
}
