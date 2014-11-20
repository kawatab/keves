/* Keves/wrapped_kev.hpp - wrappeds for Keves
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

#include "keves_iterator.hpp"
#include "keves_value.hpp"

class WrappedKev : public MutableKev {
public:
  static constexpr kev_type TYPE = WRAPPED;

  WrappedKev();
  WrappedKev(const WrappedKev&) = default;
  WrappedKev(WrappedKev&&) = default;
  WrappedKev& operator=(const WrappedKev&) = delete;
  WrappedKev& operator=(WrappedKev&&) = delete;
  ~WrappedKev() = default;


  WrappedKev(KevesValue, KevesValue, KevesValue, KevesValue);
  WrappedKev(KevesValue, const WrappedKev*);

  void copyFrom(const WrappedKev&);
  void copyFrom(WrappedKev&&);

  KevesValue form() const {
    return form_;
  }

  KevesValue local_vars() const {
    return local_vars_;
  }

  KevesValue free_vars() const {
    return free_vars_;
  }

  KevesValue global_vars() const {
    return global_vars_;
  }

  void set(KevesValue, KevesValue, KevesValue, KevesValue);

  void set_form(KevesValue form) {
    form_ = form;
  }

  void set_local_vars(KevesValue vars) {
    local_vars_ = vars;
  }

  void set_free_vars(KevesValue vars) {
    free_vars_ = vars;
  }

  void set_global_vars(KevesValue vars) {
    global_vars_ = vars;
  }

  static KevesValue getDatum(KevesValue);
  static WrappedKev* make(KevesIterator*, KevesValue, KevesValue, KevesValue, KevesValue);

private:
  KevesValue form_;
  KevesValue local_vars_;
  KevesValue free_vars_;
  KevesValue global_vars_;

  
  ////////////////////////////////////////////////////////////
  // Section For GC !!!                                     //
  ////////////////////////////////////////////////////////////

public:
  static constexpr size_t alloc_size(const MutableKev*) {
    return sizeof(WrappedKev);
  }

  template<class T>
  static MutableKev* copyTo(T* zone, MutableKev* kev) {
    return FixedLengthKev<WrappedKev>::From(kev)->copyTo(zone);
  }

  template<class T>
  static quintptr* copyContents(T* zone, MutableKev* kev) {
    FixedLengthKev<WrappedKev>* wrapped(FixedLengthKev<WrappedKev>::From(kev));
    wrapped->form_ = zone->copy(wrapped->form_);
    wrapped->local_vars_ = zone->copy(wrapped->local_vars_);
    wrapped->free_vars_ = zone->copy(wrapped->free_vars_);
    wrapped->global_vars_ = zone->copy(wrapped->global_vars_);
    return wrapped->border();
  }

  ////////////////////////////////////////////////////////////
  // Section For serialize !!!                              //
  ////////////////////////////////////////////////////////////

public:
  template<class IO, class STACK>
  static void PushChildren(STACK* pending, KevesValue value) {
    const WrappedKev* wrapped(value);
    IO::PushValue(pending, wrapped->form_);
    IO::PushValue(pending, wrapped->local_vars_);
    IO::PushValue(pending, wrapped->free_vars_);
    IO::PushValue(pending, wrapped->global_vars_);
  }

  template<class IO, class LIST, class STREAM>
  static void WriteObject(const LIST& list, STREAM& out, KevesValue value) {
    const WrappedKev* wrapped(value);

    out << static_cast<uioword>(wrapped->type())
	<< IO::WrapAddress(list, wrapped->form_)
	<< IO::WrapAddress(list, wrapped->local_vars_)
	<< IO::WrapAddress(list, wrapped->free_vars_)
	<< IO::WrapAddress(list, wrapped->global_vars_);
  }

  template<class IO, class STREAM, class GC>
  static Kev* ReadObject(STREAM& in, GC* gc) {
    uioword form, local_vars, free_vars, global_vars;
    in >> form >> local_vars >> free_vars >> global_vars;
    return make(gc,
		IO::template fromUioword<Kev>(form),
		IO::template fromUioword<Kev>(local_vars),
		IO::template fromUioword<Kev>(free_vars),
		IO::template fromUioword<Kev>(global_vars));
  }
  
  template<class IO, class LIST>
  static void RevertObject(const LIST& object_list, MutableKevesValue value) {
    WrappedKev* wrapped(value);
    IO::RevertValue(object_list, &wrapped->form_);
    IO::RevertValue(object_list, &wrapped->local_vars_);
    IO::RevertValue(object_list, &wrapped->free_vars_);
    IO::RevertValue(object_list, &wrapped->global_vars_);
  }
};
