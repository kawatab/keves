// keves/kev/symbol.hpp - symbols for Keves
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

#include "kev/string.hpp"


class SymbolKev : public MutableKev, public KevesString {
public:
  static constexpr kev_type TYPE = SYMBOL;

  SymbolKev();
  SymbolKev(const SymbolKev&) = default;
  SymbolKev(SymbolKev&&) = default;
  SymbolKev& operator=(const SymbolKev&) = delete;
  SymbolKev& operator=(SymbolKev&&) = delete;
  ~SymbolKev() = default;

  explicit SymbolKev(KevesString);
  explicit SymbolKev(const StringKev&);
  explicit SymbolKev(StringKev&&);
  SymbolKev(KevesIterator*, const QString&);

  void copyFrom(const SymbolKev& org);
  void copyFrom(SymbolKev&& org);

  KevesString getKevesString() const;
  void set(KevesIterator*, const QString&);
  void set(StringCoreKev*, int, int);
  StringKev toStringKev() const;

  template<class ZONE>
  static SymbolKev* make(ZONE* zone, const QString&);

  template<class ZONE>
  static SymbolKev* make(ZONE* zone, StringCoreKev*, int, int);

  template<class ZONE>
  static std::function<SymbolKev*(void*)> ctor(ZONE* zone, const QString& str) {
    return [zone, &str](void* ptr) {
      StringCoreKev* core(StringCoreKev::make(zone, str));
      return new(ptr) SymbolKev(core, 0, core->size());
    };
  }

private:
  SymbolKev(StringCoreKev*, int, int);


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(SymbolKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<SymbolKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<SymbolKev>* symbol(FixedLengthKev<SymbolKev>::from(kev));
    VariableLengthKev<StringCoreKev>* core(VariableLengthKev<StringCoreKev>::from(symbol->core_));
    symbol->core_ = zone->copy(core);
    return symbol->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const SymbolKev* symbol(value);
    BASE::pushValue(pending, symbol->core_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const SymbolKev* symbol(value);

    out << static_cast<uioword>(symbol->type())
	<< BASE::indexAddress(list, symbol->core_)
	<< static_cast<ioword>(symbol->idx_)
	<< static_cast<ioword>(symbol->len_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword core;
    ioword idx, len;
    in >> core >> idx >> len;
    return make(gc,
		KevesValue::template fromUioword<StringCoreKev>(core),
		idx,
		len);
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue kev) {
    SymbolKev* symbol(kev);
    BASE::revertValue(object_list, &symbol->core_);
  }
};
