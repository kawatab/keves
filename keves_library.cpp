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
#include <QDataStream>
#include "keves_base.hpp"
#include "keves_base-inl.hpp"


KevesImportLibrary::KevesImportLibrary(KevesBase* base,
				       const QStringList& id,
				       const QList<ver_num_t> ver_num)
  : library_(base->GetLibrary(id, ver_num)),
    id_(),
    ver_num_(),
    bind_list_() {
  Q_ASSERT(library_);

  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

void KevesImportLibrary::LoadLibrary(KevesBase* base) {
  library_ = base->GetLibrary(id_, ver_num_);
}

void KevesImportLibrary::AddBind(const char* id) {
  bind_list_ << id;
}

KevesValue KevesImportLibrary::FindBind(const char* id) {
  bind_list_ << id;
  return library_->FindBind(id);
}
  
const QStringList& KevesImportLibrary::GetID() const {
  return id_;
}

const QList<ver_num_t>& KevesImportLibrary::GetVerNum() const {
  return ver_num_;
}

const QStringList& KevesImportLibrary::GetBindList() const {
  return bind_list_;
}

void KevesImportLibrary::SetID(const QStringList& id,
			       const QList<ver_num_t>& ver_num) {
  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

int KevesImportLibrary::CountBinds() const {
  return bind_list_.size();
}

void KevesImportLibrary::Display() const {
  std::cout << "(";
  for (auto name : id_) std::cout << qPrintable(name) << " ";
  std::cout << "(";
    
  if (ver_num_.size() > 0) {
    auto iter(ver_num_.begin());
    auto end_iter(ver_num_.end());
    std::cout << *iter++;
    while (iter != end_iter) std::cout << " " << *iter++;
  }

  std::cout << "))";
    
  for (auto bind : bind_list_) std::cout << "  " << qPrintable(bind);

  std::cout << " (number of binds: " << CountBinds() << ')' <<  std::endl;
}


KevesImportLibraryList::KevesImportLibraryList(KevesBase* base)
  : base_(base),
    import_libs_(),
    count_(-1) {}

const QList<KevesImportLibrary>& KevesImportLibraryList::GetList() const {
  return import_libs_;
}

void KevesImportLibraryList::SetLibrary(const QStringList& id,
					const QList<ver_num_t>& ver_num) {
  count_ = 0;

  for (auto lib : import_libs_) {
    if (KevesBase::Match(lib.GetID(), id)) return;
    ++count_;
  }

  KevesImportLibrary lib(base_, id, ver_num);
  import_libs_ << lib;
}

KevesValue KevesImportLibraryList::FindBind(const char* id) {
  return import_libs_[count_].FindBind(id);
}

KevesLibrary::KevesLibrary(const QStringList& id,
			   const QList<ver_num_t>& ver_num)
  : id_(), ver_num_(), bind_list_() {
  SetID(id, ver_num);
}

void KevesLibrary::SetID(const QStringList& id,
			 const QList<ver_num_t>& ver_num) {
  id_.clear();
  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

void KevesLibrary::AddBind(const char* id, KevesValue kev) {
  QPair<QString, uioword> bind(id, kev.toUIntPtr());
  bind_list_ << bind;
}

KevesValue KevesLibrary::FindBind(const QString& id) {
  for (auto bind : bind_list_) {
    if (bind.first.compare(id) == 0)
      return KevesValue::FromUioword<Kev>(bind.second);
  }

  return EMB_UNDEF;
}

const QList<QPair<QString, uioword> >* KevesLibrary::GetBindList() const {
  return &bind_list_;
}

bool KevesLibrary::Match(const QStringList& id) const {
  return KevesBase::Match(id_, id);
}

void KevesLibrary::Display(KevesBase* base) const {
  std::cout << "(";
  for (auto name : id_) std::cout << qPrintable(name) << " ";
  std::cout << "(";

  if (ver_num_.size() > 0) {
    auto iter(ver_num_.begin());
    auto end_iter(ver_num_.end());
    std::cout << *iter++;
    while (iter != end_iter) std::cout << " " << *iter++;
  }

  std::cout << "))";

  for (auto bind : bind_list_) {
    std::cout << "  " << qPrintable(bind.first) << ": "
	      << qPrintable(base->ToString(KevesValue::FromUioword<Kev>(bind.second)))
	      << std::endl;
  }

  std::cout << std::endl;
}

bool KevesLibrary::WriteToFile(KevesBase* base,
			       const QString& file_name,
			       const KevesImportLibraryList& import_libs) {
  QFile file(file_name);

  if (!file.open(QIODevice::WriteOnly)) {
    std::cerr << "An error was occurred when writing the file: "
	      << qPrintable(file_name)
	      << ", \""
	      << GetErrorString(file.error())
	      << "\"\n";

    return false;
  }

  QList<const Kev*> object_list;

  // Get import binds to object_list
  int import_bind_count(GetImportBinds(base,
				       &object_list,
				       import_libs.GetList()));

  if (import_bind_count < 0) return false;

  // Set export binds to object_list
  for (auto bind : bind_list_) {
    base->AppendObjectList(&object_list,
			   KevesValue::FromUioword<Kev>(bind.second));
  }

  int total_bind_count(object_list.size());

  // Write header of library
  QDataStream out(&file);
  out << id_ << ver_num_ << IndexBinds(object_list) << import_libs.GetList();

  // write objects
  for (int i(import_bind_count); i < total_bind_count; ++i) {
    auto func(base->ft_WriteObject(object_list.at(i)->type()));
    (*func)(object_list, out, object_list.at(i));
  }
  
  file.close();
  return true;
}

QList<QPair<QString, uioword> > KevesLibrary::IndexBinds(const QList<const Kev*>& object_list) {
  QList<QPair<QString, uioword> > indexed_list;

  for (auto bind : bind_list_) {
    int index(object_list.indexOf(KevesValue::FromUioword<Kev>(bind.second)));
    indexed_list << QPair<QString, uioword>(bind.first, index);
  }

  return indexed_list;
}

KevesLibrary* KevesLibrary::ReadFromFile(const QString& file_name,
					 KevesBase* base) {
  QFile file(file_name);
  KevesLibrary* lib(new KevesLibrary());

  if (!file.open(QIODevice::ReadOnly)) {
    std::cerr << "An error was occurred when reading the file: "
	      << qPrintable(file_name)
	      << ", \""
	      << GetErrorString(file.error())
	      << "\"\n";

    return lib;
  }

  // Read header of library
  QList<KevesImportLibrary> import_libs;
  QDataStream in(&file);
  in >> lib->id_ >> lib->ver_num_ >> lib->bind_list_ >> import_libs;

  // Get import binds
  QList<const Kev*> object_list;
  for (auto import_lib : import_libs) import_lib.LoadLibrary(base);
  int import_bind_count(lib->GetImportBinds(base, &object_list, import_libs));

  if (import_bind_count < 0) return new KevesLibrary();

  // Read objects
  uioword value;

  while (!in.atEnd()) {
    in >> value;
    object_list << (*base->ft_ReadObject(value))(in, base);
  }

  base->RevertObjects(object_list, import_bind_count);

  // Revert export binds
  lib->SetExportBinds(object_list);

  file.close();
  return lib;
}

int KevesLibrary::GetImportBinds(KevesBase* base,
				 QList<const Kev*>* object_list,
				 const QList<KevesImportLibrary>& import_libs) {
  for (auto import_lib : import_libs) {
    KevesLibrary* lib(base->GetLibrary(import_lib.GetID(),
				       import_lib.GetVerNum()));

    if (lib) {
      std::cout << "Find Library!!!!!!" << std::endl;

      for (auto bind : import_lib.GetBindList()) {
	KevesValue value(lib->FindBind(bind));

	if (value == EMB_UNDEF) {
	  std::cerr << "Bind: " << qPrintable(bind)
		    << " was not found!!!" << std::endl;

	  return -1; // Failed
	}
	
	std::cout << "Find Bind: " << qPrintable(bind) << std::endl;
	*object_list << value.ToPtr();
      }
    }
  }

  return object_list->size();
}

void KevesLibrary::SetExportBinds(const QList<const Kev*>& object_list) {
  for (auto& bind : bind_list_) {
    MutableKevesValue kev(const_cast<Kev*>(object_list.at(bind.second)));
    bind.second = kev.toUIntPtr();
  }
}

const char* KevesLibrary::GetErrorString(QFile::FileError error_code) {
  Q_ASSERT(error_code >= 0 && error_code <=14);

  const char* error_string[] = {
    "No error occurred.",
    "An error occurred when reading from the file.",
    "An error occurred when writing to the file.",
    "A fatal error occurred.",
    "",
    "The file could not be opened.",
    "The operation was aborted.",
    "A timeout occurred.",
    "An unspecified error occurred.",
    "The file could not be removed.",
    "The file could not be renamed.",
    "The position in the file could not be changed.",
    "The file could not be resized.",
    "The file could not be accessed.",
    "No error occurred."
  };

  return error_string[error_code];
}

QDataStream& operator<<(QDataStream& out,
			const KevesImportLibrary& import_binds) {
  out << import_binds.id_ << import_binds.ver_num_ << import_binds.bind_list_;
  return out;
}

QDataStream& operator>>(QDataStream& in, KevesImportLibrary& import_binds) {
  in >> import_binds.id_ >> import_binds.ver_num_ >> import_binds.bind_list_;
  return in;
}
