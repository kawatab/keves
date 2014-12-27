// keves/lib/rnrs-unicode/rnrs-unicode.cpp - library of (rnrs unicode)
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


#include "lib/rnrs-unicode/rnrs-unicode.hpp"

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
#include "kev/procedure-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "value/char.hpp"


void LibRnrsUnicode::init(KevesCommon* common) {
  std::cout << "LibRnrsUnicode::init()" << std::endl;

  // setID("rnrs", "unicode-bin");
  // setVerNum(6);
					       
  sym_char_alphabetic_q_ = SymbolKev::make(common, "char-alphabetic?");
  sym_char_downcase_ = SymbolKev::make(common, "char-downcase");
  sym_char_general_category_ = SymbolKev::make(common, "char-general-category");
  sym_char_lower_case_q_ = SymbolKev::make(common, "char-lower-case?");
  sym_char_numeric_q_ = SymbolKev::make(common, "char-numeric?");
  sym_char_title_case_q_ = SymbolKev::make(common, "char-title-case?");
  sym_char_titlecase_ = SymbolKev::make(common, "char-titlecase");
  sym_char_foldcase_ = SymbolKev::make(common, "char-foldcase");
  sym_char_ci_eq_q_ = SymbolKev::make(common, "char-ci=?");
  sym_char_ci_lt_q_ = SymbolKev::make(common, "char-ci<?");
  sym_char_ci_gt_q_ = SymbolKev::make(common, "char-ci>?");
  sym_char_ci_lte_q_ = SymbolKev::make(common, "char-ci<=?");
  sym_char_ci_gte_q_ = SymbolKev::make(common, "char-ci>=?");
  sym_char_upcase_ = SymbolKev::make(common, "char-upcase");
  sym_char_upper_case_q_ = SymbolKev::make(common, "char-upper-case?");
  sym_char_whitespace_q_ = SymbolKev::make(common, "char-whitespace?");
  sym_string_downcase_ = SymbolKev::make(common, "string-downcase");
  sym_string_foldcase_ = SymbolKev::make(common, "string-foldcase");
  sym_string_titlecase_ = SymbolKev::make(common, "string-titlecase");
  sym_string_upcase_ = SymbolKev::make(common, "string-upcase");

  proc_char_upcase_.set(&Function::make<Function::IsChar, CharUpcase>, sym_char_upcase_);
  proc_char_downcase_.set(&Function::make<Function::IsChar, CharDowncase>, sym_char_downcase_);
  proc_char_titlecase_.set(&Function::make<Function::IsChar, CharTitlecase>, sym_char_titlecase_);
  proc_char_foldcase_.set(&Function::make<Function::IsChar, CharFoldcase>, sym_char_foldcase_);
  proc_char_ci_eq_q_.set(&Function::compare<Function::IsChar, IsCharCiEqual>, sym_char_ci_eq_q_);
  proc_char_ci_lt_q_.set(&Function::compare<Function::IsChar, IsCharCiLT>, sym_char_ci_lt_q_);
  proc_char_ci_gt_q_.set(&Function::compare<Function::IsChar, IsCharCiGT>, sym_char_ci_gt_q_);
  proc_char_ci_lte_q_.set(&Function::compare<Function::IsChar, IsCharCiLTE>, sym_char_ci_lte_q_);
  proc_char_ci_gte_q_.set(&Function::compare<Function::IsChar, IsCharCiGTE>, sym_char_ci_gte_q_);
  proc_char_alphabetic_q_.set(&Function::predicate<Function::IsChar, IsCharAlphabetic>, sym_char_alphabetic_q_);
  proc_char_numeric_q_.set(&Function::predicate<Function::IsChar, IsCharNumeric>, sym_char_numeric_q_);
  proc_char_whitespace_q_.set(&Function::predicate<Function::IsChar, IsCharWhitespace>, sym_char_whitespace_q_);
  proc_char_upper_case_q_.set(&Function::predicate<Function::IsChar, IsCharUpperCase>, sym_char_upper_case_q_);
  proc_char_lower_case_q_.set(&Function::predicate<Function::IsChar, IsCharLowerCase>, sym_char_lower_case_q_);
  proc_char_title_case_q_.set(&Function::predicate<Function::IsChar, IsCharTitleCase>, sym_char_title_case_q_);
  proc_char_general_category_.set(&Function::make<Function::IsChar, CharGeneralCategory>, sym_char_general_category_);
  proc_string_upcase_.set(&Function::make<Function::IsString, StringUpcase>, sym_string_upcase_);
  proc_string_downcase_.set(&Function::make<Function::IsString, StringDowncase>, sym_string_downcase_);
  proc_string_titlecase_.set(&Function::make<Function::IsString, StringTitlecase>, sym_string_titlecase_);
  proc_string_foldcase_.set(&Function::make<Function::IsString, StringFoldcase>, sym_string_foldcase_);

  addBind("char-alphabetic?", &proc_char_alphabetic_q_);
  addBind("char-upcase", &proc_char_upcase_);
  addBind("char-downcase", &proc_char_downcase_);
  addBind("char-titlecase", &proc_char_titlecase_);
  addBind("char-foldcase", &proc_char_foldcase_);
  addBind("char-ci=?", &proc_char_ci_eq_q_);
  addBind("char-ci<?", &proc_char_ci_lt_q_);
  addBind("char-ci>?", &proc_char_ci_gt_q_);
  addBind("char-ci<=?", &proc_char_ci_lte_q_);
  addBind("char-ci>=?", &proc_char_ci_gte_q_);
  addBind("char-numeric?", &proc_char_numeric_q_);
  addBind("char-whitespace?", &proc_char_whitespace_q_);
  addBind("char-upper-case?", &proc_char_upper_case_q_);
  addBind("char-lower-case?", &proc_char_lower_case_q_);
  addBind("char-title-case?", &proc_char_title_case_q_);
  addBind("char-general-category", &proc_char_general_category_);
  addBind("string-upcase", &proc_string_upcase_);
  addBind("string-downcase", &proc_string_downcase_);
  addBind("string-titlecase", &proc_string_titlecase_);
  addBind("string-foldcase", &proc_string_foldcase_);

}

void LibRnrsUnicode::CharUpcase::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = chr.toUpper();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsUnicode::CharDowncase::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = chr.toLower();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsUnicode::CharTitlecase::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = chr.toTitleCase();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsUnicode::CharFoldcase::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = chr.toCaseFolded();
  return KevesVM::returnValue(vm, pc);
}

bool LibRnrsUnicode::IsCharCiEqual::func(KevesValue qev1, KevesValue qev2) {
  return KevesChar(qev1).toCaseFolded() == KevesChar(qev2).toCaseFolded();
}

bool LibRnrsUnicode::IsCharCiLT::func(KevesValue qev1, KevesValue qev2) {
  return KevesChar(qev1).toCaseFolded() < KevesChar(qev2).toCaseFolded();
}

bool LibRnrsUnicode::IsCharCiGT::func(KevesValue qev1, KevesValue qev2) {
  return KevesChar(qev1).toCaseFolded() > KevesChar(qev2).toCaseFolded();
}

bool LibRnrsUnicode::IsCharCiLTE::func(KevesValue qev1, KevesValue qev2) {
  return KevesChar(qev1).toCaseFolded() <= KevesChar(qev2).toCaseFolded();
}

bool LibRnrsUnicode::IsCharCiGTE::func(KevesValue qev1, KevesValue qev2) {
  return KevesChar(qev1).toCaseFolded() >= KevesChar(qev2).toCaseFolded();
}

bool LibRnrsUnicode::IsCharAlphabetic::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isLetter();
}

bool LibRnrsUnicode::IsCharNumeric::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isNumber();
}

bool LibRnrsUnicode::IsCharWhitespace::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isSpace();
}

bool LibRnrsUnicode::IsCharUpperCase::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isUpper();
}

bool LibRnrsUnicode::IsCharLowerCase::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isLower();
}

bool LibRnrsUnicode::IsCharTitleCase::func(KevesValue qev) {
  KevesChar chr(qev);
  return chr.isTitleCase();
}

void LibRnrsUnicode::CharGeneralCategory::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);

  const char* cat[] { "XX", "Mn", "Mc", "Me", "Nd", "Nl", "No", "Zs",
		      "Zp", "Zl", "Cc", "Cf", "Cs", "Co", "Cn", "Lu",
		      "Ll", "Lt", "Lm", "Lo", "Pc", "Pd", "Ps", "Pe",
		      "Pi", "Pf", "Po", "Sm", "Sc", "Sk", "So", };

  QChar::Category cat_n(chr.category());
  StringKevWithArray<0x4> cat_str(cat[cat_n]);
  SymbolKev cat_sym(cat_str);
  vm->acc_ = &cat_sym;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsUnicode::StringUpcase::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev upcase(original->toUpper(vm->gc()));
  vm->acc_ = &upcase;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsUnicode::StringDowncase::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev downcase(original->toLower(vm->gc()));
  vm->acc_ = &downcase;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsUnicode::StringTitlecase::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev titlecase(original->toTitleCase(vm->gc()));
  vm->acc_ = &titlecase;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsUnicode::StringFoldcase::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev foldcase(original->toCaseFolded(vm->gc()));
  vm->acc_ = &foldcase;
  return KevesVM::returnValueSafe(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesCommon* common) {
    LibRnrsUnicode* library(new LibRnrsUnicode());
    library->init(common);
    return library;
  }
}
