// keves/kev/condition-inl.hpp - conditions for Keves
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


template<class ZONE>
SimpleConditionKev* SimpleConditionKev::make(ZONE* zone, const RecordKev* field, KevesValue value) {
  auto ctor = [field, value](void* ptr) {
    return new(ptr) SimpleConditionKev(field, value);
  };

  return zone->make(ctor, alloc_size(nullptr));
}


template<class ZONE>
CompoundConditionKev* CompoundConditionKev::make(ZONE* zone, VectorKev* values) {
  auto ctor = [values](void* ptr) {
    return new(ptr) CompoundConditionKev(values);
  };

  return zone->make(ctor, alloc_size(nullptr));
}
