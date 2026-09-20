#include "sc2_common.h"

#include <random>
#include <cmath>
// #include <thread>

// Avoiding use of "thread_local" as that isn't supported in older versions of Xcode.
#if defined(__clang__) || defined(__GNUC__)
#define TLS_OBJECT __thread
#else
#define TLS_OBJECT __declspec(thread)
#endif

namespace sc2 {

auto Point3D::operator+=(const Point3D& rhs) -> Point3D& {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

auto Point3D::operator-=(const Point3D& rhs) -> Point3D& {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

auto Point3D::operator*=(float rhs) -> Point3D& {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

auto Point3D::operator/=(float rhs) -> Point3D& {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

auto Point3D::operator==(const Point3D& rhs) const -> bool {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

auto Point3D::operator!=(const Point3D& rhs) const -> bool {
    return !(*this == rhs);
}

auto operator+(const Point3D& lhs, const Point3D& rhs) -> Point3D {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

auto operator-(const Point3D& lhs, const Point3D& rhs) -> Point3D {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

auto operator*(const Point3D& lhs, float rhs) -> Point3D {
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

auto operator*(float lhs, const Point3D& rhs) -> Point3D {
    return rhs * lhs;
}

auto operator/(const Point3D& lhs, float rhs) -> Point3D {
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

auto operator/(float lhs, const Point3D& rhs) -> Point3D {
    return rhs / lhs;
}

auto Point2D::operator+=(const Point2D& rhs) -> Point2D& {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

auto Point2D::operator-=(const Point2D& rhs) -> Point2D& {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

auto Point2D::operator*=(float rhs) -> Point2D& {
    x *= rhs;
    y *= rhs;
    return *this;
}

auto Point2D::operator/=(float rhs) -> Point2D& {
    x /= rhs;
    y /= rhs;
    return *this;
}

auto Point2D::operator==(const Point2D& rhs) const -> bool {
    return x == rhs.x && y == rhs.y;
}

auto Point2D::operator!=(const Point2D& rhs) const -> bool {
    return !(*this == rhs);
}

auto operator+(const Point2D& lhs, const Point2D& rhs) -> Point2D {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

auto operator-(const Point2D& lhs, const Point2D& rhs) -> Point2D {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

auto operator*(const Point2D& lhs, float rhs) -> Point2D {
    return {lhs.x * rhs, lhs.y * rhs};
}

auto operator*(float lhs, const Point2D& rhs) -> Point2D {
    return rhs * lhs;
}

auto operator/(const Point2D& lhs, float rhs) -> Point2D {
    return {lhs.x / rhs, lhs.y / rhs};
}

auto operator/(float lhs, const Point2D& rhs) -> Point2D {
    return rhs / lhs;
}

auto Point2DI::operator==(const Point2DI& rhs) const -> bool {
    return x == rhs.x && y == rhs.y;
}

auto Rect2DI::Width() const -> int {
    return to.x - from.x;
}

auto Rect2DI::Height() const -> int {
    return to.y - from.y;
}

auto Rect2DI::Contain(const sc2::Point2DI& point) const -> bool {
    return point.x >= this->from.x && point.x < this->Width() && point.y >= this->from.y && point.y < this->Height();
}
namespace {
struct RandomGenerator {
    RandomGenerator() : mt(rd()) {
    }
    std::random_device rd;
    std::mt19937 mt;
};

auto GetGenerator() -> std::mt19937& {
    static TLS_OBJECT RandomGenerator* generator;
    if (generator == nullptr) {
        generator = new RandomGenerator();
    }
    return generator->mt;
}
}  // namespace

auto GetRandomScalar() -> float {
    static std::uniform_real_distribution<float> dist(-1.0F, 1.0F);
    return dist(GetGenerator());
}

auto GetRandomFraction() -> float {
    static std::uniform_real_distribution<float> dist(0.0F, 1.0F);
    return dist(GetGenerator());
}

auto GetRandomInteger(int min, int max) -> int {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(GetGenerator());
}

auto Distance2D(const Point2D& a, const Point2D& b) -> float {
    const Point2D diff = a - b;
    return std::sqrt(Dot2D(diff, diff));
}

auto DistanceSquared2D(const Point2D& a, const Point2D& b) -> float {
    const Point2D diff = a - b;
    return Dot2D(diff, diff);
}

auto CompareDistance2D(const Point2D& a, const Point2D& b, const Point2D& datum) -> bool {
    return DistanceSquared2D(datum, a) < DistanceSquared2D(datum, b);
}

void Normalize2D(Point2D& a) {
    a /= std::sqrt(Dot2D(a, a));
}

auto Dot2D(const Point2D& a, const Point2D& b) -> float {
    return (a.x * b.x) + (a.y * b.y);
}

auto Slope ( const Point2D& a, const Point2D& b ) -> float {
    if ( a.x - b.x == 0 ) {
        if ( a.y - b.y > 0 ) {
            return kInfiniteSlope;
        }
        if ( a.y - b.y < 0 ) {
            return -kInfiniteSlope;
        }
    }
    return ( a.y - b.y ) / ( a.x - b.x );
}

auto Distance3D(const Point3D& a, const Point3D& b) -> float {
    const Point3D diff = a - b;
    return std::sqrt(Dot3D(diff, diff));
}

auto DistanceSquared3D(const Point3D& a, const Point3D& b) -> float {
    const Point3D diff = a - b;
    return Dot3D(diff, diff);
}

void Normalize3D(Point3D& a) {
    a /= std::sqrt(Dot3D(a, a));
}

auto Dot3D(const Point3D& a, const Point3D& b) -> float {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

}  // namespace sc2
