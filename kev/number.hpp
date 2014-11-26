// keves/kev/number.hpp - pairs for Keves
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

#include <cmath>
#include <functional>
#include "kev/bignum.hpp"
#include "value/fixnum.hpp"


class ExactComplexNumberKev;
class RationalNumberKev;
class InexactComplexNumberKev;
class FlonumKev;
class StringKev;
class QChar;


class RationalNumberKev : public MutableKev {
  friend class ExactComplexNumberKev;

public:
  static constexpr kev_type TYPE = RATIONAL_NUM;

  RationalNumberKev();
  RationalNumberKev(const RationalNumberKev&) = default;
  RationalNumberKev(RationalNumberKev&&) = default;
  RationalNumberKev& operator=(const RationalNumberKev&) = delete;
  RationalNumberKev& operator=(RationalNumberKev&&) = delete;
  ~RationalNumberKev() = default;
  
  RationalNumberKev(bool, const Bignum*, const Bignum*);
  RationalNumberKev(KevesGC*, const mpq_class&);

  RationalNumberKev abs() const;
  FlonumKev add(FlonumKev) const;
  RationalNumberKev add(KevesGC*, KevesFixnum) const;
  RationalNumberKev add(KevesGC*, RationalNumberKev) const;
  FlonumKev add(KevesGC*) const;
  ExactComplexNumberKev add(KevesGC*, ExactComplexNumberKev) const;
  InexactComplexNumberKev add(InexactComplexNumberKev) const;
  void CopyFrom(const RationalNumberKev&);
  void CopyFrom(RationalNumberKev&&);
  const Bignum* denominator() const;
  RationalNumberKev divide(KevesGC*, KevesFixnum) const;
  RationalNumberKev divide(KevesGC*, RationalNumberKev) const;
  FlonumKev divide(FlonumKev) const;
  ExactComplexNumberKev divide(KevesGC*, ExactComplexNumberKev) const;
  InexactComplexNumberKev divide(InexactComplexNumberKev) const;
  RationalNumberKev ceiling(KevesGC*) const;
  RationalNumberKev floor(KevesGC*) const;
  RationalNumberKev round(KevesGC*) const;
  RationalNumberKev truncate(KevesGC*) const;
  RationalNumberKev inverse() const;
  bool IsEmpty() const;
  bool isEqualTo(RationalNumberKev) const;
  bool isEven() const;
  bool isInteger() const;
  bool isNegative() const;
  bool isOdd() const;
  bool isPositive() const;
  bool isZero() const;
  mpq_class makeMPQ() const;
  RationalNumberKev multiply(KevesGC*, KevesFixnum) const;
  RationalNumberKev multiply(KevesGC*, RationalNumberKev) const;
  ExactComplexNumberKev multiply(KevesGC*, ExactComplexNumberKev) const;
  FlonumKev multiply(FlonumKev) const;
  InexactComplexNumberKev multiply(InexactComplexNumberKev) const;
  void multiplyByExponentiationWithBase2(KevesGC*, int);
  const Bignum* numerator() const;
  KevesValue optimize();
  RationalNumberKev reciprocal() const;
  void reduce(KevesGC*);

  void set(bool neg, const Bignum* numerator, const Bignum* denominator){
    this->neg_ = neg;
    this->numerator_ = numerator;
    this->denominator_ = denominator;
  }

  RationalNumberKev subtract(KevesGC*, KevesFixnum) const;
  RationalNumberKev subtract(KevesGC*, RationalNumberKev) const;
  ExactComplexNumberKev subtract(KevesGC*, ExactComplexNumberKev) const;
  FlonumKev subtract(FlonumKev) const;
  InexactComplexNumberKev subtract(InexactComplexNumberKev) const;
  QString toBinary(bool, int) const;
  FlonumKev toInexact() const;
  KevesValue toKevesFixnum() const;
  QString toQString(bool sign) const;

  int compare(RationalNumberKev) const;
  bool isLT(RationalNumberKev) const;
  bool isLT(FlonumKev) const;
  bool isLTE(RationalNumberKev) const;
  bool isGT(RationalNumberKev) const;
  bool isGT(FlonumKev) const;
  bool isGTE(RationalNumberKev) const;

  static RationalNumberKev fromInt(KevesGC*, bg_int);
  static KevesValue makeRationalFromString(KevesGC*, bool, StringKev, StringKev);
  static KevesValue makeRationalFromString(KevesGC*, bool, StringKev, StringKev, int);
  static KevesValue makeFractionFromString(KevesGC*, bool, StringKev, StringKev, int);
  static KevesValue makeIntegerFromString(KevesGC*, bool, StringKev, int);
  static RationalNumberKev* makeFromInt(KevesIterator*, bg_int);
  static RationalNumberKev* makeFromInt(KevesGC*, bg_int);
  static RationalNumberKev* makeFromLong(KevesGC*, bg_long);
  static RationalNumberKev* makeFromMPQ(KevesGC*, const mpq_class&);
  static RationalNumberKev* makeFromString(KevesGC*, StringKev);
  static RationalNumberKev* makeFromString(KevesGC*, StringKev, int);

  template<class ZONE>
  static std::function<RationalNumberKev*(void*)> ctorFromInt(ZONE* zone, bg_int real) {
    return [zone, real](void* ptr) {
      return real < 0 ?
        new(ptr) RationalNumberKev(true,
				   Bignum::makeFromInt(zone, -real),
				   Bignum::makeFromInt(zone, 1)) :
	new(ptr) RationalNumberKev(false,
				   Bignum::makeFromInt(zone, real),
				   Bignum::makeFromInt(zone, 1));
    };
  }

  template<class ZONE>
  static std::function<RationalNumberKev*(void*)> ctorFromLong(ZONE* zone, bg_long real) {
    return [zone, real](void* ptr) {
      return real < 0 ?
        new(ptr) RationalNumberKev(true,
				   Bignum::makeFromLong(zone, -real),
				   Bignum::makeFromLong(zone, 1)) :
	new(ptr) RationalNumberKev(false,
				   Bignum::makeFromLong(zone, real),
				   Bignum::makeFromLong(zone, 1));
    };
  }

  template<class ZONE>
  static std::function<RationalNumberKev*(void*)> ctorFromMPQ(ZONE* zone, const mpq_class& mpq_num) {
    return [zone, &mpq_num](void* ptr) {
      return new(ptr) RationalNumberKev(sgn(mpq_num) < 0,
					Bignum::makeFromMPZ(zone,
							    mpq_num.get_num()),
					Bignum::makeFromMPZ(zone,
							    mpq_num.get_den()));
    };
  }

private:
  double toDouble() const;

  template<class ZONE>
  static RationalNumberKev* Make(ZONE* zone);

protected:
  bool neg_;
  const Bignum* numerator_;
  const Bignum* denominator_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(RationalNumberKev);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<RationalNumberKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<RationalNumberKev>* num(FixedLengthKev<RationalNumberKev>::From(kev));
    VariableLengthKev<Bignum>* numerator(VariableLengthKev<Bignum>::From(num->numerator()));
    VariableLengthKev<Bignum>* denominator(VariableLengthKev<Bignum>::From(num->denominator()));
    num->numerator_ = zone->Copy(numerator);
    num->denominator_ = zone->Copy(denominator);
    return num->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const RationalNumberKev* rational(value);
    BASE::PushValue(pending, rational->numerator_);
    BASE::PushValue(pending, rational->denominator_);
  }

  template<class BASE, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const RationalNumberKev* rational(value);

    out << static_cast<uioword>(rational->type())
	<< static_cast<ioword>(rational->neg_)
	<< BASE::IndexAddress(list, rational->numerator_)
	<< BASE::IndexAddress(list, rational->denominator_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword neg;
    uioword numerator, denominator;
    in >> neg >> numerator >> denominator;
    RationalNumberKev* rational(Make(gc));
    
    rational->set(static_cast<bool>(neg),
		  KevesValue::template FromUioword<Bignum>(numerator),
		  KevesValue::template FromUioword<Bignum>(denominator));

    return rational;
  }
  
  template<class BASE, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue kev) {
    RationalNumberKev* rational(kev);
    BASE::RevertValue(object_list, &rational->numerator_);
    BASE::RevertValue(object_list, &rational->denominator_);
  }
};



class RationalNumberKevFromInt : public RationalNumberKev {
public:
  RationalNumberKevFromInt() = delete;
  RationalNumberKevFromInt(const RationalNumberKevFromInt&) = delete;
  RationalNumberKevFromInt(RationalNumberKevFromInt&&) = delete;
  RationalNumberKevFromInt& operator=(const RationalNumberKevFromInt&);
  RationalNumberKevFromInt& operator=(RationalNumberKevFromInt&&);
  ~RationalNumberKevFromInt() = default;

  explicit RationalNumberKevFromInt(bg_int);
  RationalNumberKevFromInt(bg_int, bg_int);

private:
  BignumInt bignum_numerator_;
  BignumInt bignum_denominator_;
};



class RationalNumberKevFromLong : public RationalNumberKev {
public:
  RationalNumberKevFromLong() = delete;
  RationalNumberKevFromLong(const RationalNumberKevFromLong&) = delete;
  RationalNumberKevFromLong(RationalNumberKevFromLong&&) = delete;
  RationalNumberKevFromLong& operator=(const RationalNumberKevFromLong&);
  RationalNumberKevFromLong& operator=(RationalNumberKevFromLong&&);
  ~RationalNumberKevFromLong() = default;

  RationalNumberKevFromLong(bg_long);
  RationalNumberKevFromLong(bg_long, bg_long);
  
private:
  BignumLong bignum_numerator_;
  BignumLong bignum_denominator_;
};

class FlonumKev : public MutableKev {
public:
  static constexpr int MAX_LENGTH = 30;
  static constexpr int MAX_SIGNIFICAND_LENGTH = MAX_LENGTH - 5;
  static constexpr kev_type TYPE = FLONUM;

  FlonumKev();
  FlonumKev(const FlonumKev&) = default;
  FlonumKev(FlonumKev&&) = default;
  FlonumKev& operator=(const FlonumKev&) = delete;
  FlonumKev& operator=(FlonumKev&&) = delete;
  ~FlonumKev() = default;
  
  explicit FlonumKev(double);

  FlonumKev abs() const;
  FlonumKev acos() const;
  FlonumKev add(KevesFixnum) const;
  FlonumKev add(RationalNumberKev) const;
  FlonumKev add(FlonumKev) const;
  InexactComplexNumberKev add(ExactComplexNumberKev) const;
  InexactComplexNumberKev add(InexactComplexNumberKev) const;
  FlonumKev asin() const;
  FlonumKev atan() const;
  FlonumKev ceiling() const;
  FlonumKev cos() const;
  FlonumKev* copy(KevesGC*) const;
  void CopyFrom(const FlonumKev&);
  void CopyFrom(FlonumKev&&);
  FlonumKev divide(KevesFixnum) const;
  FlonumKev divide(RationalNumberKev) const;
  FlonumKev divide(FlonumKev) const;
  InexactComplexNumberKev divide(ExactComplexNumberKev) const;
  InexactComplexNumberKev divide(InexactComplexNumberKev) const;
  FlonumKev exp() const;
  FlonumKev expt(FlonumKev) const;
  FlonumKev floor() const;
  bool isEqualTo(FlonumKev) const;
  bool isEven() const;
  bool isFinite() const;
  bool isInfinity() const;
  bool isInteger() const;
  bool isLT(FlonumKev) const;
  bool isLTE(FlonumKev) const;
  bool isGT(FlonumKev) const;
  bool isGTE(FlonumKev) const;
  bool isNaN() const;
  bool isNegative() const;
  bool isNegativeInfinity() const;
  bool isOdd() const;
  bool isPositive() const;
  bool isPositiveInfinity() const;
  bool isZero() const;
  FlonumKev inverse() const;
  FlonumKev log() const;
  FlonumKev multiply(KevesFixnum) const;
  FlonumKev multiply(RationalNumberKev) const;
  FlonumKev multiply(FlonumKev) const;
  InexactComplexNumberKev multiply(ExactComplexNumberKev) const;
  InexactComplexNumberKev multiply(InexactComplexNumberKev) const;
  FlonumKev reciprocal() const;
  FlonumKev round() const;

  void set(double value) {
    value_ = value;
  }

  FlonumKev setQuotient(FlonumKev);
  void set_sum(FlonumKev, KevesFixnum);
  void set_sum(FlonumKev, RationalNumberKev);
  void set_sum(FlonumKev, FlonumKev);
  FlonumKev sin() const;
  FlonumKev sqrt() const;
  FlonumKev subtract(KevesFixnum) const;
  FlonumKev subtract(RationalNumberKev) const;
  FlonumKev subtract(FlonumKev) const;
  InexactComplexNumberKev subtract(ExactComplexNumberKev) const;
  InexactComplexNumberKev subtract(InexactComplexNumberKev) const;
  FlonumKev tan() const;
  double toDouble() const;
  RationalNumberKev toExact(KevesGC*) const;
  KevesValue toKevesFixnum() const;
  QString toQString(bool sign, int) const;
  FlonumKev truncate() const;

  template<class ZONE>
  static FlonumKev* Make(ZONE* zone, double);

  static FlonumKev fromString(StringKev, bool*);

private:
  static bool copyExponentPart(const QChar*, const QChar*, char*, int*);

  double value_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(FlonumKev);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<FlonumKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* /*zone*/, MutableKev* kev) {
    return FixedLengthKev<FlonumKev>::From(kev)->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class /*BASE*/, class STACK>
  static void PushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    return;
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void WriteObject(const LIST& /*list*/, STREAM& out, KevesValue value) {
    const FlonumKev* flonum(value);
    out << static_cast<uioword>(flonum->type()) << flonum->value_;
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    double value;
    in >> value;
    return Make(gc, value);
  }
  
  template<class /*BASE*/, class LIST>
  static void RevertObject(const LIST& /*object_list*/,
			   MutableKevesValue /*kev*/) {
    return;
  }
};

class InexactComplexNumberKev;

class ExactComplexNumberKev : public MutableKev {
public:
  static constexpr kev_type TYPE = EXCT_CPLX_NUM;

  ExactComplexNumberKev();
  ExactComplexNumberKev(const ExactComplexNumberKev&) = default;
  ExactComplexNumberKev(ExactComplexNumberKev&&) = default;
  ExactComplexNumberKev& operator=(const ExactComplexNumberKev&) = delete;
  ExactComplexNumberKev& operator=(ExactComplexNumberKev&&) = delete;
  ~ExactComplexNumberKev() = default;
  
  ExactComplexNumberKev(const RationalNumberKev&, const RationalNumberKev&);

  InexactComplexNumberKev add(FlonumKev) const;
  InexactComplexNumberKev add(InexactComplexNumberKev) const;
  ExactComplexNumberKev add(KevesGC*, KevesFixnum) const;
  ExactComplexNumberKev add(KevesGC*, RationalNumberKev) const;
  ExactComplexNumberKev add(KevesGC*, ExactComplexNumberKev) const;
  void CopyFrom(const ExactComplexNumberKev&);
  void CopyFrom(ExactComplexNumberKev&&);
  ExactComplexNumberKev divide(KevesGC*, KevesFixnum) const;
  ExactComplexNumberKev divide(KevesGC*, RationalNumberKev) const;
  InexactComplexNumberKev divide(FlonumKev) const;
  ExactComplexNumberKev divide(KevesGC*, ExactComplexNumberKev) const;
  InexactComplexNumberKev divide(InexactComplexNumberKev) const;
  RationalNumberKev imag() const;
  ExactComplexNumberKev inverse() const;
  bool IsEmpty() const;
  ExactComplexNumberKev multiply(KevesGC*, KevesFixnum) const;
  ExactComplexNumberKev multiply(KevesGC*, RationalNumberKev) const;
  ExactComplexNumberKev multiply(KevesGC*, ExactComplexNumberKev) const;
  InexactComplexNumberKev multiply(FlonumKev) const;
  InexactComplexNumberKev multiply(InexactComplexNumberKev) const;
  ExactComplexNumberKev reciprocal(KevesGC*) const;
  RationalNumberKev real() const;
  void set(const RationalNumberKev&, const RationalNumberKev&);
  void set_imag(const RationalNumberKev&);
  void set_real(const RationalNumberKev&);
  ExactComplexNumberKev subtract(KevesGC*, KevesFixnum) const;
  ExactComplexNumberKev subtract(KevesGC*, RationalNumberKev) const;
  InexactComplexNumberKev subtract(FlonumKev) const;
  ExactComplexNumberKev subtract(KevesGC*, ExactComplexNumberKev) const;
  InexactComplexNumberKev subtract(InexactComplexNumberKev) const;
  bool isIntegerValued() const;
  bool isRealValued() const;
  QString toBinary(int) const;
  InexactComplexNumberKev toInexact() const;
  QString toQString() const;

  template<class ZONE>
  static ExactComplexNumberKev* Make(ZONE* zone);

  static KevesValue makeFromBinary(KevesGC*, StringKev, StringKev, int);

protected:
  bool real_neg_;
  bool imag_neg_;
  const Bignum* real_numerator_;
  const Bignum* real_denominator_;
  const Bignum* imag_numerator_;
  const Bignum* imag_denominator_;

  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(ExactComplexNumberKev);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<ExactComplexNumberKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<ExactComplexNumberKev>* num(FixedLengthKev<ExactComplexNumberKev>::From(kev));
    VariableLengthKev<Bignum>* real_numerator(VariableLengthKev<Bignum>::From(num->real().numerator()));
    VariableLengthKev<Bignum>* real_denominator(VariableLengthKev<Bignum>::From(num->real().denominator()));
    VariableLengthKev<Bignum>* imag_numerator(VariableLengthKev<Bignum>::From(num->imag().numerator()));
    VariableLengthKev<Bignum>* imag_denominator(VariableLengthKev<Bignum>::From(num->imag().denominator()));
    num->real_numerator_ = zone->Copy(real_numerator);
    num->real_denominator_ = zone->Copy(real_denominator);
    num->imag_numerator_ = zone->Copy(imag_numerator);
    num->imag_denominator_ = zone->Copy(imag_denominator);
    return num->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const ExactComplexNumberKev* complex(value);
    BASE::PushValue(pending, complex->real_numerator_);
    BASE::PushValue(pending, complex->real_denominator_);
    BASE::PushValue(pending, complex->imag_numerator_);
    BASE::PushValue(pending, complex->imag_denominator_);
  }

  template<class BASE, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const ExactComplexNumberKev* complex(value);

    out << static_cast<uioword>(complex->type())
	<< static_cast<ioword>(complex->real_neg_)
	<< static_cast<ioword>(complex->imag_neg_)
	<< BASE::IndexAddress(list, complex->real_numerator_)
	<< BASE::IndexAddress(list, complex->real_denominator_)
	<< BASE::IndexAddress(list, complex->imag_numerator_)
	<< BASE::IndexAddress(list, complex->imag_denominator_);
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword real_neg, imag_neg;
    uioword real_numerator, real_denominator, imag_numerator, imag_denominator;

    in >> real_neg >> imag_neg
       >> real_numerator >> real_denominator
       >> imag_numerator >> imag_denominator;

    ExactComplexNumberKev* complex(Make(gc));
    complex->real_neg_ = real_neg;
    complex->imag_neg_ = imag_neg;

    complex->real_numerator_
      = KevesValue::template FromUioword<Bignum>(real_numerator);

    complex->real_denominator_
      = KevesValue::template FromUioword<Bignum>(real_denominator);

    complex->imag_numerator_
      = KevesValue::template FromUioword<Bignum>(imag_numerator);

    complex->imag_denominator_
      = KevesValue::template FromUioword<Bignum>(imag_denominator);
    
    return complex;
  }
  
  template<class BASE, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    ExactComplexNumberKev* complex(value);
    BASE::RevertValue(object_list, &complex->real_numerator_);
    BASE::RevertValue(object_list, &complex->real_denominator_);
    BASE::RevertValue(object_list, &complex->imag_numerator_);
    BASE::RevertValue(object_list, &complex->imag_denominator_);
  }
};



class InexactComplexNumberKev : public MutableKev {
public:
  static constexpr kev_type TYPE = INEX_CPLX_NUM;

  InexactComplexNumberKev();
  InexactComplexNumberKev(const InexactComplexNumberKev&) = default;
  InexactComplexNumberKev(InexactComplexNumberKev&&) = default;
  InexactComplexNumberKev& operator=(const InexactComplexNumberKev&) = delete;
  InexactComplexNumberKev& operator=(InexactComplexNumberKev&&) = delete;
  ~InexactComplexNumberKev() = default;
  
  InexactComplexNumberKev(double, double);
  InexactComplexNumberKev(const FlonumKev&, const FlonumKev&);

  InexactComplexNumberKev add(KevesFixnum) const;
  InexactComplexNumberKev add(RationalNumberKev) const;
  InexactComplexNumberKev add(FlonumKev) const;
  InexactComplexNumberKev add(ExactComplexNumberKev) const;
  InexactComplexNumberKev add(InexactComplexNumberKev) const;
  void CopyFrom(const InexactComplexNumberKev&);
  void CopyFrom(InexactComplexNumberKev&&);
  InexactComplexNumberKev divide(KevesFixnum) const;
  InexactComplexNumberKev divide(RationalNumberKev) const;
  InexactComplexNumberKev divide(FlonumKev) const;
  InexactComplexNumberKev divide(ExactComplexNumberKev) const;
  InexactComplexNumberKev divide(InexactComplexNumberKev) const;
  FlonumKev imag() const;
  bool isEqualTo(InexactComplexNumberKev) const;
  bool isIntegerValued() const;
  bool isRealValued() const;
  InexactComplexNumberKev inverse() const;
  InexactComplexNumberKev multiply(KevesFixnum) const;
  InexactComplexNumberKev multiply(RationalNumberKev) const;
  InexactComplexNumberKev multiply(FlonumKev) const;
  InexactComplexNumberKev multiply(ExactComplexNumberKev) const;
  InexactComplexNumberKev multiply(InexactComplexNumberKev) const;
  FlonumKev real() const;
  InexactComplexNumberKev reciprocal() const;
  void set(double, double);
  void set(const FlonumKev&, const FlonumKev&);
  InexactComplexNumberKev subtract(KevesFixnum) const;
  InexactComplexNumberKev subtract(RationalNumberKev) const;
  InexactComplexNumberKev subtract(FlonumKev) const;
  InexactComplexNumberKev subtract(ExactComplexNumberKev) const;
  InexactComplexNumberKev subtract(InexactComplexNumberKev) const;
  QString toQString(int) const;

  static KevesValue makeFromBinary(KevesGC*, StringKev, StringKev, int);
  static InexactComplexNumberKev* makeFromFlonums(KevesGC*, FlonumKev, FlonumKev);
  
private:
  double real_;
  double imag_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(InexactComplexNumberKev);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<InexactComplexNumberKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* /*zone*/, MutableKev* kev) {
    return FixedLengthKev<InexactComplexNumberKev>::From(kev)->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class /*BASE*/, class STACK>
  static void PushChildren(STACK* /*pending*/, KevesValue /*value*/) {
    return;
  }

  template<class /*BASE*/, class LIST, class STREAM>
  static void WriteObject(const LIST& /*list*/, STREAM& out, KevesValue value) {
    const InexactComplexNumberKev* complex(value);
    out << static_cast<uioword>(complex->type())
	<< complex->real_
	<< complex->imag_;
  }

  template<class /*BASE*/, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    double real, imag;
    in >> real >> imag;
    FlonumKev flonum_real(real);
    FlonumKev flonum_imag(imag);
    return makeFromFlonums(gc, flonum_real, flonum_imag);
  }
  
  template<class /*BASE*/, class LIST>
  static void RevertObject(const LIST& /*object_list*/,
			   MutableKevesValue /*kev*/) {
    return;
  }
};

namespace NumberKev {
  KevesValue difference(KevesGC*, KevesFixnum, RationalNumberKev);
  FlonumKev difference(KevesFixnum, FlonumKev);
  ExactComplexNumberKev difference(KevesGC*, KevesFixnum, ExactComplexNumberKev);
  int getPrefix(StringKev, int*, bool*);
  bool isGT(KevesValue, KevesValue);

  struct IsGT {
    static bool func(KevesFixnum, KevesFixnum);
    static bool func(RationalNumberKev, RationalNumberKev);
    static bool func(FlonumKev, FlonumKev);

    static constexpr bool func() {
      return false;
    }
  };
  
  bool isGTE(KevesValue, KevesValue);

  struct IsGTE {
    static bool func(KevesFixnum, KevesFixnum);
    static bool func(RationalNumberKev, RationalNumberKev);
    static bool func(FlonumKev, FlonumKev);

    static constexpr bool func() {
      return false;
    }
  };

  bool isLT(KevesValue, KevesValue);

  struct IsLT {
    static bool func(KevesFixnum, KevesFixnum);
    static bool func(RationalNumberKev, RationalNumberKev);
    static bool func(FlonumKev, FlonumKev);

    static constexpr bool func() {
      return false;
    }
  };

  bool isLTE(KevesValue, KevesValue);

  struct IsLTE {
    static bool func(KevesFixnum, KevesFixnum);
    static bool func(RationalNumberKev, RationalNumberKev);
    static bool func(FlonumKev, FlonumKev);

    static constexpr bool func() {
      return false;
    }
  };

  bool isNegative(KevesValue);

  struct IsNegative {
    static bool func(KevesFixnum num) {
      return num.isNegative();
    }

    static bool func(const RationalNumberKev& num) {
      return num.isNegative();
    }

    static bool func(const FlonumKev& num) {
      return num.isNegative();
    }

    static constexpr bool func() {
      return false;
    }
  };
  
  bool isPositive(KevesValue);

  struct IsPositive {
    static bool func(KevesFixnum num) {
      return num.isPositive();
    }

    static bool func(const RationalNumberKev& num) {
      return num.isPositive();
    }

    static bool func(const FlonumKev& num) {
      return num.isPositive();
    }

    static constexpr bool func() {
      return false;
    }
  };
  
  bool isZero(KevesValue);

  struct IsZero {
    static bool func(KevesFixnum num) {
      return num.isZero();
    }

    static bool func(const RationalNumberKev& num) {
      return num.isZero();
    }

    static bool func(const FlonumKev& num) {
      return num.isZero();
    }

    static bool func(const ExactComplexNumberKev& num) {
      return num.real().isZero() && num.imag().isZero();
    }

    static bool func(const InexactComplexNumberKev& num) {
      return num.real().isZero() && num.imag().isZero();
    }

    static constexpr bool func() {
      return false;
    }
  };
  
  FlonumKev makeFlonum(KevesValue);
  KevesValue makeFromBinary(KevesGC*, StringKev, int, bool);
  KevesValue makeFromDecimal(KevesGC*, StringKev, int, bool);
  KevesValue makeFromMPQ(KevesGC*, const mpq_class&);
  KevesValue makeRealNumberFromBinary(KevesGC*, StringKev, int*, int);
  KevesValue makeRealNumberFromBinary_helper1(KevesGC*, StringKev, int*, int, bool);
  KevesValue makeRealNumberFromDecimal(KevesGC*, StringKev, int*, int*, bool*);
  KevesValue makeRealNumberFromDecimal_helper1(KevesGC*, StringKev, int*, int*, bool*, bool);
  KevesValue makeRealNumberFromDecimal_helper2(KevesGC*, StringKev, int*, int*, bool*, bool, StringKev);
  KevesValue makeRealNumberFromDecimal_helper3(KevesGC*, StringKev, int*, int*, bool*, bool, StringKev, StringKev);
  StringKev* makeStringFromBinary(KevesGC*, KevesValue, int);
  StringKev* makeStringFromDecimal(KevesGC*, KevesValue, int);
  StringKev* makeStringFromNumber(KevesGC*, KevesValue, int, int);
  KevesValue max(KevesGC*, KevesValue, KevesValue);
  KevesValue min(KevesGC*, KevesValue, KevesValue);
  RationalNumberKev quotient(KevesGC*, KevesFixnum, RationalNumberKev);
  FlonumKev quotient(KevesFixnum, FlonumKev);
  ExactComplexNumberKev quotient(KevesGC*, KevesFixnum, ExactComplexNumberKev);
  InexactComplexNumberKev quotient(KevesFixnum, InexactComplexNumberKev);

  template<class NUMBER_KEV> static bool predicateForReal(KevesValue x) {
    if (x.IsFixnum()) {
      KevesFixnum num(x);
      return NUMBER_KEV::func(num);
    }

    if (x.IsPtr()) {
      switch (x.type()) {
      case RationalNumberKev::TYPE:
	{
	  const RationalNumberKev* num(x);
	  return NUMBER_KEV::func(*num);
	}
	
      case FlonumKev::TYPE:
	{
	  const FlonumKev* num(x);
	  return NUMBER_KEV::func(*num);
	}

      default:
	break;
      }
    }
    
    return NUMBER_KEV::func();
  }

  template<class NUMBER_KEV> static bool predicate(KevesValue x) {
    if (x.IsFixnum()) {
      KevesFixnum num(x);
      return NUMBER_KEV::func(num);
    }

    if (x.IsPtr()) {
      switch (x.type()) {
      case RationalNumberKev::TYPE: {
	const RationalNumberKev* num(x);
	return NUMBER_KEV::func(*num);
      }
	
      case FlonumKev::TYPE: {
	const FlonumKev* num(x);
	return NUMBER_KEV::func(*num);
      }

      case ExactComplexNumberKev::TYPE: {
	const ExactComplexNumberKev* num(x);
	return NUMBER_KEV::func(*num);
      }

      case InexactComplexNumberKev::TYPE: {
	const InexactComplexNumberKev* num(x);
	return NUMBER_KEV::func(*num);
      }
	
      default:
	break;
      }
    }
    
    return NUMBER_KEV::func();
  }

  template<class> bool compare_helper1(KevesValue, KevesValue);
  template<class> bool compare_helper2(KevesValue, KevesValue);

  template<class NUMBER_KEV> bool compare(KevesValue x, KevesValue y) {
    if (y.IsFixnum()) {
      KevesFixnum num2(y);
      
      if (x.IsFixnum()) {
	KevesFixnum num1(x);
	return NUMBER_KEV::func(num1, num2);
      }
      
      if (x.IsPtr()) {
	if (x.type() == RationalNumberKev::TYPE) {
	  const RationalNumberKev* num1(x);
	  const RationalNumberKevFromInt rational(num2);
	  return NUMBER_KEV::func(*num1, rational);
	}
	
	if (x.type() == FlonumKev::TYPE) {
	  const FlonumKev* num1(x);
	  const FlonumKev flonum(num2);
	  return NUMBER_KEV::func(*num1, flonum);
	}
	
      }
      
      return NUMBER_KEV::func();
    }

    return compare_helper1<NUMBER_KEV>(x, y);
  }
  
  template<class NUMBER_KEV> bool compare_helper1(KevesValue x, KevesValue y) {
    if (y.Is<RationalNumberKev>()) {
      const RationalNumberKev* num2(y);
      
      if (x.IsFixnum()) {
	KevesFixnum num1(x);
	return NUMBER_KEV::func(RationalNumberKevFromInt(num1), *num2);
      }
      
      if (x.IsPtr()) {
	if (x.type() == RationalNumberKev::TYPE) {
	  const RationalNumberKev* num1(x);
	  return NUMBER_KEV::func(*num1, *num2);
	}
	
	if (x.type() == FlonumKev::TYPE) {
	  const FlonumKev* num1(x);
	  return NUMBER_KEV::func(*num1, num2->toInexact());
	}

      }
      
      return NUMBER_KEV::func();
    }
    
    return compare_helper2<NUMBER_KEV>(x, y);
  }
  
  template<class NUMBER_KEV> bool compare_helper2(KevesValue x, KevesValue y) {
    if (y.Is<FlonumKev>()) {
      const FlonumKev* num2(y);
      
      if (x.IsFixnum()) {
	KevesFixnum num1(x);
	FlonumKev flonum(num1);
	return NUMBER_KEV::func(flonum, *num2);
      }
      
      if (x.IsPtr()) {
	if (x.type() == RationalNumberKev::TYPE) {
	  const RationalNumberKev* num1(x);
	  return NUMBER_KEV::func(num1->toInexact(), *num2);
	}
	
	if (x.type() == FlonumKev::TYPE) {
	  const FlonumKev* num1(x);
	  return NUMBER_KEV::func(*num1, *num2);
	}
      }
    }
    
    return NUMBER_KEV::func();
  }
  
  // private:
  // int getSignOfNumberString(StringKev, bool*);
  // int getEndOfDigitString(StringKev, int);
  // KevesValue makeFromDecimal(KevesGC*, StringKev, const StringKev*, int, bool, RationalNumberKev*);
}
