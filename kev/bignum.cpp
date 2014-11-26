// Keves/bignum.cpp - bignums for Keves
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


#include "bignum.hpp"

#include <memory>
#include <QString>

#include "keves_gc.hpp"


Bignum::Bignum(int len)
  : MutableKev(TYPE), len_(len) {
  clear();
}

bg_uint* Bignum::array() {
  return reinterpret_cast<bg_uint*>(static_cast<Bignum*>(this) + 1);
}

const bg_uint* Bignum::array() const {
  return reinterpret_cast<const bg_uint*>(static_cast<const Bignum*>(this) + 1);
}

bg_uint* Bignum::begin() {
  return array();
}

const bg_uint* Bignum::begin() const {
  return array();
}

bg_uint* Bignum::end() {
  return array() + len_;
}

const bg_uint* Bignum::end() const {
  return array() + len_;
}

void Bignum::CopyArray(const Bignum* org) {
  const quintptr* org_begin(reinterpret_cast<const quintptr*>(org + 1));
  const quintptr* org_end(org_begin + (sizeof(bg_uint) * len_ + sizeof(quintptr) - 1) / sizeof(quintptr));
  quintptr* this_begin(reinterpret_cast<quintptr*>(this + 1));
  std::copy(org_begin, org_end, this_begin);
}
  
void Bignum::initMPZ(mpz_class* mpz_num) const {
  mpz_import(mpz_num->get_mpz_t(), len_, -1, sizeof(bg_uint), 0, 0, array());
}

QString Bignum::toBinary(int radix) const {
  Q_ASSERT(len_ >= 0);

  const bg_uint* lowest(array());

  if (len_ == 0 || (len_ == 1 && *lowest == 0))
    return QString('0');

  constexpr int BIN(2);
  constexpr int OCT(8);
  constexpr int HEX(16);
  int pos(len_ - 1);
  const bg_uint* itr(lowest + pos);
  QString str;

  auto append_oct = [&](bg_uint i) {
    constexpr int FIX_LEN(10);
    QString result(FIX_LEN, '0');
    QString body(QString::number(i, OCT));
    int len(body.length());
    result.replace(FIX_LEN - len, len, body);
    str.append(result);
  };

  if (radix == BIN) {
    constexpr int FIX_LEN(32);
    
    do {
      QString result(FIX_LEN, '0');
      QString body(QString::number(*itr, BIN));
      int len(body.length());
      result.replace(FIX_LEN - len, len, body);
      str.append(result);
    } while (itr-- > lowest);
  } else if (radix == OCT) {
    int carry(0);
    
    switch (pos % 3) {
      while (itr-- > lowest) {
      case 2:
	append_oct(*itr >> 2); // 3-32 bit
	carry = (*itr & 3) << 28; // 1-2 bit -> 29-30 bit
	--itr;
	
      case 1:
	append_oct(carry + (*itr >> 4)); // 5-32 bit
	str.append(QString::number((*itr >> 1) & 7)); // 2-4 bit
	carry = (*itr & 1) << 29; // 1 bit -> 30 bit
	--itr;
	
      case 0:
	append_oct(carry + (*itr >> 3)); // 4-32 bit
	str.append(QString::number(*itr & 7)); // 1-3 bit
      }
    default:
      break;
    }
  } else if (radix == HEX) {
    constexpr int FIX_LEN(8);
    
    do {
      QString result(FIX_LEN, '0');
      QString body(QString::number(*itr, HEX));
      int len(body.length());
      result.replace(FIX_LEN - len, len, body);
      str.append(result);
    } while (itr-- > lowest);
  }
  
  int len(str.length());

  for (int i(0); i < len; ++i) {
    if (str.at(i) != '0')
      return str.right(len - i);
  }

  return str;
}

QString Bignum::toQString() const {
  mpz_class mpz_num;
  initMPZ(&mpz_num);
  int size(mpz_sizeinbase(mpz_num.get_mpz_t(), 10) + 2);
  std::unique_ptr<char[]> str(new char[size]);
  mpz_get_str(str.get(), 10, mpz_num.get_mpz_t());
  QString temp(str.get());
  return temp;
}

Bignum* Bignum::makeFromInt(KevesGC* gc, bg_uint n) {
  auto ctor = [n](void* ptr) { return new(ptr) BignumInt(n); };
  return gc->Make(ctor, sizeof(BignumInt));
}

Bignum* Bignum::makeFromLong(KevesGC* gc, bg_ulong n) {
  auto ctor = [n](void* ptr) { return new(ptr) BignumLong(n); };
  return gc->Make(ctor, sizeof(BignumLong));
}

Bignum* Bignum::makeFromMPZ(KevesGC* gc, const mpz_class& num) {
  int len(calculateLength(num));

  auto ctor = [&num, len](void* ptr) {
    Bignum* result(new(ptr) Bignum(len));
    
    mpz_export(result->array(), nullptr, -1, sizeof(bg_uint), 0, 0,
	       num.get_mpz_t());
    
    return result;
  };

  return gc->Make(ctor, alloc_size(len));
}

Bignum* Bignum::makeFromLength(KevesGC* gc, int len) {
  auto ctor = [len](void *ptr) { return new(ptr) Bignum(len); };
  return gc->Make(ctor, alloc_size(len));
}

int Bignum::calculateLength(bg_uint a, int b) {
  constexpr double log0x1p32[]
    { log(2.0) / log(0x1p32), // 1. <= x < 2.0
      log(3.0) / log(0x1p32), // 2.0 <= x < 3.0
      log(4.0) / log(0x1p32), // 3.0 <= x < 4.0
      log(5.0) / log(0x1p32), // 4.0 <= x < 5.0
      log(6.0) / log(0x1p32), // 5.0 <= x < 6.0
      log(7.0) / log(0x1p32), // 6.0 <= x < 7.0
      log(8.0) / log(0x1p32), // 7.0 <= x < 8.0
      log(9.0) / log(0x1p32), // 8.0 <= x < 9.0
      log(10.0) / log(0x1p32)
    }; // 9.0 <= x < 10.0

  return 1 + static_cast<int>(b * (log(10.0) / log(0x1p32) + log0x1p32[a]));
}

int Bignum::calculateLength(const mpz_class& mpz_num) {
  constexpr int num_bit(8 * sizeof(bg_uint));
  return (mpz_sizeinbase(mpz_num.get_mpz_t(), 2) + num_bit - 1) / num_bit;
}

Bignum* Bignum::makeFromString(KevesGC* gc, const QString& str, int radix) {
  if (radix != 2 && radix != 8 && radix != 10 && radix != 16) 
    return nullptr;
  
  mpz_class mpz_num(str.toLocal8Bit().constData(), radix);
  int len(calculateLength(mpz_num));
  Bignum* result(makeFromLength(gc, len));
  
  mpz_export(result->array(), nullptr, -1, sizeof(bg_uint), 0, 0,
	     mpz_num.get_mpz_t());
  
  return result;
}

void Bignum::clear() {
  std::fill_n(array(), len_, 0);
}

Bignum* Bignum::ldexp(KevesGC* gc, bg_ulong significand, int exponent) {
  int div(exponent / (8 * sizeof(bg_uint)));
  int mod(exponent % (8 * sizeof(bg_uint)));
  Bignum* bignum(makeFromLength(gc, div + 3));
  bg_uint* itr(bignum->begin());

  for (int i(0); i < div; ++i) *itr++ = 0;

  if (significand < (1ULL << (8 * sizeof(bg_uint)))) {
    bg_ulong last(significand << mod);

    if (last < (1ULL << (8 * sizeof(bg_uint)))) {
      *itr = last;
      bignum->len_ -= 2;
    } else {
      *itr++ = last % (1ULL << 8 * sizeof(bg_uint));
      *itr = last >> (8 * sizeof(bg_uint));
      --bignum->len_;
    }
  } else {
    bg_ulong lower(significand % (1ULL << 8 * sizeof(bg_uint)));
    bg_ulong higher(significand >> 8 * sizeof(bg_uint));
    lower <<= mod;
    higher <<= mod;
    *itr++ = lower % (1ULL << 8 * sizeof(bg_uint));

    *itr++ = (lower >> 8 * sizeof(bg_uint))
      + higher % (1ULL << 8 * sizeof(bg_uint));

    *itr = higher >> 8 * sizeof(bg_uint);
    if (*itr == 0) --bignum->len_;
  }

  return bignum;
}

Bignum* Bignum::exponentiationWithBase10(KevesGC* gc, int a) {
  mpz_class mpz;
  mpz_ui_pow_ui(mpz.get_mpz_t(), 10, a);
  return makeFromMPZ(gc, mpz);
}

void Bignum::CopyFrom(const Bignum& org) {
  this->len_ = org.len_;
  std::copy(org.begin(), org.end(), this->begin());
}

bg_uint Bignum::toUInt(bool* ok) const {
  return (*ok = len_ == 1) ? *begin() : 0;
}

bg_ulong Bignum::toULong(bool* ok) const {
  const bg_uint* itr(begin());

  return (*ok = len_ <= 2) ?
    *itr + *(itr + 1) * (1ULL << 8 * sizeof(bg_uint)) :
    0;
}

bool Bignum::isEven() const {
  return !(*this->begin() & 1);
}

bool Bignum::isOdd() const {
  return *this->begin() & 1;
}

bool Bignum::isOne() const {
  return this->len_ == 1 && *this->begin() == 1;
}

bool Bignum::isZero() const {
  return this->len_ == 1 && *this->begin() == 0;
}

bool Bignum::equals(bg_uint other) const {
  return this->len_ == 1 && *this->begin() == other;
}

bool Bignum::equals(const Bignum& other) const {
  return (this->len_ == other.len_) &&
    std::equal(this->begin(), this->end(), other.begin());
}

bool Bignum::isLT(bg_uint other) const {
  return this->len_ == 1 && *this->begin() < other;
}
  
bool Bignum::isLT(const Bignum& other) const {
  return this->len_ != other.len_ ?
    this->len_ < other.len_ :
    [this, &other] {
    const bg_uint* itr_this(this->end());
    const bg_uint* itr_other(other.end());
    
    for (int i(0); i < this->len_; ++i) {
      --itr_this;
      --itr_other;
      
      if (*itr_this < *itr_other)
	return true;
      
      if (*itr_this > *itr_other)
	return false;
    }
    
    return false;
  }();
}
  
bool Bignum::isLTE(bg_uint other) const {
  return this->len_ == 1 && *this->begin() <= other;
}

bool Bignum::isLTE(const Bignum& other) const {
  return this->len_ != other.len_ ?
    this->len_ < other.len_ :
    [this, &other] {
    const bg_uint* itr_this(this->end());
    const bg_uint* itr_other(other.end());
    
    for (int i(0); i < this->len_; ++i) {
      --itr_this;
      --itr_other;
      
      if (*itr_this < *itr_other)
	return true;
      
      if (*itr_this > *itr_other)
	return false;
    }
    
    return true;
  }();
}
  
bool Bignum::isGT(bg_uint other) const {
  return this->len_ > 1 || (this->len_ == 1 && *this->begin() > other);
}

bool Bignum::isGT(const Bignum& other) const {
  return other.isLT(*this);
}

bool Bignum::isGTE(bg_uint other) const {
  return this->len_ > 1 || (this->len_ == 1 && *this->begin() >= other);
}

bool Bignum::isGTE(const Bignum& other) const {
  return other.isGTE(*this);
}



BignumInt::BignumInt(bg_uint n) : Bignum(1), num_(n) {
}

BignumInt::BignumInt(const BignumInt& other) : BignumInt(other.num_) {
}



BignumLong::BignumLong(bg_ulong n) : Bignum(2) {
  num_[0] = n & ((1ULL << std::numeric_limits<bg_uint>::digits) - 1);
  num_[1] = n >> std::numeric_limits<bg_uint>::digits;
  if (num_[1] == 0) set_len(1);
}

BignumLong::BignumLong(const BignumLong& other) : Bignum(2) {
  this->set_len(other.len());
  this->num_[0] = other.num_[0];
  this->num_[1] = other.num_[1];
}
