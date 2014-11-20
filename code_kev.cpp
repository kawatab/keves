/* code_kev.cpp - codes for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 *  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "code_kev.hpp"

#include "keves_gc.hpp"


CodeKev::CodeKev(int size)
  : MutableKev(TYPE), size_(size) {
  clear();
}

void CodeKev::CopyArray(const CodeKev* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + size_);
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}
  
CodeKev* CodeKev::make(KevesGC* gc, int size) {
  auto ctor = [size](void* ptr) { return new(ptr) CodeKev(size); };
  return gc->Make(ctor, alloc_size(size));
}

KevesValue* CodeKev::array() {
  return reinterpret_cast<KevesValue*>(this + 1);
}

const KevesValue* CodeKev::array() const {
  return reinterpret_cast<const KevesValue*>(this + 1);
}

KevesIterator CodeKev::begin() {
  return KevesIterator(array());
}

const_KevesIterator CodeKev::begin() const {
  return const_KevesIterator(array());
}

KevesIterator CodeKev::end() {
  return begin() + size_;
}

const_KevesIterator CodeKev::end() const {
  return begin() + size_;
}

void CodeKev::clear() {
  std::fill_n(array(), size_, EMB_UNDEF);
}
