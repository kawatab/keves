// keves/kev/frame-inl.hpp - frames for Keves
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
ArgumentFrameKev* ArgumentFrameKev::Make(ZONE* zone, int size) {
  auto ctor = [size](void* ptr) {
    ArgumentFrameKev* arg_frame(new(ptr) ArgumentFrameKev(size));
    std::fill_n(arg_frame->array(), size, EMB_UNDEF);
    return arg_frame;
  };

  return zone->Make(ctor, alloc_size(size));
}


template<class ZONE>
LocalVarFrameKev* LocalVarFrameKev::Make(ZONE* zone, int size) {
  auto ctor = [size](void* ptr) {
    LocalVarFrameKev* env_frame(new(ptr) LocalVarFrameKev(size));
    std::fill_n(env_frame->array(), size, EMB_UNDEF);
    return env_frame;
  };
  
  return zone->Make(ctor, alloc_size(size));
}

template<class ZONE>
FreeVarFrameKev* FreeVarFrameKev::Make(ZONE* zone, int size, FreeVarFrameKev* next) {
  auto ctor = [size, next](void* ptr) {
    FreeVarFrameKev* closure(new(ptr) FreeVarFrameKev(size, next));
    std::fill_n(closure->array(), size, EMB_UNDEF);
    return closure;
  };
  
  return zone->Make(ctor, alloc_size(size));
}

template<class ZONE>
StackFrameKev* StackFrameKev::Make(ZONE* zone) {
  auto ctor = [](void* ptr) {
    return new(ptr) StackFrameKev();
  };

  return zone->Make(ctor, alloc_size(nullptr));
}
