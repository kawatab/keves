// keves/kev/wind.hpp - winds for Keves
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

  template<class ZONE>
  static WindKev* make(ZONE* zone, KevesValue before, KevesValue thunk,
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
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<WindKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<WindKev>* wind(FixedLengthKev<WindKev>::from(kev));
    wind->before_ = zone->copy(wind->before_);
    wind->thunk_ = zone->copy(wind->thunk_);
    wind->after_ = zone->copy(wind->after_);
    return wind->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const WindKev* wind(value);
    BASE::pushValue(pending, wind->before_);
    BASE::pushValue(pending, wind->thunk_);
    BASE::pushValue(pending, wind->after_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const WindKev* wind(value);

    out << static_cast<uioword>(wind->type())
	<< BASE::indexAddress(list, wind->before_)
	<< BASE::indexAddress(list, wind->thunk_)
	<< BASE::indexAddress(list, wind->after_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword before, thunk, after;
    in >> before >> thunk >> after;
    return make(gc,
		KevesValue::template fromUioword<Kev>(before),
		KevesValue::template fromUioword<Kev>(thunk),
		KevesValue::template fromUioword<Kev>(after));
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue kev) {
    WindKev* wind(kev);
    BASE::revertValue(object_list, &wind->before_);
    BASE::revertValue(object_list, &wind->thunk_);
    BASE::revertValue(object_list, &wind->after_);
  }
};
