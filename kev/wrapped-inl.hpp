// keves/kev/wrapped.hpp - wrappeds for Keves
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
WrappedKev* WrappedKev::make(ZONE* zone, KevesValue form, KevesValue local_vars,
			     KevesValue free_vars, KevesValue global_vars) {
  auto ctor = [form, local_vars, free_vars, global_vars](void *ptr) {
    return new(ptr) WrappedKev(form, local_vars, free_vars, global_vars);
  };

  return zone->make(ctor, alloc_size(nullptr));
}
