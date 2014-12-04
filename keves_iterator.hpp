// keves/keves_iterator.cpp - iterators for Keves
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

#include "keves_value.hpp"

class const_KevesIterator;
class KevesVM;

typedef void (*vm_func)(KevesVM*, const_KevesIterator);


class const_KevesIterator {
public:
  const_KevesIterator() = default;
  const_KevesIterator(const const_KevesIterator&) = default;
  const_KevesIterator(const_KevesIterator&&) = default;
  const_KevesIterator& operator=(const const_KevesIterator&) = default;
  const_KevesIterator& operator=(const_KevesIterator&&) = default;
  ~const_KevesIterator() = default;

  explicit const_KevesIterator(const KevesValue* ptr)
  : ptr_(const_cast<KevesValue*>(ptr)) {
  }

  const KevesValue& operator*() const {
    return *ptr_;
  }

  const KevesValue* operator->() const {
    return ptr_;
  }

  friend bool operator==(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ == rhs.ptr_;
  }

  friend bool operator!=(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ != rhs.ptr_;
  }

  friend bool operator<(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ < rhs.ptr_;
  }

  friend bool operator<=(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ <= rhs.ptr_;
  }

  friend bool operator>(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ > rhs.ptr_;
  }

  friend bool operator>=(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ >= rhs.ptr_;
  }

  friend const_KevesIterator operator+(const_KevesIterator lhs, int rhs) {
    return const_KevesIterator(lhs.ptr_ + rhs);
  }

  friend const_KevesIterator operator-(const_KevesIterator lhs, int rhs) {
    return const_KevesIterator(lhs.ptr_ - rhs);
  }

  friend quintptr operator-(const_KevesIterator lhs, const_KevesIterator rhs) {
    return lhs.ptr_ - rhs.ptr_;
  }

  const_KevesIterator& operator+=(int i) {
    ptr_ += i;
    return *this;
  }

  const_KevesIterator& operator-=(int i) {
    ptr_ -= i;
    return *this;
  }

  const_KevesIterator& operator++() {
    ++ptr_;
    return *this;
  }

  const_KevesIterator operator++(int) {
    const_KevesIterator temp(*this);
    ++ptr_;
    return temp;
  }

  const_KevesIterator& operator--() {
    --ptr_;
    return *this;
  }

  const_KevesIterator operator--(int) {
    const_KevesIterator temp(*this);
    --ptr_;
    return temp;
  }

protected:
  KevesValue* ptr_ = nullptr;
};

class KevesIterator : public const_KevesIterator {
public:
  KevesIterator() = default;
  KevesIterator(const KevesIterator&) = default;
  KevesIterator(KevesIterator&&) = default;
  KevesIterator& operator=(const KevesIterator&) = default;
  KevesIterator& operator=(KevesIterator&&) = default;
  ~KevesIterator() = default;

  explicit KevesIterator(KevesValue* ptr) : const_KevesIterator(ptr) {
  }

  template<class CTOR> auto make(CTOR ctor) -> decltype(ctor(nullptr)) {
    void* ptr(alloc(ctor.alloc_size()));
    return ctor(ptr);
  }
    
  template<class CTOR> auto make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
    void* ptr(alloc(size));
    return ctor(ptr);
  }
    
  KevesValue& operator*() {
    return *ptr_;
  }

  KevesValue* operator->() {
    return ptr_;
  }

  friend KevesIterator operator+(KevesIterator lhs, int rhs) {
    return KevesIterator(lhs.ptr_ + rhs);
  }

  friend KevesIterator operator-(KevesIterator lhs, int rhs) {
    return KevesIterator(lhs.ptr_ - rhs);
  }

  friend quintptr operator-(KevesIterator lhs, const_KevesIterator rhs) {
    return static_cast<const_KevesIterator const>(lhs) - rhs;
  }

  KevesIterator& operator+=(int addend) {
    ptr_ += addend;
    return *this;
  }

  KevesIterator& operator-=(int subtrahend) {
    ptr_ -= subtrahend;
    return *this;
  }

  KevesIterator& operator++() {
    ++ptr_;
    return *this;
  }

  KevesIterator operator++(int) {
    KevesIterator temp(*this);
    ++ptr_;
    return temp;
  }

  KevesIterator& operator--() {
    --ptr_;
    return *this;
  }

  KevesIterator operator--(int) {
    KevesIterator temp(*this);
    --ptr_;
    return temp;
  }

private:
  void* alloc(size_t);
};
