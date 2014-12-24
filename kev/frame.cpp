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
void ArgumentFrameKev::copyArray(const ArgumentFrameKev* org) {
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

  return gc->make(ctor, alloc_size(new_size));
}

void ArgumentFrameKev::copyTo(ArgumentFrameKev* other, int size) const {
  const KevesValue* iter(this->array());
  std::copy(iter, iter + size, other->array());
}


// class LocalVarFrameKev ----------------------------------------
void LocalVarFrameKev::copyArray(const LocalVarFrameKev* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + size_);
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}

/*
void LocalVarFrameKev::copyTo(LocalVarFrameKev* other, int num) const {
  Q_ASSERT(type() == TYPE);

  const KevesValue* iter(this->array());
  std::copy(iter, iter + num, other->array());
}
*/


/*
// class FreeVarFrameKev ----------------------------------------
void FreeVarFrameKev::copyArray(const FreeVarFrameKev* org) {
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
  
  return gc->make(ctor, alloc_size(this->size() + addition));
}
*/


// class StackFrameKev ----------------------------------------
void StackFrameKev::setArgumentsToLocalVars() {
  for (int i(1); i < argn_; ++i) envp_->array()[i - 1] = argp_->at(i);
}

void StackFrameKev::extendArgFrame(KevesGC* gc) {
  argp_ = argp_->extend(gc);
}

/*
void StackFrameKev::extendEnvFrame(LocalVarFrameKev* frame) {
  if (envn_ > 0) envp_->copyTo(frame, envn_);
  envp_ = frame;
}
*/
void StackFrameKev::extendEnvFrame(KevesGC* gc, int frame_size) {
  envp_ = LocalVarFrameKev::make(gc, frame_size, envp_);
  envn_ += frame_size;
}

void StackFrameKev::makeEnvFrame(KevesGC* gc, int frame_size) {
  envp_ = LocalVarFrameKev::make(gc, frame_size, nullptr);
  envn_ = frame_size;
}

void StackFrameKev::assignLastLocalVar(KevesValue kev, int offset) {
  /*
  Q_ASSERT(offset >= 0 && offset < envp_->size());
  envp_->array()[envn_ - offset - 1] = kev;
  */
  assignLocalVar(envn_ - offset - 1, kev);
}

void StackFrameKev::assignLocalVar(int index, KevesValue kev) {
  Q_ASSERT(index >= 0 && index < envn_);

  LocalVarFrameKev* frame(envp_);

  while (frame->size() <= index) {
    index -= frame->size();
    frame = frame->next();
  }
    
  frame->array()[index] = kev;
}

void StackFrameKev::assignFreeVar(int index, KevesValue kev) {
  Q_ASSERT(index >= 0 && index < clsr_->size());

  LocalVarFrameKev* frame(clsr_);
  
  while (frame->size() <= index) {
    index -= frame->size();
    frame = frame->next();
  }
  
  frame->array()[index] = kev;
}

KevesValue StackFrameKev::lastLocalVar(int offset) const {
  /*
  Q_ASSERT(index >= 0 && index < envn_);
  return envp_->at(envn_ - index -1);
  */
  Q_ASSERT(offset >= 0 && offset < envn_);

  int index(envn_ - offset - 1);
  LocalVarFrameKev* frame(envp_);

  while (frame->size() <= index) {
    index -= frame->size();
    frame = frame->next();
  }
    
  return frame->array()[index];
}

KevesValue StackFrameKev::freeVar(int index) const {
  LocalVarFrameKev* frame(clsr_);
  
  while (frame->size() <= index) {
    index -= frame->size();
    frame = frame->next();
  }
  
  return frame->at(index);
}

LocalVarFrameKev* StackFrameKev::close() {
  int count(envn_);
  LocalVarFrameKev* frame(envp_);

  while (count > 0) {
    Q_ASSERT(frame);

    count -= frame->size();
    frame->next();
  }

  Q_ASSERT(count == 0);

  frame->set_next(clsr_);
  return envp_;
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

/*
void StackFrameKev::copyEnvFrameTo(FreeVarFrameKev* closure) const {
  KevesValue* env_array(envp_->array());
  std::copy(env_array, env_array + envn_, closure->array());
}
*/
