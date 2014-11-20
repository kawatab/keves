/* Keves/keves_gc.cpp - GC for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keves_gc.hpp"

#include <iostream>
#include "code_kev.hpp"
#include "pair_kev.hpp"
#include "string_kev.hpp"
#include "symbol_kev.hpp"
#include "vector_kev.hpp"


#define FUNCTION_TABLE(array, func)					\
  array[CodeKev::TYPE]		       = CodeKev::func;			\
  array[StringCoreKev::TYPE]	       = StringCoreKev::func;		\
  array[StringKev::TYPE]	       = StringKev::func;		\
  array[SymbolKev::TYPE]	       = SymbolKev::func;		\
  array[VectorKev::TYPE]	       = VectorKev::func;		\
  array[PairKev::TYPE]		       = PairKev::func;			\

KevesGC::KevesGC(int)
  : /* survivor_(),*/ tenured_(), /*permanent_(),*/
    tenured_list_(), /*permanent_list_(),*/  free_list_(),
    marked_list_(), unchecked_list_(),
    /*stack_lower_limit_(), stack_higher_limit_() ,*/
    jmp_exit_(),
    /* prev_global_vars_(), curt_global_vars_(),
    registers_(),
    vals_(),*/
    pc_(), acc_(), gr1_(), gr2_(), gr3_(),
    // life_span_(life_span),
    // count_of_mark_and_sweep_(), mark_and_sweep_frequency_(frequency),
    ft_size_() {
  // survivor_.Set(this, 0x1000000UL); // 4 Mi Word = 16 MiB
  tenured_.Set(this);
  // permanent_.Set(this);

  FUNCTION_TABLE(ft_size_, alloc_size);
}

#undef FUNCTION_TABLE

KevesGC::~KevesGC() {
  for (int i(0); i < vm_list_.size(); ++i) delete vm_list_[i];
}

#define FUNCTION_TABLE2(arr, fn, cl)					\
  arr[CodeKev::TYPE]		     = CodeKev::fn<cl>;			\
  arr[StringCoreKev::TYPE]	     = StringCoreKev::fn<cl>;		\
  arr[StringKev::TYPE]		     = StringKev::fn<cl>;		\
  arr[SymbolKev::TYPE]		     = SymbolKev::fn<cl>;		\
  arr[VectorKev::TYPE]		     = VectorKev::fn<cl>;		\
  arr[PairKev::TYPE]		     = PairKev::fn<cl>;			\

KevesGC::Tenured::Tenured() : gc_(), ft_CopyTo_(), ft_CopyContents_() {
  FUNCTION_TABLE2(ft_CopyTo_, CopyTo, Tenured);
  FUNCTION_TABLE2(ft_CopyContents_, CopyContents, Tenured);
}

#undef FUNCTION_TABLE2

/*
void KevesGC::Set(jmp_buf* jmp_exit, KevesValue* acc, KevesValue* gr, void* higher, void* lower, const_KevesIterator pc) {
  jmp_exit_ = jmp_exit;
  acc_ = acc;
  gr_ = gr;
  stack_higher_limit_ = higher;
  stack_lower_limit_ = lower;
  pc_ = pc;
  reset();
}
*/

void KevesGC::Append(KevesVM* vm) {
  vm_list_.append(vm);
}

void KevesGC::Reset() {
  while (!tenured_list_.IsEmpty()) tenured_list_.Pop().Dispose();

  for (size_t i(0); i < MAX_RECYCLE_SIZE; ++i) {
    while (!free_list_[i].IsEmpty()) free_list_[i].Pop().Dispose();
  }
  
  marked_list_.Clear();
  unchecked_list_.Clear();
  // count_of_mark_and_sweep_ = 0;
  elapsed_time_ = 0;
}

size_t KevesGC::alignedSize(size_t size) {
  return
    static_cast<unsigned int>((size + sizeof(quintptr) - 1) / sizeof(quintptr))
    * sizeof(quintptr);
}

void* KevesGC::Tenured::Alloc(size_t alloc_size) {
  if (alloc_size < MAX_RECYCLE_SIZE && !gc_->free_list_[alloc_size].IsEmpty()) {
    KevesNode<0> node(gc_->free_list_[alloc_size].Pop());
    gc_->tenured_list_.Push(node);
    return node.ToPtr();
  }
  
  KevesBaseNode node(new char[alignedSize(alloc_size) + sizeof(KevesPrefix)]);
  
  if (node.IsEmpty()) {
    std::cout << "Tenured is full!!!" << std::endl;
    longjmp(*gc_->jmp_exit_, -2);
  }
  
  gc_->tenured_list_.Push(node);
  return node.ToPtr();
}

void KevesGC::PushToUncheckedList(MutableKev* kev) {
  unchecked_list_.Push(kev);
}

void KevesGC::PushToTenuredList(KevesBaseNode node) {
  tenured_list_.Push(node);
}

void KevesGC::PushToMarkedList(MutableKev* kev) {
  kev->MarkLive();
  marked_list_.Push(kev);
}

void KevesGC::MarkLive(MutableKev* kev) {
  if (IsInTenuredWithoutMark(kev)) PushToMarkedList(kev);
}

void KevesGC::PushToFreeList(KevesBaseNode node) {
  MutableKevesValue kev(node->IsCopied() ? node->GetNewAddress() : node.ToKev());
  size_t size(ft_size_[kev.type()](kev.ToPtr()));
  free_list_[size < MAX_RECYCLE_SIZE ? size : 0].Push(node);
}

bool KevesGC::IsInEden(MutableKev* kev) const {
  return kev >= stack_lower_limit_ && kev < stack_higher_limit_;
}

bool KevesGC::IsInTenured(MutableKev* kev) const {
  return kev->IsMarkedDynamic() && !kev->IsMarkedPermanent();
}

bool KevesGC::IsInTenuredWithoutMark(MutableKev* kev) const {
  return IsInTenured(kev) && !kev->IsMarkedLive();
}

void KevesGC::Execute(const_KevesIterator pc) {
  clock_t start_time(clock());
  pc_ = pc;

  UnmarkAllObjects();
  
  *acc_ = tenured_.Copy(*acc_);

  *gr1_ = tenured_.Copy(*gr1_);
  *gr2_ = tenured_.Copy(*gr2_);
  *gr3_ = tenured_.Copy(*gr3_);
  
  MarkAndCopy();
  Sweep();
  
  elapsed_time_ += clock() - start_time;
  longjmp(*jmp_exit_, 0);
}

void KevesGC::MarkAndCopy() {
  while (!marked_list_.IsEmpty()) {
    MutableKev* kev(marked_list_.Pop().ToKev());
    tenured_.CopyContents(kev);
  }
}
  
void KevesGC::Sweep() {
  KevesList<KevesNode<0> > new_list;

  while (!tenured_list_.IsEmpty()) {
    KevesNode<0> node(tenured_list_.Pop());

    if (!node->IsCopied() && node->IsMarkedLive()) {
      new_list.Push(node);
      node->ResetLive();
    } else {
      PushToFreeList(node);
    }
  }

  tenured_list_ = new_list;
}

void KevesGC::UnmarkAllObjects() {
  for (KevesNode<0> node(tenured_list_.top());
       !node.IsEmpty();
       node = node.GetNext()) {
    if (!node->IsCopied() && !node->IsMarkedPermanent()) node->ResetLive();
  }

  marked_list_.Clear();
}

KevesValue KevesGC::Tenured::Copy(MutableKevesValue value) {
  if (!value.IsPtr())
    return value;

  MutableKev* old_address(value.ToPtr());

  if (old_address->IsCopied()) {
    Q_ASSERT(!gc_->IsInTenuredWithoutMark(old_address->GetNewAddress()));
    return old_address->GetNewAddress();
  }

  if (gc_->IsInTenuredWithoutMark(old_address)) {
    gc_->PushToMarkedList(old_address);
    return value;
  }

  if (!gc_->IsInEden(old_address)) return value;
  
  MutableKev* copy(ft_CopyTo_[old_address->type()](this, old_address));
  old_address->SetNewAddress(copy);
  return copy;
}

void KevesGC::Tenured::CopyContents(MutableKev* kev) {
  ft_CopyContents_[kev->type()](this, kev);
}
