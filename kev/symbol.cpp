// keves/kev/symbol.cpp - symbols for Keves
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


#include "kev/symbol.hpp"


SymbolKev::SymbolKev()
  : SymbolKev(KevesString()) {}

SymbolKev::SymbolKev(KevesString str)
  : MutableKev(TYPE), KevesString(str) {}

SymbolKev::SymbolKev(const StringKev& str)
  : SymbolKev(str.getKevesString()) {}

SymbolKev::SymbolKev(StringKev&& str)
  : SymbolKev(str.getKevesString()) {}

SymbolKev::SymbolKev(StringCoreKev* core, int idx, int len)
  : SymbolKev(KevesString(core, idx, len)) {}

void SymbolKev::copyFrom(const SymbolKev& org) {
  this->KevesString::operator=(org.getKevesString());
}

void SymbolKev::copyFrom(SymbolKev&& org) {
  this->KevesString::operator=(org.getKevesString());
}

void SymbolKev::set(StringCoreKev* core, int idx, int len) {
  this->KevesString::set(core, idx, len);
}

StringKev SymbolKev::toStringKev() const {
  return StringKev(this->getKevesString());
}

KevesString SymbolKev::getKevesString() const {
  return static_cast<KevesString>(*this);
}
