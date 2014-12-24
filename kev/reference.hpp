// keves/kev/reference.hpp - references for Keves
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


class ReferenceKev: public MutableKev {
public:
  static constexpr kev_type TYPE = REFERENCE;

  constexpr ReferenceKev()
    : ReferenceKev(EMB_UNDEF) {}

  ReferenceKev(const ReferenceKev&) = default;
  ReferenceKev(ReferenceKev&&) = default;
  ReferenceKev& operator=(const ReferenceKev&) = delete;
  ReferenceKev& operator=(ReferenceKev&&) = delete;
  ~ReferenceKev() = default;
  
  explicit constexpr ReferenceKev(KevesValue kev)
    : MutableKev(TYPE), val_(kev) {}

  KevesValue val() const {
    return val_;
  }

  void set(KevesValue value) {
    val_ = value;
  }

  template<class ZONE>
  static ReferenceKev* make(ZONE* zone, KevesValue);

private:
  KevesValue val_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(ReferenceKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<ReferenceKev>::from(kev)->copyTo(zone);
  }
  
  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<ReferenceKev>* reference(FixedLengthKev<ReferenceKev>::from(kev));
    reference->val_ = zone->copy(reference->val_);
    return reference->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const ReferenceKev* ref(value);
    BASE::pushValue(pending, ref->val_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const ReferenceKev* ref(value);
    out << static_cast<uioword>(ref->type())
	<< BASE::indexAddress(list, ref->val_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword val;
    in >> val;
    return make(gc, KevesValue::template fromUioword<Kev>(val));
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue kev) {
    ReferenceKev* ref(kev);
    BASE::revertValue(object_list, &ref->val_);
  }
};
