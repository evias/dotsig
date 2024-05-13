/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "factory.h"
#include "functions.h" // dotsig::strtolower
#include "ecdsa.h" // dotsig::ECDSA::Identity
#include "pkcs.h" // dotsig::PKCS::Identity
#include "openpgp.h" // dotsig::OpenPGP::Identity

bool dotsig::Factory::Register(
  const std::string& id,
  std::function<IIdentity* ()> maker_fn
) {
  return m_factories.insert(std::make_pair(id, maker_fn)).second;
}

dotsig::IIdentity* dotsig::Factory::MakeIdentity(
  const std::string& id
) {
  auto find_it = m_factories.find(id);
  if (m_factories.end() != find_it)
    return (find_it->second)();

  return 0;
}

void dotsig::InitializeFactory(dotsig::Factory* factory) {
  // enables ECDSA standard
  factory->Register("ecdsa", [] {
    return new dotsig::ECDSA::Identity();
  });

  // enables PKCS standard
  factory->Register("pkcs", [] {
    return new dotsig::PKCS::Identity();
  });

  // enables OpenPGP standard with default RSA (PKCS1 v1.5) signature scheme
  factory->Register("openpgp", [] {
    return new dotsig::OpenPGP::RSA_Identity();
  });

  // enables OpenPGP standard with RSA (PKCS1 v1.5) signature scheme
  factory->Register("openpgp:rsa", [] {
    return new dotsig::OpenPGP::RSA_Identity();
  });

  // enables OpenPGP standard with DSA signature scheme
  factory->Register("openpgp:dsa", [] {
    return new dotsig::OpenPGP::DSA_Identity();
  });

  // enables OpenPGP standard with ECDSA signature scheme
  factory->Register("openpgp:ecdsa", [] {
    return new dotsig::OpenPGP::ECDSA_Identity();
  });

  // enables OpenPGP standard with EdDSA (Ed25519) signature scheme
  factory->Register("openpgp:eddsa", [] {
    return new dotsig::OpenPGP::EdDSA_Identity();
  });
}