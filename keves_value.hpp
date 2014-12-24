// keves/keves_value.hpp - values for Keves
// Keves will be an R6RS Scheme implementation.
//
// Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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

#include <QtGlobal>

typedef qint64 ioword;
typedef quint64 uioword;
typedef qint32 ver_num_t;
typedef qint32 fx_int;
typedef qint64 fx_long;


class ArgumentFrameKev;
class LocalVarFrameKev;
// class FreeVarFrameKev;
class StackFrameKev;

enum emb_value {
  EMB_UNDEF	    = 0x000,
  EMB_FALSE	    = 0x009,
  EMB_TRUE	    = 0x109,
  EMB_NULL	    = 0x209,
  // EMB_TOPLEVEL   = 0x309,
  EMB_DUMMY	    = 0x409,
  EMB_PAIRSTART	    = 0x509,
  EMB_PAIRTERMINATE = 0x609,
  EMB_COMMENT	    = 0x709,
  EMB_QUOTE	    = 0x809,
  EMB_VECTOR	    = 0x909,
  EMB_DOTPAIR	    = 0xa09
};

enum kev_type {
  JUMP		= 000,
  DESTINATION	= 001,
  CODE  	= 002,
  BIGNUM       	= 003,
  RATIONAL_NUM  = 004,		// 000 100
  FLONUM	= 005,		// 000 101
  EXCT_CPLX_NUM	= 006,		// 000 110
  INEX_CPLX_NUM	= 007,		// 000 111
  STRING_CORE	= 010,
  STRING	= 011,
  SYMBOL	= 012,
  VECTOR	= 013,
  WIND		= 014,
  REFERENCE	= 015,
  RECORD	= 016,
  GENERATOR	= 017,
  CONDITION_SMP = 020,		// 010 000
  CONDITION_CMP = 021,		// 010 001
  WRAPPED	= 022,		// 010 010
  TEMPLATE	= 023,		// 010 011
  CPS		= 024,		// 010 100
  LAMBDA	= 025,		// 010 101
  // UNUSED   	= 026,		// 010 110
  CONTINUATION	= 027,		// 010 111
  // UNUSED	= 030,
  ARG_FRAME	= 031,
  LOCAL_FRAME	= 032,
  FREE_FRAME	= 033,
  PAIR      	= 034,
  MACRO		= 035,
  ENVIRONMENT	= 036,
  LIBRARY   	= 037,
  STACK_FRAME	= 040,		// 100 000
  STACK_FRAME_B = 041,		// 100 001
  STACK_FRAME_D = 042,		// 100 010
  STACK_FRAME_E = 043		// 100 011
};

/* ----------------------------------------
 * |xxxxxxxx|xxxxPLDG|CCCCCCCC|TTTTTTTF| 
 * x: an unused bit
 * C: a bit of counter for generation GC
 * D: a bit of dynamic objects; True means that the object is allocated dynamically.
 * F: a refernce flag; True means that this is reference.
 * G: a guard bit against counter
 * L: a bit for mark-and-sweep GC; True means that the object lives.
 * P: a bit of permanent objects; True means that the object is permanent.
 * T: a bit of object type
 * ----------------------------------------
 */

class Kev {
protected:
enum {
  COPY	= 0x000001,
  ALIGN = 0x000003,
  TYP 	= 0x0000ff,
  COUNT = 0x000100,
  GUARD = 0x010000,
  DYNAM	= 0x020000,
  LIVE	= 0x040000,
  PERMN	= 0x080000
};

  Kev() = delete;

  Kev(const Kev& other)
    : value_(other.value_ & TYP) {}

  Kev(Kev&&) = default;
  Kev& operator=(const Kev&) = default;
  Kev& operator=(Kev&&) = default;
  ~Kev() = default;
  
  explicit constexpr Kev(kev_type type)
    : value_(static_cast<quintptr>(type) << 1) {}

public:
  constexpr kev_type type() const {
    return static_cast<kev_type>((value_ & TYP) >> 1);
  }

protected:
  void set_type(kev_type type) {
    value_ = (value_ & ~TYP) | (static_cast<quintptr>(type) << 1);
  }

  quintptr value_; 
};

class MutableKev : public Kev {
protected:
  MutableKev() = delete;
  MutableKev(const MutableKev&) = default;
  MutableKev(MutableKev&&) = default;
  MutableKev& operator=(const MutableKev&) = default;
  MutableKev& operator=(MutableKev&&) = default;
  ~MutableKev() = default;

  explicit constexpr MutableKev(kev_type type) : Kev(type) {
  }

public:
  uchar countUp() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ = (value_ + COUNT) & ~GUARD;
    return value_ >> 8;
  }

  MutableKev* getNewAddress() const {
    Q_ASSERT((value_ & ALIGN) == COPY);
    return reinterpret_cast<MutableKev*>(value_ & ~ALIGN);
  }

  bool isCopied() const {
    return value_ & COPY;
  }

  bool isMarkedLive() const {
    Q_ASSERT((value_ & COPY) == 0);
    return value_ & LIVE;
  }

  void markLive() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ |= LIVE;
  }

  void resetLive() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ &= ~LIVE;
  }

  bool isMarkedDynamic() const {
    Q_ASSERT((value_ & COPY) == 0);
    return value_ & DYNAM;
  }

  void markDynamic() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ |= DYNAM;
  }

  bool isMarkedPermanent() const {
    Q_ASSERT((value_ & COPY) == 0);
    return value_ & PERMN;
  }

  void markPermanent() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ |= PERMN;
  }

  void resetCountAndMark() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ &= TYP;
  }

  void resetMark() {
    Q_ASSERT((value_ & COPY) == 0);
    value_ &= 0xffff;
  }

  void setNewAddress(const Kev* kev) {
    Q_ASSERT(this != kev && (value_ & COPY) == 0);
    value_ = reinterpret_cast<quintptr>(kev) | COPY;
  }

protected:
  void set_type(kev_type type) {
    this->Kev::set_type(type);
  }
};

class KevesValue {
  // 00000000: undef
  // ......00: pointer of Kev
  // ....1001: Instant value
  
  enum {
    ALIGN  = 0x3,
    REF    = 0x2, // ......10: reference to Kev for GC
    FIXNUM = 0x3, // ......11: KevesFixnum
    INST   = 0x5, // 00000101: KevesInstruct
    CHAR   = 0xd  // 00001101: KevesChar
  };

public:
  KevesValue() = default;
  KevesValue(const KevesValue&) = default;
  KevesValue(KevesValue&&) = default;
  KevesValue& operator=(const KevesValue&) = default;
  KevesValue& operator=(KevesValue&&) = default;
  ~KevesValue() = default;

  constexpr KevesValue(const Kev* kev)
    : val_(reinterpret_cast<quintptr>(kev)) {}

  constexpr KevesValue(emb_value val)
    : val_(val) {}
  
  explicit KevesValue(const ArgumentFrameKev* frame)
  : val_(reinterpret_cast<quintptr>(frame)) {
  }
  
  explicit KevesValue(const LocalVarFrameKev* frame)
  : val_(reinterpret_cast<quintptr>(frame)) {
  }
  
  /*
  explicit KevesValue(const FreeVarFrameKev* frame)
  : val_(reinterpret_cast<quintptr>(frame)) {
  }
  */
  
  explicit KevesValue(const StackFrameKev* frame)
  : val_(reinterpret_cast<quintptr>(frame)) {
  }
  
  bool isKev(kev_type type) const {
    return this->isPtr() && this->type() == type;
  }

  template<class KEV> bool is() {
    return this->isPtr() && this->type() == KEV::TYPE;
  }

  bool isComplexNumber() const {
    return isFixnum() ||
      (isPtr() && (type() | (RATIONAL_NUM ^ INEX_CPLX_NUM)) == INEX_CPLX_NUM);
    // (isPtr() && ((type() | (RATIONAL_NUM ^ EXCT_CPLX_NUM)) == EXCT_CPLX_NUM ||
    // type() == INEX_CPLX_NUM));
  }

  bool isCondition() const {
    return isPtr() && (type() | (CONDITION_SMP ^ CONDITION_CMP)) == CONDITION_CMP;
  }

  bool isDestination() const {
    return isPtr() && type() == DESTINATION;
  }

  bool isEmpty() const {
    return !val_;
  }

  bool isExactNumber() const {
    return isFixnum() ||
      (isPtr() && (type() | (RATIONAL_NUM ^ EXCT_CPLX_NUM)) == EXCT_CPLX_NUM);
  }

  bool isInexactNumber() const {
    return isPtr() && (type() | (FLONUM ^ INEX_CPLX_NUM)) == INEX_CPLX_NUM;
  }
  
  bool isJump() const {
    return isPtr() && type() == JUMP;
  }

  bool isMacro() const {
    return isPtr() && type() == MACRO;
  }

  bool isNumber() const {
    return isFixnum() ||
      (isPtr() && (type() | (RATIONAL_NUM ^ INEX_CPLX_NUM)) == INEX_CPLX_NUM);
  }

  bool isPair() const {
    return isPtr() && type() == PAIR;
  }

  bool isProcedure() const {
    return isPtr() && (type() | (CPS ^ CONTINUATION)) == CONTINUATION;
  }


  bool isPtr() const {
    return ((val_ - 1) | ALIGN) < val_; // 00 (except NULL)
  }
  
  bool isBool() const {
    return (val_ | (EMB_TRUE ^ EMB_FALSE)) == EMB_TRUE; // 1001
  }

  bool isChar() const { // see keves_char.hpp
    return (val_ & 0xff) == CHAR; // 0000 1101
  }
  
  bool isFixnum() const { // see keves_fixnum.hpp
    return (val_ & ALIGN) == FIXNUM; // 11
  }
  
  bool isInstruct() const { // see keves_instruct.hpp
    return (val_ & 0xff) == INST; // 0000 0101
  }
  
  bool isRef() const {
    return (val_ & ALIGN) == REF; // 10
  }

  bool isRationalNumber() const {
    return isFixnum() || (isPtr() && type() == RATIONAL_NUM);
  }

  bool isRealNumber() const {
    return isFixnum() ||
      (isPtr() && (type() | (RATIONAL_NUM ^ FLONUM)) == FLONUM);
  }

  bool isString() const {
    return isPtr() && type() == STRING;
  }

  bool isSymbol() const {
    return isPtr() && type() == SYMBOL;
  }

  bool isGenerator() const {
    return isPtr() && type() == GENERATOR;
  }

  bool isWrapped() const {
    return isPtr() && type() == WRAPPED;
  }

  bool isTemplate() const {
    return isPtr() && type() == TEMPLATE;
  }

  bool isVector() const {
    return isPtr() && type() == VECTOR;
  }
  
  bool isCode() const {
    return isPtr() && type() == CODE;
  }
  
  template<class KEV>
  const KEV* toPtr() const {
    const KEV* temp(static_cast<const KEV*>(reinterpret_cast<const Kev*>(val_)));
    Q_ASSERT(temp->type() == KEV::TYPE);
    return temp;
  }

  const Kev* toPtr() const {
    return reinterpret_cast<const Kev*>(val_);
  }

  constexpr quintptr toUIntPtr() const {
    return val_;
  }

  kev_type type() const {
    Q_ASSERT(isPtr());
    return reinterpret_cast<const Kev*>(val_)->type();
  }

  friend bool operator==(KevesValue lhs, KevesValue rhs) {
    return lhs.val_ == rhs.val_;
  }

  friend bool operator!=(KevesValue lhs, KevesValue rhs) {
    return lhs.val_ != rhs.val_;
  }

  template<class KEV> operator const KEV*() const {
    KEV* temp(reinterpret_cast<KEV*>(val_));
    Q_ASSERT(temp->type() == KEV::TYPE);
    return temp;
  }

  template<class KEV>
  static KEV* fromUioword(uioword value) {
    return reinterpret_cast<KEV*>(static_cast<quintptr>(value));
  }

protected:
  quintptr val_ = 0;
};

class MutableKevesValue : public KevesValue {
public:
  MutableKevesValue() = default;
  MutableKevesValue(const MutableKevesValue&) = default;
  MutableKevesValue(MutableKevesValue&&) = default;
  MutableKevesValue& operator=(const MutableKevesValue&) = default;
  MutableKevesValue& operator=(MutableKevesValue&&) = default;
  ~MutableKevesValue() = default;

  constexpr MutableKevesValue(KevesValue kev) : KevesValue(kev) {
  }

  constexpr MutableKevesValue(MutableKev* kev) : KevesValue(kev) {
  }
  
  MutableKev* toPtr() {
    return reinterpret_cast<MutableKev*>(val_);
  }

  template<class KEV> KEV* toPtr() {
    KEV* temp(reinterpret_cast<KEV*>(val_));
    Q_ASSERT(temp->type() == KEV::TYPE);
    return temp;
  }

  template<class KEV> operator KEV*() {
    KEV* temp(reinterpret_cast<KEV*>(val_));
    Q_ASSERT(temp->type() == KEV::TYPE);
    return temp;
  }
};

template<class KEV> class FixedLengthKev : public KEV {
public:
  FixedLengthKev() = delete;
  FixedLengthKev(const FixedLengthKev<KEV>&) = default;
  FixedLengthKev(FixedLengthKev<KEV>&&) = default;
  FixedLengthKev<KEV>& operator=(const FixedLengthKev<KEV>&) = default;
  FixedLengthKev<KEV>& operator=(FixedLengthKev<KEV>&&) = default;
  ~FixedLengthKev() = default;

  explicit FixedLengthKev(KEV other) : KEV(other) {
  }
  
  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1);
  }
  
  template<class ZONE>
  FixedLengthKev<KEV>* copyTo(ZONE* zone) const {
    Q_ASSERT((this->MutableKev::type() | (STACK_FRAME ^ STACK_FRAME_E)) || this->type() == KEV::TYPE);

    auto ctor = [this](void* ptr) { return new(ptr) FixedLengthKev<KEV>(*this); };
    return zone->make(ctor, KEV::alloc_size(this));
  }

  static FixedLengthKev<KEV>* from(const MutableKev* kev) {
    return static_cast<FixedLengthKev<KEV>*>(const_cast<MutableKev*>(kev));
  }
};

template<class KEV> class VariableLengthKev : public KEV {
public:
  VariableLengthKev() = delete;
  VariableLengthKev(const VariableLengthKev<KEV>&) = default;
  VariableLengthKev(VariableLengthKev<KEV>&&) = default;
  VariableLengthKev& operator=(const VariableLengthKev<KEV>&) = default;
  VariableLengthKev& operator=(VariableLengthKev<KEV>&&) = default;
  ~VariableLengthKev() = default;

  explicit VariableLengthKev(KEV other)
    : KEV(other) {}
  
  explicit VariableLengthKev(int size)
    : KEV(size) {}
  
  template<class ZONE>
  VariableLengthKev<KEV>* copyTo(ZONE* zone) const {
    Q_ASSERT(this->type() == KEV::TYPE);

    auto ctor = [this](void* ptr) {
      VariableLengthKev<KEV>* temp(new(ptr) VariableLengthKev<KEV>(this->size()));
      temp->copyArray(this);
      return temp;
    };
    
    return zone->make(ctor, KEV::alloc_size(this));
  }

  static VariableLengthKev<KEV>* from(const MutableKev* kev) {
    return static_cast<VariableLengthKev<KEV>*>(const_cast<MutableKev*>(kev));
  }
};
