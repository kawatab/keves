// keves/kev/generator.hpp - generators for Keves
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
#include "kev/procedure.hpp"

class GeneratorKev : public MutableKev {
public:
  static constexpr kev_type TYPE = GENERATOR;

  GeneratorKev();
  GeneratorKev(const GeneratorKev&) = default;
  GeneratorKev(GeneratorKev&&) = default;
  GeneratorKev& operator=(const GeneratorKev&) = delete;
  GeneratorKev& operator=(GeneratorKev&&) = delete;
  ~GeneratorKev() = default;

  explicit GeneratorKev(LambdaKev*);

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  //////// Section For GC begin !!! //////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(GeneratorKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<GeneratorKev>::from(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T*, MutableKev* kev) {
    return FixedLengthKev<GeneratorKev>::from(kev)->border();
  }

  //////// Section For GC end !!! ////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  void copyFrom(const GeneratorKev&);
  void copyFrom(GeneratorKev&&);

  void set(LambdaKev* proc) {
    this->proc_ = proc;
  }

  LambdaKev* proc() const {
    return proc_;
  }

private:
  LambdaKev* proc_;
};
