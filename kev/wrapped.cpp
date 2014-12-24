// keves/kev/wrapped.cpp - wrappeds for Keves
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


#include "kev/wrapped.hpp"


WrappedKev::WrappedKev()
  : WrappedKev(EMB_UNDEF, EMB_NULL, EMB_NULL, EMB_NULL) {}

WrappedKev::WrappedKev(KevesValue form, KevesValue local_vars,
		       KevesValue free_vars, KevesValue global_vars)
  : MutableKev(TYPE),
    form_(form),
    local_vars_(local_vars),
    free_vars_(free_vars),
    global_vars_(global_vars) {
  if (!form.isWrapped()) return;
  
  const WrappedKev* wrapped(form);
  this->copyFrom(*wrapped);
}

WrappedKev::WrappedKev(KevesValue form, const WrappedKev* env)
  : WrappedKev(form, env->local_vars(), env->free_vars(), env->global_vars()) {}

void WrappedKev::copyFrom(const WrappedKev& other) {
  this->form_ = other.form_;
  this->local_vars_ = other.local_vars_;
  this->free_vars_ = other.free_vars_;
  this->global_vars_ = other.global_vars_;
}

void WrappedKev::copyFrom(WrappedKev&& other) {
  this->form_ = other.form_;
  this->local_vars_ = other.local_vars_;
  this->free_vars_ = other.free_vars_;
  this->global_vars_ = other.global_vars_;
}

void WrappedKev::set(KevesValue form, KevesValue local_vars, KevesValue free_vars, KevesValue global_vars) {
  this->form_ = form;
  this->local_vars_ = local_vars;
  this->free_vars_ = free_vars;
  this->global_vars_ = global_vars;
}

KevesValue WrappedKev::getDatum(KevesValue obj) {
  if (!obj.isWrapped())
    return obj;

  const WrappedKev* wrapped(obj);
  return wrapped->form();
}
