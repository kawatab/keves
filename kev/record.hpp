// keves/kev/record.hpp - records for Keves
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

#include <iostream>
#include <QString>
#include "keves_value.hpp"


class RecordKev : public MutableKev {
public:
  static constexpr kev_type TYPE = RECORD;

  RecordKev() = delete;
  RecordKev(const RecordKev&) = default;
  RecordKev(RecordKev&&) = default;
  RecordKev& operator=(const RecordKev&) = delete;
  RecordKev& operator=(RecordKev&&) = delete;
  ~RecordKev() = default;

  explicit RecordKev(const char* name);

  const char* name() const {
    return name_;
  }

  template<class ZONE>
  const RecordKev* Make(ZONE zone, const char* name);

private:
  const char* name_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(RecordKev);
  }

  template<class T>
  static MutableKev* CopyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<RecordKev>::From(kev)->CopyTo(zone);
  }

  template<class T>
  static quintptr* CopyContents(T*, MutableKev* kev) {
    return FixedLengthKev<RecordKev>::From(kev)->border();
  }


  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    std::cerr << "RecordKev cannot be serialize!!!\n";
    Q_ASSERT(0);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& /*list*/, STREAM& /*out*/, KevesValue /*value*/) {
    std::cerr << "RecordKev cannot be serialize!!!\n";
    Q_ASSERT(0);
  }

  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& /*in*/, GC* /*gc*/) {
    std::cerr << "RecordKev cannot be serialize!!!\n";
    Q_ASSERT(0);
    return nullptr;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& /*object_list*/, MutableKevesValue /*value*/) {
    std::cerr << "RecordKev cannot be serialize!!!\n";
    Q_ASSERT(0);
  }
};
