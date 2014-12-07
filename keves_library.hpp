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

typedef qint32 ver_num_t;


class KevesBase;


class KevesImportBinds {
  friend QDataStream& operator<<(QDataStream& out,
				 const KevesImportBinds& library);

  friend QDataStream& operator>>(QDataStream& in,
				 KevesImportBinds& library);

public:
  KevesImportBinds() = default;
  KevesImportBinds(const KevesImportBinds&) = default;
  // KevesImportBinds(const KevesImportBinds&&) = default; // Qt4.8 not support C+11
  KevesImportBinds& operator=(const KevesImportBinds&) = default;
  // KevesImportBinds& operator=(const KevesImportBinds&&) = delete; // Qt4.8 not support C+11
  ~KevesImportBinds() = default;

  void AddBind(const char* id);
  int CountBinds() const;
  void Display() const;
  const QStringList& GetID() const;
  const QList<ver_num_t>& GetVerNum() const;
  const QStringList& GetBindList() const;
  void SetID(const QString& id1);
  void SetID(const QString& id1, const QString& id2);
  void SetID(const QString& id1, const QString& id2, const QString& id3);
  void SetVerNum(ver_num_t ver_num1);
  void SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2);
  void SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2, ver_num_t ver_num3);
    
private:
  QStringList id_;
  QList<ver_num_t> ver_num_;
  QStringList bind_list_;
};

  
class KevesLibrary {
public:
  KevesLibrary() = default;
  KevesLibrary(const KevesLibrary&) = delete;
  KevesLibrary(const KevesLibrary&&) = delete;
  KevesLibrary& operator=(const KevesLibrary&) = delete;
  KevesLibrary& operator=(const KevesLibrary&&) = delete;
  ~KevesLibrary() = default;

  void AddBind(const char* id, KevesValue kev);
  void AppendImportLib(const KevesImportBinds& import_lib);
  void Display(KevesBase* base) const;
  KevesValue FindBind(const QString& id);
  const QList<QPair<QString, uioword> >* GetBindList() const;
  KevesValue& GetCode();
  KevesValue GetCode() const;
  QList<QPair<QString, uioword> > IndexBinds(const QList<const Kev*>& object_list);
  bool Match(const QStringList& id) const;
  bool Match(const QString& id1) const;
  bool Match(const QString& id1, const QString& id2) const;
  bool Match(const QString& id1, const QString& id2, const QString& id3) const;
  void SetID(const QString& id1);
  void SetID(const QString& id1, const QString& id2);
  void SetID(const QString& id1, const QString& id2, const QString& id3);
  void SetVerNum(ver_num_t ver_num1);
  void SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2);
  void SetVerNum(ver_num_t ver_num1, ver_num_t ver_num2, ver_num_t ver_num3);
  bool WriteToFile(const QString& file_name, KevesBase* base);

  static KevesLibrary* ReadFromFile(const QString& file_name, KevesBase* base);

private:
  int CountImportBinds() const;
  int GetImportBinds(KevesBase* base, QList<const Kev*>* object_list);
  void SetExportBinds(const QList<const Kev*>& object_list);

  static const char* GetErrorString(QFile::FileError error_code);

  QStringList id_;
  QList<ver_num_t> ver_num_;
  QList<QPair<QString, uioword> > bind_list_;
  QList<KevesImportBinds> import_libs_;
protected:
  KevesValue code_;
};
