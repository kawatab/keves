// keves/kev/jump.hpp - jumps for Keves
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

#include "keves_iterator.hpp"
#include "keves_value.hpp"


// class JumpKev ----------------------------------------
class JumpKev : public MutableKev {
public:
  static constexpr kev_type TYPE = JUMP;

  JumpKev();
  JumpKev(const JumpKev&) = default;
  JumpKev(JumpKev&&) = default;
  JumpKev& operator=(const JumpKev&) = delete;
  JumpKev& operator=(JumpKev&&) = delete;
  ~JumpKev() = default;

  explicit JumpKev(KevesIterator dest);
  
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(JumpKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<JumpKev>::from(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T*, MutableKev* kev) {
    return FixedLengthKev<JumpKev>::from(kev)->border();
  }

  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  const KevesIterator destination() const {
    return destination_;
  }

  void set(KevesIterator destination) {
    this->destination_ = destination;
  }

  template<class ZONE>
  static JumpKev* make(ZONE* zone);

protected:
  KevesIterator destination_;
};
    
// class DestinationKev ----------------------------------------
class DestinationKev : public MutableKev {
public:
  static constexpr kev_type TYPE = DESTINATION;

  DestinationKev() = delete;
  DestinationKev(const DestinationKev&) = default;
  DestinationKev(DestinationKev&&) = default;
  DestinationKev& operator=(const DestinationKev&) = delete;
  DestinationKev& operator=(DestinationKev&&) = delete;
  ~DestinationKev() = default;

  explicit DestinationKev(const JumpKev* kev);
  
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(DestinationKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<DestinationKev>::from(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<DestinationKev>* dest(FixedLengthKev<DestinationKev>::from(kev));
    FixedLengthKev<JumpKev>* label(FixedLengthKev<JumpKev>::from(dest->label_));
    dest->label_ = zone->copy(label);
    return dest->border();
  }
  
  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  const JumpKev* label() const {
    return label_;
  }
  
protected:
  const JumpKev* label_;
};

