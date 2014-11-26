// Keves/keves_gc-inl.hpp - GC for Keves
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


template<class KEV>
KEV* KevesGC::ToMutable(const KEV* kev) {
  KEV* mutable_kev(const_cast<KEV*>(kev));
    
  if (IsInTenured(mutable_kev) && !mutable_kev->IsMarkedDynamic()) {
    mutable_kev->MarkDynamic(); // IMPORTANT !!!
    this->PushToMarkedList(mutable_kev); // IMPORTANT !!!
  }
    
  return mutable_kev;
}

/*
template<class CTOR>
auto KevesGC::Make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
  return tenured_.Make(ctor, size);
}
*/

template<class CTOR>
void KevesGC::MakeArray(CTOR ctor, size_t elem_size, int num) {
  tenured_.MakeArray(ctor, elem_size, num);
}
  
template<class ZONE, class KEV>
KEV* KevesGC::CopyAndSetNewAddress(ZONE* zone, KEV* kev) {
  KEV* old_address(kev);
  kev = kev->CopyTo(zone);
  old_address->SetNewAddress(kev);
  return kev;
}

template<class KEV>
void KevesGC::SetFunctionTable() {
  ft_size_[KEV::TYPE] = KEV::alloc_size;
}
  
template<class KEV>
KEV* KevesGC::Tenured::Copy(KEV* kev) {
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

/*
template<class CTOR>
auto KevesGC::Tenured::Make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
  void* ptr(gc_->tenured_.Alloc(size));
  decltype(ctor(nullptr)) temp(ctor(ptr));
  temp->MarkDynamic(); // IMPORTANT !!!
  gc_->PushToMarkedList(temp); // IMPORTANT !!!
  return temp;
}
*/

template<class CTOR>
void KevesGC::Tenured::MakeArray(CTOR ctor, size_t elem_size, int num) {
  while (num-- > 0) ctor(Alloc(elem_size));
}
    
template<class KEV>
void KevesGC::Tenured::SetFunctionTable() {
  ft_CopyTo_[KEV::TYPE] = KEV::template CopyTo<Tenured>;
  ft_CopyContents_[KEV::TYPE] = KEV::template CopyContents<Tenured>;
}
