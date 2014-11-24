// Keves/keves_library.hpp - library class for Keves
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


#pragma once

#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include "keves_value.hpp"

typedef qint32 ver_num_t;


struct KevesLibraryID {
  QStringList id_;
  QList<ver_num_t> ver_num_;
  QStringList binds;
};
  
class KevesLibrary {
public:
  KevesLibrary() = default;
  KevesLibrary(const KevesLibrary&) = default;
  // KevesLibrary(const KevesLibrary&&) = default; // Qt4.8 not support C+11
  KevesLibrary& operator=(const KevesLibrary&) = default;
  // KevesLibrary& operator=(const KevesLibrary&&) = delete; // Qt4.8 not support C+11
  virtual ~KevesLibrary() = default;

  void AddBind(const char* id, KevesValue kev);
  KevesValue FindBind(const char* id);
  // virtual KevesLibrary* Init(KevesGC* gc) = 0;
  void DisplayLibraryName() const;

  QStringList id_;
  QList<ver_num_t> ver_num_;
  QList<QPair<QString, uioword> > bind_list_;
  KevesLibraryID import_libs_;
  QList<Kev*> object_list_;
  KevesValue code_;
};

QDataStream& operator<<(QDataStream& out, const KevesLibrary& library);
QDataStream& operator>>(QDataStream& in, KevesLibrary& library);
