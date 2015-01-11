// keves/kev/environment.hpp - environments for Keves
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


class PairKev;
class SymbolKev;
class KevesGC;

class EnvironmentKev : public MutableKev {
public:
  static constexpr kev_type TYPE = ENVIRONMENT;

  EnvironmentKev();
  EnvironmentKev(const EnvironmentKev&) = default;
  EnvironmentKev(EnvironmentKev&&) = default;
  EnvironmentKev& operator=(const EnvironmentKev&) = delete;
  EnvironmentKev& operator=(EnvironmentKev&&) = delete;
  ~EnvironmentKev() = default;

  explicit EnvironmentKev(KevesValue value);

  KevesValue values() const {
    return values_;
  }

  void set_values(KevesValue list) {
    values_ = list;
  }

  void append(const SymbolKev* id,
	      KevesValue val,
	      PairKev* pair1,
	      PairKev* pair2);

  void clear();
  void copyFrom(const EnvironmentKev& values);
  KevesValue find(const SymbolKev* id) const;
  KevesValue find(const char* id) const;
  bool isEmpty() const;
  int size() const;

  template<class ZONE>
  static EnvironmentKev* make(ZONE* zone, KevesValue values);

private:
  KevesValue values_;


  ////////////////////////////////////////////////////////////
  // Secrion For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(EnvironmentKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<EnvironmentKev>::from(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<EnvironmentKev>* env(FixedLengthKev<EnvironmentKev>::from(kev));
    env->values_ = zone->copy(env->values_);
    return env->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const EnvironmentKev* environment(value);
    IO::pushValue(pending, environment->values_);
  }

  template<class IO, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const EnvironmentKev* environment(value);

    out << static_cast<uioword>(environment->type())
	<< IO::indexAddress(list, environment->values_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword value;
    in >> value;
    return make(gc, KevesValue::template fromUioword<Kev>(value));
  }
  
  template<class IO, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    EnvironmentKev* environment(value);
    IO::revertValue(object_list, &environment->values_);
  }
};
