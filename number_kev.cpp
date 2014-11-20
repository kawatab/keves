/* Keves/number_kev.cpp - numbers for Keves
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


#include "number_kev.hpp"

#include <iostream>
#include <sstream>

#include "keves_gc.hpp"
#include "string_kev.hpp"

namespace {
  int getSignOfNumberString(StringKev str, bool* ptr_neg) {
    QChar chr(str.At(0));
    return (*ptr_neg = chr == QChar('-')) || chr == QChar('+');
  }
  
  int getEndOfDigitString(StringKev str, int radix) {
    int pos(0);
    int len(str.size());
    
    if (radix == 2) {
      for (; pos < len; ++pos) {
	QChar chr(str.At(pos));
	
	if (chr != '0' && chr != '1')
	  break;
      }
    } else if (radix == 8) {
      for (; pos < len; ++pos) {
	QChar chr(str.At(pos));

	if (chr < '0' || chr > '7')
	  break;
      }
    } else if (radix == 10) {
      while (pos < len && str.At(pos).isDigit()) ++pos;
    } else if (radix == 16) {
      for (; pos < len; ++pos) {
	QChar chr(str.At(pos));
	
	if ((chr < '0' || chr > '9') &&
	    (chr < 'A' || chr > 'F') &&
	    (chr < 'a' || chr > 'f'))
	  break;
      }
    }

    return pos;
  }

  KevesValue NumberKev_makeFromDecimal(KevesGC* gc, StringKev str, const StringKev* next_str, int radix, bool exct, RationalNumberKev* real) {
    FlonumKev flonum;
    
    if (str.Equals("0") || str.Equals("+0") || str.Equals("-0")) {
      if (next_str) {
	RationalNumberKev* zero(RationalNumberKev::makeFromInt(gc, 0));
	return NumberKev_makeFromDecimal(gc, *next_str, nullptr, radix, exct, zero);
      }

      KevesValue int_num(real->toKevesFixnum());
      return int_num.IsFixnum() ? int_num : KevesValue(real);
    }

    if (str.Equals("inf.0") || str.Equals("+inf.0")) {
      flonum.set(std::numeric_limits<double>::infinity());
    } else if (str.Equals("-inf.0")) {
      flonum.set(-std::numeric_limits<double>::infinity());
    } else if (str.Equals("nan.0") || str.Equals("+nan.0") || str.Equals("-nan.0")) {
      flonum.set(std::numeric_limits<double>::quiet_NaN());
    } else {
      if (radix != 10) {
	exct = str.IndexOf('.') < 0 &&
	  str.IndexOf("e") < 0 &&
	  str.IndexOf("f") < 0 &&
	  str.IndexOf("d") < 0 &&
	  str.IndexOf("l") < 0;
      }
      
      if (exct) {
	RationalNumberKev* rational(RationalNumberKev::makeFromString(gc, str));

	if (next_str)
	  return NumberKev_makeFromDecimal(gc, *next_str, nullptr, radix, exct, rational);
	RationalNumberKev* imag(rational);
	ExactComplexNumberKev* complex(ExactComplexNumberKev::make(gc));
	complex->set(*real, *imag);
	return complex;
      }
      
      bool ok;
      flonum.CopyFrom(FlonumKev::fromString(str, &ok));
      
      if (!ok)
	return EMB_UNDEF;
    }
    
    if (next_str) {
      if (next_str->Equals("0") || next_str->Equals("+0") || next_str->Equals("-0")) {
	FlonumKev* real(flonum.copy(gc));
	return real;
      }

      bool ok;
      FlonumKev flonum_imag(FlonumKev::fromString(*next_str, &ok));

      return ok ?
	static_cast<KevesValue>(InexactComplexNumberKev::makeFromFlonums(gc, flonum, flonum_imag)) :
	EMB_UNDEF;
    }

    FlonumKev flonum_real(real->toInexact());
    return InexactComplexNumberKev::makeFromFlonums(gc, flonum_real, flonum);
  }
}
  
// class NumberKev ----------------------------------------
KevesValue NumberKev::makeFromBinary(KevesGC* gc, StringKev str, int radix, bool exct) {
  StringKevWithArray<0x4> str_zero("0");
  StringKev str_real;
  StringKev str_imag;
  int len(str.size());
  int pos_imag(str.IndexOf("+", 1));

  if (pos_imag < 0) pos_imag = str.IndexOf("-", 1);
  
  if (pos_imag > 0) {
    str_real.CopyFrom(str.Left(pos_imag));
    str_imag.CopyFrom(str.Mid(pos_imag, len - pos_imag - 1));
  } else if (str.Right(1).Equals("i")) {
    str_real.CopyFrom(str_zero);
    str_imag.CopyFrom(str.Left(len - 1));
  } else {
    str_real.CopyFrom(str.Left(len));
    str_imag.CopyFrom(str_zero);
  }
  
  return exct ?
    ExactComplexNumberKev::makeFromBinary(gc, str_real, str_imag, radix) :
    InexactComplexNumberKev::makeFromBinary(gc, str_real, str_imag, radix);
}

KevesValue NumberKev::makeFromDecimal(KevesGC* gc, StringKev str, int radix, bool exct) {
  int len(str.size());
  StringKevWithArray<0x4> str_zero("0");
  StringKevWithArray<0x4> str_one("1");
  
  int pos_plus(str.IndexOf("+", 1));
  int pos_minus(str.IndexOf("-", 1));
  
  int pos_imag((pos_plus < 0 || (pos_minus > 0 && pos_plus > pos_minus)) ?
		  pos_minus : pos_plus);
  
  StringKev str_real(str.Left(pos_imag));
  QChar chr_last(str_real.Right(1).At(0));
  
  if (chr_last == 'e' || chr_last == 'E' ||
      chr_last == 's' || chr_last == 'S' ||
      chr_last == 'f' || chr_last == 'F' ||
      chr_last == 'd' || chr_last == 'D' ||
      chr_last == 'l' || chr_last == 'L') {
    int pos_plus(str.IndexOf("+", pos_imag + 1));
    int pos_minus(str.IndexOf("-", pos_imag + 1));
    pos_imag = pos_plus < 0 ? pos_minus : pos_plus;
    str_real.CopyFrom(str.Mid(0, pos_imag));
  }
  
  StringKev str_imag(str.Mid(pos_imag, len - pos_imag - 1));
  
  if (pos_imag < 0) {
    if (str.Right(1).Equals("i")) {
      str_real.CopyFrom(str_zero);
      str_imag.CopyFrom(str.Mid(0, len - 1));
    } else {
      str_real.CopyFrom(str.Mid(0, len));
      str_imag.CopyFrom(str_zero);
    }
  }
  
  return NumberKev_makeFromDecimal(gc, str_real, &str_imag, radix, exct, nullptr);
}

KevesValue NumberKev::makeRealNumberFromBinary(KevesGC* gc, StringKev str, int* position, int radix) {
  if (str.size() == 0) { // string is empty
    *position = -1; // error
    return EMB_UNDEF;
  }

  // get sign
  bool negative;
  int pos(getSignOfNumberString(str, &negative));
  
  if (pos == str.size()) { // no digit
    *position = -1;
    return EMB_UNDEF;
  }
  
  const StringKev rest1(str.Right(str.size() - pos));
  *position += pos;
  return makeRealNumberFromBinary_helper1(gc, rest1, position, radix, negative);
}

KevesValue NumberKev::makeRealNumberFromBinary_helper1(KevesGC* gc, StringKev rest, int* position, int radix, bool negative) {
  // get first digit part
  int pos(getEndOfDigitString(rest, radix));
  const StringKev num_a(rest.Left(pos));
  const StringKev rest1(rest.Right(rest.size() - pos));
  *position += pos;
  
  if (rest1.size() > 0) {
    QChar chr(rest1.At(0));
    const StringKev rest2(rest1.Right(rest1.size() - 1));
    *position += 1;
    
    // get second digit part
    int pos(getEndOfDigitString(rest2, radix));
    const StringKev num_b(rest2.Left(pos));
    const StringKev rest3(rest2.Right(rest2.size() - pos));
    *position += pos;
    
    if (chr == QChar('/')) { // rational
      if (num_a.size() > 0 && num_b.size() > 0) {
	int idx(0);
	int len(num_a.size());

	while (idx < len && num_a.At(idx) == QChar('0')) ++idx;

	if (idx == len) {
	  *position = -1;
	  return EMB_UNDEF;
	}

	return RationalNumberKev::makeRationalFromString(gc, negative, num_a, num_b, radix);
      }
      
      *position = -1;
      return EMB_UNDEF;
    }
  }
  
  const StringKev num_b(rest.Left(0));

  return RationalNumberKev::makeIntegerFromString(gc, negative, num_a, radix);
}

KevesValue NumberKev::makeRealNumberFromDecimal(KevesGC* gc, StringKev str, int* position, int* radix, bool* exact) {
  if (str.size() == 0) { // string is empty
    *position = -1; // error
    return EMB_UNDEF;
  }
  
  // get sign
  bool negative;
  int pos(getSignOfNumberString(str, &negative));
  
  if (pos == str.size()) { // no digit
    *position = -1;
    return EMB_UNDEF;
  }

  const StringKev rest1(str.Right(str.size() - pos));
  *position += pos;

  return makeRealNumberFromDecimal_helper1(gc, rest1, position,
					   radix, exact, negative);
}

KevesValue NumberKev::makeRealNumberFromDecimal_helper1(KevesGC* gc, StringKev rest, int* position, int* radix, bool* exact, bool negative) {
  // get first digit part
  int pos(getEndOfDigitString(rest, 10));
  const StringKev num_a(rest.Left(pos));
  const StringKev rest1(rest.Right(rest.size() - pos));
  *position += pos;
  
  if (rest1.size() > 0) {
    QChar chr(rest1.At(0));
    const StringKev rest2(rest1.Right(rest1.size() - 1));
    *position += 1;
    
    // get second digit part
    int pos(getEndOfDigitString(rest2, 10));
    const StringKev num_b(rest2.Left(pos));
    const StringKev rest3(rest2.Right(rest2.size() - pos));
    *position += pos;
    
    if (chr == QChar('/')) { // rational
      if (num_a.size() > 0 && num_b.size() > 0) {
	int idx(0);
	int len(num_a.size());

	while (idx < len && num_a.At(idx) == QChar('0')) ++idx;

	if (idx == len) {
	  *position = -1;
	  return EMB_UNDEF;
	}
	
	return RationalNumberKev::makeRationalFromString(gc, negative,
							 num_a, num_b);
      }

      *position = -1;
      return EMB_UNDEF;
    }

    if (chr == QChar('.')) { // fraction
      if (*radix == 0) *exact = false;
      
      return makeRealNumberFromDecimal_helper3(gc, rest3, position, radix, exact, negative, num_a, num_b);
    }

    *position -= pos + 1;
  }
  
  return makeRealNumberFromDecimal_helper2(gc, rest1, position,
					   radix, exact, negative, num_a);
}

KevesValue NumberKev::makeRealNumberFromDecimal_helper2(KevesGC* gc, StringKev rest, int* position, int* radix, bool* exact, bool negative, StringKev num_a) {
  if (num_a.size() > 0) {
    const StringKev num_b(rest.Left(0));
    return makeRealNumberFromDecimal_helper3(gc, rest, position, radix, exact, negative, num_a, num_b);
  }

  if (*position == 1) {
    const StringKev num_c(rest.Left(5));
    
    if (num_c.Equals("inf.0")) {
      FlonumKev flonum(negative ?
		       -std::numeric_limits<double>::infinity() :
		       std::numeric_limits<double>::infinity());
      *position += 5;
      return flonum.copy(gc);
    }

    if (num_c.Equals("nan.0")) {
      FlonumKev flonum(std::numeric_limits<double>::quiet_NaN());
      *position += 5;
      return flonum.copy(gc);
    }
  }

  *position = -1;
  return EMB_UNDEF; // error
}

KevesValue NumberKev::makeRealNumberFromDecimal_helper3(KevesGC* gc, StringKev rest, int* position, int* radix, bool* exact, bool negative, StringKev str_int, StringKev str_fract) {
  if (str_int.size() + str_fract.size() == 0) { // both strings is empty.
    *position = -1; // error
    return EMB_UNDEF;
  }
  
  int expt(0);

  if (rest.size() > 1) {
    QChar chr(rest.At(0));
    
    if (chr == QChar('e') || chr == QChar('E') ||
	chr == QChar('s') || chr == QChar('S') ||
	chr == QChar('f') || chr == QChar('F') ||
	chr == QChar('d') || chr == QChar('D') ||
	chr == QChar('l') || chr == QChar('L')) {
      if (*radix == 0) *exact = false;

      const StringKev rest1(rest.Right(rest.size() - 1));
      *position += 1;

      bool expt_neg;
      int pos(getSignOfNumberString(rest1, &expt_neg));
      const StringKev rest2(rest1.Right(rest1.size() - pos));
      *position += pos;
      
      if (rest2.size() == 0) { // exponent part is empty
	*position = -1; // error
	return EMB_UNDEF;
      }
      
      pos = getEndOfDigitString(rest2, 10);
      const StringKev rest3(rest2.Right(rest2.size() - pos));
      const StringKev str_expt(rest2.Left(pos));
      *position += pos;
      
      QString qstr_expt(str_expt.ToQString());
      bool ok;
      expt = qstr_expt.toUInt(&ok);
      
      if (!ok || expt > 0xfff) { // exponent is too large
	*position = -1; // error
	return EMB_UNDEF;
      }
      
      if (expt_neg) expt = -expt;
    }
  }
  
  return RationalNumberKev::makeFractionFromString(gc, negative, str_int, str_fract, expt);
}

int NumberKev::getPrefix(StringKev str, int* radix, bool* exact) {
  int pos(0);

  constexpr int seq[] { 0xf00, 0x022, 0xf00, 0x02a, 0x300, 0xf00, // a B c D E f
			0xf00, 0xf00, 0x200, 0xf00, 0xf00, // g h I j k
			0xf00, 0xf00, 0xf00, 0x028, 0xf00, // l m n O p
			0xf00, 0xf00, 0xf00, 0xf00, 0xf00, // q r s t u
			0xf00, 0xf00, 0x030, 0xf00, 0xf00 }; // v w X y z

  if (str.size() > 2) {
    QChar chr1(str.At(0));
    int code1(str.At(1).unicode());
    int exact_and_radix;
    int req_exact(0);
    int req_radix(0);

    if (chr1 == '#') {
      if (code1 >= 'A' && code1 <= 'Z') {
	exact_and_radix = seq[code1 - 'A'];
      } else if (code1 >= 'a' && code1 <= 'z') {
	exact_and_radix = seq[code1 - 'a'];
      } else {
	return -1;
      }
      
      pos = 2;

      if (str.size() > 4) {
	QChar chr2(str.At(2));
	int code2(str.At(3).unicode());

	if (chr2 == '#') {
	  if (code2 >= 'A' && code2 <= 'Z') {
	    exact_and_radix += seq[code2 - 'A'];
	  } else if (code2 >= 'a' && code2 <= 'z') {
	    exact_and_radix += seq[code2 - 'a'];
	  } else {
	    return -1;
	  }
	  
	  pos = 4;
	}
      }

      req_exact = exact_and_radix >> 8;
      req_radix = exact_and_radix & 0xff;

      if (req_exact > 0x03 || req_radix > 0x30)
	return -1;

      req_radix &= 0x1f;
      
      if (*radix == 0) {
	if (req_exact & 0x02) {
	  *exact = req_exact & 0x01;
	  *radix = req_radix == 0 ? 10 : req_radix;
	} else { // default exactness
	  *exact = true;
	  *radix = req_radix == 10 ? 0 : req_radix;
	}

	return pos;
      } else if (req_radix == 0) { // request exactness only
	*exact = req_exact & 0x01;
      } else if (req_radix == 10) {
	if (req_exact & 0x02) {
	  *exact = req_exact & 0x01;
	  *radix = 10;
	} else {
	  *exact = true;
	  *radix = 0;
	}
      } else { // req_radix is 2, 8 or 16
	if (req_exact & 0x02) *exact = req_exact & 0x01;
	*radix = req_radix;
      }
    }
  }
  
  return pos;
}

StringKev* NumberKev::makeStringFromNumber(KevesGC* gc, KevesValue num, int radix, int precision) {
  return radix == 10 ?
    makeStringFromDecimal(gc, num, precision) :
    makeStringFromBinary(gc, num, radix);
}

StringKev* NumberKev::makeStringFromDecimal(KevesGC* gc, KevesValue num, int precision) {
  QString str;

  if (num.IsFixnum()) {
    str.operator=(QString::number(KevesFixnum(num)));
  } else if (num.Is<RationalNumberKev>()) {
    try {
      str.operator=(num.ToPtr<RationalNumberKev>()->toQString(false));
    } catch (int i) {
      std::cout << "Allocation is failed!" << std::endl;
    }
  } else if (num.Is<FlonumKev>()) {
    str.operator=(num.ToPtr<FlonumKev>()->toQString(false, precision));
  } else if (num.Is<ExactComplexNumberKev>()) {
    try {
      str.operator=(num.ToPtr<ExactComplexNumberKev>()->toQString());
    } catch (int i) {
      std::cout << "Allocation is failed!" << std::endl;
    }
  } else if (num.Is<InexactComplexNumberKev>()) {
    str.operator=(num.ToPtr<InexactComplexNumberKev>()->toQString(precision));
  }

  return StringKev::Make(gc, str);
}

StringKev* NumberKev::makeStringFromBinary(KevesGC* gc, KevesValue num, int radix) {
  QString str;

  if (num.IsFixnum()) {
    str.operator=(QString::number(KevesFixnum(num), radix));
  } else if (num.Is<RationalNumberKev>()) {
    str.operator=(num.ToPtr<RationalNumberKev>()->toBinary(false, radix));
  } else if (num.Is<FlonumKev>()) {
    RationalNumberKev exact(num.ToPtr<FlonumKev>()->toExact(gc));
    str.operator=(exact.toBinary(false, radix));
  } else if (num.Is<ExactComplexNumberKev>()) {
    str.operator=(num.ToPtr<ExactComplexNumberKev>()->toBinary(radix));
  } else if (num.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* complex(num);
    RationalNumberKev real(complex->real().toExact(gc));
    RationalNumberKev imag(complex->imag().toExact(gc));
    str.operator=(real.toBinary(false, radix));
    str.append(imag.toBinary(true, radix));
    str.append('i');
  }

  return StringKev::Make(gc, str);
}

mpq_class RationalNumberKev::makeMPQ() const {
  mpz_class mpz_num;
  mpz_class mpz_den;
  numerator_->initMPZ(&mpz_num);
  denominator_->initMPZ(&mpz_den);

  if (!neg_) {
    mpq_class mpq_num(mpz_num, mpz_den);
    mpq_num.canonicalize();
    return mpq_num;
  }

  mpq_class mpq_num(-mpz_num, mpz_den);
  mpq_num.canonicalize();
  return mpq_num;
}

KevesValue NumberKev::makeFromMPQ(KevesGC* gc, const mpq_class& mpq_num) {
  mpz_class denominator(mpq_num.get_den());

  if (denominator == 1) {
    mpz_class numerator(mpq_num.get_num());

    if (Bignum::calculateLength(numerator) == 1) {
      bg_uint num(numerator.get_ui());
      
      if (sgn(mpq_num) < 0) {
	if (num <= static_cast<bg_uint>(-KevesFixnum::MIN_VALUE))
	  return KevesFixnum(-static_cast<fx_int>(num));
	
	if (num <= static_cast<bg_uint>(KevesFixnum::MAX_VALUE))
	  return KevesFixnum(static_cast<fx_int>(num));

      }
    }
  }

  return RationalNumberKev::makeFromMPQ(gc, mpq_num);
}
    
KevesValue NumberKev::difference(KevesGC* gc, KevesFixnum minuend, RationalNumberKev subtrahend) {
  if (subtrahend.isNegative()) {
    mpq_class difference(minuend + subtrahend.makeMPQ());
    return makeFromMPQ(gc, difference);
  }

  mpq_class difference(minuend - subtrahend.makeMPQ());
  return makeFromMPQ(gc, difference);
}

FlonumKev NumberKev::difference(KevesFixnum minuend, FlonumKev subtrahend) {
  return subtrahend.inverse().add(minuend);
}

ExactComplexNumberKev NumberKev::difference(KevesGC* gc, KevesFixnum minuend, ExactComplexNumberKev subtrahend) {
  ExactComplexNumberKev difference(subtrahend.real().subtract(gc, minuend),
      subtrahend.imag());
  
  return difference.inverse();
}

RationalNumberKev NumberKev::quotient(KevesGC* gc, KevesFixnum dividend, RationalNumberKev divisor) {
  return { gc, dividend / divisor.makeMPQ() };
}

FlonumKev NumberKev::quotient(KevesFixnum dividend, FlonumKev divisor) {
  return FlonumKev(dividend / divisor.toDouble());
}

ExactComplexNumberKev NumberKev::quotient(KevesGC* gc, KevesFixnum dividend, ExactComplexNumberKev divisor) {
  return divisor.reciprocal(gc).multiply(gc, dividend);
}

InexactComplexNumberKev NumberKev::quotient(KevesFixnum dividend, InexactComplexNumberKev divisor) {
  return divisor.reciprocal().multiply(dividend);
}

FlonumKev NumberKev::makeFlonum(KevesValue x) {
  if (x.IsFixnum()) {
    KevesFixnum fixnum(x);
    return FlonumKev(fixnum);
  }
  
  if (x.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(x);
    return rational->toInexact();
  }

  if (x.Is<FlonumKev>()) {
    const FlonumKev* flonum(x);
    return *flonum;
  }

  return FlonumKev();
}

bool NumberKev::isLT(KevesValue x, KevesValue y) {
  return compare<IsLT>(x, y);
}

bool NumberKev::IsLT::func(KevesFixnum num1, KevesFixnum num2) {
  return num1 < num2;
}

bool NumberKev::IsLT::func(RationalNumberKev num1, RationalNumberKev num2) {
  return num1.isLT(num2);
}

bool NumberKev::IsLT::func(FlonumKev num1, FlonumKev num2) {
  return num1.isLT(num2);
}

bool NumberKev::isGT(KevesValue x, KevesValue y) {
  return compare<IsGT>(x, y);
}

bool NumberKev::IsGT::func(KevesFixnum num1, KevesFixnum num2) {
  return num1 > num2;
}

bool NumberKev::IsGT::func(RationalNumberKev num1, RationalNumberKev num2) {
  return num1.isGT(num2);
}

bool NumberKev::IsGT::func(FlonumKev num1, FlonumKev num2) {
  return num1.isGT(num2);
}

bool NumberKev::isLTE(KevesValue x, KevesValue y) {
  return compare<IsLTE>(x, y);
}

bool NumberKev::IsLTE::func(KevesFixnum num1, KevesFixnum num2) {
  return num1 <= num2;
}

bool NumberKev::IsLTE::func(RationalNumberKev num1, RationalNumberKev num2) {
  return num1.isLTE(num2);
}

bool NumberKev::IsLTE::func(FlonumKev num1, FlonumKev num2) {
  return num1.isLTE(num2);
}

bool NumberKev::isGTE(KevesValue x, KevesValue y) {
  return compare<IsGTE>(x, y);
}

bool NumberKev::IsGTE::func(KevesFixnum num1, KevesFixnum num2) {
  return num1 >= num2;
}

bool NumberKev::IsGTE::func(RationalNumberKev num1, RationalNumberKev num2) {
  return num1.isGTE(num2);
}

bool NumberKev::IsGTE::func(FlonumKev num1, FlonumKev num2) {
  return num1.isGTE(num2);
}

KevesValue NumberKev::max(KevesGC* gc, KevesValue x, KevesValue y) {
  if (y.IsFixnum()) {
    KevesFixnum num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return num1 > num2 ? num1 : num2;
    }

    if (x.Is<RationalNumberKev>())
      return max(gc, x, RationalNumberKev::makeFromInt(gc, num2));
    
    if (x.Is<FlonumKev>())
      return max(gc, x, FlonumKev(num2).copy(gc));

  } else if (y.Is<RationalNumberKev>()) {
    const RationalNumberKev* num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return max(gc, RationalNumberKev::makeFromInt(gc, num1), num2);
    }

    if (x.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(x);
      return num1->isGT(*num2) ? num1 : num2;
    }

    if (x.Is<FlonumKev>())
      return max(gc, x, num2->toInexact().copy(gc));
    
  } else if (y.Is<FlonumKev>()) {
    const FlonumKev* num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return max(gc, FlonumKev(num1).copy(gc), num2);
    }

    if (x.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(x);
      return max(gc, num1->toInexact().copy(gc), num2);
    }

    if (x.Is<FlonumKev>()) {
      const FlonumKev* num1(x);
      return num1->isGT(*num2) ? num1 :num2;
    }

  }

  return EMB_FALSE;
}

KevesValue NumberKev::min(KevesGC* gc, KevesValue x, KevesValue y) {
  if (y.IsFixnum()) {
    KevesFixnum num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return num1 < num2 ? num1 : num2;
    }

    if (x.Is<RationalNumberKev>())
      return min(gc, x, RationalNumberKev::makeFromInt(gc, num2));

    if (x.Is<FlonumKev>())
      return min(gc, x, FlonumKev(num2).copy(gc));
    
  } else if (y.Is<RationalNumberKev>()) {
    const RationalNumberKev* num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return min(gc, RationalNumberKev::makeFromInt(gc, num1), num2);
    }

    if (x.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(x);
      return num1->isLT(*num2) ? num1 : num2;
    }

    if (x.Is<FlonumKev>())
      return min(gc, x, num2->toInexact().copy(gc));
    
  } else if (y.Is<FlonumKev>()) {
    const FlonumKev* num2(y);

    if (x.IsFixnum()) {
      KevesFixnum num1(x);
      return min(gc, FlonumKev(num1).copy(gc), num2);
    }

    if (x.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(x);
      return min(gc, num1->toInexact().copy(gc), num2);
    }

    if (x.Is<FlonumKev>()) {
      const FlonumKev* num1(x);
      return num1->isLT(*num2) ? num1 :num2;
    }
  }

  return EMB_FALSE;
}

bool NumberKev::isNegative(KevesValue x) {
  return predicateForReal<IsNegative>(x);
}

bool NumberKev::isPositive(KevesValue x) {
  return predicateForReal<IsPositive>(x);
}

bool NumberKev::isZero(KevesValue x) {
  return predicate<IsZero>(x);
}

// class RationalNumberKev ----------------------------------------
RationalNumberKev::RationalNumberKev()
  : RationalNumberKev(false, nullptr, nullptr) {
}

RationalNumberKev::RationalNumberKev(bool neg, const Bignum* num, const Bignum* den) : MutableKev(TYPE), neg_(neg), numerator_(num), denominator_(den) {
}

RationalNumberKev::RationalNumberKev(KevesGC* gc, const mpq_class& mpq_num)
  : RationalNumberKev(sgn(mpq_num) < 0,
		      Bignum::makeFromMPZ(gc, mpq_num.get_num()),
		      Bignum::makeFromMPZ(gc, mpq_num.get_den())) {
}

void RationalNumberKev::CopyFrom(const RationalNumberKev& other) {
  this->neg_ = other.neg_;
  this->numerator_ = other.numerator_;
  this->denominator_ = other.denominator_;
}

void RationalNumberKev::CopyFrom(RationalNumberKev&& other) {
  this->neg_ = other.neg_;
  this->numerator_ = other.numerator_;
  this->denominator_ = other.denominator_;
}

RationalNumberKev RationalNumberKev::fromInt(KevesGC* gc, fx_int n) {
  bool neg;
  if ((neg = n < 0)) n = -n;

  return { neg, Bignum::makeFromInt(gc, n), Bignum::makeFromInt(gc, 1) };
}

const Bignum* RationalNumberKev::numerator() const {
  return numerator_;
}

const Bignum* RationalNumberKev::denominator() const {
  return denominator_;
}

void RationalNumberKev::reduce(KevesGC* gc) {
  mpq_class mpq_num(makeMPQ());
  mpq_num.canonicalize();
  this->CopyFrom({ gc, mpq_num });
}

RationalNumberKev RationalNumberKev::add(KevesGC* gc, KevesFixnum addend) const {
  mpq_class result(makeMPQ() + addend);
  return { gc, result };
}

RationalNumberKev RationalNumberKev::add(KevesGC* gc, RationalNumberKev addend) const {
  mpq_class result(makeMPQ() + addend.makeMPQ());
  return { gc, result };
}

FlonumKev RationalNumberKev::add(FlonumKev addend) const {
  return this->toInexact().add(addend);
}

ExactComplexNumberKev RationalNumberKev::add(KevesGC* gc, ExactComplexNumberKev addend) const {
  return { this->add(gc, addend.real()), addend.imag() };
}

InexactComplexNumberKev RationalNumberKev::add(InexactComplexNumberKev addend) const {
  return this->toInexact().add(addend);
}

RationalNumberKev RationalNumberKev::subtract(KevesGC* gc, KevesFixnum subtrahend) const {
  mpq_class result(makeMPQ() - subtrahend);
  return { gc, result };
}

RationalNumberKev RationalNumberKev::subtract(KevesGC* gc, RationalNumberKev subtrahend) const {
  mpq_class result(makeMPQ() - subtrahend.makeMPQ());
  return { gc, result };
}

FlonumKev RationalNumberKev::subtract(FlonumKev subtrahend) const {
  return this->toInexact().subtract(subtrahend);
}

ExactComplexNumberKev RationalNumberKev::subtract(KevesGC* gc, ExactComplexNumberKev subtrahend) const {
  return { this->subtract(gc, subtrahend.real()), subtrahend.imag().inverse() };
}

InexactComplexNumberKev RationalNumberKev::subtract(InexactComplexNumberKev subtrahend) const {
  return this->toInexact().subtract(subtrahend);
}

RationalNumberKev RationalNumberKev::abs() const {
  Q_ASSERT(type() == TYPE);
  return { false, this->numerator_, this->denominator_ };
}

RationalNumberKev RationalNumberKev::ceiling(KevesGC* gc) const {
  Q_ASSERT(type() == TYPE);

  if (this->isInteger())
    return *this;

  mpq_class mpq_num(makeMPQ());
  mpz_class quot(mpq_num.get_num() / mpq_num.get_den());
  return { gc, neg_ ? mpq_class(quot) : mpq_class(quot + 1) };
}

RationalNumberKev RationalNumberKev::floor(KevesGC* gc) const {
  Q_ASSERT(type() == TYPE);

  if (this->isInteger())
    return *this;

  mpq_class mpq_num(makeMPQ());
  mpz_class quot(mpq_num.get_num() / mpq_num.get_den());
  return { gc, neg_ ? mpq_class(quot - 1) : mpq_class(quot) };
}

RationalNumberKev RationalNumberKev::round(KevesGC* gc) const {
  Q_ASSERT(type() == TYPE);

  if (this->isInteger())
    return *this;
  
  mpq_class mpq_num(makeMPQ() + (neg_ ?
				 mpq_class(-1, 2) :
				 mpq_class(1, 2)));

  if (mpq_num.get_den() == 1) {
    return mpz_even_p(mpq_num.get_num().get_mpz_t()) ?
      RationalNumberKev(gc, mpq_num) :
      RationalNumberKev(gc, mpq_num + (neg_ ? 1 : -1));
  }

  mpz_class quot(mpq_num.get_num() / mpq_num.get_den());
  return { gc, mpq_class(quot) };
}

RationalNumberKev RationalNumberKev::truncate(KevesGC* gc) const {
  Q_ASSERT(type() == TYPE);

  if (this->isInteger())
    return *this;

  mpq_class mpq_num(makeMPQ());
  mpz_class quot(mpq_num.get_num() / mpq_num.get_den());
  return { gc, mpq_class(quot) };
}

void RationalNumberKev::multiplyByExponentiationWithBase2(KevesGC* gc, int exponent) {
  if (exponent < 0) {
    this->denominator_ = Bignum::ldexp(gc, 1, -exponent);
  } else if (exponent > 0) {
    bool ok;
    bg_ulong significand(this->numerator_->toULong(&ok));
    this->numerator_ = Bignum::ldexp(gc, significand, exponent);
  }
}

KevesValue RationalNumberKev::optimize() {
  KevesValue fixnum(this->toKevesFixnum());
  return fixnum.IsFixnum() ? fixnum : KevesValue(this);
}

RationalNumberKev RationalNumberKev::reciprocal() const {
  Q_ASSERT(type() == TYPE);
  return { isNegative(), denominator_, numerator_ };
}

RationalNumberKev RationalNumberKev::inverse() const {
  Q_ASSERT(type() == TYPE);
  return { !isNegative(), numerator_, denominator_ };
}

int RationalNumberKev::compare(const RationalNumberKev other) const {
  return cmp(this->makeMPQ(), other.makeMPQ());
}

bool RationalNumberKev::isGT(FlonumKev other) const {
  return this->toDouble() > other.toDouble();
}

bool RationalNumberKev::isGT(RationalNumberKev other) const {
  Q_ASSERT(other.type() == TYPE);

  if (this->neg_ != other.neg_)
    return other.neg_;

  return this->neg_ ? compare(other) < 0 : compare(other) > 0;
}

bool RationalNumberKev::isGTE(RationalNumberKev other) const {
  Q_ASSERT(other.type() == TYPE);

  if (this->neg_ != other.neg_)
    return other.neg_;

  return this->neg_ ? compare(other) <= 0 : compare(other) >= 0;
}

bool RationalNumberKev::isEqualTo(RationalNumberKev other) const {
  Q_ASSERT(other.type() == TYPE);

  return this->numerator_->equals(*other.numerator_) &&
    this->denominator_->equals(*other.denominator_);
}

bool RationalNumberKev::IsEmpty() const {
  return numerator_ == nullptr || numerator_ == nullptr;
}

bool RationalNumberKev::isInteger() const {
  return denominator_->isOne();
}

bool RationalNumberKev::isNegative() const {
  return neg_;
}

bool RationalNumberKev::isZero() const {
  return numerator_->isZero();
}

bool RationalNumberKev::isEven() const {
  return denominator_->isOne() && this->numerator_->isEven();
}

bool RationalNumberKev::isOdd() const {
  return denominator_->isOne() && this->numerator_->isOdd();
}

bool RationalNumberKev::isPositive() const {
  return !neg_;
}

bool RationalNumberKev::isLT(FlonumKev other) const {
  return this->toDouble() < other.toDouble();
}

bool RationalNumberKev::isLT(RationalNumberKev other) const {
  Q_ASSERT(other.type() == TYPE);

  if (this->neg_ != other.neg_)
    return this->neg_;

  return this->neg_ ? compare(other) > 0 : compare(other) < 0;
}

bool RationalNumberKev::isLTE(RationalNumberKev other) const {
  Q_ASSERT(other.type() == TYPE);

  if (this->neg_ != other.neg_)
    return this->neg_;

  return this->neg_ ? compare(other) >= 0 : compare(other) <= 0;
}

RationalNumberKev RationalNumberKev::multiply(KevesGC* gc, KevesFixnum multiplier) const {
  return { gc, makeMPQ() * multiplier };
}

RationalNumberKev RationalNumberKev::multiply(KevesGC* gc, RationalNumberKev multiplier) const {
  Q_ASSERT(multiplier.type() == TYPE);

  return { gc, makeMPQ() * multiplier.makeMPQ() };
}

FlonumKev RationalNumberKev::multiply(FlonumKev multiplier) const {
  return this->toInexact().multiply(multiplier);
}

ExactComplexNumberKev RationalNumberKev::multiply(KevesGC* gc, ExactComplexNumberKev multiplier) const {
  return { this->multiply(gc, multiplier.real()),
      this->multiply(gc, multiplier.imag()) };
}

InexactComplexNumberKev RationalNumberKev::multiply(InexactComplexNumberKev multiplier) const {
  FlonumKev multiplicand(this->toInexact());
  return { multiplicand.multiply(multiplier.real()),
      multiplicand.multiply(multiplier.imag()) };
}

RationalNumberKev RationalNumberKev::divide(KevesGC* gc, KevesFixnum divisor) const {
  Q_ASSERT(!divisor.isZero());

  return { gc, makeMPQ() / divisor };
}

RationalNumberKev RationalNumberKev::divide(KevesGC* gc, RationalNumberKev divisor) const {
  Q_ASSERT(!divisor.isZero());

  return { gc, makeMPQ() / divisor.makeMPQ() };
}

FlonumKev RationalNumberKev::divide(FlonumKev divisor) const {
  return this->toInexact().divide(divisor);
}

ExactComplexNumberKev RationalNumberKev::divide(KevesGC* gc, ExactComplexNumberKev divisor) const {
  mpq_class divisor_real(divisor.real().makeMPQ());
  mpq_class divisor_imag(divisor.imag().makeMPQ());

  mpq_class num1(divisor_real * divisor_real);
  mpq_class num2(divisor_imag * divisor_imag);
  mpq_class num3(num1 + num2);
  mpq_class reciprocal_real(divisor_real / num3);
  mpq_class reciprocal_imag(-divisor_imag / num3);

  mpq_class dividend(makeMPQ());

  return { RationalNumberKev(gc, dividend * reciprocal_real),
      RationalNumberKev(gc, dividend * reciprocal_imag) };
}

InexactComplexNumberKev RationalNumberKev::divide(InexactComplexNumberKev divisor) const {
  InexactComplexNumberKev reciprocal(divisor.reciprocal());
  return reciprocal.multiply(*this);
}

double RationalNumberKev::toDouble() const {
  return makeMPQ().get_d();
}

KevesValue RationalNumberKev::toKevesFixnum() const {
  if (denominator_->isOne()) {
    bool ok;
    bg_uint num(numerator_->toUInt(&ok));

    if (ok) {
      if (isNegative()) {
	if (num <= static_cast<bg_uint>(-KevesFixnum::MIN_VALUE))
	  return KevesFixnum(-static_cast<fx_int>(num));
	
      } else {
	if (num <= static_cast<bg_uint>(KevesFixnum::MAX_VALUE))
	  return KevesFixnum(static_cast<fx_int>(num));

      }
    }
  }

  return EMB_UNDEF;
}

QString RationalNumberKev::toBinary(bool sign, int radix) const {
  QString str(neg_ ? "-" : (sign ? "+" : ""));
  str.append(numerator_->toBinary(radix));
  
  if (denominator_->isGT(1)) {
    str.append('/');
    str.append(denominator_->toBinary(radix));
  }

  return str;
}

QString RationalNumberKev::toQString(bool sign) const {
  QString str(neg_ ? "-" : (sign ? "+" : ""));
  str.append(numerator_->toQString());
  
  if (denominator_->isGT(1)) {
    str.append('/');
    str.append(denominator_->toQString());
  }

  return str;
}

FlonumKev RationalNumberKev::toInexact() const {
  return FlonumKev(this->toDouble());
}

/*
RationalNumberKev* RationalNumberKev::makeFromInt(KevesIterator* iter, bg_int real) {
  return iter->make(ctorFromInt(iter, real), alloc_size(nullptr));
}
*/

RationalNumberKev* RationalNumberKev::make(KevesGC* gc) {
  auto ctor = [](void* ptr) { return new(ptr) RationalNumberKev(); };
  return gc->Make(ctor, alloc_size(nullptr));
}

RationalNumberKev* RationalNumberKev::makeFromInt(KevesGC* gc, bg_int real) {
  return gc->Make(ctorFromInt(gc, real), alloc_size(nullptr));
}

RationalNumberKev* RationalNumberKev::makeFromLong(KevesGC* gc, bg_long real) {
  return gc->Make(ctorFromLong(gc, real), alloc_size(nullptr));
}

RationalNumberKev* RationalNumberKev::makeFromMPQ(KevesGC* gc, const mpq_class& mpq_num) {
  return gc->Make(ctorFromMPQ(gc, mpq_num), alloc_size(nullptr));
}

RationalNumberKev* RationalNumberKev::makeFromString(KevesGC* gc, StringKev str) {
  int i;
  int pos(str.IndexOf("/"));
  QString str1;
  QString str2;
  RationalNumberKev* number(RationalNumberKev::make(gc));

  if (pos > 0) { // rational
    str1.operator=(str.Left(pos).ToQString());
    str2.operator=(str.Right(str.size() - pos - 1).ToQString());

    number->CopyFrom(str1.left(1) == "+" ?
		     RationalNumberKev(false,
			 Bignum::makeFromString(gc,
						str1.right(str1.size() - 1)),
			 Bignum::makeFromString(gc, str2)) :
		     str1.left(1) == "-" ?
		     RationalNumberKev(true,
			 Bignum::makeFromString(gc,
						str1.right(str1.size() - 1)),
			 Bignum::makeFromString(gc, str2)) :
		     RationalNumberKev(false,
			 Bignum::makeFromString(gc, str1),
			 Bignum::makeFromString(gc, str2)));
  } else { // not rational
    if ((pos = str.IndexOf("e")) < 0 &&
	(pos = str.IndexOf("s")) < 0 &&
	(pos = str.IndexOf("f")) < 0 &&
	(pos = str.IndexOf("d")) < 0 &&
	(pos = str.IndexOf("l")) < 0) {
      str1.operator=(str.ToQString());
      i = 0;
    } else {
      bool ok;
      str1.operator=(str.Left(pos).ToQString());
      str2.operator=(str.Right(str.size() - pos - 1).ToQString());
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

    number->CopyFrom(str1.left(1) == "+" ?
		     RationalNumberKev(false,
			 Bignum::makeFromString(gc,
						str1.right(str1.size() - 1)),
			 Bignum::exponentiationWithBase10(gc, i)) :
		     str1.left(1) == "-" ?
		     RationalNumberKev(true,
			 Bignum::makeFromString(gc, str1.right(str1.size() - 1)),
			 Bignum::exponentiationWithBase10(gc, i)) :
		     RationalNumberKev(false,
			 Bignum::makeFromString(gc, str1),
			 Bignum::exponentiationWithBase10(gc, i)));
  }

  if (number->denominator()->isZero()) {
    Q_ASSERT(0);
    return number;
  }
  
  number->reduce(gc);
  return number;
}

RationalNumberKev* RationalNumberKev::makeFromString(KevesGC* gc, StringKev str, int radix) {
  StringKev str_numerator;
  StringKev str_denominator;
  StringKevWithArray<0x4> str_one("1");
  
  int pos(str.IndexOf("/"));

  if (pos > 0) { // rational
    str_numerator.CopyFrom(str.Left(pos));
    str_denominator.CopyFrom(str.Right(str.size() - pos - 1));
  } else {
    str_numerator.CopyFrom(str);
    str_denominator.CopyFrom(str_one);
  }
  
  RationalNumberKev* number(RationalNumberKev::make(gc));

  if (str_numerator.Left(1).Equals("+")) {
    number->neg_ = false;
    str_numerator.CopyFrom(str_numerator.Right(str_numerator.size() - 1));
  } else if (str_numerator.Left(1).Equals("-")) {
    number->neg_ = true;
    str_numerator.CopyFrom(str_numerator.Right(str_numerator.size() - 1));
  } else {
    number->neg_ = false;
  }
  
  number->numerator_ = Bignum::makeFromString(gc, str_numerator.ToQString(), radix);
  number->denominator_ = Bignum::makeFromString(gc, str_denominator.ToQString(), radix);

  return number;
}

KevesValue RationalNumberKev::makeRationalFromString(KevesGC* gc, bool negative, StringKev str_num, StringKev str_den) {
  RationalNumberKev* number(RationalNumberKev::make(gc));
  number->neg_ = negative;
  number->numerator_ = Bignum::makeFromString(gc, str_num.ToQString());
  number->denominator_ = Bignum::makeFromString(gc, str_den.ToQString());
  number->reduce(gc);
  return number->optimize();
}

KevesValue RationalNumberKev::makeRationalFromString(KevesGC* gc, bool negative, StringKev str_num, StringKev str_den, int radix) {
  RationalNumberKev* number(RationalNumberKev::make(gc));
  number->neg_ = negative;
  number->numerator_ = Bignum::makeFromString(gc, str_num.ToQString(), radix);
  number->denominator_ = Bignum::makeFromString(gc, str_den.ToQString(), radix);
  number->reduce(gc);
  return number->optimize();
}

KevesValue RationalNumberKev::makeFractionFromString(KevesGC* gc, bool negative, StringKev str_int, StringKev str_fract, int expt) {
  RationalNumberKev* number(RationalNumberKev::make(gc));
  
  QString str(str_int.ToQString() + str_fract.ToQString());
  expt -= str_fract.size();
  for (; expt > 0; --expt) str.append('0');

  number->set(negative,
	      Bignum::makeFromString(gc, str),
	      Bignum::exponentiationWithBase10(gc, -expt));
  
  number->reduce(gc);
  return number->optimize();
}

KevesValue RationalNumberKev::makeIntegerFromString(KevesGC* gc, bool negative, StringKev str, int radix) {
  RationalNumberKev* number(RationalNumberKev::make(gc));
  number->neg_ = negative;
  number->numerator_ = Bignum::makeFromString(gc, str.ToQString(), radix);
  number->denominator_ = Bignum::makeFromString(gc, "1");
  return number->optimize();
}

// class RationalNumberKevFromInt ----------------------------------------
RationalNumberKevFromInt::RationalNumberKevFromInt(bg_int n)
  : bignum_numerator_(n < 0 ? -n : n), bignum_denominator_(1) {
  neg_ = n < 0;
  numerator_ = &bignum_numerator_;
  denominator_ = &bignum_denominator_;
}

RationalNumberKevFromInt::RationalNumberKevFromInt(bg_int numerator, bg_int denominator)
  : bignum_numerator_(std::abs(numerator)),
    bignum_denominator_(std::abs(denominator)) {
  neg_ = (numerator > 0) != (denominator > 0);
  numerator_ = &bignum_numerator_;
  denominator_ = &bignum_denominator_;
  
  Q_ASSERT(denominator != 0);
  
  bg_int num1(numerator = std::abs(numerator));
  bg_int num2(denominator = std::abs(denominator));

  if (num1 == 0) {
    bignum_numerator_.CopyFrom(BignumInt(0));
    bignum_denominator_.CopyFrom(BignumInt(1));
  } else {
    if (num1 < num2) {
      bg_int temp(num1);
      num1 = num2;
      num2 = temp;
    }
    
    bg_int num3(num1 % num2);

    while (num3 != 0) {
      num1 = num2;
      num2 = num3;
      num3 = num1 % num2;
    }
    
    bg_int gcd(num2);
    bignum_numerator_.CopyFrom(BignumInt(static_cast<bg_uint>(numerator / gcd)));
    bignum_denominator_.CopyFrom(BignumInt(static_cast<bg_uint>(denominator / gcd)));
  }
}

RationalNumberKevFromLong::RationalNumberKevFromLong(bg_long n)
  : bignum_numerator_(n < 0 ? -n : n), bignum_denominator_(1) {
  neg_ = n < 0;
  numerator_ = &bignum_numerator_;
  denominator_ = &bignum_denominator_;
}
  
RationalNumberKevFromLong::RationalNumberKevFromLong(bg_long numerator, bg_long denominator)
  : bignum_numerator_(std::abs(numerator)),
    bignum_denominator_(std::abs(denominator)) {
  neg_ = (numerator > 0) != (denominator > 0);
  numerator_ = &bignum_numerator_;
  denominator_ = &bignum_denominator_;
  
  Q_ASSERT(denominator != 0);
  
  bg_long num1(numerator = std::abs(numerator));
  bg_long num2(denominator = std::abs(denominator));

  if (num1 == 0) {
    bignum_numerator_.CopyFrom(BignumLong(0));
    bignum_denominator_.CopyFrom(BignumLong(1));
  } else {
    if (num1 < num2) {
      bg_long temp(num1);
      num1 = num2;
      num2 = temp;
    }
    
    bg_long num3(num1 % num2);

    while (num3 != 0) {
      num1 = num2;
      num2 = num3;
      num3 = num1 % num2;
    }
    
    bg_long gcd(num2);
    bignum_numerator_.CopyFrom(BignumLong(static_cast<bg_ulong>(numerator / gcd)));
    bignum_denominator_.CopyFrom(BignumLong(static_cast<bg_ulong>(denominator / gcd)));
  }
}



// class FlonumKev ----------------------------------------
// --------------------------------------------------------------
FlonumKev::FlonumKev() : FlonumKev(0) {
}

FlonumKev::FlonumKev(double a) : MutableKev(TYPE), value_(a) {
}

void FlonumKev::CopyFrom(const FlonumKev& other) {
  this->value_ = other.value_;
}

void FlonumKev::CopyFrom(FlonumKev&& other) {
  this->value_ = other.value_;
}

FlonumKev FlonumKev::fromString(StringKev str, bool* ok) {
  if (str.Equals("inf.0") || str.Equals("+inf.0")) {
    *ok = true;
    return FlonumKev(std::numeric_limits<double>::infinity());
  }

  if (str.Equals("-inf.0")) {
    *ok = true;
    return FlonumKev(-std::numeric_limits<double>::infinity());
  }

  if (str.Equals("nan.0") || str.Equals("+nan.0") || str.Equals("-nan.0")) {
    *ok = true;
    return FlonumKev(std::numeric_limits<double>::quiet_NaN());
  }
  
  char num_str[MAX_LENGTH + 1];
  char den_str[MAX_LENGTH + 1];
  int num_idx(0);
  int den_idx(0);
  int num_extra_exp(0);
  int den_extra_exp(0);
  bool negative;
  const QChar* iter(str.data());
  const QChar* end_iter(iter + str.size());
  
  // get sign
  if (*iter == '+') {
    negative = false;
    ++iter;
  } else if (*iter == '-') {
    negative = true;
    ++iter;
  } else {
    negative = false;
  }

  // skip negligible zeros
  while (iter != end_iter && *iter == '0') ++iter;

  // copy significant
  for (; iter != end_iter; ++iter) {
    if (iter->isDigit()) {
      if (num_idx < MAX_SIGNIFICAND_LENGTH) {
	num_str[num_idx++] = iter->toAscii();
      } else {
	++num_extra_exp;
      }
      
      continue;
    }

    if (*iter == '.') { // not rational number
      if (num_idx < MAX_SIGNIFICAND_LENGTH) num_str[num_idx++] = '.';
      ++iter;
      
      for (; iter != end_iter; ++iter) {
	if (iter->isDigit()) {
	  if (num_idx < MAX_SIGNIFICAND_LENGTH) {
	    num_str[num_idx++] = iter->toAscii();
	  }
	  
	  continue;
	}
	
	if ((*iter == 'e' || *iter == 'f' || *iter == 'd' || *iter == 'l') &&
	    copyExponentPart(iter, end_iter, num_str, &num_idx)) {
	  break;
	}
	
	*ok = false;
	return FlonumKev();
      }

      break;
    }

    if (*iter == 'e' || *iter == 'f' || *iter == 'd' || *iter == 'l') {
      if (!copyExponentPart(iter, end_iter, num_str, &num_idx)) {
	*ok = false;
	return FlonumKev();
      }
      
      break;
    }

    if (*iter == '/') { // reational number
      ++iter;
      // skip negligible zeros
      while (iter != end_iter && *iter == '0') ++iter;

      // copy denominator
      for (; iter != end_iter; ++iter) {
	if (!iter->isDigit()) {
	  *ok = false;
	  return FlonumKev();
	}
	
	if (den_idx < MAX_LENGTH) den_str[den_idx++] = iter->toAscii();
	else ++den_extra_exp;
      }
    
      num_str[num_idx] = '\0';
      den_str[den_idx] = '\0';
      std::istringstream num_ss(num_str);
      std::istringstream den_ss(den_str);
      long double num_long_double;
      long double den_long_double;
      *ok = (num_ss >> num_long_double) && (den_ss >> den_long_double);
      num_long_double /= den_long_double;
      int extra_exp(num_extra_exp - den_extra_exp);

      if (extra_exp != 0) num_long_double *= pow(10.0L, extra_exp);

      return FlonumKev(static_cast<double>(negative ?
					   -num_long_double :
					   num_long_double));
    }
    
    *ok = false;
    return FlonumKev();
  }
  
  num_str[num_idx] = '\0';
  std::istringstream ss(num_str);
  double value;
  *ok = ss >> value;

  if (num_extra_exp != 0) value *= pow(10, num_extra_exp);

  return FlonumKev(negative ? -value : value);
}

bool FlonumKev::copyExponentPart(const QChar* iter, const QChar* end_iter, char* str, int* idx) {
  str[(*idx)++] = 'e';
  ++iter;
  
  if (*iter == '+' || *iter == '-') {
    str[(*idx)++] = iter->toAscii();
    ++iter;
  }
  
  if (iter == end_iter)
    return false;
  
  // skip negligible zeros
  while (iter != end_iter && *iter == '0') ++iter;
  
  for (; iter != end_iter; ++iter) {
    if (!iter->isDigit())
      return false; // error

    if (*idx < MAX_LENGTH) str[(*idx)++] = iter->toAscii();
  }
  
  return true;
}

FlonumKev FlonumKev::abs() const {
  return FlonumKev(std::abs(value_));
}

FlonumKev FlonumKev::ceiling() const {
  return FlonumKev(std::ceil(value_));
}

FlonumKev FlonumKev::floor() const {
  return FlonumKev(std::floor(value_));
}

FlonumKev FlonumKev::round() const {
  double add_half(value_ + 0.5);
  double r(std::floor(add_half));
  return FlonumKev(add_half == r ? std::floor(r / 2) * 2 : r);
}

FlonumKev FlonumKev::truncate() const {
  return FlonumKev(value_ < 0 ? std::ceil(value_) : std::floor(value_));
}

FlonumKev FlonumKev::inverse() const {
  return FlonumKev(-value_);
}

FlonumKev FlonumKev::sqrt() const {
  return FlonumKev(std::sqrt(value_));
}

FlonumKev FlonumKev::sin() const {
  return FlonumKev(std::sin(value_));
}

FlonumKev FlonumKev::cos() const {
  return FlonumKev(std::cos(value_));
}

FlonumKev FlonumKev::tan() const {
  return FlonumKev(std::tan(value_));
}

FlonumKev FlonumKev::asin() const {
  return FlonumKev(std::asin(value_));
}

FlonumKev FlonumKev::acos() const {
  return FlonumKev(std::acos(value_));
}

FlonumKev FlonumKev::atan() const {
  return FlonumKev(std::atan(value_));
}

FlonumKev FlonumKev::exp() const {
  return FlonumKev(std::exp(value_));
}

FlonumKev FlonumKev::expt(FlonumKev other) const {
  return FlonumKev(std::pow(this->value_, other.value_));
}

FlonumKev FlonumKev::log() const {
  return FlonumKev(std::log(value_));
}

FlonumKev FlonumKev::reciprocal() const {
  return
    isNaN() ?
    *this :
    FlonumKev(isPositiveInfinity() ? 0.0 :
      isNegativeInfinity() ? -0.0 :
      1.0 / value_);
}

FlonumKev FlonumKev::add(KevesFixnum addend) const {
  return FlonumKev(this->value_ + addend);
}

void FlonumKev::set_sum(FlonumKev num1, KevesFixnum num2) {
  this->value_ = num1.value_ + num2;
}

FlonumKev FlonumKev::add(RationalNumberKev addend) const {
  return this->add(addend.toInexact());
}

void FlonumKev::set_sum(FlonumKev num1, RationalNumberKev num2) {
  this->set_sum(num1, num2.toInexact());
}

FlonumKev FlonumKev::add(FlonumKev addend) const {
  return FlonumKev(this->value_ + addend.value_);
}

void FlonumKev::set_sum(FlonumKev num1, FlonumKev num2) {
  this->value_ = num1.value_ + num2.value_;
}

InexactComplexNumberKev FlonumKev::add(ExactComplexNumberKev addend) const {
  return this->add(addend.toInexact());
}

InexactComplexNumberKev FlonumKev::add(InexactComplexNumberKev addend) const {
  return { this->add(addend.real()), addend.imag() };
}

FlonumKev FlonumKev::subtract(KevesFixnum subtrahend) const {
  return FlonumKev(this->value_ - subtrahend);
}

FlonumKev FlonumKev::subtract(RationalNumberKev subtrahend) const {
  return this->subtract(subtrahend.toInexact());
}

FlonumKev FlonumKev::subtract(FlonumKev subtrahend) const {
  return FlonumKev(this->value_ - subtrahend.value_);
}

InexactComplexNumberKev FlonumKev::subtract(ExactComplexNumberKev subtrahend) const {
  return this->subtract(subtrahend.toInexact());
}

InexactComplexNumberKev FlonumKev::subtract(InexactComplexNumberKev subtrahend) const {
  return { this->subtract(subtrahend.real()), subtrahend.imag().inverse() };
}

bool FlonumKev::isInfinity() const {
  return this->value_ == std::numeric_limits<double>::infinity() ||
    this->value_ == -std::numeric_limits<double>::infinity();
}

bool FlonumKev::isFinite() const {
  return this->value_ != std::numeric_limits<double>::infinity() &&
    this->value_ != -std::numeric_limits<double>::infinity() &&
    this->value_ == this->value_;
}

bool FlonumKev::isNegative() const {
  return this->value_ < 0.0;
}

bool FlonumKev::isPositive() const {
  return this->value_ > 0.0;
}

bool FlonumKev::isZero() const {
  return this->value_ == 0.0;
}

bool FlonumKev::isNaN() const {
  return this->value_ != this->value_;
}

bool FlonumKev::isEqualTo(FlonumKev other) const {
  return this->value_ == other.value_;
}

bool FlonumKev::isGT(FlonumKev other) const {
  return this->value_ > other.value_;
}

bool FlonumKev::isGTE(FlonumKev other) const {
  return this->value_ >= other.value_;
}

bool FlonumKev::isEven() const {
  Q_ASSERT(std::numeric_limits<fx_long>::digits > std::numeric_limits<double>::digits);
  double d(this->value_);

  if (d > std::numeric_limits<fx_long>::max() ||
      d < std::numeric_limits<fx_long>::min()) {
    return false; // It is impossible to decide, because digits is not enough.
  }

  fx_long i(d);
  return !(static_cast<double>(i) != d || (i > 0 ? (i & 1) : (-i & 1)));
}

bool FlonumKev::isOdd() const {
  Q_ASSERT(std::numeric_limits<fx_long>::digits > std::numeric_limits<double>::digits);
  double d(this->value_);

  if (d > std::numeric_limits<fx_long>::max() ||
      d < std::numeric_limits<fx_long>::min()) {
    return false; // It is impossible to decide, because digits is not enough.
  }

  fx_long i(d);
  return static_cast<double>(i) == d && (i > 0 ? (i & 1) : (-i & 1));
}

bool FlonumKev::isInteger() const {
  Q_ASSERT(std::numeric_limits<fx_long>::digits > std::numeric_limits<double>::digits);
  double d(this->value_);

  if (d > std::numeric_limits<fx_long>::max() ||
      d < std::numeric_limits<fx_long>::min()) {
    return false; // It is not reliable because digits is not enough.
  }

  fx_long i(d);
  return static_cast<double>(i) == d;
}

bool FlonumKev::isLT(FlonumKev other) const {
  return this->value_ < other.value_;
}

bool FlonumKev::isLTE(FlonumKev other) const {
  return this->value_ <= other.value_;
}

FlonumKev FlonumKev::multiply(KevesFixnum multiplier) const {
  return FlonumKev(this->value_ * multiplier);
}

FlonumKev FlonumKev::multiply(RationalNumberKev multiplier) const {
  return this->multiply(multiplier.toInexact());
}

FlonumKev FlonumKev::multiply(FlonumKev multiplier) const {
  return FlonumKev(this->value_ * multiplier.value_);
}

InexactComplexNumberKev FlonumKev::multiply(ExactComplexNumberKev multiplier) const {
  return
    { this->multiply(multiplier.real()), this->multiply(multiplier.imag()) };
}

InexactComplexNumberKev FlonumKev::multiply(InexactComplexNumberKev multiplier) const {
  return
    { this->multiply(multiplier.real()), this->multiply(multiplier.imag()) };
}

FlonumKev FlonumKev::divide(KevesFixnum divisor) const {
  return FlonumKev(this->value_ / divisor);
}

FlonumKev FlonumKev::divide(RationalNumberKev divisor) const {
  return this->divide(divisor.toInexact());
}

FlonumKev FlonumKev::divide(FlonumKev divisor) const {
  return FlonumKev(this->value_ / divisor.value_);
}

InexactComplexNumberKev FlonumKev::divide(ExactComplexNumberKev divisor) const {
  return this->divide(divisor.toInexact());
}

InexactComplexNumberKev FlonumKev::divide(InexactComplexNumberKev divisor) const {
  InexactComplexNumberKev reciprocal(divisor.reciprocal());
  return this->multiply(reciprocal);
}

FlonumKev FlonumKev::setQuotient(FlonumKev other) {
  this->value_ /= other.value_;
  return *this;
}

bool FlonumKev::isNegativeInfinity() const {
  return this->value_ == -std::numeric_limits<double>::infinity();
}

bool FlonumKev::isPositiveInfinity() const {
  return this->value_ == std::numeric_limits<double>::infinity();
}

double FlonumKev::toDouble() const {
  return this->value_;
}

QString FlonumKev::toQString(bool sign, int precision) const {
  if (this->isNaN())
    return QString("+nan.0");
  
  if (this->isPositiveInfinity())
    return QString("+inf.0");

  if (this->isNegativeInfinity())
    return QString("-inf.0");

  precision = (precision > 0 && precision < 54) ?
    static_cast<double>(precision) * std::log(2) / std::log(10) : 16;
  
  QString str((sign && !this->isNegative()) ? "+" : "");
  str.append(QString::number(value_, 'g', precision));
  if (str.indexOf('.') < 0) str.append(".0");
  return str;
}

KevesValue FlonumKev::toKevesFixnum() const {
  return (value_ >= KevesFixnum::MIN_VALUE &&
	  value_ <= KevesFixnum::MAX_VALUE &&
	  std::ceil(value_) == value_) ?
    KevesFixnum(value_) : KevesValue(EMB_UNDEF);
}

RationalNumberKev FlonumKev::toExact(KevesGC* gc) const {
  Q_ASSERT(!isNaN() && !isInfinity() && !isZero());

  constexpr double exactness(1.0e14);
  constexpr int n(36);
  int i(0);
  double f[n];
  double x(std::abs(this->value_));
  double y;
  double e(x / exactness);

  if (x < 1.0) {
    f[i++] = 0.0;
    y = 1.0 / (x + e);
    x = 1.0 / (x - e);
  } else {
    y = x + e;
    x = x - e;
  }
  
  for (; i < n; ++i) {
    if (std::floor(x) != std::floor(y)) {
      f[i] = 1.0 + std::min(std::floor(x), std::floor(y));
      ++i;
      break;
    }
    
    f[i] = std::floor(x);
    x = 1.0 / (x - std::floor(x));
    y = 1.0 / (y - std::floor(y));
  }
  
  mpq_class exact(0);

  while (i-- > 1) exact.operator=(1 / (exact + f[i]));
  
  if (this->value_ < 0) exact.operator=(-f[i] - exact);
  else exact.operator=(exact + f[i]);

  return { gc, exact };
}

FlonumKev* FlonumKev::make(KevesGC* gc, double d) {
  auto ctor = [d](void* ptr) { return new(ptr) FlonumKev(d); };
  return gc->Make(ctor, alloc_size(nullptr));
}

FlonumKev* FlonumKev::copy(KevesGC* gc) const {
  Q_ASSERT(type() == TYPE);

  auto ctor = [this](void* ptr) { return new(ptr) FlonumKev(*this); };
  return gc->Make(ctor, alloc_size(nullptr));
}



// class ExactComplexNumberKev ----------------------------------------
ExactComplexNumberKev::ExactComplexNumberKev()
  : ExactComplexNumberKev(RationalNumberKev(), RationalNumberKev()) {
}

ExactComplexNumberKev::ExactComplexNumberKev(const RationalNumberKev& real, const RationalNumberKev& imag) : MutableKev(TYPE),
    real_neg_(real.neg_), imag_neg_(imag.neg_),
    real_numerator_(real.numerator_), real_denominator_(real.denominator_),
    imag_numerator_(imag.numerator_), imag_denominator_(imag.denominator_) {
}

void ExactComplexNumberKev::CopyFrom(const ExactComplexNumberKev& other) {
  this->real_neg_ = other.real_neg_;
  this->imag_neg_ = other.imag_neg_;
  this->real_numerator_ = other.real_numerator_;
  this->real_denominator_ = other.real_denominator_;
  this->imag_numerator_ = other.imag_numerator_;
  this->imag_denominator_ = other.imag_denominator_;
}

void ExactComplexNumberKev::CopyFrom(ExactComplexNumberKev&& other) {
  this->real_neg_ = other.real_neg_;
  this->imag_neg_ = other.imag_neg_;
  this->real_numerator_ = other.real_numerator_;
  this->real_denominator_ = other.real_denominator_;
  this->imag_numerator_ = other.imag_numerator_;
  this->imag_denominator_ = other.imag_denominator_;
}

bool ExactComplexNumberKev::isRealValued() const {
  return imag().isZero();
}

bool ExactComplexNumberKev::isIntegerValued() const {
  return imag().isZero() && real().isInteger();
}

ExactComplexNumberKev ExactComplexNumberKev::inverse() const {
  Q_ASSERT(type() == TYPE);
  ExactComplexNumberKev inverse(*this);
  inverse.real_neg_ = !inverse.real_neg_;
  inverse.imag_neg_ = !inverse.imag_neg_;
  return inverse;
}

bool ExactComplexNumberKev::IsEmpty() const {
  return real().IsEmpty() || imag().IsEmpty();
}

ExactComplexNumberKev ExactComplexNumberKev::reciprocal(KevesGC* gc) const {
  if (this->real().isZero()) {
    Q_ASSERT(!this->imag().isZero());
    return { this->real(), this->imag().reciprocal().inverse() };
  }

  if (this->imag().isZero()) 
    return { this->real().reciprocal(), this->imag() };

  mpq_class mpq_real(real().makeMPQ());
  mpq_class mpq_imag(imag().makeMPQ());
  mpq_class num1(mpq_real * mpq_real);
  mpq_class num2(mpq_imag * mpq_imag);
  mpq_class num3(num1 + num2);
  
  return { RationalNumberKev(gc, mpq_real / num3),
      RationalNumberKev(gc, mpq_imag / num3).inverse() };
}

QString ExactComplexNumberKev::toQString() const {
  return
    imag().isZero() ?
    real().toQString(false) :
    (real().isZero() ?
     imag().toQString(true) :
     real().toQString(false).append(imag().toQString(true))).append('i');
}

QString ExactComplexNumberKev::toBinary(int radix) const {
  return
    imag().isZero() ?
    real().toBinary(false, radix) :
    (real().isZero() ?
     imag().toBinary(true, radix) :
     real().toBinary(false, radix).append(imag().toBinary(true, radix))).append('i');
}

RationalNumberKev ExactComplexNumberKev::real() const {
  return { real_neg_, real_numerator_, real_denominator_ };
}

RationalNumberKev ExactComplexNumberKev::imag() const {
  return { imag_neg_, imag_numerator_, imag_denominator_ };
}

void ExactComplexNumberKev::set(const RationalNumberKev& real, const RationalNumberKev& imag) {
  this->set_real(real);
  this->set_imag(imag);
}

void ExactComplexNumberKev::set_real(const RationalNumberKev& real) {
  real_neg_ = real.neg_;
  real_numerator_ = real.numerator_;
  real_denominator_ = real.denominator_;
}

void ExactComplexNumberKev::set_imag(const RationalNumberKev& imag) {
  imag_neg_ = imag.neg_;
  imag_numerator_ = imag.numerator_;
  imag_denominator_ = imag.denominator_;
}

InexactComplexNumberKev ExactComplexNumberKev::toInexact() const {
  return { real().toInexact(), imag().toInexact() };
}

KevesValue ExactComplexNumberKev::makeFromBinary(KevesGC* gc, StringKev str_real, StringKev str_imag, int radix) {
  RationalNumberKev* real(RationalNumberKev::makeFromString(gc, str_real, radix));
  RationalNumberKev* imag(RationalNumberKev::makeFromString(gc, str_imag, radix));

  if (!real || !imag)
    return EMB_NULL;

  if (imag->numerator()->isZero()) {
    KevesValue int_num(KevesFixnum(real->toKevesFixnum()));
    return int_num.IsFixnum() ? int_num : KevesValue(real);
  }

  ExactComplexNumberKev* complex(make(gc));
  complex->set(*real, *imag);
  return complex;
}

ExactComplexNumberKev* ExactComplexNumberKev::make(KevesGC* gc) {
  auto ctor = [](void* ptr) { return new(ptr) ExactComplexNumberKev(); };
  return gc->Make(ctor, alloc_size(nullptr));
}

ExactComplexNumberKev ExactComplexNumberKev::add(KevesGC* gc, KevesFixnum addend) const {
  return { this->real().add(gc, addend), this->imag() };
}

ExactComplexNumberKev ExactComplexNumberKev::add(KevesGC* gc, RationalNumberKev addend) const {
  return { this->real().add(gc, addend), this->imag() };
}

InexactComplexNumberKev ExactComplexNumberKev::add(FlonumKev addend) const {
  return this->toInexact().add(addend);
}

ExactComplexNumberKev ExactComplexNumberKev::add(KevesGC* gc, ExactComplexNumberKev addend) const {
  return { this->real().add(gc, addend.real()),
      this->imag().add(gc, addend.imag()) };
}

InexactComplexNumberKev ExactComplexNumberKev::add(InexactComplexNumberKev addend) const {
  return this->toInexact().add(addend);
}

ExactComplexNumberKev ExactComplexNumberKev::subtract(KevesGC* gc, KevesFixnum subtrahend) const {
  return { this->real().subtract(gc, subtrahend), this->imag() };
}

ExactComplexNumberKev ExactComplexNumberKev::subtract(KevesGC* gc, RationalNumberKev subtrahend) const {
  return { this->real().subtract(gc, subtrahend), this->imag() };
}

InexactComplexNumberKev ExactComplexNumberKev::subtract(FlonumKev subtrahend) const {
  return this->toInexact().subtract(subtrahend);
}

ExactComplexNumberKev ExactComplexNumberKev::subtract(KevesGC* gc, ExactComplexNumberKev subtrahend) const {
  return { this->real().subtract(gc, subtrahend.real()),
      this->imag().subtract(gc, subtrahend.imag()) };
}

InexactComplexNumberKev ExactComplexNumberKev::subtract(InexactComplexNumberKev subtrahend) const {
  return this->toInexact().subtract(subtrahend);
}

ExactComplexNumberKev ExactComplexNumberKev::multiply(KevesGC* gc, KevesFixnum multiplier) const {
  return { this->real().multiply(gc, multiplier),
      this->imag().multiply(gc, multiplier) };
}

ExactComplexNumberKev ExactComplexNumberKev::multiply(KevesGC* gc, RationalNumberKev multiplier) const {
  return { this->real().multiply(gc, multiplier),
      this->imag().multiply(gc, multiplier) };
}

InexactComplexNumberKev ExactComplexNumberKev::multiply(FlonumKev multiplier) const {
  return this->toInexact().multiply(multiplier);
}

ExactComplexNumberKev ExactComplexNumberKev::multiply(KevesGC* gc, ExactComplexNumberKev multiplier) const {
  mpq_class multiplicand_real(real().makeMPQ());
  mpq_class multiplicand_imag(imag().makeMPQ());
  mpq_class multiplier_real(multiplier.real().makeMPQ());
  mpq_class multiplier_imag(multiplier.imag().makeMPQ());
  mpq_class num1(multiplicand_real * multiplier_real);
  mpq_class num2(multiplicand_imag * multiplier_imag);
  mpq_class num3(multiplicand_real * multiplier_imag);
  mpq_class num4(multiplicand_imag * multiplier_real);

  return { RationalNumberKev(gc, num1 - num2),
      RationalNumberKev(gc, num3 + num4) };
}

InexactComplexNumberKev ExactComplexNumberKev::multiply(InexactComplexNumberKev multiplier) const {
  return this->toInexact().multiply(multiplier);
}

ExactComplexNumberKev ExactComplexNumberKev::divide(KevesGC* gc, KevesFixnum divisor) const {
  return { this->real().divide(gc, divisor), this->imag().divide(gc, divisor) };
}

ExactComplexNumberKev ExactComplexNumberKev::divide(KevesGC* gc, RationalNumberKev divisor) const {
  return { this->real().divide(gc, divisor), this->imag().divide(gc, divisor) };
}

InexactComplexNumberKev ExactComplexNumberKev::divide(FlonumKev divisor) const {
  return this->toInexact().divide(divisor);
}

ExactComplexNumberKev ExactComplexNumberKev::divide(KevesGC* gc, ExactComplexNumberKev divisor) const {
  return divisor.reciprocal(gc).multiply(gc, *this);
}

InexactComplexNumberKev ExactComplexNumberKev::divide(InexactComplexNumberKev divisor) const {
  return this->toInexact().multiply(divisor.reciprocal());
}

// class InexactComplexNumberKev ----------------------------------------
// --------------------------------------------------
InexactComplexNumberKev::InexactComplexNumberKev()
  : InexactComplexNumberKev(0.0, 0.0) {
}

InexactComplexNumberKev::InexactComplexNumberKev(double real, double imag)
  : MutableKev(TYPE), real_(real), imag_(imag) {
}

InexactComplexNumberKev::InexactComplexNumberKev(const FlonumKev& real, const FlonumKev& imag) : InexactComplexNumberKev(real.toDouble(), imag.toDouble()) {
}

void InexactComplexNumberKev::CopyFrom(const InexactComplexNumberKev& other) {
  this->real_ = other.real_;
  this->imag_ = other.imag_;
}

void InexactComplexNumberKev::CopyFrom(InexactComplexNumberKev&& other) {
  this->real_ = other.real_;
  this->imag_ = other.imag_;
}

void InexactComplexNumberKev::set(double real, double imag) {
  real_ = real;
  imag_ = imag;
}

void InexactComplexNumberKev::set(const FlonumKev& real, const FlonumKev& imag) {
  real_ = real.toDouble();
  imag_ = imag.toDouble();
}

QString InexactComplexNumberKev::toQString(int precision) const {
  return real().toQString(false, precision)
    + (imag().isZero() ? "+0.0i" : imag().toQString(true, precision) + "i");
}

FlonumKev InexactComplexNumberKev::real() const {
  return FlonumKev(real_);
}

FlonumKev InexactComplexNumberKev::imag() const {
  return FlonumKev(imag_);
}

InexactComplexNumberKev InexactComplexNumberKev::reciprocal() const {
  FlonumKev real(this->real());
  FlonumKev imag(this->imag());

  if (real.isZero() && !imag.isZero()) {
    imag.CopyFrom(imag.inverse().reciprocal());
  } else if (!real.isZero() && imag.isZero()) {
    real.CopyFrom(real.inverse().reciprocal());
  } else if (real.isZero() && imag.isZero()) {
    real.set(real.isNegative() ?
	     std::numeric_limits<double>::infinity() :
	     -std::numeric_limits<double>::infinity());
    imag.set(imag.isNegative() ?
	     std::numeric_limits<double>::infinity() :
	     -std::numeric_limits<double>::infinity());
  } else {
    FlonumKev d(real.multiply(real).add(imag.multiply(imag)));
    real.setQuotient(d);
    imag.setQuotient(d.inverse());
  }

  return { real, imag };
}

InexactComplexNumberKev InexactComplexNumberKev::inverse() const {
  return { -real_, -imag_ };
}

bool InexactComplexNumberKev::isRealValued() const {
  return imag().isZero();
}

bool InexactComplexNumberKev::isIntegerValued() const {
  return imag().isZero() && real().isInteger();
}

InexactComplexNumberKev InexactComplexNumberKev::add(KevesFixnum addend) const {
  FlonumKev real(this->real().add(addend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::add(RationalNumberKev addend) const {
  FlonumKev real(this->real().add(addend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::add(FlonumKev addend) const {
  FlonumKev real(this->real().add(addend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::add(ExactComplexNumberKev addend) const {
  return this->add(addend.toInexact());
}

InexactComplexNumberKev InexactComplexNumberKev::add(InexactComplexNumberKev addend) const {
  return { this->real_ + addend.real_, this->imag_ + addend.imag_ };
}

InexactComplexNumberKev InexactComplexNumberKev::subtract(KevesFixnum subtrahend) const {
  FlonumKev real(this->real().subtract(subtrahend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::subtract(RationalNumberKev subtrahend) const {
  FlonumKev real(this->real().subtract(subtrahend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::subtract(FlonumKev subtrahend) const {
  FlonumKev real(this->real().subtract(subtrahend));
  return { real, this->imag() };
}

InexactComplexNumberKev InexactComplexNumberKev::subtract(ExactComplexNumberKev subtrahend) const {
  return this->subtract(subtrahend.toInexact());
}

InexactComplexNumberKev InexactComplexNumberKev::subtract(InexactComplexNumberKev subtrahend) const {
  return { this->real_ - subtrahend.real_, this->imag_ - subtrahend.imag_ };
}

InexactComplexNumberKev InexactComplexNumberKev::multiply(KevesFixnum multiplier) const {
  FlonumKev real(this->real().multiply(multiplier));
  FlonumKev imag(this->imag().multiply(multiplier));
  return { real, imag };
}

InexactComplexNumberKev InexactComplexNumberKev::multiply(RationalNumberKev multiplier) const {
  FlonumKev real(this->real().multiply(multiplier));
  FlonumKev imag(this->imag().multiply(multiplier));
  return { real, imag };
}

InexactComplexNumberKev InexactComplexNumberKev::multiply(FlonumKev multiplier) const {
  FlonumKev real(this->real().multiply(multiplier));
  FlonumKev imag(this->imag().multiply(multiplier));
  return { real, imag };
}

InexactComplexNumberKev InexactComplexNumberKev::multiply(ExactComplexNumberKev multiplier) const {
  return this->multiply(multiplier.toInexact());
}

InexactComplexNumberKev InexactComplexNumberKev::multiply(InexactComplexNumberKev multiplier) const {
  FlonumKev num1(this->real().multiply(multiplier.imag()));
  FlonumKev num2(this->imag().multiply(multiplier.real()));
  FlonumKev num3(this->real().multiply(multiplier.real()));
  FlonumKev num4(this->imag().multiply(multiplier.imag()));
  return { num3.subtract(num4), num1.add(num2) };
}

InexactComplexNumberKev InexactComplexNumberKev::divide(KevesFixnum divisor) const {
  return { this->real().divide(divisor), this->imag().divide(divisor) };
}

InexactComplexNumberKev InexactComplexNumberKev::divide(RationalNumberKev divisor) const {
  return { this->real().divide(divisor), this->imag().divide(divisor) };
}

InexactComplexNumberKev InexactComplexNumberKev::divide(FlonumKev divisor) const {
  return { this->real().divide(divisor), this->imag().divide(divisor) };
}

InexactComplexNumberKev InexactComplexNumberKev::divide(ExactComplexNumberKev divisor) const {
  return this->divide(divisor.toInexact());
}

InexactComplexNumberKev InexactComplexNumberKev::divide(InexactComplexNumberKev divisor) const {
  return this->multiply(divisor.reciprocal());
}

bool InexactComplexNumberKev::isEqualTo(InexactComplexNumberKev other) const {
  return this->real().isEqualTo(other.real()) &&
    this->imag().isEqualTo(other.imag());
}

KevesValue InexactComplexNumberKev::makeFromBinary(KevesGC* gc, StringKev str_real, StringKev str_imag, int radix) {
  double real(str_real.Equals("0") ?
	      0.0 :
	      static_cast<double>(str_real.ToInt(nullptr, radix)));

  if (str_imag.Equals("0"))
    return FlonumKev::make(gc, real);

  double imag(static_cast<double>(str_imag.ToInt(nullptr, radix)));
  
  auto ctor = [real, imag](void* ptr) {
    return new(ptr) InexactComplexNumberKev(real, imag);
  };

  return gc->Make(ctor, alloc_size(nullptr));
}

InexactComplexNumberKev* InexactComplexNumberKev::makeFromFlonums(KevesGC* gc, FlonumKev real, FlonumKev imag) {
  auto ctor = [real, imag](void* ptr) {
    return new(ptr) InexactComplexNumberKev(real, imag);
  };

  return gc->Make(ctor, alloc_size(nullptr));
}
