// keves/keves_file_io.cpp - file handler for Keves
// Keves will be an R6RS Scheme implementation.
//
// Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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


#include "keves_file_io.hpp"

#include <QDataStream>
#include "keves_base.hpp"
#include "keves_library.hpp"
#include "kev/bignum.hpp"
#include "kev/code.hpp"
#include "kev/frame.hpp"
#include "kev/number.hpp"
#include "kev/pair.hpp"
#include "kev/procedure.hpp"
#include "kev/reference.hpp"
#include "kev/string.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"
#include "kev/wind.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"


KevesFileIO::KevesFileIO(const QString& file_name)
  : file_(file_name) {}


bool KevesFileIO::Write(KevesBase* base, const KevesLibrary& lib, KevesValue value) {
  if (!file_.open(QIODevice::WriteOnly)) return false;

  QDataStream out(&file_);

  out << lib;

  if (!value.IsPtr()) return false;
  
  QList<const Kev*> list(base->GetObjectList(value.ToPtr()));
  int list_size(list.size());
  
  for (int index(0); index < list_size; ++index) {
    const Kev* temp(list.at(index));
    (*base->ft_WriteObject(temp->type()))(list, out, temp);
  }
  
  file_.close();
  return true;
}

KevesLibrary* KevesFileIO::Read(KevesBase* base) {
  KevesLibrary* lib(new KevesLibrary());

  if (!file_.open(QIODevice::ReadOnly)) return lib;

  QDataStream in(&file_);
  in >> *lib;
  
  uioword value;

  while (!in.atEnd()) {
    in >> value;
    lib->GetObjectList().append((*base->ft_ReadObject(value))(in, base));
  }

  base->RevertObjects(lib->GetObjectList());
  lib->GetCode() = lib->GetObjectList().at(0);

  return lib;
}
