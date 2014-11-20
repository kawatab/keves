/* Keves/environment_kev.cpp - environments for Keves
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


#include "environment_kev.hpp"

#include "pair_kev.hpp"
#include "keves_gc.hpp"
#include "symbol_kev.hpp"


EnvironmentKev::EnvironmentKev() : EnvironmentKev(nullptr) {
}

EnvironmentKev::EnvironmentKev(const KevesValue* table)
  : EnvironmentKev(EMB_NULL, table) {
}

EnvironmentKev::EnvironmentKev(KevesValue values, const KevesValue* table)
  : MutableKev(TYPE), values_(values), table_(table) {
}

void EnvironmentKev::CopyFrom(const EnvironmentKev& other) {
  this->values_ = other.values_;
  this->table_ = other.table_;
}

void EnvironmentKev::CopyFrom(EnvironmentKev&& other) {
  this->values_ = other.values_;
  this->table_ = other.table_;
}

/*
void EnvironmentKev::append(KevesIterator* iter, const char* id, KevesValue val) {
  SymbolKev* symbol(SymbolKev::make(iter, id));
  PairKev* bind(PairKev::make(iter, symbol, val));
  values_ = PairKev::make(iter, bind, values_);
}

void EnvironmentKev::append(KevesIterator* iter, const SymbolKev* symbol, KevesValue val) {
  PairKev* bind(PairKev::make(iter, symbol, val));
  values_ = PairKev::make(iter, bind, values_);
}
*/

void EnvironmentKev::append(const SymbolKev* id, KevesValue val, PairKev* pair1, PairKev* pair2) {
  pair1->Set(pair2, values_);
  pair2->Set(id, val);
  values_ = pair1;
}

void EnvironmentKev::set_values(const PairKev* list) {
  values_ = list;
}

void EnvironmentKev::clear() {
  values_ = EMB_NULL;
}

KevesValue EnvironmentKev::at(int idx) const {
  return table_[idx];
}

KevesValue EnvironmentKev::find(const SymbolKev* symbol) const {
  KevesValue list(values_);
  while (list != EMB_NULL) {
    const PairKev* temp(list.ToPtr<PairKev>()->car());
    const SymbolKev* name(temp->car());

    if (name->Equals(*symbol))
      return temp->cdr();
    
    list = list.ToPtr<PairKev>()->cdr();
  }
  return EMB_FALSE;
}

KevesValue EnvironmentKev::find(const char* symbol) const {
  KevesValue list(values_);

  while (list != EMB_NULL) {
    const PairKev* temp(list.ToPtr<PairKev>()->car());
    const SymbolKev* name(temp->car());

    if (name->Equals(symbol))
      return temp->cdr();

    list = list.ToPtr<PairKev>()->cdr();
  }

  return EMB_FALSE;
}

bool EnvironmentKev::IsEmpty() const {
  return values_ == EMB_NULL;
}

int EnvironmentKev::size() const {
  KevesValue list(values_);
  int count(0);

  while (list != EMB_NULL) {
    list = list.ToPtr<PairKev>()->cdr();
    ++count;
  }

  return count;
}

const KevesValue* EnvironmentKev::table() const {
  return table_;
}

KevesValue EnvironmentKev::values() const {
  return values_;
}

EnvironmentKev* EnvironmentKev::make(KevesGC* gc, KevesValue values, const KevesValue* table) {
  auto ctor = [values, table](void* ptr) {
    return new(ptr) EnvironmentKev(values, table);
  };

  return gc->Make(ctor, alloc_size(nullptr));
}
