/*! \file sc2_common.h
    \brief Common data types, including points, rectangles and colors.
*/
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#include <s2clientprotocol/debug.pb.h>

namespace sc2 {

//! 3D point.
//!< \sa Distance3D(const Point3D& a, const Point3D& b) DistanceSquared3D(const
//!< Point3D& a, const Point3D& b) Normalize3D(Point3D& a) Dot3D(const Point3D&
//!< a, const Point3D& b)
struct Point3D
{
    float x { 0.0f };
    float y { 0.0f };
    float z { 0.0f };

    constexpr Point3D ( ) { }

    constexpr Point3D ( const float in_x, const float in_y, const float in_z )
        : x ( in_x ),
          y ( in_y ),
          z ( in_z ) { }

    auto operator += ( const Point3D& rhs ) -> Point3D&;
    auto operator -= ( const Point3D& rhs ) -> Point3D&;
    auto operator *= ( float rhs ) -> Point3D&;
    auto operator /= ( float rhs ) -> Point3D&;

    auto operator == ( const Point3D& rhs ) const -> bool;
    auto operator != ( const Point3D& rhs ) const -> bool;
};

auto operator + ( const Point3D& lhs, const Point3D& rhs ) -> Point3D;
auto operator - ( const Point3D& lhs, const Point3D& rhs ) -> Point3D;
auto operator * ( const Point3D& lhs, float rhs ) -> Point3D;
auto operator * ( float lhs, const Point3D& rhs ) -> Point3D;
auto operator / ( const Point3D& lhs, float rhs ) -> Point3D;
auto operator / ( float lhs, const Point3D& rhs ) -> Point3D;

//! 2D point.
// When referring to Point2D, (0, 0) is bottom left.
//!< \sa Distance2D(const Point2D& a, const Point2D& b) DistanceSquared2D(const
//!< Point2D& a, const Point2D& b) Normalize2D(Point2D& a) Dot2D(const Point2D&
//!< a, const Point2D& b)
struct Point2D
{
    float x;
    float y;

    constexpr Point2D ( )
        : x ( 0.0f ),
          y ( 0.0f ) { }

    constexpr Point2D ( const Point3D& a )
        : x ( a.x ),
          y ( a.y ) { }

    constexpr Point2D ( float in_x, float in_y )
        : x ( in_x ),
          y ( in_y ) { }

    auto operator += ( const Point2D& rhs ) -> Point2D&;
    auto operator -= ( const Point2D& rhs ) -> Point2D&;
    auto operator *= ( float rhs ) -> Point2D&;
    auto operator /= ( float rhs ) -> Point2D&;

    auto operator == ( const Point2D& rhs ) const -> bool;
    auto operator != ( const Point2D& rhs ) const -> bool;
};

auto operator + ( const Point2D& lhs, const Point2D& rhs ) -> Point2D;
auto operator - ( const Point2D& lhs, const Point2D& rhs ) -> Point2D;
auto operator * ( const Point2D& lhs, float rhs ) -> Point2D;
auto operator * ( float lhs, const Point2D& rhs ) -> Point2D;
auto operator / ( const Point2D& lhs, float rhs ) -> Point2D;
auto operator / ( float lhs, const Point2D& rhs ) -> Point2D;

using Vector2D = Point2D;
using Vector3D = Point3D;

//! 2D rectangle.
struct Rect2D
{
    Point2D from;
    Point2D to;
};

//! 2D integer point.
struct Point2DI
{
    int x;
    int y;

    constexpr Point2DI ( int in_x = 0, int in_y = 0 )
        : x ( in_x ),
          y ( in_y ) { }

    constexpr Point2DI ( const Point2D& point )
        : x ( static_cast<int> ( point.x ) ),
          y ( static_cast<int> ( point.y ) ) { }

    auto operator == ( const Point2DI& rhs ) const -> bool;
};

//! 2D integer rectangle.
struct Rect2DI
{
    Point2DI from;
    Point2DI to;

    Rect2DI ( ) = default;

    Rect2DI (
      const Point2DI& in_from,
      const Point2DI& in_to
    ) // NOLINT(*-easily-swappable-parameters)
        : from ( in_from ),
          to ( in_to ) { }

    [[nodiscard]] auto Width ( ) const -> int;

    [[nodiscard]] auto Height ( ) const -> int;

    [[nodiscard]] auto Contain ( const sc2::Point2DI& point ) const -> bool;
};

//! RGB Color.
struct Color
{
    Color ( ) { }

    constexpr Color (
      const uint8_t in_r,
      const uint8_t in_g,
      const uint8_t in_b
    )
        : r ( in_r ),
          g ( in_g ),
          b ( in_b ) { }

    void SetMutableColor ( SC2APIProtocol::Color* mutable_color ) const {
        mutable_color->set_r ( r );
        mutable_color->set_g ( g );
        mutable_color->set_b ( b );
    }

private:
    uint8_t r { 255 };
    uint8_t g { 255 };
    uint8_t b { 255 };
};

namespace Colors {
static constexpr Color White  = { 255, 255, 255 };
static constexpr Color Red    = { 255, 0, 0 };
static constexpr Color Green  = { 0, 255, 0 };
static constexpr Color Yellow = { 255, 255, 0 };
static constexpr Color Blue   = { 0, 0, 255 };
static constexpr Color Teal   = { 0, 255, 255 };
static constexpr Color Purple = { 255, 0, 255 };
static constexpr Color Black  = { 0, 0, 0 };
static constexpr Color Gray   = { 128, 128, 128 };
}; // namespace Colors

//! Gets a random floating-point number between -1.0 and 1.0.
//!< \return Random floating-point number between -1.0 and 1.0.
auto GetRandomScalar ( ) -> float;
//! Gets a random floating-point number between 0.0 and 1.0.
//!< \return Random floating-point number between 0.0 and 1.0.
auto GetRandomFraction ( ) -> float;
//! Gets a random integer between min and max inclusive.
//!< \param min Smallest value a random number could be.
//!< \param max Largest value a random number could be.
//!< \return Random integer between min and max.
auto GetRandomInteger ( int min, int max ) -> int;

//! Gets a random entry from the container.
//!< \param container Array, list or whatever container.
//!< \return A random entry.
template<typename Container>
auto GetRandomEntry ( Container& container ) -> Container::value_type& {
    typename Container::iterator iter = container.begin( );
    std::advance (
      iter,
      GetRandomInteger ( 0, static_cast<int> ( container.size( ) ) - 1 )
    );
    return *iter;
}

//! The distance between two 2D points.
//!< \param a First point.
//!< \param b Second point.
//!< \return Distance.
auto Distance2D ( const Point2D& a, const Point2D& b ) -> float;
//! The distance squared between two 2D points, faster than Distance2D.
//!< \param a First point.
//!< \param b Second point.
//!< \return Distance^2.
auto DistanceSquared2D ( const Point2D& a, const Point2D& b ) -> float;

//! @returns @c true if Point @c a is closer
auto CompareDistance2D (
  const Point2D& a,
  const Point2D& b,
  const Point2D& datum
) -> bool;

//! Normalize a 2D point.
//!< \param a Point.
void Normalize2D ( Point2D& a );
//! The dot product of two 2D vectors.
//!< \param a First vector.
//!< \param b Second vector.
//!< \return Dot product.
auto Dot2D ( const Point2D& a, const Point2D& b ) -> float;


constexpr float kInfiniteSlope = std::numeric_limits<float>::max();
auto Slope ( const Point2D& a, const Point2D& b ) -> float;

//! The distance between two 3D points.
//!< \param a First point.
//!< \param b Second point.
//!< \return Distance.
auto Distance3D ( const Point3D& a, const Point3D& b ) -> float;
//! The distance squared between two 3D points, faster than Distance3D.
//!< \param a First point.
//!< \param b Second point.
//!< \return Distance^2.
auto DistanceSquared3D ( const Point3D& a, const Point3D& b ) -> float;
//! Normalize a 3D point.
//!< \param a Point.
void Normalize3D ( Point3D& a );
//! The dot product of two 3D vectors.
//!< \param a First vector.
//!< \param b Second vector.
//!< \return Dot product.
auto Dot3D ( const Point3D& a, const Point3D& b ) -> float;

} // namespace sc2
