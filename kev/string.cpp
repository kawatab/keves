// keves/kev/string.cpp - strings for Keves
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


#include "kev/string.hpp"

#include "keves_gc.hpp"


// class StringCoreKev ----------------------------------------
void StringCoreKev::copyArray(const StringCoreKev* org) {
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

bool StringCoreKev::isEmpty() const {
  return !array();
}

StringCoreKev* StringCoreKev::copy(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    std::copy(org_data, org_data + len, string->array());
    return string;
  };

  return gc->make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::toCaseFolded(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toCaseFolded();
    return string;
  };

  return gc->make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::toLower(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toLower();
    return string;
  };

  return gc->make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::toUpper(KevesGC* gc, const QChar* org_data, int len) const {
  auto ctor = [org_data, len](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(len));
    QChar* dist_data(string->array());
    for (int i(0); i < len; ++i) dist_data[i] = org_data[i].toUpper();
    return string;
  };

  return gc->make(ctor, alloc_size(len));
}

StringCoreKev* StringCoreKev::toTitleCase(KevesGC* gc, const QChar* org_data, int len) const {
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

  return gc->make(ctor, alloc_size(len));
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

void KevesString::set(StringCoreKev* core, int idx, int len) {
  this->core_ = core;
  this->idx_ = idx;
  this->len_ = len;
}

KevesString KevesString::append(KevesGC* gc, const KevesString& other) const {
  int this_size(this->size());
  int other_size(other.size());
  StringCoreKev* core(StringCoreKev::make(gc, this_size + other_size));
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

QChar KevesString::at(int idx) const {
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

void KevesString::fill(QChar chr) {
  std::fill_n(data(), size(), chr);
}

bool KevesString::isEmpty() const {
  return !data();
}

int KevesString::index() const {
  return idx_;
}

int KevesString::indexOf(char chr) const {
  return QString::fromRawData(data(), len_).indexOf(chr);
}

int KevesString::indexOf(const char* chr) const {
  return QString::fromRawData(data(), len_).indexOf(chr);
}

int KevesString::indexOf(const char* chr, int pos) const {
  return QString::fromRawData(data(), len_).indexOf(chr, pos);
}

bool KevesString::equals(const char* other) const {
  const QChar* ptr(this->data());
  const QChar* end_ptr(ptr + this->len_);

  for (; ptr != end_ptr; ++ptr, ++other) {
    if (*other == '\0' || *ptr != QChar(*other))
      return false;
  }

  return *other == '\0';
}

bool KevesString::equals(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator==(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::isLT(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator<(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::isGT(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator>(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::isLTE(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator<=(QString::fromRawData(other.data(), other.len_));
}

bool KevesString::isGTE(const KevesString& other) const {
  return QString::fromRawData(this->data(), this->len_)
    .operator>=(QString::fromRawData(other.data(), other.len_));
}

KevesString KevesString::left(int len) const {
  return (len < 0 || len > len_) ? *this : KevesString(core_, idx_, len);
}

KevesString KevesString::mid(int pos, int len) const {
  return (pos < 0 || pos >= len_) ? KevesString() :
    KevesString(core_,
		pos + idx_,
		(len < 0 || pos + len > len_) ? len_ - pos : len);
}

KevesString KevesString::right(int len) const {
  return (len < 0 || len > len_) ?
    *this : KevesString(core_, idx_ + len_ - len, len);
}

double KevesString::toDouble(bool* ok = nullptr) const {
  return QString::fromRawData(data(), len_).toDouble(ok);
}

int KevesString::toInt(bool* ok = nullptr, int base = 10) const {
  return QString::fromRawData(data(), len_).toInt(ok, base);
}

QByteArray KevesString::toLocal8Bit() const {
  return QString::fromRawData(data(), len_).toLocal8Bit();
}

QString KevesString::toQString() const {
  return QString::fromRawData(data(), len_);
}

unsigned int KevesString::toUInt(bool* ok = nullptr, int base = 10) const {
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

void StringKev::copyFrom(const StringKev& org) {
  this->core_ = org.core_;
  this->idx_ = org.idx_;
  this->len_ = org.len_;
}

void StringKev::copyFrom(StringKev&& org) {
  this->core_ = org.core_;
  this->idx_ = org.idx_;
  this->len_ = org.len_;
}

StringKev StringKev::left(int len) {
  return StringKev(this->KevesString::left(len));
}

StringKev StringKev::left(int len) const {
  return StringKev(this->KevesString::left(len));
}

StringKev StringKev::mid(int pos, int len) {
  return StringKev(this->KevesString::mid(pos, len));
}

StringKev StringKev::mid(int pos, int len) const {
  return StringKev(this->KevesString::mid(pos, len));
}

StringKev StringKev::right(int len) {
  return StringKev(this->KevesString::right(len));
}

StringKev StringKev::right(int len) const {
  return StringKev(this->KevesString::right(len));
}

StringKev StringKev::copy(KevesGC* gc) const {
  StringCoreKev* core(this->core()->copy(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

void StringKev::set(int idx, QChar chr) {
  Q_ASSERT(idx >= 0 && idx < len_);
  this->data()[idx] = chr;
}

StringKev StringKev::toCaseFolded(KevesGC* gc) const {
  StringCoreKev* core(this->core()->toCaseFolded(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::toLower(KevesGC* gc) const {
  StringCoreKev* core(this->core()->toLower(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::toUpper(KevesGC* gc) const {
  StringCoreKev* core(this->core()->toUpper(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::toTitleCase(KevesGC* gc) const {
  StringCoreKev* core(this->core()->toTitleCase(gc, data(), size()));
  return StringKev(core, 0, core->size());
}

StringKev StringKev::append(KevesGC* gc, const StringKev& str) const {
  return StringKev(this->KevesString::append(gc, static_cast<KevesString>(str)));
}
