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
#include "kev/pair.hpp"


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

  explicit EnvironmentKev(const KevesValue*);
  EnvironmentKev(KevesValue, const KevesValue*);

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(EnvironmentKev);
  }

  template<class T>
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<EnvironmentKev>::From(kev)->CopyTo(zone);
  }

  template<class T>
  static quintptr* CopyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<EnvironmentKev>* env(FixedLengthKev<EnvironmentKev>::From(kev));
    env->values_ = zone->Copy(env->values_);
    return env->border();
  }

  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  void append(const SymbolKev*, KevesValue, PairKev*, PairKev*);
  KevesValue at(int) const;
  void clear();
  void CopyFrom(const EnvironmentKev&);
  void CopyFrom(EnvironmentKev&&);
  KevesValue find(const SymbolKev*) const;
  KevesValue find(const char*) const;
  bool IsEmpty() const;
  void set_values(const PairKev*);
  int size() const;
  const KevesValue* table() const;
  KevesValue values() const;

  template<class ZONE>
  static EnvironmentKev* Make(ZONE* zone, KevesValue, const KevesValue*);

private:
  KevesValue values_;
  const KevesValue* table_;
};
