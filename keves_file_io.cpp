/* Keves/keves_filehandler.cpp - file handler for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keves_file_io.hpp"
#include "keves_file_io-inl.hpp"

#include <iostream>
#include <iomanip>
#include <QDataStream>
#include <QFile>
#include <QStack>
#include "bignum.hpp"
#include "code_kev.hpp"
#include "keves_char.hpp"
#include "keves_fixnum.hpp"
#include "keves_frame.hpp"
#include "keves_gc.hpp"
#include "keves_instruct.hpp"
#include "keves_library.hpp"
#include "number_kev.hpp"
#include "pair_kev.hpp"
#include "procedure_kev.hpp"
#include "reference_kev.hpp"
#include "string_kev.hpp"
#include "symbol_kev.hpp"
#include "vector_kev.hpp"
#include "wind_kev.hpp"


#define FUNCTION_TABLE1(arr, fn, t1)		\
  arr[CodeKev::TYPE]		     = CodeKev::fn<KevesFileIO, t1>;	\
  arr[Bignum::TYPE]		     = Bignum::fn<KevesFileIO, t1>;	\
  arr[RationalNumberKev::TYPE]	     = RationalNumberKev::fn<KevesFileIO, t1>;	\
  arr[FlonumKev::TYPE]		     = FlonumKev::fn<KevesFileIO, t1>;	\
  arr[ExactComplexNumberKev::TYPE]   = ExactComplexNumberKev::fn<KevesFileIO, t1>;	\
  arr[InexactComplexNumberKev::TYPE] = InexactComplexNumberKev::fn<KevesFileIO, t1>;	\
  arr[StringCoreKev::TYPE]	     = StringCoreKev::fn<KevesFileIO, t1>;	\
  arr[StringKev::TYPE]		     = StringKev::fn<KevesFileIO, t1>;	\
  arr[SymbolKev::TYPE]		     = SymbolKev::fn<KevesFileIO, t1>;	\
  arr[VectorKev::TYPE]		     = VectorKev::fn<KevesFileIO, t1>;	\
  arr[WindKev::TYPE]		     = WindKev::fn<KevesFileIO, t1>;	\
  arr[ReferenceKev::TYPE]	     = ReferenceKev::fn<KevesFileIO, t1>;	\
  arr[LambdaKev::TYPE]		     = LambdaKev::fn<KevesFileIO, t1>;	\
  arr[ArgumentFrameKev::TYPE]	     = ArgumentFrameKev::fn<KevesFileIO, t1>;	\
  arr[LocalVarFrameKev::TYPE]	     = LocalVarFrameKev::fn<KevesFileIO, t1>;	\
  arr[FreeVarFrameKev::TYPE]	     = FreeVarFrameKev::fn<KevesFileIO, t1>;	\
  arr[StackFrameKev::TYPE]	     = StackFrameKev::fn<KevesFileIO, t1>;	\
  arr[PairKev::TYPE]		     = PairKev::fn<KevesFileIO, t1>;

#define FUNCTION_TABLE2(arr, fn, t1, t2)	\
  arr[CodeKev::TYPE]		     = CodeKev::fn<KevesFileIO, t1, t2>;	\
  arr[Bignum::TYPE]		     = Bignum::fn<KevesFileIO, t1, t2>;	\
  arr[RationalNumberKev::TYPE]	     = RationalNumberKev::fn<KevesFileIO, t1, t2>;	\
  arr[FlonumKev::TYPE]		     = FlonumKev::fn<KevesFileIO, t1, t2>;	\
  arr[ExactComplexNumberKev::TYPE]   = ExactComplexNumberKev::fn<KevesFileIO, t1, t2>;	\
  arr[InexactComplexNumberKev::TYPE] = InexactComplexNumberKev::fn<KevesFileIO, t1, t2>;	\
  arr[StringCoreKev::TYPE]	     = StringCoreKev::fn<KevesFileIO, t1, t2>;	\
  arr[StringKev::TYPE]		     = StringKev::fn<KevesFileIO, t1, t2>;	\
  arr[SymbolKev::TYPE]		     = SymbolKev::fn<KevesFileIO, t1, t2>;	\
  arr[VectorKev::TYPE]		     = VectorKev::fn<KevesFileIO, t1, t2>;	\
  arr[WindKev::TYPE]		     = WindKev::fn<KevesFileIO, t1, t2>;	\
  arr[ReferenceKev::TYPE] 	     = ReferenceKev::fn<KevesFileIO, t1, t2>;	\
  arr[LambdaKev::TYPE]		     = LambdaKev::fn<KevesFileIO, t1, t2>;	\
  arr[ArgumentFrameKev::TYPE]	     = ArgumentFrameKev::fn<KevesFileIO, t1, t2>;	\
  arr[LocalVarFrameKev::TYPE]	     = LocalVarFrameKev::fn<KevesFileIO, t1, t2>;	\
  arr[FreeVarFrameKev::TYPE]	     = FreeVarFrameKev::fn<KevesFileIO, t1, t2>;	\
  arr[StackFrameKev::TYPE]	     = StackFrameKev::fn<KevesFileIO, t1, t2>;	\
  arr[PairKev::TYPE]		     = PairKev::fn<KevesFileIO, t1, t2>;

KevesFileIO::KevesFileIO(const QString& file_name)
  : file_(file_name) {
  FUNCTION_TABLE1(ft_pushChildren_, PushChildren, QStack<const Kev*>);
  FUNCTION_TABLE2(ft_writeObject_, WriteObject, QList<const Kev*>, QDataStream);
  FUNCTION_TABLE2(ft_readObject_, ReadObject, QDataStream, KevesGC);
  FUNCTION_TABLE1(ft_revertObject_, RevertObject, QList<Kev*>);
};

#undef FUNCTION_TABLE1
#undef FUNCTION_TABLE2

bool KevesFileIO::write(const QList<KevesLibrary>& libs, KevesValue value) {
  if (!file_.open(QIODevice::WriteOnly)) return false;

  QDataStream out(&file_);

  out << libs;

  if (!value.IsPtr()) return false;
  
  QList<const Kev*> list(getObjectList(value.ToPtr()));
  int list_size(list.size());
  
  for (int index(0); index < list_size; ++index) {
    const Kev* temp(list.at(index));
    (*ft_writeObject_[temp->type()])(list, out, temp);
  }
  
  file_.close();
  return true;
}

const QList<const Kev*> KevesFileIO::getObjectList(const Kev* kev) {
  QList<const Kev*> list;
  QStack<const Kev*> pending;
  pending.push(kev);

  while (!pending.isEmpty()) {
    const Kev* temp(pending.pop());

    if (list.indexOf(temp) < 0) {
      (*ft_pushChildren_[temp->type()])(&pending, temp);
      list.append(temp);
    }
  }

  return list;
}
      
void KevesFileIO::PushValue(QStack<const Kev*>* pending, KevesValue value) {
  if (value.IsPtr()) pending->push(value.ToPtr());
}

uioword KevesFileIO::WrapAddress(const QList<const Kev*>& table,
				 KevesValue value) {
  if (!value.IsPtr()) return value.toUIntPtr();
  
  int index(table.indexOf(value.ToPtr()));

  return index < 0 ?
    value.toUIntPtr() : (static_cast<quintptr>(index) << 2 | INDEX);
}

KevesValue KevesFileIO::read(KevesGC* gc) {
  if (!file_.open(QIODevice::ReadOnly)) return EMB_FALSE;

  QDataStream in(&file_);

  QList<KevesLibrary> libs;
  
  in >> libs;
  displayLibraryName(libs.at(0));
  
  for (int i(1); i < libs.size(); ++i)
    displayLibraryName(libs.at(i));

  return readObjects(in, gc);
}

KevesValue KevesFileIO::readObjects(QDataStream& in, KevesGC* gc) {
  QList<Kev*> object_list;
  uioword value;

  while (!in.atEnd()) {
    in >> value;
    object_list.append((*ft_readObject_[value])(in, gc));
  }

  revertObjects(object_list);
  
  return object_list.at(0);
}

void KevesFileIO::revertObjects(const QList<Kev*>& object_list) {
  for (int i(0); i < object_list.size(); ++i) {
    MutableKevesValue kev(object_list.at(i));
    (*ft_revertObject_[kev.type()])(object_list, kev);
  }   
}

void KevesFileIO::RevertValue(const QList<Kev*>& object_list,
				KevesValue* value) {
  if (isIndex(*value)) *value = object_list.at(value->toUIntPtr() >> 2);
}
  
void KevesFileIO::displayLibraryName(const KevesLibrary& lib) {
  std::cout << "(";

  for (int i(0); i < lib.id.size(); ++i)
    std::cout << qPrintable(lib.id.at(i)) << " ";
    
  switch (lib.ver_num.size()) {
  case 0:
    std::cout << "()";
    break;

  case 1:
    std::cout << "(" << lib.ver_num.at(0) << ")";
    break;

  default:
    std::cout << "(" << lib.ver_num.at(0);

    for (int i(1); i < lib.ver_num.size(); ++i)
      std::cout << " " << lib.ver_num.at(i);

    std::cout << ")";
  }

  std::cout << ")" << std::endl;
    
}
