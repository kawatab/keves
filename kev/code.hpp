// keves/kev/code.hpp - codes for Keves
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


class CodeKev : public MutableKev {
public:
  static constexpr kev_type TYPE = CODE;

  CodeKev() = delete;
  CodeKev(const CodeKev&) = delete;
  CodeKev(CodeKev&&) = delete;
  CodeKev& operator=(const CodeKev&) = delete;
  CodeKev& operator=(CodeKev&&) = delete;
protected:
  ~CodeKev() = default;

public:
  explicit CodeKev(int size);

  KevesValue* array();
  const KevesValue* array() const;
  KevesIterator begin();
  const_KevesIterator begin() const;
  void clear();
  KevesIterator end();
  const_KevesIterator end() const;

  int size() const {
    return size_;
  }

  template<class ZONE>
  static CodeKev* make(ZONE* zone, int size);
  
private:
  static size_t alloc_size(int size) {
    return sizeof(CodeKev) + sizeof(KevesValue) * size;
  }

  const int size_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const CodeKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<CodeKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<CodeKev>* code(VariableLengthKev<CodeKev>::from(kev));
    // KevesValue* begin(code->array());
    // KevesValue* end(iter + code->size());
    // for (; iter != end_iter; ++iter) *iter = zone->copy(*iter);
    // std::for_each(begin, end, [](KevesValue* iter) { *iter = zone->copy(*iter) });
    for (KevesValue& elem : *code) elem = zone->copy(elem);
    return code->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1)  + size_;
  }

  void copyArray(const CodeKev* org);
  
  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const CodeKev* code(value);
    BASE::pushArray(pending, code);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const CodeKev* code(value);

    out << static_cast<uioword>(code->type())
	<< static_cast<ioword>(code->size());

    BASE::writeArray(list, out, code);
  }

  template<class BASE, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    CodeKev* code(CodeKev::make(gc, size));
    BASE::readArray(in, code);
    return code;
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    CodeKev* code(value);
    BASE::revertArray(object_list, code);
  }
};
