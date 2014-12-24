// keves/kev/vector.hpp - vectors for Keves
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

#include "keves_iterator.hpp"
#include "keves_value.hpp"


class VectorKev : public MutableKev {
public:
  static constexpr kev_type TYPE = VECTOR;

  VectorKev() = delete;
  VectorKev(const VectorKev&) = delete;
  VectorKev(VectorKev&&) = delete;
  VectorKev& operator=(const VectorKev&) = delete;
  VectorKev& operator=(VectorKev&&) = delete;
protected:
  ~VectorKev() = default;

public:
  const KevesValue* array() const;
  KevesValue* array();
  KevesValue at(int i) const;
  const_KevesIterator begin() const;
  KevesIterator begin();
  void clear();
  const_KevesIterator end() const;
  KevesIterator end();
  void fill(KevesValue);
  void replace(int, KevesValue);

  int size() const {
    return size_;
  }

  template<class ZONE>
  static VectorKev* make(ZONE* zone, int);

protected:
  explicit VectorKev(int);

private:
  static size_t alloc_size(int size) {
    return sizeof(VectorKev) + sizeof(KevesValue) * size;
  }

  const int size_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const VectorKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<VectorKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<VectorKev>* vector(VariableLengthKev<VectorKev>::from(kev));
    for (KevesValue& elem : *vector) elem = zone->copy(elem);
    return vector->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void copyArray(const VectorKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const VectorKev* vector(value);
    BASE::pushArray(pending, vector);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const VectorKev* vector(value);

    out << static_cast<uioword>(vector->type())
	<< static_cast<ioword>(vector->size_);

    BASE::writeArray(list, out, vector);
  }

  template<class BASE, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    VectorKev* vector(VectorKev::make(gc, size));
    BASE::readArray(in, vector);
    return vector;
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    VectorKev* vector(value);
    BASE::revertArray(object_list, vector);
  }
};

template<int SIZE> class VectorKevWithArray : public VectorKev {
public:
  VectorKevWithArray(int size) : VectorKev(size) {
    Q_ASSERT(SIZE >= size);
  }
  
private:
  KevesValue array[SIZE];
};
