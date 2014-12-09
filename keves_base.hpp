// keves/keves_base_kev.hpp - base of Keves
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

#include <QList>
#include <QMutex>
#include <QStack>
#include <QThreadPool>
#include "keves_builtin_values.hpp"
#include "keves_list.hpp"
#include "keves_textual_port.hpp"
#include "value/instruct.hpp"


class KevesLibrary;
class KevesVM;
class QString;


class KevesBase {
public:
  KevesBase();
  KevesBase(const KevesBase&) = delete;
  KevesBase(const KevesBase&&) = delete;
  KevesBase& operator=(const KevesBase&) = delete;
  KevesBase& operator=(const KevesBase&&) = delete;
  ~KevesBase();

  ////////////////////////////////////////////////////////////////
  // General functions                                          //
  ////////////////////////////////////////////////////////////////
public:
  void AddLibrary(KevesLibrary* library);

  KevesLibrary* GetLibrary(const QStringList& id,
			   const QList<ver_num_t>& ver_num);

  const StringKev* GetMesgText(const QString& key) const;
  void RunThread();

  template<class KEV>
  KEV* ToMutable(const KEV* kev);

  QString ToString(KevesValue value) const;

  KevesBuiltinValues* builtin() {
    return &builtin_;
  }
  
  vm_func* cmd_table() {
    return cmd_table_;
  }
  
  KevesTextualOutputPort* default_result_field() {
    return &default_result_field_;
  }
  
  const KevesInstructTable* instruct_table() const {
    return &instruct_table_;
  }

  static bool Match(const QStringList& list1, const QStringList& list2);

private:
  void InitCMDTable();
  void InitLibraryList();
  void ToString_code(QString*, KevesValue, int) const;
  void ToString_element(QString*, KevesValue) const;
  void ToString_list(QString*, KevesValue, int) const;
  void ToString_vector(QString*, KevesValue, int) const;


  ////////////////////////////////////////////////////////////////
  // for indexing of address                                    //
  ////////////////////////////////////////////////////////////////
public:
  enum { ALIGN = 0x3, INDEX = 0x2 };

  void AppendObjectList(QList<const Kev*>* list, const Kev* kev);

  static uioword IndexAddress(const QList<const Kev*>& object_list,
			      KevesValue value);

  template<class KEV>
  static void ReadArray(QDataStream& in, KEV* kev);

  template<class KEV>
  static void RevertArray(const QList<const Kev*>& object_list, KEV* kev);
  
  void RevertObjects(const QList<const Kev*>& object_list, int start_pos);

  template<class KEV>
  static void RevertValue(const QList<const Kev*>& object_list, KEV** kev);
  
  static void RevertValue(const QList<const Kev*>& object_list, KevesValue* value);

  template<class KEV>
  static void PushArray(QStack<const Kev*>* pending, KEV* kev);

  template<class KEV>
  static void PushValue(QStack<const Kev*>* pending, KEV* kev);
  
  static void PushValue(QStack<const Kev*>* pending, KevesValue value);

  template<class KEV>
  static void WriteArray(const QList<const Kev*>& list, QDataStream& out, KEV* kev);

  Kev* (*ft_ReadObject(uioword value))(QDataStream&, KevesBase*) {
    return ft_ReadObject_[value];
  }
  
  void (*ft_WriteObject(kev_type type))(const QList<const Kev*>&, QDataStream&, KevesValue) {
    return ft_WriteObject_[type];
  }
  
private:
  static bool IsIndex(Kev* kev) {
    return IsIndex(KevesValue(kev));
  }

  static bool IsIndex(KevesValue value) {
    return (value.toUIntPtr() & ALIGN) == INDEX; // 10
  }
  
  template<class KEV>
  void SetFunctionTable();
  

  ////////////////////////////////////////////////////////////////
  // Allocate and Make Objects                                  //
  ////////////////////////////////////////////////////////////////
public:
  template<class CTOR>
  auto Make(CTOR ctor, size_t size) -> decltype(ctor(nullptr)) {
    void* ptr(Alloc(size));
    decltype(ctor(nullptr)) temp(ctor(ptr));
    PushToSharedList(temp);
    return temp;
  }

  KevesList<KevesNode<0> >* shared_list() {
    return &shared_list_;
  }

private:
  void* Alloc(size_t alloc_size);
  void PushToSharedList(MutableKev* kev);


  ////////////////////////////////////////////////////////////////
  // variables                                                  //
  ////////////////////////////////////////////////////////////////
private:
  KevesInstructTable instruct_table_;
  vm_func cmd_table_[END_OF_LIST];
  KevesBuiltinValues builtin_;
  KevesTextualOutputPort default_result_field_;
  QThreadPool thread_pool_;
  QList<KevesLibrary*> library_list_;
  QList<QPair<QStringList, QString> > library_name_list_;
  KevesList<KevesNode<0> > shared_list_;
  void (*ft_PushChildren_[0177])(QStack<const Kev*>*, KevesValue);
  void (*ft_RevertObject_[0177])(const QList<const Kev*>&, MutableKevesValue);
  Kev* (*ft_ReadObject_[0177])(QDataStream&, KevesBase*);
  void (*ft_WriteObject_[0177])(const QList<const Kev*>&, QDataStream&, KevesValue);
  QMutex mutex_;
};
