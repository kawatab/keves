// keves/kev/frame.hpp - frames for Keves
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

#include "keves_iterator.hpp"
#include "keves_value.hpp"

class KevesGC;


class ArgumentFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = ARG_FRAME;

  ArgumentFrameKev() = delete;
  ArgumentFrameKev(const ArgumentFrameKev&) = delete;
  ArgumentFrameKev(ArgumentFrameKev&&) = delete;
  ArgumentFrameKev& operator=(const ArgumentFrameKev&) = delete;
  ArgumentFrameKev& operator=(ArgumentFrameKev&&) = delete;
  ~ArgumentFrameKev() = default;

  explicit ArgumentFrameKev(int size)
    : MutableKev(TYPE), size_(size) {}

  void append(int* num_this, const ArgumentFrameKev* other, int num_other);

  KevesValue* array() {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<KevesValue*>(this + 1);
  }

  const KevesValue* array() const {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<const KevesValue*>(this + 1);
  }

  void assign(int index, KevesValue kev) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = kev;
  }
  
  KevesValue at(int index) const {
    Q_ASSERT(index >= 0 && index < size_);
    return array()[index];
  }
  
  ArgumentFrameKev* extend(KevesGC* gc) const;

  int size() const {
    return size_;
  }

  template<class ZONE>
  static ArgumentFrameKev* make(ZONE* zone, int size);

private:
  void copyTo(ArgumentFrameKev* other, int size) const;

  static size_t alloc_size(int size) {
    return sizeof(ArgumentFrameKev) + sizeof(KevesValue) * size;
  }

  const int size_;
  
  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const ArgumentFrameKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<ArgumentFrameKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<ArgumentFrameKev>*
      argp(VariableLengthKev<ArgumentFrameKev>::from(kev));

    int size(argp->size());
    KevesValue* value_ptr(argp->array());

    for (int i(0); i < size; ++i, ++value_ptr)
      *value_ptr = zone->copy(*value_ptr);

    return argp->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void copyArray(const ArgumentFrameKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const ArgumentFrameKev* frame(value);
    BASE::pushArray(pending, frame);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const ArgumentFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< static_cast<ioword>(frame->size());

    BASE::writeArray(list, out, frame);
  }
  
  template<class BASE, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    ArgumentFrameKev* frame(ArgumentFrameKev::make(gc, size));
    BASE::readArray(in, frame);
    return frame;
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    ArgumentFrameKev* frame(value);
    BASE::revertArray(object_list, frame);
  }
};


template<int SIZE> class ArgumentFrameKevWithArray : public ArgumentFrameKev {
public:
  ArgumentFrameKevWithArray()
    : ArgumentFrameKev(SIZE), array_() {}

  ArgumentFrameKevWithArray(const ArgumentFrameKevWithArray&) = delete;
  ArgumentFrameKevWithArray(ArgumentFrameKevWithArray&&) = delete;
  ArgumentFrameKevWithArray& operator=(const ArgumentFrameKevWithArray&) = delete;
  ArgumentFrameKevWithArray& operator=(ArgumentFrameKevWithArray&&) = delete;
  ~ArgumentFrameKevWithArray() = default;

private:
  KevesValue array_[SIZE];
};


class LocalVarFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = LOCAL_FRAME;

  LocalVarFrameKev()
    : LocalVarFrameKev(0, nullptr) {}
  
  LocalVarFrameKev(const LocalVarFrameKev&) = delete;
  LocalVarFrameKev(LocalVarFrameKev&&) = delete;
  LocalVarFrameKev& operator=(const LocalVarFrameKev&) = delete;
  LocalVarFrameKev& operator=(LocalVarFrameKev&&) = delete;
  ~LocalVarFrameKev() = default;

  LocalVarFrameKev(int size)
    : LocalVarFrameKev(size, nullptr) {
    Q_ASSERT(size_ >= 0);
  }
  
  LocalVarFrameKev(int size, LocalVarFrameKev* next)
    : MutableKev(TYPE), size_(size), next_(next) {
    Q_ASSERT(size_ >= 0);
  }
  
  KevesValue* array() {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<KevesValue*>(this + 1);
  }
  
  const KevesValue* array() const {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<const KevesValue*>(this + 1);
  }
  
  void assign(int index, KevesValue value) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = value;
  }
  
  KevesValue at(int index) const {
    Q_ASSERT(index >= 0 && index < size_);
    return array()[index];
  }
  
  LocalVarFrameKev* next() const {
    return next_;
  }

  
  void set_next(LocalVarFrameKev* next) {
    next_ = next;
  }
  
  int size() const {
    return size_;
  }

  template<class ZONE>
  static LocalVarFrameKev* make(ZONE* zone, int size, LocalVarFrameKev* next);

private:
  static size_t alloc_size(int size) {
    return sizeof(LocalVarFrameKev) + sizeof(KevesValue) * size;
  }
  
  const int size_;
  LocalVarFrameKev* next_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const LocalVarFrameKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<LocalVarFrameKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<LocalVarFrameKev>*
      envp(VariableLengthKev<LocalVarFrameKev>::from(kev));

    int envn(envp->size());
    KevesValue* value_ptr(envp->array());

    for (int i(0); i < envn; ++i, ++value_ptr)
      *value_ptr = zone->copy(*value_ptr);
    
    return envp->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void copyArray(const LocalVarFrameKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const LocalVarFrameKev* frame(value);
    BASE::pushValue(pending, frame->next_);
    BASE::pushArray(pending, frame);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const LocalVarFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< static_cast<ioword>(frame->size())
	<< BASE::indexAddress(list, frame->next_);

    BASE::writeArray(list, out, frame);
  }
  
  template<class BASE, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    ioword size;
    uioword next;
    in >> size >> next;

    LocalVarFrameKev* frame
      (make(gc,
	    size,
	    KevesValue::template fromUioword<LocalVarFrameKev>(next)));

    BASE::readArray(in, frame);
    return frame;
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    LocalVarFrameKev* frame(value);

    if (KevesValue(frame->next_).isRef())
      frame->next_ = MutableKevesValue(object_list.at(KevesValue(frame->next_).toUIntPtr() >> 2));

    BASE::revertArray(object_list, frame);
  }
};


template<int SIZE> class LocalVarFrameKevWithArray : public LocalVarFrameKev {
public:
  LocalVarFrameKevWithArray()
    : LocalVarFrameKev(SIZE, nullptr), array_() {}
  
  LocalVarFrameKevWithArray(const LocalVarFrameKevWithArray&) = delete;
  LocalVarFrameKevWithArray(LocalVarFrameKevWithArray&&) = delete;
  LocalVarFrameKevWithArray& operator=(const LocalVarFrameKevWithArray&) = delete;
  LocalVarFrameKevWithArray& operator=(LocalVarFrameKevWithArray&&) = delete;
  ~LocalVarFrameKevWithArray() = default;

private:
  KevesValue array_[SIZE];
};


class StackFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = STACK_FRAME;
  
  StackFrameKev()
    : MutableKev(TYPE),
      fp_(), pc_(), argp_(), envp_(), clsr_(),
      argn_(), envn_(), sfp_() {}
  
  StackFrameKev(const StackFrameKev&) = default;
  StackFrameKev(StackFrameKev&&) = default;
  StackFrameKev& operator=(const StackFrameKev&) = default;
  StackFrameKev& operator=(StackFrameKev&&) = default;
  ~StackFrameKev() = default;

  void appendArgument(ArgumentFrameKev* temp_argp, int temp_argn) {
    argp_->append(&argn_, temp_argp, temp_argn);
  }

  int argn() const {
    return argn_;
  }

  ArgumentFrameKev* argp() const {
    return argp_;
  }

  KevesValue argument(int index) const {
    Q_ASSERT(index >= 0 && index < argn_);
    return argp_->at(index);
  }

  void assignFreeVar(int index, KevesValue kev);
  void assignLastLocalVar(KevesValue kev, int offset = 0);
  void assignLocalVar(int index, KevesValue kev);

  void changeToBottomFrame() {
    this->MutableKev::set_type(STACK_FRAME_B);
  }

  void clearArgFrame() {
    argn_ = 0;
  }

  LocalVarFrameKev* close();
  
  LocalVarFrameKev* clsr() {
    return clsr_;
  }

  const LocalVarFrameKev* clsr() const {
    return clsr_;
  }

  // void copyEnvFrameTo(FreeVarFrameKev*) const;

  int envn() const {
    return envn_;
  }

  LocalVarFrameKev* envp() {
    return envp_;
  }

  const LocalVarFrameKev* envp() const {
    return envp_;
  }

  int envFrameSize() const {
    return envp_->size();
  }

  void extendArgFrame(KevesGC* gc);
  // void extendArgFrame(ArgumentFrameKev*);
  void extendEnvFrame(KevesGC* gc, int frame_size);

  StackFrameKev* fp() {
    return fp_;
  }

  const StackFrameKev* fp() const {
    return fp_;
  }

  KevesValue freeVar(int index) const;
  void initializeEnvFrame(KevesGC* gc, int);

  KevesValue lastArgument(int index = 0) const {
    Q_ASSERT(index >= 0 && index < argn_);
    return argp_->at(argn_ - index - 1);
  }

  KevesValue lastLocalVar(int index = 0) const;

  template<class ZONE>
  static StackFrameKev* make(ZONE* zone);

  void makeEnvFrame(KevesGC* gc, int frame_size);

  const_KevesIterator pc() const {
    return pc_;
  }

  void popArgument() {
    --argn_;
  }

  void pushArgument(KevesValue kev) {
    Q_ASSERT(argn_ < argp_->size());
    argp_->assign(argn_++, kev);
  }

  /*
  void pushLocal(KevesValue kev) {
    Q_ASSERT(envn_ < envp_->size());
    envp_->replace(envn_++, kev);
  }
  */

  void replaceFirstArgument(KevesValue kev) {
    Q_ASSERT(argn_ > 0);
    argp_->assign(0, kev);
  }

  void replaceLastArgument(KevesValue kev, int offset = 0) {
    Q_ASSERT(offset >= 0 && offset < argn_);
    argp_->assign(argn_ - offset - 1, kev);
  }

  void set_arg(ArgumentFrameKev* argp, int argn) {
    Q_ASSERT(argn > 0 && argn < argp->size());
    argp_ = argp;
    argn_ = argn;
  }

  void set_fp(StackFrameKev* fp) {
    fp_ = fp;
  }

  void set_pc(const_KevesIterator pc) {
    pc_ = pc;
  }

  void set_sfp(StackFrameKev* sfp) {
    sfp_ = sfp;
  }

  void setArgumentsToLocalVars();

  void setArgFrame(ArgumentFrameKev* frame, int n = 0) {
    argp_ = frame;
    argn_ = n;
  }

  void setClosure(LocalVarFrameKev* frame) {
    clsr_ = frame;
  }

  void setEnvFrame(LocalVarFrameKev* frame, int n = 0) {
    envp_ = frame;
    envn_ = n;
  }

  void sharePreviousArgFrame() {
    argp_ = fp_->argp_;
    argn_ = fp_->argn_;
  }

  StackFrameKev* sfp() {
    return sfp_;
  }

  const StackFrameKev* sfp() const {
    return sfp_;
  }

  const_KevesIterator unwind() {
    *this = *this->fp_;
    return this->pc_;
  }

  void wind(const_KevesIterator, StackFrameKev*, ArgumentFrameKev*);
  void windWithExceptionHandler(const_KevesIterator, StackFrameKev*, ArgumentFrameKev*);
  void windWithValues(const_KevesIterator, StackFrameKev*, ArgumentFrameKev*);

  static bool is(const StackFrameKev* frame) {
    return (frame->MutableKev::type() | (STACK_FRAME ^ STACK_FRAME_E)) == STACK_FRAME_E;
  }

  static bool isNormal(const StackFrameKev* frame) {
    return frame->MutableKev::type() == STACK_FRAME;
  }

  static bool isBottom(const StackFrameKev* frame) {
    return frame->MutableKev::type() == STACK_FRAME_B;
  }

protected:
  StackFrameKev* fp_;
  const_KevesIterator pc_;
  ArgumentFrameKev* argp_;
  LocalVarFrameKev* envp_;
  LocalVarFrameKev* clsr_;
  int argn_;
  int envn_;
  StackFrameKev* sfp_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev* /*kev*/) {
    return sizeof(StackFrameKev);
  }

  template<class ZONE>
  static MutableKev* copyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<StackFrameKev>::from(kev)->copyTo(zone);
  }

  template<class ZONE>
  static quintptr* copyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<StackFrameKev>* frame(FixedLengthKev<StackFrameKev>::from(kev));
    Q_ASSERT(StackFrameKev::is(frame));
    VariableLengthKev<ArgumentFrameKev>* argp(VariableLengthKev<ArgumentFrameKev>::from(frame->argp_));
    VariableLengthKev<LocalVarFrameKev>* envp(VariableLengthKev<LocalVarFrameKev>::from(frame->envp_));
    VariableLengthKev<LocalVarFrameKev>* clsr(VariableLengthKev<LocalVarFrameKev>::from(frame->clsr_));
    FixedLengthKev<StackFrameKev>* fp(FixedLengthKev<StackFrameKev>::from(frame->fp_));
    FixedLengthKev<StackFrameKev>* sfp(FixedLengthKev<StackFrameKev>::from(frame->sfp_));
    frame->argp_ = zone->copy(argp);
    frame->envp_ = zone->copy(envp);
    frame->clsr_ = zone->copy(clsr);
    frame->fp_ = zone->copy(fp);
    frame->sfp_ = zone->copy(sfp);

    return frame->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class BASE, class STACK>
  static void pushChildren(STACK* pending, KevesValue value) {
    const StackFrameKev* frame(value);
    BASE::pushValue(pending, frame->argp_);
    BASE::pushValue(pending, frame->envp_);
    BASE::pushValue(pending, frame->clsr_);
    BASE::pushValue(pending, frame->fp_);
    BASE::pushValue(pending, frame->sfp_);
  }

  template<class BASE, class LIST, class STREAM>
  static void writeObject(const LIST& list, STREAM& out, KevesValue value) {
    const StackFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< BASE::indexAddress(list, frame->argp_)
	<< static_cast<uioword>(frame->argn_)
	<< BASE::indexAddress(list, frame->envp_)
	<< static_cast<uioword>(frame->envn_)
	<< BASE::indexAddress(list, frame->clsr_)
	<< BASE::indexAddress(list, frame->fp_)
	<< BASE::indexAddress(list, frame->sfp_);
  }
  
  template<class /*BASE*/, class STREAM, class GC>
  static Kev* readObject(STREAM& in, GC* gc) {
    uioword argp, envp, clsr, fp, sfp;
    ioword argn, envn;
    in >> argp >> argn >> envp >> envn >> clsr >> fp >> sfp;

    StackFrameKev* frame(StackFrameKev::make(gc));
    frame->argp_ = KevesValue::template fromUioword<ArgumentFrameKev>(argp);
    frame->argn_ = argn;
    frame->envp_ = KevesValue::template fromUioword<LocalVarFrameKev>(envp);
    frame->envn_ = envn;
    frame->clsr_ = KevesValue::template fromUioword<LocalVarFrameKev>(clsr);
    frame->fp_ = KevesValue::template fromUioword<StackFrameKev>(fp);
    frame->sfp_ = KevesValue::template fromUioword<StackFrameKev>(sfp);

    return frame;
  }
  
  template<class BASE, class LIST>
  static void revertObject(const LIST& object_list, MutableKevesValue value) {
    StackFrameKev* frame(value);
    BASE::revertValue(object_list, &frame->argp_);
    BASE::revertValue(object_list, &frame->envp_);
    BASE::revertValue(object_list, &frame->clsr_);
    BASE::revertValue(object_list, &frame->fp_);
    BASE::revertValue(object_list, &frame->sfp_);
  }
};

class StackFrameKev_B : public StackFrameKev {
public:
  static constexpr kev_type TYPE = STACK_FRAME_B;

  StackFrameKev_B() = delete;
  StackFrameKev_B(const StackFrameKev_B&) = delete;
  StackFrameKev_B(StackFrameKev_B&&) = delete;
  StackFrameKev_B& operator=(const StackFrameKev_B&) = delete;
  StackFrameKev_B& operator=(StackFrameKev_B&&) = delete;
  ~StackFrameKev_B() = delete;
};

class StackFrameKev_D : public StackFrameKev {
public:
  static constexpr kev_type TYPE = STACK_FRAME_D;

  StackFrameKev_D() = delete;
  StackFrameKev_D(const StackFrameKev_D&) = delete;
  StackFrameKev_D(StackFrameKev_D&&) = delete;
  StackFrameKev_D& operator=(const StackFrameKev_D&) = delete;
  StackFrameKev_D& operator=(StackFrameKev_D&&) = delete;
  ~StackFrameKev_D() = delete;
};

class StackFrameKev_E : public StackFrameKev {
public:
  static constexpr kev_type TYPE = STACK_FRAME_E;

  StackFrameKev_E() = delete;
  StackFrameKev_E(const StackFrameKev_E&) = delete;
  StackFrameKev_E(StackFrameKev_E&&) = delete;
  StackFrameKev_E& operator=(const StackFrameKev_E&) = delete;
  StackFrameKev_E& operator=(StackFrameKev_E&&) = delete;
  ~StackFrameKev_E() = delete;
};
