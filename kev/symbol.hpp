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

  void CopyFrom(const SymbolKev& org);
  void CopyFrom(SymbolKev&& org);

  KevesString GetKevesString() const;
  void Set(KevesIterator*, const QString&);
  void Set(StringCoreKev*, int, int);
  StringKev ToStringKev() const;

  template<class ZONE>
  static SymbolKev* Make(ZONE* zone, const QString&);

  template<class ZONE>
  static SymbolKev* Make(ZONE* zone, StringCoreKev*, int, int);

  template<class ZONE>
  static std::function<SymbolKev*(void*)> ctor(ZONE* zone, const QString& str) {
    return [zone, &str](void* ptr) {
      StringCoreKev* core(StringCoreKev::Make(zone, str));
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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<SymbolKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<SymbolKev>* symbol(FixedLengthKev<SymbolKev>::From(kev));
    VariableLengthKev<StringCoreKev>* core(VariableLengthKev<StringCoreKev>::From(symbol->core_));
    symbol->core_ = zone->Copy(core);
    return symbol->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const SymbolKev* symbol(value);
    BASE::PushValue(pending, symbol->core_);
  }

  template<class BASE, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const SymbolKev* symbol(value);

    out << static_cast<uioword>(symbol->type())
	<< BASE::IndexAddress(list, symbol->core_)
	<< static_cast<ioword>(symbol->idx_)
	<< static_cast<ioword>(symbol->len_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword core;
    ioword idx, len;
    in >> core >> idx >> len;
    return Make(gc,
		KevesValue::template FromUioword<StringCoreKev>(core),
		idx,
		len);
  }
  
  template<class BASE, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue kev) {
    SymbolKev* symbol(kev);
    BASE::RevertValue(object_list, &symbol->core_);
  }
};
