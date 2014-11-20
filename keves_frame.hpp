/* Keves/keves_frame.hpp - frames for Keves
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

#include "keves_fixnum.hpp"
#include "keves_iterator.hpp"
#include "keves_value.hpp"
// #include "reference_kev.hpp"

class KevesGC;


class ArgumentFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = ARG_FRAME;

  ArgumentFrameKev() = delete;
  ArgumentFrameKev(const ArgumentFrameKev&) = delete;
  ArgumentFrameKev(ArgumentFrameKev&&) = delete;
  ArgumentFrameKev& operator=(const ArgumentFrameKev&) = delete;
  ArgumentFrameKev& operator=(ArgumentFrameKev&&) = delete;

protected:
  ~ArgumentFrameKev() = default;

  explicit ArgumentFrameKev(int size)
    : MutableKev(TYPE), size_(size) {}

public:
  void append(int*, const ArgumentFrameKev*, int);

  KevesValue at(int index) const {
    Q_ASSERT(index >= 0 && index < size_);
    return array()[index];
  }
  
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
  
  ArgumentFrameKev* extend(KevesGC* gc) const;

  void replace(int index, KevesValue kev) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = kev;
  }
  
  int size() const {
    return size_;
  }

  static ArgumentFrameKev* make(KevesGC* gc, int size);

private:
  void CopyTo(ArgumentFrameKev* other, int size) const;

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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<ArgumentFrameKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<ArgumentFrameKev>*
      argp(VariableLengthKev<ArgumentFrameKev>::From(kev));

    int size(argp->size());
    KevesValue* value_ptr(argp->array());

    for (int i(0); i < size; ++i, ++value_ptr)
      *value_ptr = zone->Copy(*value_ptr);

    return argp->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void CopyArray(const ArgumentFrameKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const ArgumentFrameKev* frame(value);
    IO::PushArray(pending, frame);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const ArgumentFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< static_cast<ioword>(frame->size());

    IO::WriteArray(list, out, frame);
  }
  
  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword size;
    in >> size;
    ArgumentFrameKev* frame(ArgumentFrameKev::make(gc, size));
    IO::ReadArray(in, frame);
    return frame;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    ArgumentFrameKev* frame(value);
    IO::RevertArray(object_list, frame);
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

  LocalVarFrameKev() = delete;
  LocalVarFrameKev(const LocalVarFrameKev&) = delete;
  LocalVarFrameKev(LocalVarFrameKev&&) = delete;
  LocalVarFrameKev& operator=(const LocalVarFrameKev&) = delete;
  LocalVarFrameKev& operator=(LocalVarFrameKev&&) = delete;
protected:
  ~LocalVarFrameKev() = default;

  explicit LocalVarFrameKev(int size)
    : MutableKev(TYPE), size_(size), next_(nullptr) {
    Q_ASSERT(size_ >= 0);
  }
  
public:
  void assign(int index, KevesValue kev) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = kev;
  }
  
  KevesValue at(int index) const {
    Q_ASSERT(index >= 0 && index < size_);
    return array()[index];
  }
  
  KevesValue* array() {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<KevesValue*>(this + 1);
  }
  
  const KevesValue* array() const {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<const KevesValue*>(this + 1);
  }
  
  void CopyTo(LocalVarFrameKev* other, int size) const;

  void replace(int index, KevesValue value) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = value;
  }
  
  int size() const {
    return size_;
  }

  static LocalVarFrameKev* make(KevesGC* gc, int);

private:
  static size_t alloc_size(int size) {
    return sizeof(LocalVarFrameKev) + sizeof(KevesValue) * size;
  }
  
  const int size_;
  LocalVarFrameKev* next_; // it is dummy!!!

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const LocalVarFrameKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<LocalVarFrameKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<LocalVarFrameKev>*
      envp(VariableLengthKev<LocalVarFrameKev>::From(kev));

    int envn(envp->size());
    KevesValue* value_ptr(envp->array());

    for (int i(0); i < envn; ++i, ++value_ptr)
      *value_ptr = zone->Copy(*value_ptr);
    
    return envp->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void CopyArray(const LocalVarFrameKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const LocalVarFrameKev* frame(value);
    IO::PushValue(pending, frame->next_);
    IO::PushArray(pending, frame);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const LocalVarFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< static_cast<ioword>(frame->size())
	<< IO::WrapAddress(list, frame->next_);

    IO::WriteArray(list, out, frame);
  }
  
  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword size;
    uioword next;
    in >> size >> next;
    LocalVarFrameKev* frame(LocalVarFrameKev::make(gc, size));
    IO::ReadArray(in, frame);
    return frame;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    LocalVarFrameKev* frame(value);

    if (KevesValue(frame->next_).IsRef())
      frame->next_ = MutableKevesValue(object_list.at(KevesValue(frame->next_).toUIntPtr() >> 2));

    IO::RevertArray(object_list, frame);
  }
};


template<int SIZE> class LocalVarFrameKevWithArray : public LocalVarFrameKev {
public:
  LocalVarFrameKevWithArray()
    : LocalVarFrameKev(SIZE), array_() {}
  
  LocalVarFrameKevWithArray(const LocalVarFrameKevWithArray&) = delete;
  LocalVarFrameKevWithArray(LocalVarFrameKevWithArray&&) = delete;
  LocalVarFrameKevWithArray& operator=(const LocalVarFrameKevWithArray&) = delete;
  LocalVarFrameKevWithArray& operator=(LocalVarFrameKevWithArray&&) = delete;
  ~LocalVarFrameKevWithArray() = default;

private:
  KevesValue array_[SIZE];
};


class FreeVarFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = FREE_FRAME;

  FreeVarFrameKev()
    : FreeVarFrameKev(0) {}
 
  FreeVarFrameKev(const FreeVarFrameKev&) = delete;
  FreeVarFrameKev(FreeVarFrameKev&&) = delete;
  FreeVarFrameKev& operator=(const FreeVarFrameKev&) = delete;
  FreeVarFrameKev& operator=(FreeVarFrameKev&&) = delete;
  ~FreeVarFrameKev() = default;

  explicit FreeVarFrameKev(int size)
    : MutableKev(TYPE), size_(size), next_() {}

public:
  void assign(int index, KevesValue kev) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = kev;
  }
  
  KevesValue at(int index) const {
    Q_ASSERT(index >= 0 && index < size_);
    return array()[index];
  }
  
  KevesValue* array() {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<KevesValue*>(this + 1);
  }
  
  const KevesValue* array() const {
    Q_ASSERT(type() == TYPE);
    return reinterpret_cast<const KevesValue*>(this + 1);
  }
  
  void clear();

  FreeVarFrameKev* next() const {
    return next_;
  }

  FreeVarFrameKev* prepend(KevesGC* gc, int);

  void replace(int index, KevesValue value) {
    Q_ASSERT(index >= 0 && index < size_);
    array()[index] = value;
  }
  
  int size() const {
    return size_;
  }

  // static FreeVarFrameKev* make(KevesIterator*, int, FreeVarFrameKev*);
  static FreeVarFrameKev* make(KevesGC* gc, int, FreeVarFrameKev*);

  FreeVarFrameKev(int size, FreeVarFrameKev* next)
    : MutableKev(TYPE), size_(size), next_(next) {}

private:
  static size_t alloc_size(int size) {
    return sizeof(FreeVarFrameKev) + sizeof(KevesValue) * size;
  }

  const int size_;
  FreeVarFrameKev* next_;


  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static size_t alloc_size(const MutableKev* kev) {
    return alloc_size(static_cast<const FreeVarFrameKev*>(kev)->size_);
  }

  template<class ZONE>
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return VariableLengthKev<FreeVarFrameKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    VariableLengthKev<FreeVarFrameKev>*
      clsr(VariableLengthKev<FreeVarFrameKev>::From(kev));

    clsr->next_
      = zone->Copy(VariableLengthKev<FreeVarFrameKev>::From(clsr->next_));

    int size(clsr->size());
    KevesValue* value_ptr(clsr->array());

    for (int i(0); i < size; ++i, ++value_ptr)
      *value_ptr = zone->Copy(*value_ptr);

    return clsr->border();
  }

  quintptr* border() {
    return reinterpret_cast<quintptr*>(this + 1) + size_;
  }

  void CopyArray(const FreeVarFrameKev* org);

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const FreeVarFrameKev* frame(value);
    IO::PushValue(pending, frame->next());
    IO::PushArray(pending, frame);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const FreeVarFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< static_cast<ioword>(frame->size())
	<< IO::WrapAddress(list, frame->next());

    IO::WriteArray(list, out, frame);
  }
  
  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    ioword size;
    uioword next;
    in >> size >> next;

    FreeVarFrameKev* frame(make(gc,
				size,
				IO::template fromUioword<FreeVarFrameKev>(next)));

    IO::ReadArray(in, frame);
    return frame;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    FreeVarFrameKev* frame(value);
    IO::RevertValue(object_list, &frame->next_);
    IO::RevertArray(object_list, frame);
  }
};


class StackFrameKev : public MutableKev {
public:
  static constexpr kev_type TYPE = STACK_FRAME;
  
  StackFrameKev() : MutableKev(TYPE),
		    fp_(), pc_(), argp_(), envp_(), clsr_(),
		    argn_(), envn_(), sfp_() {
  }
  
  StackFrameKev(const StackFrameKev&) = default;
  StackFrameKev(StackFrameKev&&) = default;
  StackFrameKev& operator=(const StackFrameKev&) = default;
  StackFrameKev& operator=(StackFrameKev&&) = default;
  ~StackFrameKev() = default;

  void assignLocalVar(int index, KevesValue kev) {
    Q_ASSERT(index >= 0 && index < envp_->size());
    envp_->array()[index] = kev;
  }

  void appendArgument(ArgumentFrameKev* temp_argp, int temp_argn) {
    argp_->append(&argn_, temp_argp, temp_argn);
  }

  int argn() const {
    return argn_;
  }

  ArgumentFrameKev* argp() const {
    return argp_;
  }

  KevesValue argument(int idx) const {
    Q_ASSERT(idx >= 0 && idx < argn_);
    return argp_->at(idx);
  }

  void assignFreeVar(int, KevesValue);

  void assignLastLocalVar(KevesValue kev, int offset = 0) {
    Q_ASSERT(offset >= 0 && offset < envp_->size());
    envp_->array()[envn_ - offset - 1] = kev;
  }

  void changeToBottomFrame() {
    this->MutableKev::set_type(STACK_FRAME_B);
  }

  void clearArgFrame() {
    argn_ = 0;
  }

  void clearEnvFrame() {
    envn_ = 0;
  }

  FreeVarFrameKev* clsr() {
    return clsr_;
  }

  const FreeVarFrameKev* clsr() const {
    return clsr_;
  }

  void copyEnvFrameTo(FreeVarFrameKev*) const;

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
  void extendArgFrame(ArgumentFrameKev*);
  void extendEnvFrame(LocalVarFrameKev*);

  StackFrameKev* fp() {
    return fp_;
  }

  const StackFrameKev* fp() const {
    return fp_;
  }

  KevesValue freeVar(int idx) const;
  void initializeEnvFrame(KevesGC* gc, int);

  KevesValue lastArgument(int idx = 0) const {
    Q_ASSERT(idx >= 0 && idx < argn_);
    return argp_->at(argn_ - idx - 1);
  }

  KevesValue lastLocalVar(int idx = 0) const {
    Q_ASSERT(idx >= 0 && idx < envn_);
    return envp_->at(envn_ - idx -1);
  }

  static StackFrameKev* make(KevesGC* gc);
  void makeEnvFrame(KevesGC* gc, int);

  const_KevesIterator pc() const {
    return pc_;
  }

  void popArgument() {
    --argn_;
  }

  void pushArgument(KevesValue kev) {
    Q_ASSERT(argn_ < argp_->size());
    argp_->replace(argn_++, kev);
  }

  void pushLocal(KevesValue kev) {
    Q_ASSERT(envn_ < envp_->size());
    envp_->replace(envn_++, kev);
  }

  void replaceFirstArgument(KevesValue kev) {
    Q_ASSERT(argn_ > 0);
    argp_->replace(0, kev);
  }

  void replaceLastArgument(KevesValue kev, int offset = 0) {
    Q_ASSERT(offset >= 0 && offset < argn_);
    argp_->replace(argn_ - offset - 1, kev);
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

  void setClosure(FreeVarFrameKev* frame) {
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
  FreeVarFrameKev* clsr_;
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
  static MutableKev* CopyTo(ZONE* zone, MutableKev* kev) {
    return FixedLengthKev<StackFrameKev>::From(kev)->CopyTo(zone);
  }

  template<class ZONE>
  static quintptr* CopyContents(ZONE* zone, MutableKev* kev) {
    FixedLengthKev<StackFrameKev>* frame(FixedLengthKev<StackFrameKev>::From(kev));
    Q_ASSERT(StackFrameKev::is(frame));
    VariableLengthKev<ArgumentFrameKev>* argp(VariableLengthKev<ArgumentFrameKev>::From(frame->argp_));
    VariableLengthKev<LocalVarFrameKev>* envp(VariableLengthKev<LocalVarFrameKev>::From(frame->envp_));
    VariableLengthKev<FreeVarFrameKev>* clsr(VariableLengthKev<FreeVarFrameKev>::From(frame->clsr_));
    FixedLengthKev<StackFrameKev>* fp(FixedLengthKev<StackFrameKev>::From(frame->fp_));
    FixedLengthKev<StackFrameKev>* sfp(FixedLengthKev<StackFrameKev>::From(frame->sfp_));
    frame->argp_ = zone->Copy(argp);
    frame->envp_ = zone->Copy(envp);
    frame->clsr_ = zone->Copy(clsr);
    frame->fp_ = zone->Copy(fp);
    frame->sfp_ = zone->Copy(sfp);

    return frame->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const StackFrameKev* frame(value);
    IO::PushValue(pending, frame->argp_);
    IO::PushValue(pending, frame->envp_);
    IO::PushValue(pending, frame->clsr_);
    IO::PushValue(pending, frame->fp_);
    IO::PushValue(pending, frame->sfp_);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const StackFrameKev* frame(value);

    out << static_cast<uioword>(frame->type())
	<< IO::WrapAddress(list, frame->argp_)
	<< static_cast<uioword>(frame->argn_)
	<< IO::WrapAddress(list, frame->envp_)
	<< static_cast<uioword>(frame->envn_)
	<< IO::WrapAddress(list, frame->clsr_)
	<< IO::WrapAddress(list, frame->fp_)
	<< IO::WrapAddress(list, frame->sfp_);
  }
  
  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword argp, envp, clsr, fp, sfp;
    ioword argn, envn;
    in >> argp >> argn >> envp >> envn >> clsr >> fp >> sfp;

    StackFrameKev* frame(StackFrameKev::make(gc));
    frame->argp_ = IO::template fromUioword<ArgumentFrameKev>(argp);
    frame->argn_ = argn;
    frame->envp_ = IO::template fromUioword<LocalVarFrameKev>(envp);
    frame->envn_ = envn;
    frame->clsr_ = IO::template fromUioword<FreeVarFrameKev>(clsr);
    frame->fp_ = IO::template fromUioword<StackFrameKev>(fp);
    frame->sfp_ = IO::template fromUioword<StackFrameKev>(sfp);

    return frame;
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    StackFrameKev* frame(value);
    IO::RevertValue(object_list, &frame->argp_);
    IO::RevertValue(object_list, &frame->envp_);
    IO::RevertValue(object_list, &frame->clsr_);
    IO::RevertValue(object_list, &frame->fp_);
    IO::RevertValue(object_list, &frame->sfp_);
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
