// keves/kev/condition.hpp - conditions for Keves
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
#include "kev/vector.hpp"


class RecordKev;


class SimpleConditionKev : public MutableKev {
public:
  static constexpr kev_type TYPE = CONDITION_SMP;

  SimpleConditionKev();
  SimpleConditionKev(const SimpleConditionKev&) = default;
  SimpleConditionKev(SimpleConditionKev&&) = default;
  SimpleConditionKev& operator=(const SimpleConditionKev&) = delete;
  SimpleConditionKev& operator=(SimpleConditionKev&&) = delete;
  ~SimpleConditionKev() = default;

  explicit SimpleConditionKev(const RecordKev*, KevesValue);
  
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(SimpleConditionKev);
  }

  template<class T>
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<SimpleConditionKev>::From(kev)->CopyTo(zone);
  }

  template<class T>
  static quintptr* CopyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<SimpleConditionKev>* condition(FixedLengthKev<SimpleConditionKev>::From(kev));
    condition->value_ = zone->Copy(condition->value_);
    return condition->border();
  }

  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  void CopyFrom(const SimpleConditionKev&);
  void CopyFrom(SimpleConditionKev&&);

  const RecordKev* field() const {
    return record_type_;
  }

  const QString& getFieldName() const;

  KevesValue value() const {
    return value_;
  }
  
  template<class ZONE>
  static SimpleConditionKev* Make(ZONE* zone, const RecordKev*, KevesValue);

protected:
  const RecordKev* record_type_;

private:
  KevesValue value_;
};


class CompoundConditionKev : public MutableKev {
public:
  static constexpr kev_type TYPE = CONDITION_CMP;

  CompoundConditionKev();
  CompoundConditionKev(const CompoundConditionKev&) = default;
  CompoundConditionKev(CompoundConditionKev&&) = default;
  CompoundConditionKev& operator=(const CompoundConditionKev&) = delete;
  CompoundConditionKev& operator=(CompoundConditionKev&&) = delete;
  ~CompoundConditionKev() = default;

  CompoundConditionKev(const VectorKev*);

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(CompoundConditionKev);
  }

  template<class T>
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<CompoundConditionKev>::From(kev)->CopyTo(zone);
  }
  
  template<class T>
  static quintptr* CopyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<CompoundConditionKev>* condition(FixedLengthKev<CompoundConditionKev>::From(kev));
    VariableLengthKev<VectorKev>* values(VariableLengthKev<VectorKev>::From(condition->values_));
    condition->values_ = zone->Copy(values);
    return condition->border();
  }

  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  void CopyFrom(const CompoundConditionKev&);
  void CopyFrom(CompoundConditionKev&&);

  const VectorKev* values() const {
    return values_;
  }

  template<class ZONE>
  static CompoundConditionKev* Make(ZONE* zone, VectorKev*);

protected:
  const VectorKev* values_;
};
