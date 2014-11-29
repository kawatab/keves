// keves/kev/bignum-inl.hpp - bignums for Keves
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

#include <QString>

template<class ZONE>
Bignum* Bignum::makeFromLength(ZONE* zone, int len) {
  auto ctor = [len](void *ptr) { return new(ptr) Bignum(len); };
  return zone->Make(ctor, alloc_size(len));
}

template<class ZONE>
Bignum* Bignum::makeFromString(ZONE* zone, const QString& str, int radix) {
  if (radix != 2 && radix != 8 && radix != 10 && radix != 16) 
    return nullptr;
  
  mpz_class mpz_num(str.toLocal8Bit().constData(), radix);
  int len(calculateLength(mpz_num));
  Bignum* result(makeFromLength(zone, len));
  
  mpz_export(result->array(), nullptr, -1, sizeof(bg_uint), 0, 0,
	     mpz_num.get_mpz_t());
  
  return result;
}

template<class ZONE>
Bignum* Bignum::exponentiationWithBase10(ZONE* zone, int a) {
  mpz_class mpz;
  mpz_ui_pow_ui(mpz.get_mpz_t(), 10, a);
  return makeFromMPZ(zone, mpz);
}

template<class ZONE>
Bignum* Bignum::makeFromMPZ(ZONE* zone, const mpz_class& num) {
  int len(calculateLength(num));

  auto ctor = [&num, len](void* ptr) {
    Bignum* result(new(ptr) Bignum(len));
    
    mpz_export(result->array(), nullptr, -1, sizeof(bg_uint), 0, 0,
	       num.get_mpz_t());
    
    return result;
  };

  return zone->Make(ctor, alloc_size(len));
}
