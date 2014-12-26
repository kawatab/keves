// keves/lib/rnrs-exceptions/rnrs-exceptions.hpp - library of Keves base
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


class KevesBase;
class SymbolKev;

class LibRnrsExceptions : public KevesLibrary {
public:
  LibRnrsExceptions() = default;
  LibRnrsExceptions(const LibRnrsExceptions&) = delete;
  LibRnrsExceptions(LibRnrsExceptions&&) = delete;
  LibRnrsExceptions& operator=(const LibRnrsExceptions&) = delete;
  LibRnrsExceptions& operator=(LibRnrsExceptions&&) = delete;
  ~LibRnrsExceptions() = default;

  void init(KevesBase* base);

  SymbolKev* sym_raise_;
  CPSKev proc_raise_;

private:
  struct Raise {
    static void func(KevesVM* vm, const_KevesIterator pc);
  };
};
