// keves/kev/string.hpp - strings for Keves
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

#include <algorithm>
#include <functional>
#include <QString>

#include "keves_iterator.hpp"
#include "keves_value.hpp"

class KevesGC;


class StringCoreKev : public MutableKev {
public:
  static constexpr kev_type TYPE = STRING_CORE;

  StringCoreKev() = delete;
  StringCoreKev(const StringCoreKev&) = delete;
  StringCoreKev(StringCoreKev&&) = delete;
  StringCoreKev& operator=(const StringCoreKev&) = delete;
  StringCoreKev& operator=(StringCoreKev&&) = delete;
protected:
  ~StringCoreKev() = default;

  explicit StringCoreKev(int n) : MutableKev(TYPE), size_(n)  {
  }

public:
  QChar* array();
  const QChar* array() const;
  StringCoreKev* copy(KevesGC* gc, const QChar*, int) const;
  bool isEmpty() const;

  int size() const {
    return size_;
  }

  StringCoreKev* toCaseFolded(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* toLower(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* toTitleCase(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* toUpper(KevesGC* gc, const QChar*, int) const;

  template<class ZONE>
  static StringCoreKev* make(ZONE* zone, int size);

  template<class ZONE>
  static StringCoreKev* make(ZONE* zone, const QString& str);

  template<class ZONE>
  static StringCoreKev* makeAsImmovableT(ZONE* zone, const QString& str);

private:
  void set(const QString&);

  static size_t alloc_size(int size) {
    return sizeof(StringCoreKev) + sizeof(QChar) * size;
  }

  int size_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const StringCoreKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<StringCoreKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE*, MutableKev* kev) {
    return VariableLengthKev<StringCoreKev>::from(kev)->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + (sizeof(QChar) * size_ + sizeof(quintptr) - 1) / sizeof(quintptr);
  }

  void copyArray(const StringCoreKev* org);
  
  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class /*BASE*/, class STACK>
  static void pushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    return;
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void writeObject(const LIST&, STREAM& out, KevesValue value) {
    const StringCoreKev* string(value);
    int size(string->size());

    out << static_cast<uioword>(string->type())
	<< static_cast<ioword>(size);

    const QChar* array(string->array());
    for (int i(0); i < size; ++i) out << array[i];
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    StringCoreKev* string(make(gc, size));
    QChar* array(string->array());
    for (int i(0); i < size; ++i) in >> array[i];
    return string;
  }

  template<class /*BASE*/, class LIST>
  static void revertObject(const LIST&, MutableKevesValue) {
    return;
  }
};


class KevesString {
public:
  KevesString();
  KevesString(const KevesString&) = default;
  KevesString(KevesString&&) = default;
  KevesString& operator=(const KevesString&) = default;
  KevesString& operator=(KevesString&&) = default;
  ~KevesString() = default;

  KevesString(StringCoreKev*, int idx, int len);

  KevesString append(KevesGC* gc, const KevesString&) const;
  QChar at(int) const;
  StringCoreKev* core();
  const StringCoreKev* core() const;
  QChar* data();
  const QChar* data() const;
  bool equals(const char*) const;
  bool equals(const KevesString&) const;
  void fill(QChar);
  int index() const;
  int indexOf(char) const;
  int indexOf(const char*) const;
  int indexOf(const char*, int) const;
  bool isEmpty() const;
  bool isGT(const KevesString&) const;
  bool isGTE(const KevesString&) const;
  bool isLT(const KevesString&) const;
  bool isLTE(const KevesString&) const;
  KevesString left(int) const;
  KevesString mid(int, int) const;
  KevesString right(int) const ;
  void set(StringCoreKev*, int, int);

  int size() const {
    return len_;
  }

  double toDouble(bool*) const;
  int toInt(bool*, int) const;
  QByteArray toLocal8Bit() const;
  QString toQString() const;
  unsigned int toUInt(bool*, int) const;

protected:
  StringCoreKev* core_;
  int idx_;
  int len_;
};


class StringKev : public MutableKev, public KevesString {
public:
  static constexpr kev_type TYPE = STRING;

  StringKev();
  StringKev(const StringKev&) = default;
  StringKev(StringKev&&) = default;
  StringKev& operator=(const StringKev&) = delete;
  StringKev& operator=(StringKev&&) = delete;
  ~StringKev() = default;

  explicit StringKev(const KevesString&);
  StringKev(StringCoreKev*, int, int);

  StringKev append(KevesGC* gc, const StringKev&) const;
  StringKev copy(KevesGC* gc) const;
  void copyFrom(const StringKev& org);
  void copyFrom(StringKev&& org);

  KevesString& getKevesString() {
    return *static_cast<KevesString*>(this);
  }

  const KevesString& getKevesString() const {
    return *static_cast<const KevesString*>(this);
  }

  StringKev left(int);
  StringKev left(int) const;
  StringKev mid(int, int);
  StringKev mid(int, int) const;
  StringKev right(int);
  StringKev right(int) const;
  void set(int, QChar);
  void set(const StringKev&);
  void set(StringKev&&);

  int size() const {
    return KevesString::size();
  }

  StringKev toCaseFolded(KevesGC* gc) const;
  StringKev toLower(KevesGC* gc) const;
  StringKev toTitleCase(KevesGC* gc) const;
  StringKev toUpper(KevesGC* gc) const;

  template<class ZONE>
  static StringKev* make(ZONE* zone, int);

  template<class ZONE>
  static StringKev* make(ZONE* zone, const QString&);
  // static StringKev* make(KevesIterator*, const QString&);
  // static StringKev* make(KevesValue*, size_t, const QString&);
  template<class ZONE>
  static StringKev* make(ZONE* zone, StringCoreKev*, int, int);

  template<class ZONE>
  static StringKev* makeSubstring(ZONE* zone, const StringKev*, int, int);

  template<class ZONE>
  static std::function<StringKev*(void*)> ctor(ZONE* zone, int size) {
    return [zone, size](void* ptr) {
      StringCoreKev* core(StringCoreKev::make(zone, size));
      return new(ptr) StringKev(core, 0, core->size());
    };
  }

  template<class ZONE>
  static std::function<StringKev*(void*)> ctor(ZONE* zone, const QString& str) {
    return [zone, &str](void* ptr) {
      StringCoreKev* core(StringCoreKev::make(zone, str));
      return new(ptr) StringKev(core, 0, core->size());
    };
  }

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(StringKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<StringKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<StringKev>* string(FixedLengthKev<StringKev>::from(kev));
    VariableLengthKev<StringCoreKev>* core(VariableLengthKev<StringCoreKev>::from(string->core_));
    string->core_ = zone->copy(core);
    return string->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const StringKev* string(value);
    BASE::pushValue(pending, string->core_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const StringKev* string(value);

    out << static_cast<uioword>(string->type())
	<< BASE::indexAddress(list, string->core_)
	<< static_cast<ioword>(string->idx_)
	<< static_cast<ioword>(string->len_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword core;
    ioword idx, len;
    in >> core >> idx >> len;
    return make(gc,
		KevesValue::template fromUioword<StringCoreKev>(core),
		idx,
		len);
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue kev) {
    StringKev* string(kev);
    BASE::revertValue(object_list, &string->core_);
  }
};


template<int SIZE> class StringKevWithArray : public StringKev {
public:
  StringKevWithArray() = delete;
  StringKevWithArray(const StringKevWithArray<SIZE>&) = delete;
  StringKevWithArray(StringKevWithArray<SIZE>&&) = delete;
  StringKevWithArray<SIZE>& operator=(const StringKevWithArray<SIZE>&) = delete;
  StringKevWithArray<SIZE>& operator=(StringKevWithArray<SIZE>&&) = delete;
  ~StringKevWithArray() = default;

  StringKevWithArray(const StringKev* str, int start, int end)
    : string_core_(str, start, end) {
    this->KevesString::set(&string_core_, 0, string_core_.size());
  }

  StringKevWithArray(const StringKev* str1, const StringKev* str2)
    : string_core_(str1, str2) {
    this->KevesString::set(&string_core_, 0, string_core_.size());
  }

  explicit StringKevWithArray(const char* str) : string_core_(str) {
    this->KevesString::set(&string_core_, 0, string_core_.size());
  }

  explicit StringKevWithArray(const QString& str) : string_core_(str) {
    this->KevesString::set(&string_core_, 0, string_core_.size());
  }

private:
  class StringCoreKevWithArray : public StringCoreKev {
  public:
    StringCoreKevWithArray() = delete;
    StringCoreKevWithArray(const StringCoreKevWithArray&) = delete;
    StringCoreKevWithArray(StringCoreKevWithArray&&) = delete;
    StringCoreKevWithArray& operator=(const StringCoreKevWithArray&) = delete;
    StringCoreKevWithArray& operator=(StringCoreKevWithArray&&) = delete;
    ~StringCoreKevWithArray() = default;

    StringCoreKevWithArray(const StringKev* other, int start, int size)
      : StringCoreKev(size) {
      const QChar* other_array(other->data() + start);
      std::copy(other_array, other_array + size, raw_data_);
    }
    
    StringCoreKevWithArray(const StringKev* str1, const StringKev* str2)
      : StringCoreKev(str1->size() + str2->size()) {
      Q_ASSERT(SIZE >= size_);
      
      const QChar* str1_array(str1->data());
      int str1_size(str1->size());
      std::copy(str1_array, str1_array + str1_size, raw_data_);
      
      const QChar* str2_array(str2->data());
      int str2_size(str2->size());
      std::copy(str2_array, str2_array + str2_size, raw_data_ + str1_size);
    }
    
    explicit StringCoreKevWithArray(const char* str) : StringCoreKev(strlen(str)) {
      int size(this->size());
      Q_ASSERT(SIZE >= size);
      
      // for (int idx(0); idx < size; ++idx) raw_data_[idx] = str[idx];
      std::copy_n(str, size, raw_data_);
      std::fill_n(raw_data_ + size, SIZE - size, QChar(0));
    }
    
    explicit StringCoreKevWithArray(const QString& str) : StringCoreKev(str.length()) {
      int size(this->size());
      Q_ASSERT(SIZE >= size);
      // const QChar* org_data(str.constData());
      // for (int idx(0); idx < size; ++idx) raw_data_[idx] = org_data[idx];
      std::copy_n(str.constData(), size, raw_data_);
      std::fill_n(raw_data_ + size, SIZE - size, QChar(0));
    }
    
  private:
    QChar raw_data_[SIZE];
  } string_core_;
};
