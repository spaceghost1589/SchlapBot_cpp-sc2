/*! \file sc2_types.h
    \brief Common types used by this API.

Any basic types used throughout are found here.
*/

#pragma once

#include <type_traits>
#include <cstdint>
#include <string>

#include "sc2_5.0.14_typeenums.h"

namespace sc2 {

//! Template class for defining common game enum types.
//! The template parameter is the enum. This class allows for seamless conversion between enum types and integers,
//! while maintaining strong typing. This means, for example, that a unit type ID can be converted back and forth
//! from an integer, but can't be used when another type ID, e.g., an ability ID, is required as a parameter.
template <class T>
class SC2Type {
public:
    //! Default constructor.
    SC2Type() = default;
    //! Construct from an integer, corresponds to the enum value.
    SC2Type(uint32_t type_id) : type_id_(static_cast<uint16_t>(type_id)) {
    }
    //! Construct from the enum.
    SC2Type(T type_id) : type_id_(static_cast<uint16_t>(type_id)) {
    }

    //! Test equivalence.
    //!< \return 'true' if the values are equal.
    auto operator==(SC2Type type_id) const -> bool {
        return type_id_ == type_id.type_id_;
    }
    auto operator==(T type_id) const -> bool {
        return type_id_ == static_cast<uint16_t>(type_id);
    }
    auto operator==(uint16_t type_id) const -> bool {
        return type_id_ == type_id;
    }
    auto operator==(int type_id) const -> bool {
        return static_cast<uint16_t>(type_id_) == static_cast<uint16_t>(type_id);
    }

    //! Test comparison.
    auto operator<(const SC2Type<T>& other) const -> bool {
        return type_id_ < other.type_id_;
    }

    //! Cast to integer.
    operator uint16_t() const {
        return type_id_;
    }
    //! Cast to enum type.
    operator T() const {
        return static_cast<T>(type_id_);
    }

    //! Determines if the value contained is valid.
    //!< \return 'true' if the value is valid.
    [[nodiscard]] auto IsValid() const -> bool {
        return type_id_ != 0;
    }

    //! String of the integer value.
    //!< \return The string of the value.
    [[nodiscard]] auto to_string() const -> std::string {
        return std::to_string(type_id_);
    }

    //! Explicit conversion to the enum type.
    //!< \return The enum.
    auto ToType() const -> T {
        return static_cast<T>(type_id_);
    }

    auto TypeToName() -> std::string {
        if (std::is_same_v<T, UNIT_TYPEID>) {
            return UnitTypeToName(static_cast<UNIT_TYPEID>(type_id_));
        }
        if (std::is_same_v<T, ABILITY_ID>) {
            return AbilityTypeToName(static_cast<ABILITY_ID>(type_id_));
        }
        if (std::is_same_v<T, UPGRADE_ID>) {
            return UpgradeIDToName(static_cast<UPGRADE_ID>(type_id_));
        }
        if (std::is_same_v<T, BUFF_ID>) {
            return BuffIDToName(static_cast<BUFF_ID>(type_id_));
        }
        if (std::is_same_v<T, EFFECT_ID>) {
            return EffectIDToName(static_cast<EFFECT_ID>(type_id_));
        }
    }

private:
    int type_id_ {0U};
};

using UnitTypeID = SC2Type<UNIT_TYPEID>;
using AbilityID = SC2Type<ABILITY_ID>;
using UpgradeID = SC2Type<UPGRADE_ID>;
using BuffID = SC2Type<BUFF_ID>;
using EffectID = SC2Type<EFFECT_ID>;

}  // namespace sc2
