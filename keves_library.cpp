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
#include "keves_common.hpp"
#include "keves_common-inl.hpp"


KevesImportLibrary::KevesImportLibrary(KevesCommon* common,
				       const QStringList& id,
				       const QList<ver_num_t> ver_num)
  : library_(common->getLibrary(id, ver_num)),
    id_(),
    ver_num_(),
    bind_list_() {
  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

bool KevesImportLibrary::loadLibrary(KevesCommon* common) {
  return (library_ = common->getLibrary(id_, ver_num_));
}

bool KevesImportLibrary::isAvailable() const {
  return library_;
}

void KevesImportLibrary::addBind(const char* id) {
  bind_list_ << id;
}

KevesValue KevesImportLibrary::nominateBind(const char* id) {
  if (bind_list_.indexOf(id) < 0) bind_list_ << id;
  return library_->findBind(id);
}
  
const QStringList& KevesImportLibrary::getID() const {
  return id_;
}

const QList<ver_num_t>& KevesImportLibrary::getVerNum() const {
  return ver_num_;
}

QString KevesImportLibrary::getFullName() const {
  return KevesLibrary::makeFullName(id_, ver_num_);
}

const QStringList& KevesImportLibrary::getBindList() const {
  return bind_list_;
}

void KevesImportLibrary::setID(const QStringList& id,
			       const QList<ver_num_t>& ver_num) {
  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

int KevesImportLibrary::countBinds() const {
  return bind_list_.size();
}

void KevesImportLibrary::displayProperty() const {
  std::cout << qPrintable(getFullName()) << std::endl;
  for (auto bind : bind_list_) std::cout << "  " << qPrintable(bind);

  std::cout << " (number of binds: " << countBinds() << ')' <<  std::endl;
}


KevesImportLibraryList::KevesImportLibraryList(KevesCommon* common)
  : common_(common),
    import_libs_(),
    count_(-1) {}

const QList<KevesImportLibrary>& KevesImportLibraryList::getList() const {
  return import_libs_;
}

bool KevesImportLibraryList::setLibrary(const QStringList& id,
					const QList<ver_num_t>& ver_num) {
  /*
  count_ = 0;

  for (auto lib : import_libs_) {
    if (KevesCommon::match(lib.getID(), id)) return true;
    ++count_;
  }
  */
  for (count_ = 0; count_ < import_libs_.size(); ++count_) {
    if (KevesCommon::match(import_libs_.at(count_).getID(), id))
      return true;
  }

  KevesImportLibrary lib(common_, id, ver_num);

  if (!lib.isAvailable()) {
    KevesLibrary::errorOfMissingLibrary(lib.getFullName());
    return false;
  }

  import_libs_ << lib;
  return true;
}

KevesValue KevesImportLibraryList::nominateBind(const char* id) {
  return import_libs_[count_].nominateBind(id);
}

KevesLibrary::KevesLibrary(const QStringList& id,
			   const QList<ver_num_t>& ver_num)
  : id_(), ver_num_(), bind_list_() {
  setID(id, ver_num);
}

KevesLibrary::KevesLibrary(const char* id)
  : id_(), ver_num_(), bind_list_() {
  id_ << id;
  
}
  
KevesLibrary::KevesLibrary(const char* id1, const char* id2, ver_num_t ver_num)
  : id_(), ver_num_(), bind_list_() {
  id_ << id1 << id2;
  ver_num_ << ver_num;
}

KevesLibrary::KevesLibrary(const char* id1, const char* id2,
			   ver_num_t ver_num1, ver_num_t ver_num2)
  : id_(), ver_num_(), bind_list_() {
  id_ << id1 << id2;
  ver_num_ << ver_num1 << ver_num2;
}

void KevesLibrary::setID(const QStringList& id,
			 const QList<ver_num_t>& ver_num) {
  id_.clear();
  for (auto str : id) id_ << str;
  for (auto num : ver_num) ver_num_ << num;
}

void KevesLibrary::addBind(const char* id, KevesValue kev) {
  QPair<QString, uioword> bind(id, kev.toUIntPtr());
  bind_list_ << bind;
}

KevesValue KevesLibrary::findBind(const QString& id) const {
  for (auto bind : bind_list_) {
    if (bind.first.compare(id) == 0)
      return KevesValue::fromUioword<Kev>(bind.second);
  }

  std::cerr << "Not found bound: " << qPrintable(id) << '\n';
  return EMB_UNDEF;
}

const QList<QPair<QString, uioword> >* KevesLibrary::getBindList() const {
  return &bind_list_;
}

bool KevesLibrary::match(const QStringList& id) const {
  return KevesCommon::match(id_, id);
}

QString KevesLibrary::makeFullName(const QStringList& id,
				   const QList<ver_num_t>& ver_num) {
  QString full_name("(");
  for (auto name : id) full_name.append(name).append(" ");
  full_name.append("(");
    
  if (ver_num.size() > 0) {
    auto iter(ver_num.begin());
    auto end_iter(ver_num.end());
    full_name.append(QString::number(*iter++));

    while (iter != end_iter)
      full_name.append(" ").append(QString::number(*iter++));
  }

  full_name.append("))");
  return full_name;
}

QString KevesLibrary::getFullName() const {
  return makeFullName(id_, ver_num_);
}

void KevesLibrary::displayProperty(KevesCommon* common) const {
  std::cout << qPrintable(getFullName()) << std::endl;

  for (auto bind : bind_list_) {
    std::cout << "  " << qPrintable(bind.first) << ": "
	      << qPrintable(common->toString(KevesValue::fromUioword<Kev>(bind.second)))
	      << std::endl;
  }

  std::cout << std::endl;
}

bool KevesLibrary::writeToFile(KevesCommon* common,
			       const QString& file_name,
			       const KevesImportLibraryList& import_libs) const {
  QFile file(file_name);

  if (!file.open(QIODevice::WriteOnly)) {
    errorOfFailedToSave(file);
    return false;
  }

  QList<const Kev*> object_list;

  // Get import binds to object_list
  int import_bind_count(getImportBinds(common,
				       &object_list,
				       import_libs.getList()));

  if (import_bind_count < 0) return false;

  // Set export binds to object_list
  for (auto bind : bind_list_) {
    common->appendObjectList(&object_list,
			   KevesValue::fromUioword<Kev>(bind.second));
  }

  int total_bind_count(object_list.size());

  // Write header of library
  QDataStream out(&file);
  out << id_ << ver_num_ << indexBinds(object_list) << import_libs.getList();

  // write objects
  for (int i(import_bind_count); i < total_bind_count; ++i) {
    auto func(common->ft_writeObject(object_list.at(i)->type()));
    (*func)(object_list, out, object_list.at(i));
  }
  
  file.close();
  return true;
}

void KevesLibrary::errorOfFailedToSave(const QFile& file) const {
  std::cerr << "An error was occurred when writing the file: "
	    << qPrintable(file.fileName())
	    << ", \"" << getErrorMessage(file.error()) << "\"\n";
}

QList<QPair<QString, uioword> > KevesLibrary::indexBinds(const QList<const Kev*>& object_list) const {
  QList<QPair<QString, uioword> > indexed_list;

  for (auto bind : bind_list_) {
    int index(object_list.indexOf(KevesValue::fromUioword<Kev>(bind.second)));
    indexed_list << QPair<QString, uioword>(bind.first, index);
  }

  return indexed_list;
}

KevesLibrary* KevesLibrary::readFromFile(const QString& file_name,
					 KevesCommon* common) {
  QFile file(file_name);
  KevesLibrary* lib(new KevesLibrary());

  if (!file.exists()) {
    errorOfMissingLibraryFile(file_name);
    return nullptr;
  }
  
  if (!file.open(QIODevice::ReadOnly)) {
    errorOfFailedToOpen(file);
    return nullptr;
  }

  // Read header of library
  QList<KevesImportLibrary> import_libs;
  QDataStream in(&file);
  in >> lib->id_ >> lib->ver_num_ >> lib->bind_list_ >> import_libs;

  // Get import binds
  QList<const Kev*> object_list;

  for (auto import_lib : import_libs) {
    if(!import_lib.loadLibrary(common)) {
      errorOfMissingLibrary(import_lib.getFullName());
      return nullptr;
    }
  }

  int import_bind_count(lib->getImportBinds(common, &object_list, import_libs));

  if (import_bind_count < 0) return nullptr;

  // Read objects
  uioword value;

  while (!in.atEnd()) {
    in >> value;
    object_list << (*common->ft_readObject(value))(in, common);
  }

  common->revertObjects(object_list, import_bind_count);

  // Revert export binds
  lib->setExportBinds(object_list);

  file.close();
  return lib;
}

void KevesLibrary::errorOfMissingLibrary(const QString& name) {
  std::cerr << "Not found library: " << qPrintable(name) << "\n";
}

void KevesLibrary::errorOfMissingLibrary(const QStringList& id,
					 const QList<ver_num_t>& ver_num) {
  errorOfMissingLibrary(makeFullName(id, ver_num));
}

void KevesLibrary::errorOfMissingLibraryFile(const QString& file_name) {
  std::cerr << "Library: " << qPrintable(file_name) <<  "was not found!!!\n";
}


void KevesLibrary::errorOfFailedToOpen(const QFile& file) {
  std::cerr << "An error was occurred when reading the file: "
	    << qPrintable(file.fileName())
	    << ", \"" << getErrorMessage(file.error()) << "\"\n";
}

int KevesLibrary::getImportBinds(KevesCommon* common,
				 QList<const Kev*>* object_list,
				 const QList<KevesImportLibrary>& import_libs) const {
  for (auto import_lib : import_libs) {
    KevesLibrary* lib(common->getLibrary(import_lib.getID(),
				       import_lib.getVerNum()));

    if (!lib) {
      errorOfMissingLibrary(import_lib.getFullName());
      return -1; // Failed
    }
    
    for (auto bind : import_lib.getBindList()) {
      KevesValue value(lib->findBind(bind));
      
      if (value == EMB_UNDEF) {
	errorOfMissingBind(import_lib, bind);
	return -1; // Failed
      }
      
      *object_list << value.toPtr();
    }
  }

  return object_list->size();
}

void KevesLibrary::errorOfMissingBind(const KevesImportLibrary& import_lib,
				      const QString& bind) const {
  std::cerr << "Bind: " << qPrintable(bind)
	    << " was not found in "
	    << qPrintable(import_lib.getFullName())
	    << "!!!\n";
}

void KevesLibrary::setExportBinds(const QList<const Kev*>& object_list) {
  for (auto& bind : bind_list_) {
    MutableKevesValue kev(const_cast<Kev*>(object_list.at(bind.second)));
    bind.second = kev.toUIntPtr();
  }
}

const char* KevesLibrary::getErrorMessage(QFile::FileError error_code) {
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
