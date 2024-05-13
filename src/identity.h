/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_IDENTITY_H__
#define __DOTSIG_IDENTITY_H__

#include <memory> // std::unique_ptr
#include <string> // std::string

namespace dotsig {

  /// \brief Interface for identities that consist of a private/public keypair.
  ///
  /// This interface declares all the methods that *must* be implemented in any
  /// identities class template specialization.
  ///
  /// Method overrides must be provided by child classes.
  ///
  /// \see dotsig::IIdentity::GenerateRandom
  /// \see dotsig::IIdentity::Import
  /// \see dotsig::IIdentity::Export
  /// \see dotsig::IIdentity::Sign
  /// \see dotsig::IIdentity::Verify
  class IIdentity {
  public:
    IIdentity() {}
    virtual ~IIdentity() = default;

    /// \brief Generates a random pair of private- and public-key.
    virtual void GenerateRandom() = 0;

    /// \brief Creates an identity with file \a filename and pass \a passphrase.
    virtual const IIdentity& Import(const std::string&, const std::string&) = 0;

    /// \brief Saves the private key to file \a filename with password \a passphrase.
    virtual void Export(const std::string&, const std::string&) const = 0;

    /// \brief Signs a message \a message and saves the signature to \a sig_file.
    virtual std::string Sign(const std::string&, const std::string&) const = 0;

    /// \brief Verifies a signature \a signature for a message \a message.
    virtual bool Verify(const std::string&, const std::string&) const = 0;
  };

  /// \brief Template class for identities that consist of a private/public keypair.
  ///
  /// This template class accepts two template parameters with PrivateKeyImpl
  /// and PublicKeyImpl. Importantly, the current implementation of the copy
  /// constructor makes it so that these template parameters must be compatible
  /// with Botan's pk_keys.h format. Method implementations must be provided
  /// by these classes for the methods: algorithm_identifier, private_key_bits
  /// and public_key_bits, as found in Botan::Private_Key and Botan::Public_Key.
  ///
  /// \tparam PrivateKeyImpl The private key implementation class.
  /// \tparam PublicKeyImpl  The public key implementation class.
  /// \see dotsig::IIdentity
  template <class PrivateKeyImpl, class PublicKeyImpl>
  class Identity : public IIdentity
  {
  protected:
    std::unique_ptr<PrivateKeyImpl> m_private_key;

  public:
    std::unique_ptr<PublicKeyImpl>  m_public_key;

    /// \brief Default constructor. Creates an empty identity.
    /// \see GenerateRandom
    /// \see Import
    Identity() : IIdentity() {}

    /// \brief Copy constructor. Creates an identity based on the other's private key.
    Identity(const Identity&);

    /// \brief Default class destructor.
    virtual ~Identity() = default;

    /// \brief Generates a random pair of private- and public-key.
    virtual void GenerateRandom() override = 0;

    /// \brief Creates an identity with file \a filename and pass \a passphrase.
    virtual const Identity<PrivateKeyImpl, PublicKeyImpl>& Import(
      const std::string&,
      const std::string&
    ) override = 0;

    /// \brief Saves the private key to file \a filename with password \a passphrase.
    virtual void Export(
      const std::string&,
      const std::string&
    ) const override = 0;

    /// \brief Signs a message \a message and saves the signature to \a sig_file.
    virtual std::string Sign(
      const std::string&,
      const std::string&
    ) const override = 0;

    /// \brief Verifies a signature \a signature for a message \a message.
    virtual bool Verify(
      const std::string&,
      const std::string&
    ) const override = 0;
  };

}

#endif