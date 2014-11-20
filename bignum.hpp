/* Keves/bignum.hpp - bignums for Keves
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

#include <cmath>

#include <gmpxx.h>
#include "keves_fixnum.hpp"
#include "keves_iterator.hpp"
#include "keves_value.hpp"

typedef qint32 bg_int;
typedef quint32 bg_uint;
typedef qint64 bg_long;
typedef quint64 bg_ulong;

class BignumInt;
class KevesGC;


class Bignum : public MutableKev {
public:
  static constexpr kev_type TYPE = BIGNUM;

  Bignum() = delete;
  Bignum(const Bignum&) = delete;
  Bignum(Bignum&&) = delete;
  Bignum& operator=(const Bignum&) = delete;
  Bignum& operator=(Bignum&&) = delete;
protected:
  ~Bignum() = default;
  
public:
  explicit Bignum(int len);

  void CopyFrom(const Bignum& org);
  // void CopyFrom(Bignum&&); // not suitable for variable-length object
  bool equals(const Bignum& other) const;
  bool equals(bg_uint other) const;
  void initMPZ(mpz_class* mpz_num) const;
  bool isEven() const;
  bool isGT(const Bignum& other) const;
  bool isGT(bg_uint other) const;
  bool isGTE(const Bignum& other) const;
  bool isGTE(bg_uint other) const;
  bool isLT(const Bignum& other) const;
  bool isLT(bg_uint other) const;
  bool isLTE(const Bignum& other) const;
  bool isLTE(bg_uint other) const;
  bool isOdd() const;
  bool isOne() const;
  bool isZero() const;

  int size() const {
    return len_;
  }
  
  QString toBinary(int radix) const;
  QString toQString() const;
  bg_uint toUInt(bool* ok) const;
  bg_ulong toULong(bool* ok) const;

  static int calculateLength(const mpz_class& mpz_num);
  static Bignum* exponentiationWithBase10(KevesGC* gc, int a);
  static Bignum* ldexp(KevesGC* gc, bg_ulong significand, int exponent);
  static Bignum* makeFromInt(KevesGC* gc, bg_uint n);
  static Bignum* makeFromLong(KevesGC* gc, bg_ulong n);
  static Bignum* makeFromMPZ(KevesGC* gc, const mpz_class& num);
  static Bignum* makeFromString(KevesGC* gc, const QString& str, int radix = 10);

protected:
  void clear();
  
  int len() const {
    return len_;
  }

  void set_len(int len) {
    Q_ASSERT(len > 0 && len <= len_);
    len_ = len;
  }
  
private:
  bg_uint* array();
  const bg_uint* array() const;
  bg_uint* begin();
  const bg_uint* begin() const;
  bg_uint* end();
  const bg_uint* end() const;

  static size_t alloc_size(int len) {
    return sizeof(Bignum) + sizeof(bg_uint) * len;
  }
  
  static int calculateLength(bg_uint a, int b);
  // static void clearUIntArray(bg_uint*, int);
  static Bignum* makeFromLength(KevesGC* gc, int len);
  
  int len_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const Bignum*>(kev)->len_);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<Bignum>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE*, MutableKev* kev) {
    return VariableLengthKev<Bignum>::From(kev)->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + (sizeof(bg_uint) * len_ + sizeof(quintptr) - 1) / sizeof(quintptr);
  }
  
  void CopyArray(const Bignum* org);
  
  ////////////////////////////////////////////////////////////
  // Section For serialize begin !!!                        //
  ////////////////////////////////////////////////////////////

public:
  template<class /*IO*/, class STACK>
  static void PushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    return;
  }

  template<class /*IO*/, class LIST, class STREAM>
  static void WriteObject(const LIST& /*list*/, STREAM& out, KevesValue value) {
    const Bignum* bignum(value);
    int size(bignum->size());

    out << static_cast<uioword>(bignum->type())
	<< static_cast<ioword>(size);

    const bg_uint* array(bignum->array());
    for (int i(0); i < size; ++i) out << array[i];
  }

  template<class /*IO*/, class LIST, class GC>
  static Kev* ReadObject(LIST& in, GC* gc) {
    ioword size;
    in >> size;
    Bignum* bignum(makeFromLength(gc, size));
    bg_uint* array(bignum->array());
    for (int i(0); i < size; ++i) in >> array[i];
    return bignum;
  }

  template<class /*IO*/, class LIST>
  static void RevertObject(const LIST&, MutableKevesValue /*value*/) {
    return;
  }
};


class BignumInt : public Bignum {
public:
  BignumInt() = delete;
  BignumInt(const BignumInt&);
  BignumInt(BignumInt&&) = delete;
  BignumInt& operator=(const BignumInt&) = default;
  BignumInt& operator=(BignumInt&&) = default;
  ~BignumInt() = default;

  explicit BignumInt(bg_uint n);

private:
  bg_uint num_;
};


class BignumLong : public Bignum {
public:
  BignumLong() = delete;
  BignumLong(const BignumLong&);
  BignumLong(BignumLong&&) = delete;
  BignumLong& operator=(const BignumLong&) = default;
  BignumLong& operator=(BignumLong&&) = default;
  ~BignumLong() = default;

  explicit BignumLong(bg_ulong n);
  
private:
  bg_uint num_[2];
};
