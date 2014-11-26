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
#include "value/value.hpp"

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
  StringCoreKev* Copy(KevesGC* gc, const QChar*, int) const;
  bool IsEmpty() const;

  int size() const {
    return size_;
  }

  StringCoreKev* ToCaseFolded(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* ToLower(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* ToTitleCase(KevesGC* gc, const QChar*, int) const;
  StringCoreKev* ToUpper(KevesGC* gc, const QChar*, int) const;

  template<class ZONE>
  static StringCoreKev* Make(ZONE* zone, int size);

  template<class ZONE>
  static StringCoreKev* Make(ZONE* zone, const QString& str);

  template<class ZONE>
  static StringCoreKev* MakeAsImmovableT(ZONE* zone, const QString& str);

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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<StringCoreKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE*, MutableKev* kev) {
    return VariableLengthKev<StringCoreKev>::From(kev)->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + (sizeof(QChar) * size_ + sizeof(quintptr) - 1) / sizeof(quintptr);
  }

  void CopyArray(const StringCoreKev* org);
  
  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class /*BASE*/, class STACK>
  static void PushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    return;
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void WriteObject(const LIST&, STREAM& out, KevesValue value) {
    const StringCoreKev* string(value);
    int size(string->size());

    out << static_cast<uioword>(string->type())
	<< static_cast<ioword>(size);

    const QChar* array(string->array());
    for (int i(0); i < size; ++i) out << array[i];
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    StringCoreKev* string(Make(gc, size));
    QChar* array(string->array());
    for (int i(0); i < size; ++i) in >> array[i];
    return string;
  }

  template<class /*BASE*/, class LIST>
  static void RevertObject(const LIST&, MutableKevesValue) {
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

  KevesString Append(KevesGC* gc, const KevesString&) const;
  QChar At(int) const;
  StringCoreKev* core();
  const StringCoreKev* core() const;
  QChar* data();
  const QChar* data() const;
  bool Equals(const char*) const;
  bool Equals(const KevesString&) const;
  void Fill(QChar);
  int Index() const;
  int IndexOf(char) const;
  int IndexOf(const char*) const;
  int IndexOf(const char*, int) const;
  bool IsEmpty() const;
  bool IsGT(const KevesString&) const;
  bool IsGTE(const KevesString&) const;
  bool IsLT(const KevesString&) const;
  bool IsLTE(const KevesString&) const;
  KevesString Left(int) const;
  KevesString Mid(int, int) const;
  KevesString Right(int) const ;
  void Set(StringCoreKev*, int, int);

  int size() const {
    return len_;
  }

  double ToDouble(bool*) const;
  int ToInt(bool*, int) const;
  QByteArray ToLocal8Bit() const;
  QString ToQString() const;
  unsigned int ToUInt(bool*, int) const;

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

  StringKev Append(KevesGC* gc, const StringKev&) const;
  StringKev Copy(KevesGC* gc) const;
  void CopyFrom(const StringKev& org);
  void CopyFrom(StringKev&& org);

  KevesString& GetKevesString() {
    return *static_cast<KevesString*>(this);
  }

  const KevesString& GetKevesString() const {
    return *static_cast<const KevesString*>(this);
  }

  StringKev Left(int);
  StringKev Left(int) const;
  StringKev Mid(int, int);
  StringKev Mid(int, int) const;
  StringKev Right(int);
  StringKev Right(int) const;
  void Set(int, QChar);
  void Set(const StringKev&);
  void Set(StringKev&&);

  int size() const {
    return KevesString::size();
  }

  StringKev ToCaseFolded(KevesGC* gc) const;
  StringKev ToLower(KevesGC* gc) const;
  StringKev ToTitleCase(KevesGC* gc) const;
  StringKev ToUpper(KevesGC* gc) const;

  template<class ZONE>
  static StringKev* Make(ZONE* zone, int);

  template<class ZONE>
  static StringKev* Make(ZONE* zone, const QString&);
  // static StringKev* make(KevesIterator*, const QString&);
  // static StringKev* make(KevesValue*, size_t, const QString&);
  template<class ZONE>
  static StringKev* Make(ZONE* zone, StringCoreKev*, int, int);

  template<class ZONE>
  static StringKev* MakeSubstring(ZONE* zone, const StringKev*, int, int);

  template<class ZONE>
  static std::function<StringKev*(void*)> ctor(ZONE* zone, int size) {
    return [zone, size](void* ptr) {
      StringCoreKev* core(StringCoreKev::Make(zone, size));
      return new(ptr) StringKev(core, 0, core->size());
    };
  }

  template<class ZONE>
  static std::function<StringKev*(void*)> ctor(ZONE* zone, const QString& str) {
    return [zone, &str](void* ptr) {
      StringCoreKev* core(StringCoreKev::Make(zone, str));
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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<StringKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<StringKev>* string(FixedLengthKev<StringKev>::From(kev));
    VariableLengthKev<StringCoreKev>* core(VariableLengthKev<StringCoreKev>::From(string->core_));
    string->core_ = zone->Copy(core);
    return string->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const StringKev* string(value);
    BASE::PushValue(pending, string->core_);
  }

  template<class BASE, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const StringKev* string(value);

    out << static_cast<uioword>(string->type())
	<< BASE::IndexAddress(list, string->core_)
	<< static_cast<ioword>(string->idx_)
	<< static_cast<ioword>(string->len_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword core;
    ioword idx, len;
    in >> core >> idx >> len;
    return Make(gc,
		KevesValue::template FromUioword<StringCoreKev>(core),
		idx,
		len);
  }
  
  template<class BASE, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue kev) {
    StringKev* string(kev);
    BASE::RevertValue(object_list, &string->core_);
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
    this->KevesString::Set(&string_core_, 0, string_core_.size());
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
