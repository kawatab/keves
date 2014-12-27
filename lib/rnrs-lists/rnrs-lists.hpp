// keves/lib/rnrs-lists/rnrs-lists.hpp - library of Keves base
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

class LibRnrsLists : public KevesLibrary {
public:
  LibRnrsLists() = default;
  LibRnrsLists(const LibRnrsLists&) = delete;
  LibRnrsLists(LibRnrsLists&&) = delete;
  LibRnrsLists& operator=(const LibRnrsLists&) = delete;
  LibRnrsLists& operator=(LibRnrsLists&&) = delete;
  ~LibRnrsLists() = default;

  void init(KevesCommon* common);

  SymbolKev* sym_cons_star_;
  CPSKev proc_cons_star_;

private:
  static void procConsStar(KevesVM*, const_KevesIterator);
  static void procConsStar_helper(KevesVM*, const_KevesIterator);
};
