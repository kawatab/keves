// Keves/jump_kev.hpp - jumps for Keves
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

#include "keves_gc.hpp"
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
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<JumpKev>::From(kev)->CopyTo(zone);
  }

  template<class T>
  static quintptr* CopyContents(T*, MutableKev* kev) {
    return FixedLengthKev<JumpKev>::From(kev)->border();
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

  static JumpKev* make(KevesGC* gc);

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
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<DestinationKev>::From(kev)->CopyTo(zone);
  }

  template<class T>
  static quintptr* CopyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<DestinationKev>* dest(FixedLengthKev<DestinationKev>::From(kev));
    FixedLengthKev<JumpKev>* label(FixedLengthKev<JumpKev>::From(dest->label_));
    dest->label_ = zone->Copy(label);
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

