// keves/keves_gc.cpp - GC for Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
//
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "keves_gc.hpp"
#include "keves_gc-inl.hpp"

#include <iostream>
#include "keves_vm.hpp"
#include "kev/bignum.hpp"
#include "kev/code.hpp"
#include "kev/condition.hpp"
#include "kev/frame.hpp"
#include "kev/number.hpp"
#include "kev/pair.hpp"
#include "kev/procedure.hpp"
#include "kev/reference.hpp"
#include "kev/record.hpp"
#include "kev/string.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"
#include "kev/wind.hpp"


void KevesGC::init(KevesVM* vm, KevesList<KevesNode<0> >* shared_list) {
  stack_lower_limit_ = vm->stack_lower_limit();
  stack_higher_limit_ = vm->stack_higher_limit();
  jmp_exit_ = vm->jmp_exit();
  acc_ = &vm->acc_;
  gr1_ = &vm->gr1_;
  gr2_ = &vm->gr2_;
  gr3_ = &vm->gr3_;

  shared_list_ = shared_list;

  setFunctionTable<CodeKev>();
  setFunctionTable<Bignum>();
  setFunctionTable<RationalNumberKev>();
  setFunctionTable<FlonumKev>();
  setFunctionTable<ExactComplexNumberKev>();
  setFunctionTable<InexactComplexNumberKev>();
  setFunctionTable<StringCoreKev>();
  setFunctionTable<StringKev>();
  setFunctionTable<SymbolKev>();
  setFunctionTable<VectorKev>();
  setFunctionTable<WindKev>();
  setFunctionTable<ReferenceKev>();
  setFunctionTable<RecordKev>();
  setFunctionTable<SimpleConditionKev>();
  setFunctionTable<CompoundConditionKev>();
  setFunctionTable<LambdaKev>();
  setFunctionTable<ArgumentFrameKev>();
  setFunctionTable<LocalVarFrameKev>();
  // setFunctionTable<FreeVarFrameKev>();
  setFunctionTable<StackFrameKev>();
  setFunctionTable<PairKev>();
  setFunctionTable<EnvironmentKev>();

  tenured_.set(this);
}

KevesGC::Tenured::Tenured()
  : gc_(),
    ft_CopyTo_(),
    ft_CopyContents_() {
  setFunctionTable<CodeKev>();
  setFunctionTable<Bignum>();
  setFunctionTable<RationalNumberKev>();
  setFunctionTable<FlonumKev>();
  setFunctionTable<ExactComplexNumberKev>();
  setFunctionTable<InexactComplexNumberKev>();
  setFunctionTable<StringCoreKev>();
  setFunctionTable<StringKev>();
  setFunctionTable<SymbolKev>();
  setFunctionTable<VectorKev>();
  setFunctionTable<WindKev>();
  setFunctionTable<ReferenceKev>();
  setFunctionTable<RecordKev>();
  setFunctionTable<SimpleConditionKev>();
  setFunctionTable<CompoundConditionKev>();
  setFunctionTable<LambdaKev>();
  setFunctionTable<ArgumentFrameKev>();
  setFunctionTable<LocalVarFrameKev>();
  // setFunctionTable<FreeVarFrameKev>();
  setFunctionTable<StackFrameKev>();
  setFunctionTable<PairKev>();
  setFunctionTable<EnvironmentKev>();
}

/*
void KevesGC::set(jmp_buf* jmp_exit, KevesValue* acc, KevesValue* gr, void* higher, void* lower, const_KevesIterator pc) {
  jmp_exit_ = jmp_exit;
  acc_ = acc;
  gr_ = gr;
  stack_higher_limit_ = higher;
  stack_lower_limit_ = lower;
  pc_ = pc;
  reset();
}
*/

void KevesGC::reset() {
  while (!tenured_list_.isEmpty()) tenured_list_.Pop().dispose();

  for (size_t i(0); i < MAX_RECYCLE_SIZE; ++i) {
    while (!free_list_[i].isEmpty()) free_list_[i].Pop().dispose();
  }
  
  marked_list_.clear();
  unchecked_list_.clear();
  // count_of_mark_and_sweep_ = 0;
  elapsed_time_ = 0;
}

size_t KevesGC::alignedSize(size_t size) {
  return
    static_cast<unsigned int>((size + sizeof(quintptr) - 1) / sizeof(quintptr))
    * sizeof(quintptr);
}

void* KevesGC::Tenured::Alloc(size_t alloc_size) {
  if (alloc_size < MAX_RECYCLE_SIZE && !gc_->free_list_[alloc_size].isEmpty()) {
    KevesNode<0> node(gc_->free_list_[alloc_size].Pop());
    gc_->tenured_list_.push(node);
    return node.toPtr();
  }
  
  KevesBaseNode node(new char[alignedSize(alloc_size) + sizeof(KevesPrefix)]);
  
  if (node.isEmpty()) {
    std::cout << "Tenured is full!!!" << std::endl;
    longjmp(*gc_->jmp_exit_, -2);
  }
  
  gc_->tenured_list_.push(node);
  return node.toPtr();
}

void KevesGC::pushToUncheckedList(MutableKev* kev) {
  unchecked_list_.push(kev);
}

void KevesGC::pushToTenuredList(KevesBaseNode node) {
  tenured_list_.push(node);
}

void KevesGC::pushToMarkedList(MutableKev* kev) {
  kev->markLive();
  marked_list_.push(kev);
}

void KevesGC::markLive(MutableKev* kev) {
  if (isInTenuredWithoutMark(kev)) pushToMarkedList(kev);
}

void KevesGC::pushToFreeList(KevesBaseNode node) {
  MutableKevesValue kev(node->isCopied() ? node->getNewAddress() : node.toKev());
  size_t size(ft_size_[kev.type()](kev.toPtr()));
  free_list_[size < MAX_RECYCLE_SIZE ? size : 0].push(node);
}

bool KevesGC::isInEden(MutableKev* kev) const {
  return kev >= stack_lower_limit_ && kev < stack_higher_limit_;
}

bool KevesGC::isInTenured(MutableKev* kev) const {
  return kev->isMarkedDynamic() && !kev->isMarkedPermanent();
}

bool KevesGC::isInTenuredWithoutMark(MutableKev* kev) const {
  return isInTenured(kev) && !kev->isMarkedLive();
}

void KevesGC::execute(const_KevesIterator pc) {
  clock_t start_time(clock());
  pc_ = pc;

  unmarkAllObjects();
  
  *acc_ = tenured_.copy(*acc_);

  *gr1_ = tenured_.copy(*gr1_);
  *gr2_ = tenured_.copy(*gr2_);
  *gr3_ = tenured_.copy(*gr3_);
  
  markAndCopy();
  sweep();
  
  elapsed_time_ += clock() - start_time;
  longjmp(*jmp_exit_, 0);
}

void KevesGC::markAndCopy() {
  while (!marked_list_.isEmpty()) {
    MutableKev* kev(marked_list_.Pop().toKev());
    tenured_.copyContents(kev);
  }
}
  
void KevesGC::sweep() {
  KevesList<KevesNode<0> > new_list;

  while (!tenured_list_.isEmpty()) {
    KevesNode<0> node(tenured_list_.Pop());

    if (!node->isCopied() && node->isMarkedLive()) {
      new_list.push(node);
      node->resetLive();
    } else {
      pushToFreeList(node);
    }
  }

  tenured_list_ = new_list;
}

void KevesGC::unmarkAllObjects() {
  for (KevesNode<0> node(tenured_list_.top());
       !node.isEmpty();
       node = node.getNext()) {
    if (!node->isCopied() && !node->isMarkedPermanent()) node->resetLive();
  }

  marked_list_.clear();
}

KevesValue KevesGC::Tenured::copy(MutableKevesValue value) {
  if (!value.isPtr())
    return value;

  MutableKev* old_address(value.toPtr());

  if (old_address->isCopied()) {
    Q_ASSERT(!gc_->isInTenuredWithoutMark(old_address->getNewAddress()));
    return old_address->getNewAddress();
  }

  if (gc_->isInTenuredWithoutMark(old_address)) {
    gc_->pushToMarkedList(old_address);
    return value;
  }

  if (!gc_->isInEden(old_address)) return value;
  
  MutableKev* copy(ft_CopyTo_[old_address->type()](this, old_address));
  old_address->setNewAddress(copy);
  return copy;
}

void KevesGC::Tenured::copyContents(MutableKev* kev) {
  ft_CopyContents_[kev->type()](this, kev);
}
