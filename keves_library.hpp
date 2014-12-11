// keves/keves_library.hpp - library class for Keves
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

#include <QFile>
#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include "keves_value.hpp"


class KevesBase;
class KevesLibrary;


class KevesImportLibrary {
  friend QDataStream& operator<<(QDataStream& out,
				 const KevesImportLibrary& library);

  friend QDataStream& operator>>(QDataStream& in,
				 KevesImportLibrary& library);

public:
  KevesImportLibrary() = default;
  KevesImportLibrary(const KevesImportLibrary&) = default;
  // KevesImportLibrary(const KevesImportLibrary&&) = default; // Qt4.8 not support C+11
  KevesImportLibrary& operator=(const KevesImportLibrary&) = default;
  // KevesImportLibrary& operator=(const KevesImportLibrary&&) = delete; // Qt4.8 not support C+11
  ~KevesImportLibrary() = default;

  KevesImportLibrary(KevesBase* base,
		     const QStringList& id,
		     const QList<ver_num_t> ver_num);

  void AddBind(const char* id);
  int CountBinds() const;
  void DisplayProperty() const;
  KevesValue FindBind(const char* id);
  const QStringList& GetBindList() const;
  QString GetFullName() const;
  const QStringList& GetID() const;
  const QList<ver_num_t>& GetVerNum() const;
  bool IsAvailable() const;
  bool LoadLibrary(KevesBase* base);
  void SetID(const QStringList& id, const QList<ver_num_t>& ver_num);
    
private:
  KevesLibrary* library_;
  QStringList id_;
  QList<ver_num_t> ver_num_;
  QStringList bind_list_;
};

  
class KevesImportLibraryList{
public:
  KevesImportLibraryList() = delete;
  KevesImportLibraryList(const KevesImportLibraryList&) = delete;
  KevesImportLibraryList(const KevesImportLibraryList&&) = delete;
  KevesImportLibraryList& operator=(const KevesImportLibraryList&) = delete;
  KevesImportLibraryList& operator=(const KevesImportLibraryList&&) = delete;
  ~KevesImportLibraryList() = default;

  KevesImportLibraryList(KevesBase* base);

  KevesValue FindBind(const char* id);
  const QList<KevesImportLibrary>& GetList() const;
  bool SetLibrary(const QStringList& id, const QList<ver_num_t>& ver_num);

private:
  KevesBase* base_;
  QList<KevesImportLibrary> import_libs_;
  int count_;
};


class KevesLibrary {
public:
  KevesLibrary() = default;
  KevesLibrary(const KevesLibrary&) = delete;
  KevesLibrary(const KevesLibrary&&) = delete;
  KevesLibrary& operator=(const KevesLibrary&) = delete;
  KevesLibrary& operator=(const KevesLibrary&&) = delete;
  ~KevesLibrary() = default;

  KevesLibrary(const QStringList& id, const QList<ver_num_t>& ver_num);
  
  void AddBind(const char* id, KevesValue kev);
  void DisplayProperty(KevesBase* base) const;
  KevesValue FindBind(const QString& id);
  const QList<QPair<QString, uioword> >* GetBindList() const;
  QString GetFullName() const;
  QList<QPair<QString, uioword> > IndexBinds(const QList<const Kev*>& object_list);
  bool Match(const QStringList& id) const;
  void SetID(const QStringList& id, const QList<ver_num_t>& ver_num);

  bool WriteToFile(KevesBase* base,
		   const QString& file_name,
		   const KevesImportLibraryList& import_binds);

  static void ErrorOfMissingLibrary(const QString& name);

  static void ErrorOfMissingLibrary(const QStringList& id,
				    const QList<ver_num_t>& ver_num);

  static KevesLibrary* ReadFromFile(const QString& file_name, KevesBase* base);

  static QString MakeFullName(const QStringList& id,
			      const QList<ver_num_t>& ver_num);

private:
  int GetImportBinds(KevesBase* base,
		     QList<const Kev*>* object_list,
		     const QList<KevesImportLibrary>& import_libs);

  void ErrorOfFailedToSave(const QFile& file) const;

  void ErrorOfMissingBind(const KevesImportLibrary& import_lib,
			  const QString& bind) const;

  void SetExportBinds(const QList<const Kev*>& object_list);

  static void ErrorOfFailedToOpen(const QFile& file);
  static void ErrorOfMissingLibraryFile(const QString& file_name);
  static const char* GetErrorMessage(QFile::FileError error_code);

  QStringList id_;
  QList<ver_num_t> ver_num_;
  QList<QPair<QString, uioword> > bind_list_;
};
