/* Keves/keves_file_io.hpp - file handler for Keves
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

#pragma once

#include <QFile>
#include <QList>
#include <QStack>
#include <QStringList>
#include "keves_iterator.hpp"
#include "keves_value.hpp"


class KevesGC;
class KevesLibrary;

class KevesFileIO {
  enum {
    ALIGN = 0x3,
    INDEX = 0x2
  };
  
public:
  KevesFileIO() = delete;
  KevesFileIO(const QString&);
  KevesFileIO(const KevesFileIO&) = delete;
  KevesFileIO(const KevesFileIO&&) = delete;
  KevesFileIO& operator=(const KevesFileIO&) = delete;
  KevesFileIO& operator=(const KevesFileIO&&) = delete;
  ~KevesFileIO() = default;

  KevesValue read(KevesGC* gc);
  void revertObjects(const QList<Kev*>& object_list);
  bool write(const QList<KevesLibrary>& libs, KevesValue value);

  template<class KEV>
  static KEV* fromUioword(uioword value);

  template<class KEV>
  static void PushArray(QStack<const Kev*>* pending, KEV* kev);

  template<class KEV>
  static void PushValue(QStack<const Kev*>* pending, KEV* kev);
  
  static void PushValue(QStack<const Kev*>* pending, KevesValue value);

  template<class KEV>
  static void ReadArray(QDataStream& in, KEV* kev);

  template<class KEV>
  static void RevertArray(const QList<Kev*>& object_list, KEV* kev);
  
  template<class KEV>
  static void RevertValue(const QList<Kev*>& object_list, KEV** kev);
  
  static void RevertValue(const QList<Kev*>& object_list, KevesValue* value);

  static uioword WrapAddress(const QList<const Kev*>& object_list,
			     KevesValue value);

  template<class KEV>
  static void WriteArray(const QList<const Kev*>& list, QDataStream& out,
			 KEV* kev);

private:
  const QList<const Kev*> getObjectList(const Kev* kev);
  KevesValue readObjects(QDataStream& in, KevesGC* gc);

  static void displayLibraryName(const KevesLibrary& lib);

  static bool isIndex(Kev* kev) {
    return isIndex(KevesValue(kev));
  }

  static bool isIndex(KevesValue value) {
    return (value.toUIntPtr() & ALIGN) == INDEX; // 10
  }
  
  QFile file_;
  void (*ft_pushChildren_[0177])(QStack<const Kev*>*, KevesValue);
  void (*ft_writeObject_[0177])(const QList<const Kev*>&, QDataStream&, KevesValue);
  Kev* (*ft_readObject_[0177])(QDataStream&, KevesGC*);
  void (*ft_revertObject_[0177])(const QList<Kev*>&, MutableKevesValue);
};
