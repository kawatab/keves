// keves/kev/procedure.hpp - procedures for Keves
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
#include "kev/code.hpp"
#include "kev/frame.hpp"


class KevesVM;

class CPSKev : public MutableKev {
public:
  static constexpr kev_type TYPE = CPS;

  CPSKev();
  CPSKev(const CPSKev&) = default;
  CPSKev(CPSKev&&) = default;
  CPSKev& operator=(const CPSKev&) = delete;
  CPSKev& operator=(CPSKev&&) = delete;
  ~CPSKev() = default;

  explicit CPSKev(vm_func);
  CPSKev(vm_func, KevesValue);
  
  vm_func func() const {
    return func_;
  }

  void set(vm_func func) {
    this->func_ = func;
  }

  void set(vm_func func, KevesValue who) {
    this->func_ = func;
    this->who_ = who;
  }

  KevesValue who() const {
    return who_;
  }

private:
  vm_func func_;
  KevesValue who_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(CPSKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<CPSKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<CPSKev>* cps(FixedLengthKev<CPSKev>::from(kev));
    cps->who_ = zone->copy(cps->who_);
    return cps->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

  template<class /*BASE*/, class STACK>
  static void pushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    std::cerr << "Cannot write ContinuationKev\n";
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void writeObject(const LIST& /*list*/,
			  STREAM& /*out*/,
			  KevesValue /*value*/) {
    std::cerr << "Cannot write ContinuationKev\n";
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& /*in*/, GC* /*gc*/) {
    std::cerr << "Cannot read ContinuationKev\n";
    return nullptr;
  }
  
  template<class /*BASE*/, class LIST>
  static void revertObject(const LIST& /*object_list*/,
			   MutableKevesValue /*kev*/) {
    std::cerr << "Cannot read ContinuationKev\n";
  }
};


class LambdaKev : public MutableKev {
public:
  static constexpr kev_type TYPE = LAMBDA;

  LambdaKev();
  LambdaKev(const LambdaKev&) = default;
  LambdaKev(LambdaKev&&) = default;
  LambdaKev& operator=(const LambdaKev&) = delete;
  LambdaKev& operator=(LambdaKev&&) = delete;
  ~LambdaKev() = default;

  LambdaKev(CodeKev* code, int index);
  LambdaKev(const LocalVarFrameKev* free_vars, CodeKev* code, int index);

  LambdaKev(const LocalVarFrameKev* free_vars,
	    CodeKev* code,
	    const_KevesIterator pc);

  const LocalVarFrameKev* free_vars() const {
    return free_vars_;
  }

  const CodeKev* code() const {
    return code_;
  }
  
  int index() const {
    return index_;
  }
  
  const_KevesIterator proc() const {
    return code_->begin() + index_;
  }
  
  void set(LocalVarFrameKev* free_vars, CodeKev* code, int index) {
    this->free_vars_ = free_vars;
    this->code_ = code;
    this->index_ = index;
  }
  
  template<class ZONE>
  static LambdaKev* make(ZONE* zone, LocalVarFrameKev* free_vars, CodeKev* code, int index);
  
private:
  const LocalVarFrameKev* free_vars_;
  CodeKev* code_;
  int index_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(LambdaKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<LambdaKev>::from(kev)->copyTo(zone);
  }
  
  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<LambdaKev>* lambda(FixedLengthKev<LambdaKev>::from(kev));
    VariableLengthKev<LocalVarFrameKev>* free_vars(VariableLengthKev<LocalVarFrameKev>::from(lambda->free_vars_));
    lambda->free_vars_ = zone->copy(free_vars);
    VariableLengthKev<CodeKev>* code(VariableLengthKev<CodeKev>::from(lambda->code_));
    lambda->code_ = zone->copy(code);
    return lambda->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const LambdaKev* lambda(value);
    BASE::pushValue(pending, lambda->free_vars_);
    BASE::pushValue(pending, lambda->code_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const LambdaKev* lambda(value);

    out << static_cast<uioword>(lambda->type())
	<< BASE::indexAddress(list, lambda->free_vars_)
	<< BASE::indexAddress(list, lambda->code_)
	<< static_cast<ioword>(lambda->index_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword free_vars, code;
    ioword index;
    in >> free_vars >> code >> index;
    return make(gc,
		KevesValue::template fromUioword<LocalVarFrameKev>(free_vars),
		KevesValue::template fromUioword<CodeKev>(code),
		index);
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue kev) {
    LambdaKev* lambda(kev);
    BASE::revertValue(object_list, &lambda->free_vars_);
    BASE::revertValue(object_list, &lambda->code_);
  }
};


class ContinuationKev : public MutableKev {
public:
  static constexpr kev_type TYPE = CONTINUATION;

  ContinuationKev() = delete;
  ContinuationKev(const ContinuationKev&) = default;
  ContinuationKev(ContinuationKev&&) = default;
  ContinuationKev& operator=(const ContinuationKev&) = delete;
  ContinuationKev& operator=(ContinuationKev&&) = delete;
  ~ContinuationKev() = default;

  explicit ContinuationKev(StackFrameKev registers);

  const StackFrameKev* btmp() const;

  StackFrameKev* ptr_registers() {
    return &registers_;
  } 

  StackFrameKev registers() const {
    return registers_;
  }

private:
  StackFrameKev registers_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(ContinuationKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<ContinuationKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<ContinuationKev>* conti(FixedLengthKev<ContinuationKev>::from(kev));
    StackFrameKev::copyContents(zone, conti->ptr_registers());
    return conti->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

  template<class /*BASE*/, class STACK>
  static void pushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    std::cerr << "Cannot write ContinuationKev\n";
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void writeObject(const LIST& /*list*/,
			  STREAM& /*out*/,
			  KevesValue /*value*/) {
    std::cerr << "Cannot write ContinuationKev\n";
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& /*in*/, GC* /*gc*/) {
    std::cerr << "Cannot read ContinuationKev\n";
    return nullptr;
  }
  
  template<class /*BASE*/, class LIST>
  static void revertObject(const LIST& /*object_list*/,
			   MutableKevesValue /*kev*/) {
    std::cerr << "Cannot read ContinuationKev\n";
  }
};
