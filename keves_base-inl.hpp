// Keves/keves_base-inl.hpp - base of Keves
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

  
template<class KEV>
void KevesBase::PushValue(QStack<const Kev*>* pending, KEV* kev) {
  if (kev) pending->push(kev);
}
  
template<class KEV>
void KevesBase::PushArray(QStack<const Kev*>* pending, KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) PushValue(pending, *iter);
}

template<class KEV>
void KevesBase::RevertValue(const QList<Kev*>& object_list, KEV** kev) {
  KevesValue value(*kev);
  if (IsIndex(value))
    *kev = static_cast<KEV*>(object_list.at(value.toUIntPtr() >> 2));
}

template<class KEV>
void KevesBase::ReadArray(QDataStream& in, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  uioword value;

  for (; iter != end; ++iter) {
    in >> value;
    *iter = KevesValue::FromUioword<Kev>(value);
  }
}

template<class KEV>
void KevesBase::RevertArray(const QList<Kev*>& object_list, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
    
  for (; iter != end; ++iter)
    if (iter->IsRef()) RevertValue(object_list, iter);
}

template<class KEV>
void KevesBase::WriteArray(const QList<const Kev*>& list,
			   QDataStream& out,
			   KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) out << IndexAddress(list, *iter);
}

template<class KEV>
void KevesBase::SetFunctionTable() {
  ft_PushChildren_[KEV::TYPE]
    = KEV::template PushChildren<KevesBase, QStack<const Kev*> >;

  ft_ReadObject_[KEV::TYPE]
    // = KEV::template ReadObject<KevesBase, QDataStream, KevesGC>;
    = KEV::template ReadObject<KevesBase, QDataStream, KevesBase>;

  ft_RevertObject_[KEV::TYPE]
    = KEV::template RevertObject<KevesBase, QList<Kev*> >;

  ft_WriteObject_[KEV::TYPE]
    = KEV::template WriteObject<KevesBase, QList<const Kev*>, QDataStream>;
}

template<class KEV>
KEV* KevesBase::ToMutable(const KEV* kev) {
  KEV* mutable_kev(const_cast<KEV*>(kev));
    
  /*
  if (IsInTenured(mutable_kev) && !mutable_kev->IsMarkedDynamic()) {
    mutable_kev->MarkDynamic(); // IMPORTANT !!!
    this->PushToMarkedList(mutable_kev); // IMPORTANT !!!
  }
  */
    
  return mutable_kev;
}
