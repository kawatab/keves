// keves/kev/pair.hpp - pairs for Keves
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

class PairKev : public MutableKev {
public:
  static constexpr kev_type TYPE = PAIR;

  constexpr PairKev()
    : PairKev(EMB_UNDEF, EMB_UNDEF) {}

  PairKev(const PairKev&) = default;
  PairKev(PairKev&&) = default;
  PairKev& operator=(const PairKev&) = delete;
  PairKev& operator=(PairKev&&) = delete;
  ~PairKev() = default;

  constexpr PairKev(KevesValue car, KevesValue cdr)
    : MutableKev(TYPE), car_(car), cdr_(cdr) {}

  KevesValue car() const {
    return car_;
  }

  KevesValue cdr() const {
    return cdr_;
  }

  void copyFrom(const PairKev& org);
  void copyFrom(PairKev&& org);
  void set(KevesValue car, KevesValue cdr);

  void set_car(KevesValue value) {
    car_ = value;
  }

  void set_cdr(KevesValue value) {
    cdr_ = value;
  }

  template<class ZONE>
  static PairKev* make(ZONE* zone, KevesValue car, KevesValue cdr);

private:
  KevesValue car_;
  KevesValue cdr_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(PairKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<PairKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<PairKev>* pair(FixedLengthKev<PairKev>::from(kev));
    pair->car_ = zone->copy(pair->car_);
    pair->cdr_ = zone->copy(pair->cdr_);
    return pair->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const PairKev* pair(value);
    IO::pushValue(pending, pair->car_);
    IO::pushValue(pending, pair->cdr_);
  }

  template<class IO, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const PairKev* pair(value);

    out << static_cast<uioword>(pair->type())
	<< IO::indexAddress(list, pair->car_)
	<< IO::indexAddress(list, pair->cdr_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword car, cdr;
    in >> car >> cdr;
    return make(gc,
		KevesValue::template fromUioword<Kev>(car),
		KevesValue::template fromUioword<Kev>(cdr));
  }
  
  template<class IO, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    PairKev* pair(value);
    IO::revertValue(object_list, &pair->car_);
    IO::revertValue(object_list, &pair->cdr_);
  }
};
