// keves/kev/vector.cpp - vectors for Keves
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


#include "kev/vector.hpp"


VectorKev::VectorKev(int n) : MutableKev(TYPE), size_(n) {
  clear();
}

KevesValue VectorKev::at(int i) const {
  Q_ASSERT(i >= 0 && i < size_);
  return array()[i];
}

const_KevesIterator VectorKev::begin() const {
  return const_KevesIterator(array());
}

KevesIterator VectorKev::begin() {
  return KevesIterator(array());
}

const_KevesIterator VectorKev::end() const {
  return begin() +  size_;
}

KevesIterator VectorKev::end() {
  return begin() +  size_;
}

void VectorKev::replace(int i, KevesValue obj) {
  Q_ASSERT(i >= 0 && i < size_);
  array()[i] = obj;
}
  
void VectorKev::copyArray(const VectorKev* org) {
  std::copy_n(org->array(), size_, this->array());
}

const KevesValue* VectorKev::array() const {
  return reinterpret_cast<const KevesValue*>(this + 1);
}

KevesValue* VectorKev::array() {
  return reinterpret_cast<KevesValue*>(this + 1);
}

void VectorKev::clear() {
  std::fill_n(array(), size_, EMB_UNDEF);
}

void VectorKev::fill(KevesValue obj) {
  std::fill_n(array(), size_, obj);
}
