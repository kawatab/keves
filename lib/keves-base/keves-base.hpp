// keves/lib/keves-base/keves-base.hpp - library of Keves base
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
#include "kev/procedure.hpp"


class KevesCommon;
class SymbolKev;

class LibKevesBase : public KevesLibrary {
public:
  LibKevesBase() = default;
  LibKevesBase(const LibKevesBase&) = delete;
  LibKevesBase(LibKevesBase&&) = delete;
  LibKevesBase& operator=(const LibKevesBase&) = delete;
  LibKevesBase& operator=(LibKevesBase&&) = delete;
  ~LibKevesBase() = default;

  void init(KevesCommon* common);

  SymbolKev* sym_display_;
  CPSKev proc_display_;
  SymbolKev* sym_newline_;
  CPSKev proc_newline_;

  /*
  static KevesValue makeLexicalCondition(KevesIterator*,
					 KevesValue, KevesValue, KevesValue);
  */

private:
  static void procDisplay(KevesVM*, const_KevesIterator);

  struct Newline {
    static void func(KevesVM*, const_KevesIterator);
  };
};
