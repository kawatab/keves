// Keves/keves_gc.hpp - GC for Keves
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


#pragma once

#include <setjmp.h>
#include <QList>
#include "keves_iterator.hpp"
#include "keves_value.hpp"


class KevesVM;

struct KevesPrefix {
  KevesPrefix* node[2];
};
  

class KevesBaseNode {
public:
  KevesBaseNode() : prefix_() {
  }

  KevesBaseNode(const KevesBaseNode&) = default;
  KevesBaseNode(KevesBaseNode&&) = default;
  KevesBaseNode& operator=(const KevesBaseNode&) = default;
  KevesBaseNode& operator=(KevesBaseNode&&) = default;
  ~KevesBaseNode() = default;

  explicit KevesBaseNode(void* ptr)
    : prefix_(static_cast<KevesPrefix*>(ptr)) {}
  
  explicit KevesBaseNode(MutableKev* kev)
    : prefix_(reinterpret_cast<KevesPrefix*>(kev) - 1) {
  }
    
  void Dispose() {
    delete [] reinterpret_cast<char*>(prefix_);
  }

  bool IsEmpty() const {
    return !prefix_;
  }

  void* ToPtr() const {
    return prefix_ + 1;
  }

  MutableKev* ToKev() const {
    return static_cast<MutableKev*>(ToPtr());
  }

  MutableKev* operator->() const {
    return static_cast<MutableKev*>(ToPtr());
  }
  
protected:
  KevesPrefix* prefix_;
};
  
template<int SIZE> class KevesNode : public KevesBaseNode {
public:
  KevesNode() = default;
  KevesNode(const KevesNode&) = default;
  KevesNode(KevesNode&&) = default;
  KevesNode& operator=(const KevesNode&) = default;
  KevesNode& operator=(KevesNode&&) = default;
  ~KevesNode() = default;
  
  explicit KevesNode(KevesBaseNode node) : KevesBaseNode(node) {
  }
  
  KevesNode GetNext() const {
    return prefix_->node[SIZE];
  }
  
  void SetNext(KevesBaseNode prev) const {
    prefix_->node[SIZE] = KevesNode(prev).prefix_;
  }
  
private:
  KevesNode(KevesPrefix* prefix) : KevesBaseNode(prefix) {
  }
};
  
template<class NODE> class KevesList {
public:
  KevesList()
    : top_() {}
  
  KevesList(const KevesList&) = default;
  KevesList(KevesList&&) = default;
  KevesList& operator=(const KevesList&) = default;
  KevesList& operator=(KevesList&&) = default;
  ~KevesList() = default;
  
  void Clear() {
    top_ = NODE();
  }

  bool IsEmpty() const {
    return top_.IsEmpty();
  }

  NODE top() const {
    return top_;
  }
  
  NODE Pop() {
    NODE temp(top_);
    top_ = temp.GetNext();
    return temp;
  }
  
  void Push(NODE node) {
    node.SetNext(top_);
    top_ = node;
  }
  
  void Push(KevesBaseNode base_node) {
    Push(NODE(base_node));
  }

  void Push(MutableKev* kev) {
    Push(KevesBaseNode(kev));
  }
  
private:
  NODE top_;
};


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
  KevesGC() = delete;
  KevesGC(const KevesGC&) = delete;
  KevesGC(const KevesGC&&) = delete;
  KevesGC& operator=(const KevesGC&) = delete;
  KevesGC& operator=(const KevesGC&&) = delete;
  ~KevesGC();

  KevesGC(int);
  
  void Append(KevesVM* vm);

  template<class KEV>
  KEV* ToMutable(const KEV* kev) {
    KEV* mutable_kev(const_cast<KEV*>(kev));
    
    if (IsInTenured(mutable_kev) && !mutable_kev->IsMarkedDynamic()) {
      mutable_kev->MarkDynamic(); // IMPORTANT !!!
      this->PushToMarkedList(mutable_kev); // IMPORTANT !!!
    }
    
    return mutable_kev;
  }

  void Execute(const_KevesIterator);

  double GetElapsedTime() const {
    return static_cast<double>(elapsed_time_) / CLOCKS_PER_SEC;
  }

  template<class CTOR>
  auto Make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
    return tenured_.Make(ctor, size);
  }

  template<class CTOR> void MakeArray(CTOR ctor, size_t elem_size, int num) {
    tenured_.MakeArray(ctor, elem_size, num);
  }
  
  void MarkLive(MutableKev*);

  const_KevesIterator pc() const {
    return pc_;
  }

  void Reset();
  // void Set(jmp_buf*, KevesValue*, KevesValue*, void*, void*, const_KevesIterator);

private:
  bool IsInEden(MutableKev* kev) const;
  bool IsInTenured(MutableKev*) const;
  bool IsInTenuredWithoutMark(MutableKev*) const;
  void MarkAndCopy();
  void PushToFreeList(KevesBaseNode);
  void PushToTenuredList(KevesBaseNode);
  void PushToMarkedList(MutableKev*);
  void PushToUncheckedList(MutableKev*);
  void Sweep();
  void UnmarkAllObjects();

  template<class ZONE, class KEV>
  static KEV* CopyAndSetNewAddress(ZONE* zone, KEV* kev) {
    KEV* old_address(kev);
    kev = kev->CopyTo(zone);
    old_address->SetNewAddress(kev);
    return kev;
  }

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
    KEV* Copy(KEV* kev) {
      KEV* old_address(kev);
      
      if (!old_address)
	return old_address;
      
      if (old_address->IsCopied())
	return static_cast<KEV*>(old_address->GetNewAddress());

      if (gc_->IsInEden(kev))
	return KevesGC::CopyAndSetNewAddress(this, kev);
      
      gc_->MarkLive(kev);
      return kev;
    }

    KevesValue Copy(MutableKevesValue);
    void CopyContents(MutableKev*);
    
    template<class CTOR>
    auto Make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
      void* ptr(gc_->tenured_.Alloc(size));
      decltype(ctor(nullptr)) temp(ctor(ptr));
      temp->MarkDynamic(); // IMPORTANT !!!
      gc_->PushToMarkedList(temp); // IMPORTANT !!!
      return temp;
    }

    template<class CTOR>
    void MakeArray(CTOR ctor, size_t elem_size, int num) {
      while (num-- > 0) ctor(Alloc(elem_size));
    }
    
    void Set(KevesGC* gc) {
      gc_ = gc;
    }
    
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

  QList<KevesVM*> vm_list_;
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
