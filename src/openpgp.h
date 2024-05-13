/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_OPENPGP_H__
#define __DOTSIG_OPENPGP_H__

#include <vector>
#include <botan/dsa.h> // DSA_PrivateKey, DSA_PublicKey
#include "identity.h" // dotsig::IIdentity
#include "ecdsa.h" // dotsig::ECDSA
#include "pkcs.h" // dotsig::PKCS

namespace dotsig {

  // OpenPGP DSA
  typedef Botan::DSA_PrivateKey OpenPGP_DSA_PrivateKey;
  typedef Botan::DSA_PublicKey  OpenPGP_DSA_PublicKey;

  // OpenPGP RSA with PKCS1 v1.5
  typedef dotsig::PKCS::PrivateKey OpenPGP_RSA_PrivateKey;
  typedef dotsig::PKCS::PublicKey  OpenPGP_RSA_PublicKey;

  // OpenPGP ECDSA
  typedef dotsig::ECDSA::PrivateKey OpenPGP_ECDSA_PrivateKey;
  typedef dotsig::ECDSA::PublicKey  OpenPGP_ECDSA_PublicKey;

  // OpenPGP EdDSA
  typedef dotsig::ECDSA::PrivateKey OpenPGP_EdDSA_PrivateKey;
  typedef dotsig::ECDSA::PublicKey  OpenPGP_EdDSA_PublicKey;

/// \brief Namespace that contains the OpenPGP identities class template.
namespace OpenPGP {

  /// \brief Template class for OpenPGP identities that consist of a private/public
  ///        keypair which may be generated using one of: RSA, DSA, ECDSA and EdDSA.
  ///
  /// This template class accepts three template parameters with PrivateKeyImpl,
  /// PublicKeyImpl and SubKeyImpl. Importantly, the current implementation of
  /// the copy constructor makes it so that these template parameters must be
  /// compatible with Botan's pk_keys.h format. Method implementations must be
  /// provided by these classes for the methods: algorithm_identifier, private_key_bits
  /// and public_key_bits, as found in Botan::Private_Key and Botan::Public_Key.
  ///
  /// \note The OpenPGP standard defines compatibility with multiple digital
  /// signature algorithms including: RSA (PKCS1 v1.5) [default], DSA, ECDSA, EdDSA).
  ///
  /// \note Also, the OpenPGP standard is different from basic ECDSA and PKCS
  /// implementations because it uses a concept of armored messages and keys.
  /// The actual raw signatures produced using OpenPGP identities *do not differ*
  /// from signatures created using the PKCS::Identity and ECDSA::Identity
  /// classes.
  ///
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_openpgp` and it exports PEM-encoded public
  /// keys to the user's home folder, in a file named `id_openpgp.pub`.
  ///
  /// \todo Implement expiration logic and signature/keys revocation strategies.
  /// \todo Diverge from original DSA implementations by usage of **subkeys**.
  ///
  /// \see dotsig::IIdentity
  template <
    class PrivateKeyImpl,
    class PublicKeyImpl,
    class SubKeyImpl = PublicKeyImpl
  >
  class Identity
    : public IIdentity
  {
  protected:
    typedef std::unique_ptr<SubKeyImpl> SubKeyUniquePtr;
    std::unique_ptr<PrivateKeyImpl> m_private_key;

  public:
    std::unique_ptr<PublicKeyImpl>  m_public_key;

    /// \brief Contains a list of subkeys as defined with OpenPGP.
    /// \note The subkeys functionality is not yet implemented.
    //std::vector<SubKeyUniquePtr>    m_sub_keys;

    /// \brief Contains a *padding scheme with hash function* in the case of
    ///        OpenPGP with RSA (PKCS1 v1.5), otherwise contains a *hash function*.
    /// \example "PKCS1v15(SHA-256)" // padding scheme and hash function
    /// \example "SHA-256" // hash function only (used in OpenPGP::ECDSA_Identity)
    /// \example "SHA-512" // hash function only (used in OpenPGP::EdDSA_Identity)
    std::string                     m_scheme;

    /// \brief Default constructor. Creates an empty OpenPGP identity.
    ///
    /// This method accepts a string-typed \a scheme that further defines the
    /// algorithm used during signatures creation and verification. A parameter
    /// is necessary here because OpenPGP provides multiple compatible DSA types.
    ///
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    ///
    /// \param scheme The padding scheme with hash function OR only a hash function.
    /// \see m_scheme
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    Identity(const std::string& scheme = "PKCS1v15(SHA-256)")
      : IIdentity(), m_scheme(scheme)/*, m_sub_keys({})*/ {}

    /// \brief Copy constructor. Creates an identity based on the other's private key.
    Identity(const Identity&);

    /// \brief Default class destructor.
    virtual ~Identity() = default;

    /// \brief Creates an identity with file \a filename and pass \a passphrase.
    /// \param filename The complete filesystem path to an encrypted identity file.
    /// \param passphrase A passphrase to decrypt the encrypted identity file.
    /// \return *this
    /// \see GenerateRandom
    /// \see Export
    const Identity<PrivateKeyImpl, PublicKeyImpl, SubKeyImpl>& Import(
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

    /// \brief Generates a random pair of private- and public-key.
    virtual void GenerateRandom() override = 0;
  };

  /// \brief Class template specialization for OpenPGP DSA identities.
  /// \see dotsig::OpenPGP::Identity
  /// \see dotsig::OpenPGP::DSA_Identity
  typedef dotsig::OpenPGP::Identity<
    OpenPGP_DSA_PrivateKey,
    OpenPGP_DSA_PublicKey
  >                               OpenPGP_DSA_ParentType;

  /// \brief Class for OpenPGP identities using type DSA.
  ///
  /// This class template specializes the OpenPGP Identity template class with
  /// Botan::DSA_PrivateKey and Botan::DSA_PublicKey.
  ///
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_openpgp.dsa` and it exports PEM-encoded public
  /// keys to the user's home folder, in a file named `id_openpgp.dsa.pub`.
  class DSA_Identity final
    : public OpenPGP_DSA_ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty OpenPGP DSA identity.
    ///
    /// This class template specifies *only a hash function*, with the resulting
    /// parameters specification being: `SHA-256`.
    ///
    /// \note The DSA implementation requires only a hash function to be specified.
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    DSA_Identity() : OpenPGP_DSA_ParentType("SHA-256") {}

    /// \brief Copy constructor. Creates an identity based on the other's RSA private key.
    DSA_Identity(const DSA_Identity& o) : OpenPGP_DSA_ParentType(o) {}

    /// \brief Class destructor which uses the opportunity for zeroing keys.
    virtual ~DSA_Identity();

    /// \brief Generates a random pair of OpenPGP DSA private- and public-key.
    /// \see Import
    /// \see Export
    void GenerateRandom() override;
  };


  /// \brief Class template specialization for OpenPGP ECDSA identities.
  /// \see dotsig::OpenPGP::Identity
  /// \see dotsig::OpenPGP::ECDSA_Identity
  typedef dotsig::OpenPGP::Identity<
    OpenPGP_ECDSA_PrivateKey,
    OpenPGP_ECDSA_PublicKey
  >                               OpenPGP_ECDSA_ParentType;

  /// \brief Class for OpenPGP identities using type ECDSA.
  ///
  /// This class template specializes the OpenPGP Identity template class with
  /// Botan::ECDSA_PrivateKey and Botan::ECDSA_PublicKey.
  ///
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_openpgp.ecdsa` and it exports PEM-encoded
  /// public keys to the user's home folder, in a file named `id_openpgp.ecdsa.pub`.
  class ECDSA_Identity final
    : public OpenPGP_ECDSA_ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty OpenPGP ECDSA identity.
    ///
    /// This class template specifies *only a hash function*, with the resulting
    /// parameters specification being: `SHA-256`.
    ///
    /// \note The ECDSA implementation requires only a hash function to be specified.
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    ECDSA_Identity() : OpenPGP_ECDSA_ParentType("SHA-256") {}

    /// \brief Copy constructor. Creates an identity based on the other's RSA private key.
    ECDSA_Identity(const ECDSA_Identity& o) : OpenPGP_ECDSA_ParentType(o) {}

    /// \brief Class destructor which uses the opportunity for zeroing keys.
    virtual ~ECDSA_Identity();

    /// \brief Generates a random pair of OpenPGP ECDSA private- and public-key.
    /// \see Import
    /// \see Export
    void GenerateRandom() override;
  };

  /// \brief Class template specialization for OpenPGP EdDSA (Ed25519) identities.
  /// \see dotsig::OpenPGP::Identity
  /// \see dotsig::OpenPGP::EdDSA_Identity
  typedef dotsig::OpenPGP::Identity<
    OpenPGP_ECDSA_PrivateKey,
    OpenPGP_ECDSA_PublicKey
  >                               OpenPGP_EdDSA_ParentType;

  /// \brief Class for OpenPGP identities using type EdDSA (Ed25519).
  ///
  /// This class template specializes the OpenPGP Identity template class with
  /// Botan::ECDSA_PrivateKey and Botan::ECDSA_PublicKey.
  /// Also, this class template defines a `GnuPG`-compatible Ed25519 variant of
  /// the signature scheme, instead of using the pre-hashed variants which are
  /// recommended by Botan. Using the GnuPG variant is obviously for compatiblity
  /// with the OpenPGP standard in this namespace.
  ///
  /// \note Signatures are **not compatible** with `Ed25519ph` signature scheme.
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_openpgp.eddsa` and it exports PEM-encoded
  /// public keys to the user's home folder, in a file named `id_openpgp.eddsa.pub`.
  class EdDSA_Identity final
    : public OpenPGP_EdDSA_ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty OpenPGP EdDSA (Ed25519) identity.
    ///
    /// This class template specifies *only a hash function*, with the resulting
    /// parameters specification being: `SHA-512`. This implies the use of a
    /// `GnuPG`-compatible Ed25519 variant of the signature scheme, instead of
    /// using the pre-hashed variants which are recommended. Using the GnuPG
    /// variant is obviously for compatiblity with the OpenPGP standard.
    ///
    /// \note Signatures are **not compatible** with `Ed25519ph` signature scheme.
    /// \note The EdDSA (Ed25519) implementation only a hash function to be specified.
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    EdDSA_Identity() : OpenPGP_EdDSA_ParentType("SHA-512") {}

    /// \brief Copy constructor. Creates an identity based on the other's RSA private key.
    EdDSA_Identity(const EdDSA_Identity& o) : OpenPGP_EdDSA_ParentType(o) {}

    /// \brief Class destructor which uses the opportunity for zeroing keys.
    virtual ~EdDSA_Identity();

    /// \brief Generates a random pair of OpenPGP ECDSA private- and public-key.
    /// \see Import
    /// \see Export
    void GenerateRandom() override;
  };

  /// \brief Class template specialization for OpenPGP RSA (PKCS1 v1.5) identities.
  /// \see dotsig::OpenPGP::Identity
  /// \see dotsig::OpenPGP::RSA_Identity
  typedef dotsig::OpenPGP::Identity<
    OpenPGP_RSA_PrivateKey,
    OpenPGP_RSA_PublicKey
  >                               OpenPGP_RSA_ParentType;

  /// \brief Class for OpenPGP identities using type RSA (PKCS1 v1.5).
  ///
  /// This class template specializes the OpenPGP Identity template class with
  /// Botan::RSA_PrivateKey and Botan::RSA_PublicKey.
  ///
  /// \note This identity wrapper exports BER-encoded private keys to the user's
  /// home folder, in a file named `id_openpgp` and it exports PEM-encoded public
  /// keys to the user's home folder, in a file named `id_openpgp.pub`.
  class RSA_Identity final
    : public OpenPGP_RSA_ParentType
  {
  public:
    /// \brief Default constructor. Creates an empty OpenPGP RSA (PKCS1 v1.5) identity.
    ///
    /// This class template specifies *a padding scheme and a hash function*,
    /// with the resulting parameters specification being: `PKCS1v15(SHA-256)`.
    ///
    /// \note The RSA implementation requires both a padding scheme and hash function.
    /// \note The created identity does not have a private key, make sure to
    ///       use one of GenerateRandom or Import to populate it.
    /// \see GenerateRandom
    /// \see Import
    /// \see Export
    RSA_Identity() : OpenPGP_RSA_ParentType("PKCS1v15(SHA-256)") {}

    /// \brief Copy constructor. Creates an identity based on the other's RSA private key.
    RSA_Identity(const RSA_Identity& o) : OpenPGP_RSA_ParentType(o) {}

    /// \brief Class destructor which uses the opportunity for zeroing keys.
    virtual ~RSA_Identity();

    /// \brief Generates a random pair of OpenPGP RSA private- and public-key.
    /// \see Import
    /// \see Export
    void GenerateRandom() override;
  };

} // namespace OpenPGP

} // namespace dotsig

#endif