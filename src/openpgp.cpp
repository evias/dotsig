/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "openpgp.h"
#include <botan/auto_rng.h> // AutoSeeded_RNG
#include <botan/pkcs8.h> // PKCS8::PEM_encode
#include <botan/x509_key.h> // X509::PEM_encode
#include <botan/pubkey.h> // PK_Signer
#include <botan/hex.h> // hex_encode
#include <fstream> // std::ofstream
#include <filesystem> // std::filesystem
#include <stdexcept> // std::runtime_error

dotsig::OpenPGP::Identity::~Identity() {
  // take-over ownership
  dotsig::OpenPGP::PrivateKey* priv = m_private_key.release();
  dotsig::OpenPGP::PublicKey*   pub = m_public_key.release();

  // zeroing key memory space
  std::memset(reinterpret_cast<void*>(&priv), 0, sizeof(priv));
  std::memset(reinterpret_cast<void*>(&pub), 0, sizeof(pub));

  // pointer deletion
  delete priv;
  delete pub;
}

void dotsig::OpenPGP::Identity::GenerateRandom() {
  Botan::AutoSeeded_RNG rng;

  m_private_key = std::make_unique<dotsig::OpenPGP::PrivateKey>(
    rng, 2048
  );

  m_public_key  = std::make_unique<dotsig::OpenPGP::PublicKey>(
    m_private_key->algorithm_identifier(),
    m_private_key->public_key_bits()
  );
}

const dotsig::OpenPGP::ParentType& dotsig::OpenPGP::Identity::Import(
  const std::string& filename,
  const std::string& passphrase
) {
  std::string err_string;

  // try to import an unencrypted public-key (from: id_rsa.pub, id_ecdsa.pub)
  try {
    Botan::DataSource_Stream input(filename); // non-binary mode (PEM)
    std::unique_ptr<Botan::Public_Key> pub = Botan::X509::load_key(input);

    m_public_key = std::make_unique<dotsig::OpenPGP::PublicKey>(
      pub->algorithm_identifier(),
      pub->public_key_bits()
    );

    return *this;
  }
  catch(Botan::Exception& e) {}

  // or try to load using an encrypted private key (from: id_rsa, id_ecdsa)
  try {
    Botan::DataSource_Stream input(filename, true); // binary mode (BER)
    std::unique_ptr<Botan::Private_Key> priv = Botan::PKCS8::load_key(
      input,
      passphrase
    );

    m_private_key = std::make_unique<dotsig::OpenPGP::PrivateKey>(
      priv->algorithm_identifier(), priv->private_key_bits()
    );

    m_public_key = std::make_unique<dotsig::OpenPGP::PublicKey>(
      m_private_key->algorithm_identifier(),
      m_private_key->public_key_bits()
    );

    return *this;
  }
  catch(Botan::Exception& e) {
    err_string = e.what();
  }

  // private key could not be loaded
  throw std::runtime_error("Loading identity file failed (" + err_string + ")");
}

void dotsig::OpenPGP::Identity::Export(
  const std::string& filename,
  const std::string& passphrase
) const {
  std::filesystem::directory_entry entry{filename};
  if (entry.exists())
    throw std::runtime_error("Error: File overwrite not yet supported.");

  Botan::AutoSeeded_RNG rng;
  auto priv_bytes = Botan::PKCS8::BER_encode(*m_private_key, rng, passphrase);
  auto pub_bytes  = Botan::X509::PEM_encode(*m_public_key); // expects pubkey

  std::ofstream out_priv(filename),
                out_pub(filename + ".pub");

  out_priv << std::string(priv_bytes.begin(), priv_bytes.end()) << std::flush;
  out_pub  << pub_bytes << std::flush;

  out_priv.close();
  out_pub.close();
}

std::string dotsig::OpenPGP::Identity::Sign(
  const std::string& message,
  const std::string& sig_file
) const {
  Botan::AutoSeeded_RNG rng;

  // initialize a signer instance with the message (uses EMSA PKCS1 v1.5)
  Botan::PK_Signer signer(*m_private_key, rng, "PKCS1v15(SHA-256)");
  signer.update(message);
  std::vector<uint8_t> sig = signer.signature(rng);

  // saves the signature bytes into a .sig file
  std::ofstream sig_ptr(sig_file);
  sig_ptr << std::string(sig.begin(), sig.end());
  sig_ptr.close();

  // returns hexadecimal signature notation
  return Botan::hex_encode(sig);
}

bool dotsig::OpenPGP::Identity::Verify(
  const std::string& signature,
  const std::string& message
) const {
  // initialize a verifier instance with the message (uses EMSA PKCS1 v1.5)
  Botan::PK_Verifier verifier(*m_public_key, "PKCS1v15(SHA-256)");
  verifier.update(message);

  std::vector<uint8_t> raw_signature(signature.begin(), signature.end());
  return verifier.check_signature(raw_signature);
}