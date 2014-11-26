// Keves/keves_filehandler.cpp - file handler for Keves
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
#include "keves_gc.hpp"
#include "keves_gc-inl.hpp"
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
#include "lib/library.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"


KevesFileIO::KevesFileIO(const QString& file_name)
  : file_(file_name) {}


bool KevesFileIO::Write(KevesBase* base, const QList<KevesLibrary>& libs, KevesValue value) {
  if (!file_.open(QIODevice::WriteOnly)) return false;

  QDataStream out(&file_);

  out << libs;

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

QList<KevesLibrary> KevesFileIO::Read(KevesBase* base) {
  QList<KevesLibrary> libs;

  if (!file_.open(QIODevice::ReadOnly)) return libs;

  QDataStream in(&file_);
  in >> libs;
  
  uioword value;

  while (!in.atEnd()) {
    in >> value;
    libs[0].object_list_.append((*base->ft_ReadObject(value))(in, base->gc()));
  }

  base->RevertObjects(libs[0].object_list_);
  libs[0].code_ = libs[0].object_list_.at(0);

  return libs;
}
