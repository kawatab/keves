/* code_kev.hpp - codes for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 *  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "keves_iterator.hpp"
#include "keves_value.hpp"

class KevesGC;

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

  static CodeKev* make(KevesGC* gc, int size);
  static CodeKev* makeWithTenured(KevesGC* gc, int size);
  static CodeKev* makeWithPermanent(KevesGC* gc, int size);
  
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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<CodeKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<CodeKev>* code(VariableLengthKev<CodeKev>::From(kev));
    // KevesValue* begin(code->array());
    // KevesValue* end(iter + code->size());
    // for (; iter != end_iter; ++iter) *iter = zone->Copy(*iter);
    // std::for_each(begin, end, [](KevesValue* iter) { *iter = zone->Copy(*iter) });
    for (KevesValue& elem : *code) elem = zone->Copy(elem);
    return code->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1)  + size_;
  }

  void CopyArray(const CodeKev* org);
  
  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const CodeKev* code(value);
    IO::PushArray(pending, code);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const CodeKev* code(value);

    out << static_cast<uioword>(code->type())
	<< static_cast<ioword>(code->size());

    IO::WriteArray(list, out, code);
  }

  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    CodeKev* code(CodeKev::make(gc, size));
    IO::ReadArray(in, code);
    return code;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    CodeKev* code(value);
    IO::RevertArray(object_list, code);
  }
};
