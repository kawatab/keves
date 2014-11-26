/* string_kev.cpp - strings for Keves
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


#include "kev/string.hpp"

#include "keves_gc.hpp"


// class StringCoreKev ----------------------------------------
void StringCoreKev::CopyArray(const StringCoreKev* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + (sizeof(QChar) * size_ + sizeof(quintptr) - 1) / sizeof(quintptr));
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}
  
QChar* StringCoreKev::array() {
  return size_ ? reinterpret_cast<QChar*>(this + 1) : nullptr;
}

const QChar* StringCoreKev::array() const {
  return size_ ? reinterpret_cast<const QChar*>(this + 1) : nullptr;
}

bool StringCoreKev::IsEmpty() const {
  return !array();
}

StringCoreKev* StringCoreKev::Copy(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    std::copy(org_data, org_data + len, string->array());
    return string;
  };

  return gc->Make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::ToCaseFolded(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toCaseFolded();
    return string;
  };

  return gc->Make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::ToLower(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toLower();
    return string;
  };

  return gc->Make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::ToUpper(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toUpper();
    return string;
  };

  return gc->Make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::ToTitleCase(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    bool first(true);

    for (int i(0); i < len; ++i) {
      dist_data[i] = first ? org_data[i].toTitleCase() : org_data[i].toLower();
      first = org_data[i].isSpace() || org_data[i].isNumber();
    }

    return string;
  };

  return gc->Make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::Make(KevesGC* gc, const QString& str) {
  auto ctor = [&str](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(str.size()));
    string->set(str);
    return string;
  };
  
  return gc->Make(ctor, alloc_size(str.size()));
}

StringCoreKev* StringCoreKev::Make(KevesGC* gc, int size) {
  auto ctor = [size](void* ptr) { return new(ptr) StringCoreKev(size); };
  return gc->Make(ctor, alloc_size(size));
}

void StringCoreKev::set(const QString& str) {
  const QChar* org_data(str.data());
  std::copy(org_data, org_data + size_, array());
}

// class KevesString ----------------------------------------
KevesString::KevesString() : KevesString(nullptr, 0, 0) {
}

KevesString::KevesString(StringCoreKev* core, int idx, int len)
  : core_(core), idx_(idx), len_(len)  {
}

void KevesString::Set(StringCoreKev* core, int idx, int len) {
  this->core_ = core;
  this->idx_ = idx;
  this->len_ = len;
}

KevesString KevesString::Append(KevesGC* gc, const KevesString& other) const {
  int this_size(this->size());
  int other_size(other.size());
  StringCoreKev* core(StringCoreKev::Make(gc, this_size + other_size));
  QChar* dist_iter(core->array());
  {
    const QChar* org_iter(this->data());
    std::copy(org_iter, org_iter + this_size, dist_iter);
  }
  {
    const QChar* org_iter(other.data());
    std::copy(org_iter, org_iter + other_size, dist_iter + this_size);
  }
  return KevesString(core, 0, core->size());
}

QChar KevesString::At(int idx) const {
  Q_ASSERT(idx >= 0 && idx < len_);
  return data() ? data()[idx] : QChar(0);
}

const StringCoreKev* KevesString::core() const {
  return core_;
}

StringCoreKev* KevesString::core() {
  return core_;
}

QChar* KevesString::data() {
  return core_ ? core_->array() + idx_ : nullptr;
}

const QChar* KevesString::data() const {
  return core_ ? core_->array() + idx_ : nullptr;
}

void KevesString::Fill(QChar chr) {
  std::fill_n(data(), size(), chr);
}

bool KevesString::IsEmpty() const {
  return !data();
}

int KevesString::Index() const {
  return idx_;
}

int KevesString::IndexOf(char chr) const {
  return QString::fromRawData(data(), len_).indexOf(chr);
}

int KevesString::IndexOf(const char* chr) const {
  return QString::fromRawData(data(), len_).indexOf(chr);
}

int KevesString::IndexOf(const char* chr, int pos) const {
  return QString::fromRawData(data(), len_).indexOf(chr, pos);
}

bool KevesString::Equals(const char* other) const {
  const QChar* ptr(this->data());
  const QChar* end_ptr(ptr + this->len_);

  for (; ptr != end_ptr; ++ptr, ++other) {
    if (*other == '\0' || *ptr != QChar(*other))
      return false;
  }

  return *other == '\0';
}

bool KevesString::Equals(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator==(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::IsLT(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator<(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::IsGT(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator>(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::IsLTE(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator<=(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::IsGTE(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator>=(QString::fromRawData(other.data(), other.len_));
}

KevesString KevesString::Left(int len) const {
  return (len < 0 || len > len_) ? *this : KevesString(core_, idx_, len);
}

KevesString KevesString::Mid(int pos, int len) const {
  return (pos < 0 || pos >= len_) ? KevesString() :
    KevesString(core_,
		pos + idx_,
		(len < 0 || pos + len > len_) ? len_ - pos : len);
}

KevesString KevesString::Right(int len) const {
  return (len < 0 || len > len_) ?
    *this : KevesString(core_, idx_ + len_ - len, len);
}

double KevesString::ToDouble(bool* ok = nullptr) const {
  return QString::fromRawData(data(), len_).toDouble(ok);
}

int KevesString::ToInt(bool* ok = nullptr, int base = 10) const {
  return QString::fromRawData(data(), len_).toInt(ok, base);
}

QByteArray KevesString::ToLocal8Bit() const {
  return QString::fromRawData(data(), len_).toLocal8Bit();
}

QString KevesString::ToQString() const {
  return QString::fromRawData(data(), len_);
}

unsigned int KevesString::ToUInt(bool* ok = nullptr, int base = 10) const {
  return QString::fromRawData(data(), len_).toUInt(ok, base);
}

// class StringKev ----------------------------------------
StringKev::StringKev() : StringKev(KevesString()) {
}
  
StringKev::StringKev(const KevesString& str) : MutableKev(TYPE), KevesString(str) {
}

StringKev::StringKev(StringCoreKev* core, int idx, int len)
  : StringKev(KevesString(core, idx, len)) {
}

void StringKev::CopyFrom(const StringKev& org) {
  this->core_ = org.core_;
  this->idx_ = org.idx_;
  this->len_ = org.len_;
}

void StringKev::CopyFrom(StringKev&& org) {
  this->core_ = org.core_;
  this->idx_ = org.idx_;
  this->len_ = org.len_;
}

StringKev StringKev::Left(int len) {
  return StringKev(this->KevesString::Left(len));
}

StringKev StringKev::Left(int len) const {
  return StringKev(this->KevesString::Left(len));
}

StringKev StringKev::Mid(int pos, int len) {
  return StringKev(this->KevesString::Mid(pos, len));
}

StringKev StringKev::Mid(int pos, int len) const {
  return StringKev(this->KevesString::Mid(pos, len));
}

StringKev StringKev::Right(int len) {
  return StringKev(this->KevesString::Right(len));
}

StringKev StringKev::Right(int len) const {
  return StringKev(this->KevesString::Right(len));
}

StringKev StringKev::Copy(KevesGC* gc) const {
  StringCoreKev* core(this->core()->Copy(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

void StringKev::Set(int idx, QChar chr) {
  Q_ASSERT(idx >= 0 && idx < len_);
  this->data()[idx] = chr;
}

StringKev StringKev::ToCaseFolded(KevesGC* gc) const {
  StringCoreKev* core(this->core()->ToCaseFolded(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::ToLower(KevesGC* gc) const {
  StringCoreKev* core(this->core()->ToLower(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::ToUpper(KevesGC* gc) const {
  StringCoreKev* core(this->core()->ToUpper(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::ToTitleCase(KevesGC* gc) const {
  StringCoreKev* core(this->core()->ToTitleCase(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev* StringKev::Make(KevesGC* gc, int size) {
  return gc->Make(ctor(gc, size), alloc_size(nullptr));
}

StringKev* StringKev::Make(KevesGC* gc, const QString& str) {
  return gc->Make(ctor(gc, str), alloc_size(nullptr));
}

StringKev* StringKev::MakeSubstring(KevesGC* gc, const StringKev* str, int start, int end) {
  auto ctor = [str, start, end](void* ptr) {
    return new(ptr) StringKev(str->Mid(start, end - start));
  };
    
  return gc->Make(ctor, alloc_size(nullptr));
}

StringKev* StringKev::Make(KevesGC* gc, StringCoreKev* core, int idx, int len) {
  auto ctor = [core, idx, len](void* ptr) {
    return new(ptr) StringKev(core, idx, len);
  };
    
  return gc->Make(ctor, alloc_size(nullptr));
}

StringKev StringKev::Append(KevesGC* gc, const StringKev& str) const {
  return StringKev(this->KevesString::Append(gc, static_cast<KevesString>(str)));
}
