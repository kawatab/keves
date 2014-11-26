// keves/kev/frame.cpp - frames for Keves
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


#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"

#include <iostream>
#include "keves_gc.hpp"
#include "value/fixnum.hpp"


// class ArgumentFrameKev ----------------------------------------
void ArgumentFrameKev::CopyArray(const ArgumentFrameKev* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + size_);
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}
  
void ArgumentFrameKev::append(int* num_this, const ArgumentFrameKev* other, int num_other) {
  const KevesValue* other_iter(other->array());
  std::copy(other_iter, other_iter + num_other, this->array() + *num_this);
  *num_this += num_other;
}

ArgumentFrameKev* ArgumentFrameKev::extend(KevesGC* gc) const {
  const int new_size(size() * 2);

  auto ctor = [this, new_size](void* ptr) {
    ArgumentFrameKev* new_frame(new(ptr) ArgumentFrameKev(new_size));
    
    const KevesValue* org_iter(this->array());
    KevesValue* new_iter(new_frame->array());
    
    std::copy(org_iter, org_iter + this->size(), new_iter);
    std::fill_n(new_iter + this->size(), new_size - this->size(), EMB_UNDEF);
    
    return new_frame;
  };

  return gc->Make(ctor, alloc_size(new_size));
}

void ArgumentFrameKev::CopyTo(ArgumentFrameKev* other, int size) const {
  const KevesValue* iter(this->array());
  std::copy(iter, iter + size, other->array());
}


// class LocalVarFrameKev ----------------------------------------
void LocalVarFrameKev::CopyArray(const LocalVarFrameKev* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + size_);
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}

void LocalVarFrameKev::CopyTo(LocalVarFrameKev* other, int num) const {
  Q_ASSERT(type() == TYPE);

  const KevesValue* iter(this->array());
  std::copy(iter, iter + num, other->array());
}


// class FreeVarFrameKev ----------------------------------------
void FreeVarFrameKev::CopyArray(const FreeVarFrameKev* org) {
  next_ = org->next_;

  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + size_);
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}

void FreeVarFrameKev::clear() {
  KevesValue* iter(array());
  KevesValue* const iter_end(iter + size_);
  while (iter != iter_end) *iter++ = nullptr;
}

FreeVarFrameKev* FreeVarFrameKev::prepend(KevesGC* gc, int addition) {
  auto ctor = [addition, this](void* ptr) {
    FreeVarFrameKev* closure(new(ptr) FreeVarFrameKev(addition, this));
    KevesValue* top_array(closure->array());
    std::fill_n(top_array, addition, EMB_UNDEF);
    return closure;
  };
  
  return gc->Make(ctor, alloc_size(this->size() + addition));
}


// class StackFrameKev ----------------------------------------
void StackFrameKev::setArgumentsToLocalVars() {
  for (int i(1); i < argn_; ++i) envp_->array()[i - 1] = argp_->at(i);
}

void StackFrameKev::extendArgFrame(KevesGC* gc) {
  argp_ = argp_->extend(gc);
}

void StackFrameKev::extendEnvFrame(LocalVarFrameKev* frame) {
  if (envn_ > 0) envp_->CopyTo(frame, envn_);
  envp_ = frame;
}

void StackFrameKev::makeEnvFrame(KevesGC* gc, int frame_size) {
  LocalVarFrameKev* env_frame(LocalVarFrameKev::Make(gc, frame_size));
  this->clearEnvFrame();
  this->extendEnvFrame(env_frame);
}

void StackFrameKev::assignFreeVar(int index, KevesValue kev) {
  Q_ASSERT(index >= 0 && index < clsr_->size());
  
  FreeVarFrameKev* frame(clsr_);
  
  while (frame->size() <= index) {
    index -= frame->size();
    frame = frame->next();
  }
  
  frame->array()[index] = kev;
}

KevesValue StackFrameKev::freeVar(int idx) const {
  FreeVarFrameKev* frame(clsr_);
  
  while (frame->size() <= idx) {
    idx -= frame->size();
    frame = frame->next();
  }
  
  return frame->at(idx);
}

void StackFrameKev::wind(const_KevesIterator pc, StackFrameKev* stack_frame, ArgumentFrameKev* arg_frame) {
  this->pc_ = pc;
  *stack_frame = *this;
  this->fp_ = stack_frame;
  this->setArgFrame(arg_frame);
  
  if (stack_frame->Kev::type() == STACK_FRAME_B ||
      stack_frame->Kev::type() == STACK_FRAME_E) {
    this->sfp_ = stack_frame;
    this->Kev::set_type(STACK_FRAME);
  }
}

void StackFrameKev::windWithExceptionHandler(const_KevesIterator pc, StackFrameKev* stack_frame, ArgumentFrameKev* arg_frame) {
  wind(pc, stack_frame, arg_frame);
  this->Kev::set_type(STACK_FRAME_E);
}

void StackFrameKev::windWithValues(const_KevesIterator pc, StackFrameKev* stack_frame, ArgumentFrameKev* vals) {
  wind(pc, stack_frame, vals);
  argn_ = KevesFixnum(vals->at(0));
}

void StackFrameKev::copyEnvFrameTo(FreeVarFrameKev* closure) const {
  KevesValue* env_array(envp_->array());
  std::copy(env_array, env_array + envn_, closure->array());
}
