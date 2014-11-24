// Keves/lib_keves_base.hpp - library of Keves base
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

#include "keves_library.hpp"

#include <QHash>
#include "environment_kev.hpp"
#include "generator_kev.hpp"
// #include "macro_kev.hpp"
#include "symbol_kev.hpp"

class CodeKev;
class KevesBuiltInValues;
class LibKevesParse;
class LibRnrsBase;
class LibRnrsEval;
class LibRnrsExceptions;
class LibRnrsLists;
class LibRnrsMutableStrings;
class LibRnrsSyntaxCase;
class LibRnrsUnicode;

class LibKevesBase : public KevesLibrary {
public:
  static constexpr int NUM_OF_GLOBAL_VARS = 50;
  static constexpr int SIZE_OF_BUILTIN_CODE = 0x10000;
  static constexpr int SIZE_OF_PROC_TABLE = 512;

  LibKevesBase();
  LibKevesBase(const LibKevesBase&) = delete;
  LibKevesBase(LibKevesBase&&) = delete;
  LibKevesBase& operator=(const LibKevesBase&) = delete;
  LibKevesBase& operator=(LibKevesBase&&) = delete;
  ~LibKevesBase() = default;

  KevesLibrary* Init(KevesGC* gc);

  SymbolKev* sym_display_;
  CPSKev proc_display_;
  SymbolKev* sym_newline_;
  CPSKev proc_newline_;

  static KevesValue makeLexicalCondition(KevesIterator*, KevesValue, KevesValue, KevesValue);

  static void procDisplay(KevesVM*, const_KevesIterator);

  struct Newline {
    static void func(KevesVM*, const_KevesIterator);
  };
};
