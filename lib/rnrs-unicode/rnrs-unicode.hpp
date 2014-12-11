// keves/lib/rnrs-unicode/rnrs-unicode.hpp - library of Keves base
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


#pragma once

#include <QHash>
#include "keves_library.hpp"
#include "kev/procedure.hpp"


class KevesBase;
class SymbolKev;

class LibRnrsUnicode : public KevesLibrary {
public:
  LibRnrsUnicode() = default;
  LibRnrsUnicode(const LibRnrsUnicode&) = delete;
  LibRnrsUnicode(LibRnrsUnicode&&) = delete;
  LibRnrsUnicode& operator=(const LibRnrsUnicode&) = delete;
  LibRnrsUnicode& operator=(LibRnrsUnicode&&) = delete;
  ~LibRnrsUnicode() = default;

  void Init(KevesBase* base);

  SymbolKev* sym_char_upcase_;
  CPSKev proc_char_upcase_;
  SymbolKev* sym_char_downcase_;
  CPSKev proc_char_downcase_;
  SymbolKev* sym_char_titlecase_;
  CPSKev proc_char_titlecase_;
  SymbolKev* sym_char_foldcase_;
  CPSKev proc_char_foldcase_;
  SymbolKev* sym_char_ci_eq_q_;
  CPSKev proc_char_ci_eq_q_;
  SymbolKev* sym_char_ci_lt_q_;
  CPSKev proc_char_ci_lt_q_;
  SymbolKev* sym_char_ci_gt_q_;
  CPSKev proc_char_ci_gt_q_;
  SymbolKev* sym_char_ci_lte_q_;
  CPSKev proc_char_ci_lte_q_;
  SymbolKev* sym_char_ci_gte_q_;
  CPSKev proc_char_ci_gte_q_;
  SymbolKev* sym_char_alphabetic_q_;
  CPSKev proc_char_alphabetic_q_;
  SymbolKev* sym_char_numeric_q_;
  CPSKev proc_char_numeric_q_;
  SymbolKev* sym_char_whitespace_q_;
  CPSKev proc_char_whitespace_q_;
  SymbolKev* sym_char_upper_case_q_;
  CPSKev proc_char_upper_case_q_;
  SymbolKev* sym_char_lower_case_q_;
  CPSKev proc_char_lower_case_q_;
  SymbolKev* sym_char_title_case_q_;
  CPSKev proc_char_title_case_q_;
  SymbolKev* sym_char_general_category_;
  CPSKev proc_char_general_category_;
  SymbolKev* sym_string_upcase_;
  CPSKev proc_string_upcase_;
  SymbolKev* sym_string_downcase_;
  CPSKev proc_string_downcase_;
  SymbolKev* sym_string_titlecase_;
  CPSKev proc_string_titlecase_;
  SymbolKev* sym_string_foldcase_;
  CPSKev proc_string_foldcase_;

private:
  struct CharUpcase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct CharDowncase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct CharTitlecase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct CharFoldcase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IsCharCiEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharCiLT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharCiGT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharCiLTE {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharCiGTE {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharAlphabetic {
    static bool func(KevesValue);
  };

  struct IsCharNumeric {
    static bool func(KevesValue);
  };

  struct IsCharWhitespace {
    static bool func(KevesValue);
  };

  struct IsCharUpperCase {
    static bool func(KevesValue);
  };

  struct IsCharLowerCase {
    static bool func(KevesValue);
  };

  struct IsCharTitleCase {
    static bool func(KevesValue);
  };

  struct CharGeneralCategory {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringUpcase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringDowncase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringTitlecase {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringFoldcase {
    static void func(KevesVM*, const_KevesIterator);
  };
};
