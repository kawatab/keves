// keves/keves_gc.hpp - GC for Keves
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

#include <setjmp.h>
#include "keves_iterator.hpp"
#include "keves_list.hpp"
#include "keves_value.hpp"


class KevesVM;


class KevesGC {

#ifdef QT_NO_DEBUG // release mode
  static constexpr int LIFE_SPAN = 31; // 63
  static constexpr size_t MAX_RECYCLE_SIZE = 96;
  static constexpr size_t MAX_ALLOCATION_SIZE_WITH_SURVIVOR = 0x1000;
#else // debug mode
  static constexpr int LIFE_SPAN = 31;
  static constexpr size_t MAX_RECYCLE_SIZE = 96;
  static constexpr size_t MAX_ALLOCATION_SIZE_WITH_SURVIVOR = 0x10000;
#endif

public:
  KevesGC() = default;
  KevesGC(const KevesGC&) = delete;
  KevesGC(const KevesGC&&) = delete;
  KevesGC& operator=(const KevesGC&) = delete;
  KevesGC& operator=(const KevesGC&&) = delete;
  ~KevesGC() = default;

  void init(KevesVM* vm, KevesList<KevesNode<0> >* shared_list);
  
  template<class KEV>
  KEV* toMutable(const KEV* kev);

  void execute(const_KevesIterator);

  double getElapsedTime() const {
    return static_cast<double>(elapsed_time_) / CLOCKS_PER_SEC;
  }

  template<class CTOR>
  auto make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
    return tenured_.make(ctor, size);
  }

  template<class CTOR>
  void makeArray(CTOR ctor, size_t elem_size, int num);
  
  void markLive(MutableKev*);

  const_KevesIterator pc() const {
    return pc_;
  }

  void reset();

  void set(KevesList<KevesNode<0> >* shared_list) {
    shared_list_ = shared_list;
  }
  
  // void set(jmp_buf*, KevesValue*, KevesValue*, void*, void*, const_KevesIterator);

private:
  bool isInEden(MutableKev* kev) const;
  bool isInTenured(MutableKev*) const;
  bool isInTenuredWithoutMark(MutableKev*) const;
  void markAndCopy();
  void pushToFreeList(KevesBaseNode);
  void pushToTenuredList(KevesBaseNode);
  void pushToMarkedList(MutableKev*);
  void pushToUncheckedList(MutableKev*);
  void sweep();
  void unmarkAllObjects();

  template<class ZONE, class KEV>
  static KEV* copyAndSetNewAddress(ZONE* zone, KEV* kev);

  template<class KEV>
  void setFunctionTable();
  
  static size_t alignedSize(size_t size);

  class Tenured {
  public:
    Tenured();
    Tenured(const Tenured&) = delete;
    Tenured(Tenured&&) = delete;
    Tenured& operator=(const Tenured&) = delete;
    Tenured& operator=(Tenured&&) = delete;
    ~Tenured() = default;
    
    template<class KEV>
    KEV* copy(KEV* kev);

    KevesValue copy(MutableKevesValue);
    void copyContents(MutableKev*);
    
    template<class CTOR>
    auto make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
      void* ptr(gc_->tenured_.Alloc(size));
      decltype(ctor(nullptr)) temp(ctor(ptr));
      temp->markDynamic(); // IMPORTANT !!!
      gc_->pushToMarkedList(temp); // IMPORTANT !!!
      return temp;
    }

    template<class CTOR>
    void makeArray(CTOR ctor, size_t elem_size, int num);
    
    void set(KevesGC* gc) {
      gc_ = gc;
    }
    
    template<class KEV>
    void setFunctionTable();
  
  private:
    void* Alloc(size_t);

    KevesGC* gc_;
    MutableKev* (*ft_CopyTo_[0177])(Tenured*, MutableKev*);
    quintptr* (*ft_CopyContents_[0177])(Tenured*, MutableKev*);
  } tenured_;

  KevesList<KevesNode<0> > tenured_list_;
  KevesList<KevesNode<0> > free_list_[MAX_RECYCLE_SIZE];
  KevesList<KevesNode<1> > marked_list_;
  KevesList<KevesNode<1> > unchecked_list_;
  KevesList<KevesNode<0> >* shared_list_;

  void* stack_lower_limit_;
  void* stack_higher_limit_;
  jmp_buf* jmp_exit_;
  const_KevesIterator pc_;
  KevesValue* acc_;
  KevesValue* gr1_;
  KevesValue* gr2_;
  KevesValue* gr3_;
  clock_t elapsed_time_;
  size_t (*ft_size_[0177])(const MutableKev*);
};
