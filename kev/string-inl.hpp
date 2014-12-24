// keves/kev/string-inl.hpp - strings for Keves
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
StringCoreKev* StringCoreKev::make(ZONE* zone, const QString& str) {
  auto ctor = [&str](void* ptr) {
    StringCoreKev* string(new(ptr) StringCoreKev(str.size()));
    string->set(str);
    return string;
  };
  
  return zone->make(ctor, alloc_size(str.size()));
}

template<class ZONE>
StringCoreKev* StringCoreKev::make(ZONE* zone, int size) {
  auto ctor = [size](void* ptr) { return new(ptr) StringCoreKev(size); };
  return zone->make(ctor, alloc_size(size));
}

template<class ZONE>
StringKev* StringKev::make(ZONE* zone, int size) {
  return zone->make(ctor(zone, size), alloc_size(nullptr));
}

template<class ZONE>
StringKev* StringKev::make(ZONE* zone, const QString& str) {
  return zone->make(ctor(zone, str), alloc_size(nullptr));
}

template<class ZONE>
StringKev* StringKev::makeSubstring(ZONE* zone, const StringKev* str, int start, int end) {
  auto ctor = [str, start, end](void* ptr) {
    return new(ptr) StringKev(str->mid(start, end - start));
  };
    
  return zone->make(ctor, alloc_size(nullptr));
}

template<class ZONE>
StringKev* StringKev::make(ZONE* zone, StringCoreKev* core, int idx, int len) {
  auto ctor = [core, idx, len](void* ptr) {
    return new(ptr) StringKev(core, idx, len);
  };
    
  return zone->make(ctor, alloc_size(nullptr));
}
