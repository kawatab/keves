// Keves/pair_kev.hpp - pairs for Keves
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

#include "value/value.hpp"


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

  void CopyFrom(const PairKev& org);
  void CopyFrom(PairKev&& org);
  void Set(KevesValue car, KevesValue cdr);

  void set_car(KevesValue value) {
    car_ = value;
  }

  void set_cdr(KevesValue value) {
    cdr_ = value;
  }

  static PairKev* Make(KevesGC* gc, KevesValue car, KevesValue cdr);

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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<PairKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<PairKev>* pair(FixedLengthKev<PairKev>::From(kev));
    pair->car_ = zone->Copy(pair->car_);
    pair->cdr_ = zone->Copy(pair->cdr_);
    return pair->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const PairKev* pair(value);
    IO::PushValue(pending, pair->car_);
    IO::PushValue(pending, pair->cdr_);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const PairKev* pair(value);

    out << static_cast<uioword>(pair->type())
	<< IO::IndexAddress(list, pair->car_)
	<< IO::IndexAddress(list, pair->cdr_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword car, cdr;
    in >> car >> cdr;
    return Make(gc,
		KevesValue::template FromUioword<Kev>(car),
		KevesValue::template FromUioword<Kev>(cdr));
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    PairKev* pair(value);
    IO::RevertValue(object_list, &pair->car_);
    IO::RevertValue(object_list, &pair->cdr_);
  }
};
