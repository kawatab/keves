// keves/keves_list.hpp - lists for Keves
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

#include "keves_iterator.hpp"
#include "keves_value.hpp"


struct KevesPrefix {
  KevesPrefix* node[2];
};
  

class KevesBaseNode {
public:
  KevesBaseNode() : prefix_() {}
  KevesBaseNode(const KevesBaseNode&) = default;
  KevesBaseNode(KevesBaseNode&&) = default;
  KevesBaseNode& operator=(const KevesBaseNode&) = default;
  KevesBaseNode& operator=(KevesBaseNode&&) = default;
  ~KevesBaseNode() = default;

  explicit KevesBaseNode(void* ptr)
    : prefix_(static_cast<KevesPrefix*>(ptr)) {}
  
  explicit KevesBaseNode(MutableKev* kev)
    : prefix_(reinterpret_cast<KevesPrefix*>(kev) - 1) {}
    
  void dispose() {
    delete [] reinterpret_cast<char*>(prefix_);
  }

  bool isEmpty() const {
    return !prefix_;
  }

  void* toPtr() const {
    return prefix_ + 1;
  }

  MutableKev* toKev() const {
    return static_cast<MutableKev*>(toPtr());
  }

  MutableKev* operator->() const {
    return static_cast<MutableKev*>(toPtr());
  }
  
protected:
  KevesPrefix* prefix_;
};
  

template<int SIZE>
class KevesNode : public KevesBaseNode {
public:
  KevesNode() = default;
  KevesNode(const KevesNode&) = default;
  KevesNode(KevesNode&&) = default;
  KevesNode& operator=(const KevesNode&) = default;
  KevesNode& operator=(KevesNode&&) = default;
  ~KevesNode() = default;
  
  explicit KevesNode(KevesBaseNode node)
    : KevesBaseNode(node) {}
  
  KevesNode getNext() const {
    return prefix_->node[SIZE];
  }
  
  void setNext(KevesBaseNode prev) const {
    prefix_->node[SIZE] = KevesNode(prev).prefix_;
  }
  
private:
  KevesNode(KevesPrefix* prefix)
    : KevesBaseNode(prefix) {}
};
  

template<class NODE>
class KevesList {
public:
  KevesList()
    : top_() {}
  
  KevesList(const KevesList&) = default;
  KevesList(KevesList&&) = default;
  KevesList& operator=(const KevesList&) = default;
  KevesList& operator=(KevesList&&) = default;
  ~KevesList() = default;
  
  void clear() {
    top_ = NODE();
  }

  bool isEmpty() const {
    return top_.isEmpty();
  }

  NODE top() const {
    return top_;
  }
  
  NODE Pop() {
    NODE temp(top_);
    top_ = temp.getNext();
    return temp;
  }
  
  void push(NODE node) {
    node.setNext(top_);
    top_ = node;
  }
  
  void push(KevesBaseNode base_node) {
    push(NODE(base_node));
  }

  void push(MutableKev* kev) {
    push(KevesBaseNode(kev));
  }
  
private:
  NODE top_;
};
