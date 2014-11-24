// Keves/wind_kev.hpp - winds for Keves
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

#include "keves_value.hpp"


class KevesGC;

class WindKev : public MutableKev {
public:
  static constexpr kev_type TYPE = WIND;

  WindKev() = delete;
  WindKev(const WindKev&) = default;
  WindKev(WindKev&&) = default;
  WindKev& operator=(const WindKev&) = delete;
  WindKev& operator=(WindKev&&) = delete;
  ~WindKev() = default;

  WindKev(KevesValue before, KevesValue thunk, KevesValue after);
  
  KevesValue before() const {
    return before_;
  }

  KevesValue thunk() const {
    return thunk_;
  } 

  KevesValue after() const {
    return after_;
  }

  static WindKev* make(KevesGC* gc, KevesValue before, KevesValue thunk,
		       KevesValue after);

private:
  KevesValue before_;
  KevesValue thunk_;
  KevesValue after_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev* /*kev*/) {
    return sizeof(WindKev);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<WindKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<WindKev>* wind(FixedLengthKev<WindKev>::From(kev));
    wind->before_ = zone->Copy(wind->before_);
    wind->thunk_ = zone->Copy(wind->thunk_);
    wind->after_ = zone->Copy(wind->after_);
    return wind->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const WindKev* wind(value);
    BASE::PushValue(pending, wind->before_);
    BASE::PushValue(pending, wind->thunk_);
    BASE::PushValue(pending, wind->after_);
  }

  template<class BASE, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const WindKev* wind(value);

    out << static_cast<uioword>(wind->type())
	<< BASE::IndexAddress(list, wind->before_)
	<< BASE::IndexAddress(list, wind->thunk_)
	<< BASE::IndexAddress(list, wind->after_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword before, thunk, after;
    in >> before >> thunk >> after;
    return make(gc,
		KevesValue::template FromUioword<Kev>(before),
		KevesValue::template FromUioword<Kev>(thunk),
		KevesValue::template FromUioword<Kev>(after));
  }
  
  template<class BASE, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue kev) {
    WindKev* wind(kev);
    BASE::RevertValue(object_list, &wind->before_);
    BASE::RevertValue(object_list, &wind->thunk_);
    BASE::RevertValue(object_list, &wind->after_);
  }
};
