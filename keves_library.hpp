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


class KevesCommon;
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

  KevesImportLibrary(KevesCommon* common,
		     const QStringList& id,
		     const QList<ver_num_t> ver_num);

  void addBind(const char* id);
  int countBinds() const;
  void displayProperty() const;
  KevesValue nominateBind(const char* id);
  const QStringList& getBindList() const;
  QString getFullName() const;
  const QStringList& getID() const;
  const QList<ver_num_t>& getVerNum() const;
  bool isAvailable() const;
  bool loadLibrary(KevesCommon* common);
  void setID(const QStringList& id, const QList<ver_num_t>& ver_num);
    
private:
  KevesLibrary* library_;
  QStringList id_;
  QList<ver_num_t> ver_num_;
  QStringList bind_list_;
};

  
class KevesImportLibraryList {
public:
  KevesImportLibraryList() = delete;
  KevesImportLibraryList(const KevesImportLibraryList&) = delete;
  KevesImportLibraryList(const KevesImportLibraryList&&) = delete;
  KevesImportLibraryList& operator=(const KevesImportLibraryList&) = delete;
  KevesImportLibraryList& operator=(const KevesImportLibraryList&&) = delete;
  ~KevesImportLibraryList() = default;

  KevesImportLibraryList(KevesCommon* common);

  KevesValue nominateBind(const char* id);
  const QList<KevesImportLibrary>& getList() const;
  bool setLibrary(const QStringList& id, const QList<ver_num_t>& ver_num);

private:
  KevesCommon* common_;
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
  KevesLibrary(const char* id);
  KevesLibrary(const char* id1, const char* id2, ver_num_t ver_num);

  KevesLibrary(const char* id1, const char* id2,
	       ver_num_t ver_num1, ver_num_t ver_num2);
  
  void addBind(const char* id, KevesValue kev);
  void displayProperty(KevesCommon* common) const;
  KevesValue findBind(const QString& id) const;
  const QList<QPair<QString, uioword> >* getBindList() const;
  QString getFullName() const;
  QList<QPair<QString, uioword> > indexBinds(const QList<const Kev*>& object_list) const;
  bool match(const QStringList& id) const;
  void setID(const QStringList& id, const QList<ver_num_t>& ver_num);

  bool writeToFile(KevesCommon* common,
		   const QString& file_name,
		   const KevesImportLibraryList& import_binds) const;

  static void errorOfMissingLibrary(const QString& name);

  static void errorOfMissingLibrary(const QStringList& id,
				    const QList<ver_num_t>& ver_num);

  static KevesLibrary* readFromFile(const QString& file_name,
				    KevesCommon* common);

  static QString makeFullName(const QStringList& id,
			      const QList<ver_num_t>& ver_num);

private:
  int getImportBinds(KevesCommon* common,
		     QList<const Kev*>* object_list,
		     const QList<KevesImportLibrary>& import_libs) const;

  void errorOfFailedToSave(const QFile& file) const;

  void errorOfMissingBind(const KevesImportLibrary& import_lib,
			  const QString& bind) const;

  void setExportBinds(const QList<const Kev*>& object_list);

  static void errorOfFailedToOpen(const QFile& file);
  static void errorOfMissingLibraryFile(const QString& file_name);
  static const char* getErrorMessage(QFile::FileError error_code);

  QStringList id_;
  QList<ver_num_t> ver_num_;
  QList<QPair<QString, uioword> > bind_list_;
};
