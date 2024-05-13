/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "identity.h"

template <class PrivateKeyImpl, class PublicKeyImpl>
dotsig::Identity<PrivateKeyImpl, PublicKeyImpl>::Identity(
  const Identity<PrivateKeyImpl, PublicKeyImpl>& other
) : dotsig::IIdentity() {
  m_private_key = std::make_unique<PrivateKeyImpl>(
    (other.m_private_key)->algorithm_identifier(),
    (other.m_private_key)->private_key_bits()
  );

  m_public_key = std::make_unique<PublicKeyImpl>(
    m_private_key->algorithm_identifier(),
    m_private_key->public_key_bits()
  );
}