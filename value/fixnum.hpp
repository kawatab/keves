/* keves_fixnum.hpp - fixnum for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include <limits>

#include "value/value.hpp"

typedef qint32 fx_int;
typedef qint64 fx_long;

class KevesFixnum {
public:
  static constexpr qint32 MAX_VALUE = 0x1fffffff;
  static constexpr qint32 MIN_VALUE = -0x20000000;

  KevesFixnum() = default;
  KevesFixnum(const KevesFixnum&) = default;
  KevesFixnum(KevesFixnum&&) = default;
  KevesFixnum& operator=(const KevesFixnum&) = default;
  KevesFixnum& operator=(KevesFixnum&&) = delete;
  ~KevesFixnum() = default;

  explicit constexpr KevesFixnum(qint16 i) : val_(i) {
  }

  explicit constexpr KevesFixnum(qint32 i) : val_(i) {
  }

  explicit constexpr KevesFixnum(double d) : val_(d) {
  }

  explicit KevesFixnum(KevesValue qev)
    : val_(qev.toUIntPtr() >> 2) {
    Q_ASSERT(qev.IsFixnum());
  }
  
  template<typename Other> explicit KevesFixnum(Other) = delete;

  operator qint32() const {
    return val_;
  }

  operator KevesValue() const {
    return KevesValue(reinterpret_cast<Kev*>(val_ << 2 | 3));
  }

  bool isNegative() const {
    return val_ < 0;
  }

  bool isPositive() const {
    return val_ > 0;
  }

  bool isZero() const {
    return val_ == 0;
  }

  KevesFixnum& operator++() {
    ++val_;
    return *this;
  }

  KevesFixnum operator++(int) {
    KevesFixnum temp(*this);
    ++val_;
    return temp;
  }

  KevesFixnum& operator--() {
    --val_;
    return *this;
  }

  KevesFixnum operator--(int) {
    KevesFixnum temp(*this);
    --val_;
    return temp;
  }

  KevesFixnum& operator+=(qint32 addend)  {
    val_ += addend;
    return *this;
  }

  KevesFixnum& operator-=(qint32 subtrahend)  {
    val_ -= subtrahend;
    return *this;
  }

  friend bool operator==(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ == rhs.val_;
  }

  friend bool operator!=(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ != rhs.val_;
  }

  friend bool operator>(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ > rhs.val_;
  }
  
  friend bool operator>=(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ >= rhs.val_;
  }
  
  friend bool operator<(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ < rhs.val_;
  }
  
  friend bool operator<=(KevesFixnum lhs, KevesFixnum rhs) {
    return lhs.val_ <= rhs.val_;
  }

  static bool isInRange(qint32 i) {
    return i <= MAX_VALUE && i >= MIN_VALUE;
  }

private:
  fx_int val_ = 0;
};
