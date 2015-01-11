// keves/kev/environment.cpp - environments for Keves
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


#include "kev/environment.hpp"

#include "kev/pair.hpp"
#include "kev/symbol.hpp"


EnvironmentKev::EnvironmentKev()
  : EnvironmentKev(EMB_NULL) {}

EnvironmentKev::EnvironmentKev(KevesValue values)
  : MutableKev(TYPE), values_(values) {}

void EnvironmentKev::copyFrom(const EnvironmentKev& other) {
  this->values_ = other.values_;
}

void EnvironmentKev::append(const SymbolKev* id,
			    KevesValue val,
			    PairKev* pair1,
			    PairKev* pair2) {
  pair1->set(pair2, values_);
  pair2->set(id, val);
  values_ = pair1;
}

void EnvironmentKev::clear() {
  values_ = EMB_NULL;
}

KevesValue EnvironmentKev::find(const SymbolKev* id) const {
  KevesValue list(values_);

  while (list != EMB_NULL) {
    const PairKev* temp(list.toPtr<PairKev>()->car());
    const SymbolKev* name(temp->car());

    if (name->equals(*id))
      return temp->cdr();
    
    list = list.toPtr<PairKev>()->cdr();
  }

  return EMB_FALSE;
}

KevesValue EnvironmentKev::find(const char* id) const {
  KevesValue list(values_);

  while (list != EMB_NULL) {
    const PairKev* temp(list.toPtr<PairKev>()->car());
    const SymbolKev* name(temp->car());

    if (name->equals(id))
      return temp->cdr();

    list = list.toPtr<PairKev>()->cdr();
  }

  return EMB_FALSE;
}

bool EnvironmentKev::isEmpty() const {
  return values_ == EMB_NULL;
}

int EnvironmentKev::size() const {
  KevesValue list(values_);
  int count(0);

  while (list != EMB_NULL) {
    list = list.toPtr<PairKev>()->cdr();
    ++count;
  }

  return count;
}
