// keves/keves_library.cpp - library class for Keves
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


#include "keves_library.hpp"

#include <iostream>


void KevesImportBinds::AddBind(const char* id) {
  bind_list_.append(id);
}

const QStringList& KevesImportBinds::GetID() const {
  return id_;
}

const QList<ver_num_t>& KevesImportBinds::GetVerNum() const {
  return ver_num_;
}

const QStringList& KevesImportBinds::GetBindList() const {
  return bind_list_;
}

void KevesImportBinds::SetID(const QString& id1) {
  id_.clear();
  id_ << id1;
}

void KevesImportBinds::SetID(const QString& id1, const QString& id2) {
  id_.clear();
  id_ << id1 << id2;
}

void KevesImportBinds::SetID(const QString& id1, const QString& id2,
			     const QString& id3) {
  id_.clear();
  id_ << id1 << id2 << id3;
}

void KevesImportBinds::SetVerNum(ver_num_t ver_num1) {
  ver_num_.clear();
  ver_num_ << ver_num1;
}

void KevesImportBinds::SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2) {
  ver_num_.clear();
  ver_num_ << ver_num1 << ver_num2;
}

void KevesImportBinds::SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2,
				 ver_num_t ver_num3) {
  ver_num_.clear();
  ver_num_ << ver_num1 << ver_num2 << ver_num3;
}

void KevesImportBinds::Display() const {
  std::cout << "(";
  for (auto name : id_) std::cout << qPrintable(name) << " (";
    
  if (ver_num_.size() > 0) {
    auto iter(ver_num_.begin());
    auto end_iter(ver_num_.end());
    std::cout << *iter++;
    while (iter != end_iter) std::cout << " " << *iter++;
  }

  std::cout << "))";
    
  for (auto bind : bind_list_)
    std::cout << "  " << qPrintable(bind);

  std::cout << std::endl;
}


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

/*
int KevesLibrary::GetNumberOfExportBinds() const {
  return bind_list.size();
}

int KevesLibrary::GetNumberOfImportBinds() const {
  int sum(0);
  for (auto lib : import_libs_) sum += lib.GetNumberOfExportBinds();
  return sum;
}
*/

bool KevesLibrary::Match(const QString& id1) const {
  return id_.size() == 1 && id_.at(0).compare(id1) == 0;
}

bool KevesLibrary::Match(const QString& id1, const QString& id2) const {
  std::cout << "id1: " << qPrintable(id_.at(0))
	    << "id2: " << qPrintable(id_.at(1)) << std::endl;

  return id_.size() == 2 &&
    id_.at(0).compare(id1) == 0 &&
    id_.at(1).compare(id2) == 0;
}

bool KevesLibrary::Match(const QString& id1,
			 const QString& id2,
			 const QString& id3) const {
  return id_.size() == 2 &&
    id_.at(0).compare(id1) == 0 &&
    id_.at(1).compare(id2) == 0 &&
    id_.at(2).compare(id3) == 0;
}

void KevesLibrary::SetID(const QString& id1) {
  id_.clear();
  id_ << id1;
}

void KevesLibrary::SetID(const QString& id1, const QString& id2) {
  id_.clear();
  id_ << id1 << id2;
}

void KevesLibrary::SetID(const QString& id1, const QString& id2,
			const QString& id3) {
  id_.clear();
  id_ << id1 << id2 << id3;
}

void KevesLibrary::SetVerNum(ver_num_t ver_num1) {
  ver_num_.clear();
  ver_num_ << ver_num1;
}

void KevesLibrary::SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2) {
  ver_num_.clear();
  ver_num_ << ver_num1 << ver_num2;
}

void KevesLibrary::SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2,
			     ver_num_t ver_num3) {
  ver_num_.clear();
  ver_num_ << ver_num1 << ver_num2 << ver_num3;
}

void KevesLibrary::AppendImportLib(const KevesImportBinds& import_lib) {
  import_libs_.append(import_lib);
}

void KevesLibrary::SetObjectList(const QList<Kev*>& object_list) {
  object_list_ = object_list;
}

KevesValue& KevesLibrary::GetCode() {
  return code_;
}

KevesValue KevesLibrary::GetCode() const {
  return code_;
}

QList<Kev*>& KevesLibrary::GetObjectList() {
  return object_list_;
}

const QList<Kev*>& KevesLibrary::GetObjectList() const {
  return object_list_;
}

void KevesLibrary::Display() const {
  std::cout << "(";
  for (auto name : id_) std::cout << qPrintable(name) << " (";
    
  if (ver_num_.size() > 0) {
    auto iter(ver_num_.begin());
    auto end_iter(ver_num_.end());
    std::cout << *iter++;
    while (iter != end_iter) std::cout << " " << *iter++;
  }

  std::cout << "))";

  for (auto bind : bind_list_)
    std::cout << "  " << qPrintable(bind.first) << std::flush;

  std::cout << std::endl;
  std::cout << "Import binds: " << std::endl;
  for (auto lib : import_libs_) lib.Display();
}

QDataStream& operator<<(QDataStream& out, const KevesLibrary& library) {
  QList<QString> name_list;
  
  for (auto bind : library.bind_list_)
    name_list.append(bind.first);
  
  out << library.id_ << library.ver_num_ << name_list << library.import_libs_;
  return out;
}

QDataStream& operator>>(QDataStream& in, KevesLibrary& library) {
  QList<QString> name_list;
  in >> library.id_ >> library.ver_num_ >> name_list >> library.import_libs_;

  for (auto name : name_list) {
    QPair<QString, uioword> bind(name, EMB_NULL);
    library.bind_list_.append(bind);
  }
  
  return in;
}

QDataStream& operator<<(QDataStream& out, const KevesImportBinds& import_binds) {
  out << import_binds.id_ << import_binds.ver_num_ << import_binds.bind_list_;
  return out;
}

QDataStream& operator>>(QDataStream& in, KevesImportBinds& import_binds) {
  in >> import_binds.id_ >> import_binds.ver_num_ >> import_binds.bind_list_;
  return in;
}
