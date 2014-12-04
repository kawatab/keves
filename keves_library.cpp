// Keves/keves_library.cpp - library class for Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
// 
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "lib/library.hpp"

#include "iostream"

void KevesLibrary::AddBind(const char* id, KevesValue kev) {
  QPair<QString, uioword> bind(id, kev.toUIntPtr());
  bind_list_.append(bind);
}

KevesValue KevesLibrary::FindBind(const char* id) {
  for (auto bind : bind_list_) {
    if (bind.first.compare(id) == 0)
      return KevesValue::FromUioword<Kev>(bind.second);
  }

  return EMB_UNDEF;
}

void KevesLibrary::DisplayLibraryName() const {
  std::cout << "(";

  for (auto name : id_) std::cout << qPrintable(name) << " ";
    
  switch (ver_num_.size()) {
  case 0:
    std::cout << "()";
    break;

  case 1:
    std::cout << "(" << ver_num_.at(0) << ")";
    break;

  default:
    std::cout << "(" << ver_num_.at(0);

    for (int i(1); i < ver_num_.size(); ++i)
      std::cout << " " << ver_num_.at(i);

    std::cout << ")";
  }

  std::cout << ")" << std::endl;
    
  for (auto bind : bind_list_) std::cout << qPrintable(bind.first) << std::endl;
}

QDataStream& operator<<(QDataStream& out, const KevesLibrary& library) {
  QList<QString> name_list;
  
  for (auto bind : library.bind_list_)
    name_list.append(bind.first);
  
  out << library.id_ << library.ver_num_ << name_list;
  return out;
}

QDataStream& operator>>(QDataStream& in, KevesLibrary& library) {
  QList<QString> name_list;
  in >> library.id_ >> library.ver_num_ >> name_list;

  for (auto name : name_list) {
    QPair<QString, uioword> bind(name, EMB_NULL);
    library.bind_list_.append(bind);
  }
  
  return in;
}
