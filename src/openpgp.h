/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_OPENPGP_H__
#define __DOTSIG_OPENPGP_H__

#include <botan/rsa.h> // RSA_PrivateKey, RSA_PublicKey
#include "identity.h"

namespace dotsig {

/// \brief Namespace that contains the OpenPGP identities class template.
namespace OpenPGP {

  /// \brief Type for OpenPGP private keys. Mostly added for augmented code clarity.
  typedef Botan::RSA_PrivateKey PrivateKey;

  /// \brief Type for OpenPGP public keys. Mostly added for augmented code clarity.
  typedef Botan::RSA_PublicKey  PublicKey;

  /// \brief Type that specializes the \see dotsig::Identity template class and
  ///        which is used as the parent class for OpenPGP identities.
  typedef dotsig::Identity<PrivateKey, PublicKey> ParentType;

  /// \brief Class template for OpenPGP (RSA) identities that consist of a keypair.
  ///
  /// This class template specializes the ParentType with Botan::RSA_PrivateKey
  /// and Botan::RSA_PublicKey. This implementation can be used to generate,
  /// import and/or export OpenPGP (RSA) keys, as well as to sign messages or
  /// files using the OpenPGP standard for digital signatures.
  class Identity final
    : public ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty OpenPGP identity.
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    Identity() : ParentType() {}

    /// \brief Copy constructor. Creates an identity based on the other's RSA private key.
    Identity(const Identity& o) : ParentType(o) {}

    /// \brief Class destructor which uses the opportunity for zeroing keys.
    virtual ~Identity();

    /// \brief Generates a random pair of RSA private- and public-key.
    /// \see Import
    /// \see Export
    void GenerateRandom() override;

    /// \brief Creates an identity with file \a filename and pass \a passphrase.
    /// \param filename The complete filesystem path to an encrypted identity file.
    /// \param passphrase A passphrase to decrypt the encrypted identity file.
    /// \return *this
    /// \see GenerateRandom
    /// \see Export
    const ParentType& Import(
      const std::string&,
      const std::string&
    ) override;

    /// \brief Saves the private key to file \a filename with password \a passphrase.
    /// \note A public key file will also be created at {filename}.pub.
    /// \param filename The filesystem path where the identity file will be stored.
    /// \param passphrase A passphrase to encrypt the identity file.
    /// \see GenerateRandom
    /// \see Import
    void Export(const std::string&, const std::string&) const override;

    /// \brief Signs a message \a message and saves the signature to \a sig_file.
    /// \param message The complete message for which a digital signature is created.
    /// \param sig_file The filesystem path where the signature file will be stored.
    /// \see Verify
    std::string Sign(const std::string&, const std::string&) const override;

    /// \brief Verifies a signature \a signature for a message \a message.
    /// \param signature The signature bytes, as stored in a signature file (not hex!).
    /// \param message The complete message for which a digital signature is verified.
    /// \see Sign
    bool Verify(const std::string&, const std::string&) const override;
  };

} // namespace OpenPGP

} // namespace dotsig

#endif