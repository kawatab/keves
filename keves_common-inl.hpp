// keves/keves_common-inl.hpp - common resources of Keves
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

#include <QDataStream>
  

template<class KEV>
void KevesCommon::pushValue(QStack<const Kev*>* pending, KEV* kev) {
  if (kev) pending->push(kev);
}
  
template<class KEV>
void KevesCommon::pushArray(QStack<const Kev*>* pending, KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) pushValue(pending, *iter);
}

template<class KEV>
void KevesCommon::revertValue(const QList<const Kev*>& object_list, KEV** kev) {
  MutableKevesValue value(*kev);
  if (isIndex(value))
    *kev = static_cast<KEV*>(const_cast<Kev*>(object_list.at(value.toUIntPtr() >> 2)));
}

template<class KEV>
void KevesCommon::readArray(QDataStream& in, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  uioword value;

  for (; iter != end; ++iter) {
    in >> value;
    *iter = KevesValue::fromUioword<Kev>(value);
  }
}

template<class KEV>
void KevesCommon::revertArray(const QList<const Kev*>& object_list, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
    
  for (; iter != end; ++iter)
    if (iter->isRef()) revertValue(object_list, iter);
}

template<class KEV>
void KevesCommon::writeArray(const QList<const Kev*>& list,
			   QDataStream& out,
			   KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) out << indexAddress(list, *iter);
}

template<class KEV>
void KevesCommon::setFunctionTable() {
  ft_PushChildren_[KEV::TYPE]
    = KEV::template pushChildren<KevesCommon, QStack<const Kev*> >;

  ft_ReadObject_[KEV::TYPE]
    // = KEV::template readObject<KevesCommon, QDataStream, KevesGC>;
    = KEV::template readObject<KevesCommon, QDataStream, KevesCommon>;

  ft_RevertObject_[KEV::TYPE]
    = KEV::template revertObject<KevesCommon, QList<const Kev*> >;

  ft_WriteObject_[KEV::TYPE]
    = KEV::template writeObject<KevesCommon, QList<const Kev*>, QDataStream>;
}

template<class KEV>
KEV* KevesCommon::toMutable(const KEV* kev) {
  KEV* mutable_kev(const_cast<KEV*>(kev));
    
  /*
  if (IsInTenured(mutable_kev) && !mutable_kev->IsMarkedDynamic()) {
    mutable_kev->markDynamic(); // IMPORTANT !!!
    this->pushToMarkedList(mutable_kev); // IMPORTANT !!!
  }
  */
    
  return mutable_kev;
}
