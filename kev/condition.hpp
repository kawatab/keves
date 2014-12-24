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

#include "keves_value.hpp"
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
  
  void copyFrom(const SimpleConditionKev&);
  void copyFrom(SimpleConditionKev&&);

  const RecordKev* field() const {
    return record_type_;
  }

  QString getFieldName() const;

  KevesValue value() const {
    return value_;
  }
  
  template<class ZONE>
  static SimpleConditionKev* make(ZONE* zone, const RecordKev*, KevesValue);

protected:
  const RecordKev* record_type_;

private:
  KevesValue value_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(SimpleConditionKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<SimpleConditionKev>::from(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<SimpleConditionKev>* condition(FixedLengthKev<SimpleConditionKev>::from(kev));
    condition->value_ = zone->copy(condition->value_);
    return condition->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const SimpleConditionKev* condition(value);
    IO::pushValue(pending, condition->record_type_);
    IO::pushValue(pending, condition->value_);
  }

  template<class IO, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const SimpleConditionKev* condition(value);

    out << static_cast<uioword>(condition->type())
	<< IO::indexAddress(list, condition->record_type_)
	<< IO::indexAddress(list, condition->value_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword record_type, value;
    in >> record_type >> value;
    return make(gc,
		KevesValue::template fromUioword<RecordKev>(record_type),
		KevesValue::template fromUioword<Kev>(value));
  }
  
  template<class IO, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    SimpleConditionKev* condition(value);
    IO::revertValue(object_list, &condition->record_type_);
    IO::revertValue(object_list, &condition->value_);
  }
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

  void copyFrom(const CompoundConditionKev&);
  void copyFrom(CompoundConditionKev&&);

  const VectorKev* values() const {
    return values_;
  }

  template<class ZONE>
  static CompoundConditionKev* make(ZONE* zone, VectorKev*);

protected:
  const VectorKev* values_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(CompoundConditionKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<CompoundConditionKev>::from(kev)->copyTo(zone);
  }
  
  template<class T>
  static quintptr* copyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<CompoundConditionKev>* condition(FixedLengthKev<CompoundConditionKev>::from(kev));
    VariableLengthKev<VectorKev>* values(VariableLengthKev<VectorKev>::from(condition->values_));
    condition->values_ = zone->copy(values);
    return condition->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const CompoundConditionKev* condition(value);
    IO::pushValue(pending, condition->values_);
  }

  template<class IO, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const CompoundConditionKev* condition(value);

    out << static_cast<uioword>(condition->type())
	<< IO::indexAddress(list, condition->values_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword values;
    in >> values;
    return make(gc, KevesValue::template fromUioword<VectorKev>(values));
  }
  
  template<class IO, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    CompoundConditionKev* condition(value);
    IO::revertValue(object_list, &condition->values_);
  }
};
