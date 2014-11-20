/* Keves/condition_kev.cpp - conditions for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 *  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "condition_kev.hpp"

#include "keves_gc.hpp"
#include "record_kev.hpp"

// class SimpleConditionKev ----------------------------------------
SimpleConditionKev::SimpleConditionKev()
  : SimpleConditionKev(nullptr, EMB_UNDEF) {
}

SimpleConditionKev::SimpleConditionKev(const RecordKev* rec_type, KevesValue b)
  : MutableKev(TYPE), record_type_(rec_type), value_(b) {
}

void SimpleConditionKev::CopyFrom(const SimpleConditionKev& other) {
  this->value_ = other.value_;
}

void SimpleConditionKev::CopyFrom(SimpleConditionKev&& other) {
  this->value_ = other.value_;
}

const QString& SimpleConditionKev::getFieldName() const {
  return record_type_->name();
}

SimpleConditionKev* SimpleConditionKev::make(KevesGC* gc, const RecordKev* field, KevesValue value) {
  auto ctor = [field, value](void* ptr) {
    return new(ptr) SimpleConditionKev(field, value);
  };

  return gc->Make(ctor, alloc_size(nullptr));
}


// class CompoundConditionKev ----------------------------------------
CompoundConditionKev::CompoundConditionKev()
  : CompoundConditionKev(nullptr) {
}

CompoundConditionKev::CompoundConditionKev(const VectorKev* values)
  : MutableKev(TYPE), values_(values) {
}

void CompoundConditionKev::CopyFrom(const CompoundConditionKev& other) {
  this->values_ = other.values_;
}

void CompoundConditionKev::CopyFrom(CompoundConditionKev&& other) {
  this->values_ = other.values_;
}

CompoundConditionKev* CompoundConditionKev::make(KevesGC* gc, VectorKev* values) {
  auto ctor = [values](void* ptr) {
    return new(ptr) CompoundConditionKev(values);
  };

  return gc->Make(ctor, alloc_size(nullptr));
}
