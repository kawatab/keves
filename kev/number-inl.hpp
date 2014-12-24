// keves/kev/number-inl.hpp - pairs for Keves
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


template<class ZONE>
RationalNumberKev* RationalNumberKev::make(ZONE* zone) {
  auto ctor = [](void* ptr) { return new(ptr) RationalNumberKev(); };
  return zone->make(ctor, alloc_size(nullptr));
}

template<class ZONE>
FlonumKev* FlonumKev::make(ZONE* zone, double d) {
  auto ctor = [d](void* ptr) { return new(ptr) FlonumKev(d); };
  return zone->make(ctor, alloc_size(nullptr));
}

template<class ZONE>
ExactComplexNumberKev* ExactComplexNumberKev::make(ZONE* zone) {
  auto ctor = [](void* ptr) { return new(ptr) ExactComplexNumberKev(); };
  return zone->make(ctor, alloc_size(nullptr));
}

template<class ZONE>
InexactComplexNumberKev* InexactComplexNumberKev::makeFromFlonums(ZONE* zone, FlonumKev real, FlonumKev imag) {
  auto ctor = [real, imag](void* ptr) {
    return new(ptr) InexactComplexNumberKev(real, imag);
  };

  return zone->make(ctor, alloc_size(nullptr));
}

template<class ZONE>
RationalNumberKev::RationalNumberKev(ZONE* zone, const mpq_class& mpq_num)
  : RationalNumberKev(sgn(mpq_num) < 0,
		      Bignum::makeFromMPZ(zone, mpq_num.get_num()),
		      Bignum::makeFromMPZ(zone, mpq_num.get_den())) {}

template<class ZONE>
RationalNumberKev* RationalNumberKev::makeFromString(ZONE* zone, StringKev str) {
  int i;
  int pos(str.indexOf("/"));
  QString str1;
  QString str2;
  RationalNumberKev* number(RationalNumberKev::make(zone));

  if (pos > 0) { // rational
    str1.operator=(str.left(pos).toQString());
    str2.operator=(str.right(str.size() - pos - 1).toQString());

    number->copyFrom(str1.left(1) == "+" ?
		     RationalNumberKev(false,
			 Bignum::makeFromString(zone,
						str1.right(str1.size() - 1)),
			 Bignum::makeFromString(zone, str2)) :
		     str1.left(1) == "-" ?
		     RationalNumberKev(true,
			 Bignum::makeFromString(zone,
						str1.right(str1.size() - 1)),
			 Bignum::makeFromString(zone, str2)) :
		     RationalNumberKev(false,
			 Bignum::makeFromString(zone, str1),
			 Bignum::makeFromString(zone, str2)));
  } else { // not rational
    if ((pos = str.indexOf("e")) < 0 &&
	(pos = str.indexOf("s")) < 0 &&
	(pos = str.indexOf("f")) < 0 &&
	(pos = str.indexOf("d")) < 0 &&
	(pos = str.indexOf("l")) < 0) {
      str1.operator=(str.toQString());
      i = 0;
    } else {
      bool ok;
      str1.operator=(str.left(pos).toQString());
      str2.operator=(str.right(str.size() - pos - 1).toQString());
      i = -str2.toInt(&ok);

      if (!ok)
	return nullptr; // error exponantial part is invalid
    }

    pos = str1.indexOf(".");

    if (pos > 0) {
      str1.remove(pos, 1);
      i += str1.size() - pos;
    }

    if (i < 0) {
      str2.operator=({ -i, QChar('0') });
      str1.append(str2);
      i = 0;
    }

    number->copyFrom(str1.left(1) == "+" ?
		     RationalNumberKev(false,
			 Bignum::makeFromString(zone,
						str1.right(str1.size() - 1)),
			 Bignum::exponentiationWithBase10(zone, i)) :
		     str1.left(1) == "-" ?
		     RationalNumberKev(true,
			 Bignum::makeFromString(zone, str1.right(str1.size() - 1)),
			 Bignum::exponentiationWithBase10(zone, i)) :
		     RationalNumberKev(false,
			 Bignum::makeFromString(zone, str1),
			 Bignum::exponentiationWithBase10(zone, i)));
  }

  if (number->denominator()->isZero()) {
    Q_ASSERT(0);
    return number;
  }
  
  number->reduce(zone);
  return number;
}

template<class ZONE>
void RationalNumberKev::reduce(ZONE* zone) {
  mpq_class mpq_num(makeMPQ());
  mpq_num.canonicalize();
  this->copyFrom(RationalNumberKev(zone, mpq_num));
}
