/* Keves/keves_file_io-inl.hpp - file handler for Keves
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

#pragma once


template<class KEV>
void KevesFileIO::ReadArray(QDataStream& in, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  uioword value;

  for (; iter != end; ++iter) {
    in >> value;
    *iter = fromUioword<Kev>(value);
  }
}
  
template<class KEV>
void KevesFileIO::RevertValue(const QList<Kev*>& object_list, KEV** kev) {
  KevesValue value(*kev);
  if (isIndex(value))
    *kev = static_cast<KEV*>(object_list.at(value.toUIntPtr() >> 2));
}

template<class KEV>
void KevesFileIO::PushValue(QStack<const Kev*>* pending, KEV* kev) {
  if (kev) pending->push(kev);
}
  
template<class KEV>
void KevesFileIO::RevertArray(const QList<Kev*>& object_list, KEV* kev) {
  KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
    
  for (; iter != end; ++iter)
    if (iter->IsRef()) RevertValue(object_list, iter);
}
  
template<class KEV>
void KevesFileIO::PushArray(QStack<const Kev*>* pending, KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) PushValue(pending, *iter);
}

template<class KEV>
void KevesFileIO::WriteArray(const QList<const Kev*>& list,
				    QDataStream& out,
				    KEV* kev) {
  const KevesValue* iter(kev->array());
  const KevesValue* end(iter + kev->size());
  for (; iter != end; ++iter) out << WrapAddress(list, *iter);
}

template<class KEV>
KEV* KevesFileIO::fromUioword(uioword value) {
  return reinterpret_cast<KEV*>(static_cast<quintptr>(value));
}
