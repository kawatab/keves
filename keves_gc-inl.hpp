// keves/keves_gc-inl.hpp - GC for Keves
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
KEV* KevesGC::toMutable(const KEV* kev) {
  KEV* mutable_kev(const_cast<KEV*>(kev));
    
  if (isInTenured(mutable_kev) && !mutable_kev->isMarkedDynamic()) {
    mutable_kev->markDynamic(); // IMPORTANT !!!
    this->pushToMarkedList(mutable_kev); // IMPORTANT !!!
  }
    
  return mutable_kev;
}

/*
template<class CTOR>
auto KevesGC::make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
  return tenured_.make(ctor, size);
}
*/

template<class CTOR>
void KevesGC::makeArray(CTOR ctor, size_t elem_size, int num) {
  tenured_.makeArray(ctor, elem_size, num);
}
  
template<class ZONE, class KEV>
KEV* KevesGC::copyAndSetNewAddress(ZONE* zone, KEV* kev) {
  KEV* old_address(kev);
  kev = kev->copyTo(zone);
  old_address->setNewAddress(kev);
  return kev;
}

template<class KEV>
void KevesGC::setFunctionTable() {
  ft_size_[KEV::TYPE] = KEV::alloc_size;
}
  
template<class KEV>
KEV* KevesGC::Tenured::copy(KEV* kev) {
  KEV* old_address(kev);
      
  if (!old_address)
    return old_address;
      
  if (old_address->isCopied())
    return static_cast<KEV*>(old_address->getNewAddress());

  if (gc_->isInEden(kev))
    return KevesGC::copyAndSetNewAddress(this, kev);
      
  gc_->markLive(kev);
  return kev;
}

/*
template<class CTOR>
auto KevesGC::Tenured::make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
  void* ptr(gc_->tenured_.Alloc(size));
  decltype(ctor(nullptr)) temp(ctor(ptr));
  temp->markDynamic(); // IMPORTANT !!!
  gc_->pushToMarkedList(temp); // IMPORTANT !!!
  return temp;
}
*/

template<class CTOR>
void KevesGC::Tenured::makeArray(CTOR ctor, size_t elem_size, int num) {
  while (num-- > 0) ctor(Alloc(elem_size));
}
    
template<class KEV>
void KevesGC::Tenured::setFunctionTable() {
  ft_CopyTo_[KEV::TYPE] = KEV::template copyTo<Tenured>;
  ft_CopyContents_[KEV::TYPE] = KEV::template copyContents<Tenured>;
}
