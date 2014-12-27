// keves/lib/rnrs-mutable-strings/rnrs-mutable-strings.hpp - library of Keves base
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

class LibRnrsMutableStrings : public KevesLibrary {
public:
  LibRnrsMutableStrings() = default;
  LibRnrsMutableStrings(const LibRnrsMutableStrings&) = delete;
  LibRnrsMutableStrings(LibRnrsMutableStrings&&) = delete;
  LibRnrsMutableStrings& operator=(const LibRnrsMutableStrings&) = delete;
  LibRnrsMutableStrings& operator=(LibRnrsMutableStrings&&) = delete;
  ~LibRnrsMutableStrings() = default;

  void init(KevesCommon* common);

  SymbolKev* sym_string_fill_e_;
  CPSKev proc_string_fill_e_;
  SymbolKev* sym_string_set_e_;
  CPSKev proc_string_set_e_;

private:
  struct StringFillE {
    static void func(KevesVM* vm, const_KevesIterator pc);
  };

  struct StringSetE {
    static void func(KevesVM* vm, const_KevesIterator pc);
  };
};
