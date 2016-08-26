// Copyright (c) 2011-2016 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <limits>
#include "Common/MemoryInputStream.h"
#include "Common/StringTools.h"
#include "Common/VectorOutputStream.h"
#include "Serialization/BinaryOutputStreamSerializer.h"
#include "Serialization/BinaryInputStreamSerializer.h"
#include "CryptoNoteSerialization.h"


namespace CryptoNote {

void getBinaryArrayHash(const BinaryArray& binaryArray, Crypto::Hash& hash);
Crypto::Hash getBinaryArrayHash(const BinaryArray& binaryArray);

// noexcept
template<class T>
bool toBinaryArray(const T& object, BinaryArray& binaryArray) {
  try {
    binaryArray = toBinaryArray(object);
  } catch (std::exception&) {
    return false;
  }

  return true;
}

template<>
bool toBinaryArray(const BinaryArray& object, BinaryArray& binaryArray); 

// throws exception if serialization failed
template<class T>
BinaryArray toBinaryArray(const T& object) {
  BinaryArray ba;
  ::Common::VectorOutputStream stream(ba);
  BinaryOutputStreamSerializer serializer(stream);
  serialize(const_cast<T&>(object), serializer);
  return ba;
}

template<class T>
T fromBinaryArray(const BinaryArray& binaryArray) {
  T object;
  Common::MemoryInputStream stream(binaryArray.data(), binaryArray.size());
  BinaryInputStreamSerializer serializer(stream);
  serialize(object, serializer);
  if (!stream.endOfStream()) { // check that all data was consumed
    throw std::runtime_error("failed to unpack type");
  }

  return object;
}

template<class T>
bool fromBinaryArray(T& object, const BinaryArray& binaryArray) {
  try {
    object = fromBinaryArray<T>(binaryArray);
  } catch (std::exception&) {
    return false;
  }

  return true;
}

template<class T>
bool getObjectBinarySize(const T& object, size_t& size) {
  BinaryArray ba;
  if (!toBinaryArray(object, ba)) {
    size = (std::numeric_limits<size_t>::max)();
    return false;
  }

  size = ba.size();
  return true;
}

template<class T>
size_t getObjectBinarySize(const T& object) {
  size_t size;
  getObjectBinarySize(object, size);
  return size;
}

template<class T>
bool getObjectHash(const T& object, Crypto::Hash& hash) {
  BinaryArray ba;
  if (!toBinaryArray(object, ba)) {
    hash = NULL_HASH;
    return false;
  }

  hash = getBinaryArrayHash(ba);
  return true;
}

template<class T>
bool getObjectHash(const T& object, Crypto::Hash& hash, size_t& size) {
  BinaryArray ba;
  if (!toBinaryArray(object, ba)) {
    hash = NULL_HASH;
    size = (std::numeric_limits<size_t>::max)();
    return false;
  }

  size = ba.size();
  hash = getBinaryArrayHash(ba);
  return true;
}

template<class T>
Crypto::Hash getObjectHash(const T& object) {
  Crypto::Hash hash;
  getObjectHash(object, hash);
  return hash;
}

uint64_t getInputAmount(const Transaction& transaction);
std::vector<uint64_t> getInputsAmounts(const Transaction& transaction);
uint64_t getOutputAmount(const Transaction& transaction);
void decomposeAmount(uint64_t amount, uint64_t dustThreshold, std::vector<uint64_t>& decomposedAmounts);
}
