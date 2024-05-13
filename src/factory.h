/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_FACTORY_H__
#define __DOTSIG_FACTORY_H__

#include <string> // std::string
#include <map> // std::map
#include <functional> // std::function
#include "identity.h" // dotsig::IIdentity

namespace dotsig {

  /// \brief A class that implements the factory pattern for identities.
  ///
  /// This (minimal) factory pattern implementation deals with IIdentity objects
  /// and a uses a **pointer creation function** to create pointers of Identity
  /// classes that are supported by the software. These creator functions may or
  /// may not prepare the object before it is returned, as deemed necessary.
  class Factory {
    /// \brief The factories registry as a map.
    ///
    /// Keys are supported algorithm names, e.g. "ecdsa", "pkcs" or "openpgp",
    /// and values are *function pointers* that shall execute the pointer creation
    /// for the provided class templates based on IIdentity.
    /// \see Register
    std::map<std::string, std::function<IIdentity* ()>> m_factories{};

  public:
    /// \brief Registers a new factory for an IIdentity derived class.
    /// \param id The algorithm name used in the association.
    /// \param maker_fn The *pointer creation function* which returns an IIdentity.
    /// \return True if the insertion of a new factory succeeded, false otherwise.
    bool Register(
      const std::string& id,
      std::function<IIdentity* ()> maker_fn
    );

    /// \brief Creates a new identity for algorithm \a id.
    ///
    /// This method uses the factories registry to find the associated pointer
    /// creation function using the provided \a id, and calls the function to
    /// create the IIdentity instance that is returned.
    /// \param id The algorithm name used in the association.
    /// \return The created IIdentity-derived class object or 0 to mark an error.
    IIdentity* MakeIdentity(const std::string& id);
  };

  /// \brief Initializes the factory by registering supported algorithm class templates.
  /// \param factory The factory class object. This object's registry will be modified.
  void InitializeFactory(Factory*);

}

#endif