/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_ECDSA_H__
#define __DOTSIG_ECDSA_H__

#include <botan/ecdsa.h> // ECDSA_PrivateKey, ECDSA_PublicKey
#include "identity.h"

namespace dotsig {

/// \brief Namespace that contains the ECDSA identities class template.
namespace ECDSA {

  /// \brief Type for ECDSA private keys. Mostly added for augmented code clarity.
  typedef Botan::ECDSA_PrivateKey PrivateKey;

  /// \brief Type for ECDSA public keys. Mostly added for augmented code clarity.
  typedef Botan::ECDSA_PublicKey  PublicKey;

  /// \brief Type that specializes the dotsig::Identity template class and
  ///        which is used as the parent class for ECDSA identities.
  /// \see dotsig::Identity
  typedef dotsig::Identity<PrivateKey, PublicKey> ParentType;

  /// \brief Class for ECDSA identities that consist of a keypair.
  ///
  /// This class template specializes the ParentType with Botan::ECDSA_PrivateKey
  /// and Botan::ECDSA_PublicKey. This implementation can be used to generate,
  /// import and/or export ECDSA keys, as well as to sign messages or files
  /// digitally using the ECDSA standard for digital signatures.
  ///
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_ecdsa` and it exports PEM-encoded public
  /// keys to the user's home folder, in a file named `id_ecdsa.pub`.
  class Identity final
    : public ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty ECDSA identity.
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

    /// \brief Generates a random pair of ECDSA private- and public-key.
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

    /// \brief Saves an identity to file \a filename with password \a passphrase.
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

} // namespace ECDSA

} // namespace dotsig

#endif