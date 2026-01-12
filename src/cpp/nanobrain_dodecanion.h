#ifndef NANOBRAIN_DODECANION_H
#define NANOBRAIN_DODECANION_H

#include "nanobrain_kernel.h"
#include <array>
#include <cmath>
#include <string>
#include <vector>

/**
 * Dodecanion (12-dimensional) Hypercomplex Number
 *
 * Extends octonions via Cayley-Dickson construction to create a
 * 12-dimensional algebra for representing brain states in the
 * NanoBrain Time Crystal architecture.
 *
 * Properties:
 * - Non-commutative: a * b ≠ b * a
 * - Non-associative: (a * b) * c ≠ a * (b * c)
 * - 12 basis elements: e0 (real), e1-e11 (imaginary)
 */
struct Dodecanion {
  std::array<float, 12> components;

  // ================================================================
  // Constructors
  // ================================================================

  Dodecanion();
  explicit Dodecanion(float real);
  Dodecanion(const std::array<float, 12> &comps);

  // Factory methods
  static Dodecanion zero();
  static Dodecanion one();
  static Dodecanion unit(int index); // Unit basis element e_i

  // Construct from lower-dimensional algebras
  static Dodecanion from_real(float r);
  static Dodecanion from_complex(float real, float imag);
  static Dodecanion from_quaternion(float w, float x, float y, float z);
  static Dodecanion from_octonion(const std::array<float, 8> &oct);

  // ================================================================
  // Basic Algebra Operations
  // ================================================================

  // Addition
  Dodecanion operator+(const Dodecanion &other) const;
  Dodecanion &operator+=(const Dodecanion &other);

  // Subtraction
  Dodecanion operator-(const Dodecanion &other) const;
  Dodecanion &operator-=(const Dodecanion &other);

  // Negation
  Dodecanion operator-() const;

  // Scalar multiplication
  Dodecanion operator*(float scalar) const;
  Dodecanion &operator*=(float scalar);

  // Dodecanion multiplication (non-commutative, non-associative)
  Dodecanion operator*(const Dodecanion &other) const;
  Dodecanion &operator*=(const Dodecanion &other);

  // Scalar division
  Dodecanion operator/(float scalar) const;
  Dodecanion &operator/=(float scalar);

  // Division by dodecanion (multiply by inverse)
  Dodecanion operator/(const Dodecanion &other) const;

  // ================================================================
  // Algebraic Properties
  // ================================================================

  // Conjugate: negate all imaginary components
  Dodecanion conjugate() const;

  // Norm squared: sum of squares of all components
  float norm_squared() const;

  // Norm: sqrt of norm_squared
  float norm() const;

  // Inverse: conjugate / norm_squared
  Dodecanion inverse() const;

  // Normalize to unit dodecanion
  Dodecanion normalize() const;

  // Real part (e0 component)
  float real() const;

  // Imaginary part (e1-e11 as 11-vector)
  std::array<float, 11> imaginary() const;

  // ================================================================
  // Tensor Conversion
  // ================================================================

  // Convert to NanoBrain tensor (12-element 1D tensor)
  NanoBrainTensor *to_tensor(NanoBrainKernel *kernel) const;

  // Construct from tensor
  static Dodecanion from_tensor(NanoBrainTensor *tensor,
                                NanoBrainKernel *kernel);

  // ================================================================
  // 11D Manifold Projection
  // ================================================================

  // Project to 11-dimensional manifold (for Time Crystal coordinates)
  std::array<float, 11> project_to_11d() const;

  // Reconstruct from 11D projection (assumes unit norm)
  static Dodecanion from_11d_projection(const std::array<float, 11> &coords);

  // ================================================================
  // Rotation and Transformation
  // ================================================================

  // Rotate by another dodecanion: q * this * q.inverse()
  Dodecanion rotate_by(const Dodecanion &q) const;

  // Linear interpolation (lerp)
  static Dodecanion lerp(const Dodecanion &a, const Dodecanion &b, float t);

  // Spherical linear interpolation (slerp)
  static Dodecanion slerp(const Dodecanion &a, const Dodecanion &b, float t);

  // ================================================================
  // Comparison
  // ================================================================

  bool operator==(const Dodecanion &other) const;
  bool operator!=(const Dodecanion &other) const;
  bool approximately_equal(const Dodecanion &other,
                           float epsilon = 1e-6f) const;

  // ================================================================
  // Utility
  // ================================================================

  // String representation
  std::string to_string() const;

  // Component access
  float &operator[](size_t index);
  float operator[](size_t index) const;
};

// ================================================================
// Free Functions
// ================================================================

// Scalar * Dodecanion
Dodecanion operator*(float scalar, const Dodecanion &d);

// Dot product
float dot(const Dodecanion &a, const Dodecanion &b);

// Cross product (generalized to 12D)
Dodecanion cross(const Dodecanion &a, const Dodecanion &b);

// Exponential
Dodecanion exp(const Dodecanion &d);

// Natural logarithm
Dodecanion log(const Dodecanion &d);

// Power
Dodecanion pow(const Dodecanion &base, float exponent);

// ================================================================
// Multiplication Table
// ================================================================

/**
 * Get the multiplication coefficient for basis element product e_i * e_j
 * Returns {result_index, sign} where e_i * e_j = sign * e_result_index
 */
struct BasisProduct {
  int result_index;
  int sign; // +1 or -1
};

BasisProduct get_basis_product(int i, int j);

// ================================================================
// Dodecanion Algebra Helper Class
// ================================================================

/**
 * DodecanionAlgebra - Utility class for dodecanion computations
 */
class DodecanionAlgebra {
public:
  DodecanionAlgebra(NanoBrainKernel *kernel);

  // Batch tensor operations
  std::vector<Dodecanion> batch_multiply(const std::vector<Dodecanion> &a,
                                         const std::vector<Dodecanion> &b);

  // Convert vector of dodecanions to tensor (N x 12 matrix)
  NanoBrainTensor *to_batch_tensor(const std::vector<Dodecanion> &dodecanions);

  // Convert tensor back to vector
  std::vector<Dodecanion> from_batch_tensor(NanoBrainTensor *tensor,
                                            size_t count);

  // Geometric mean of multiple dodecanions
  Dodecanion geometric_mean(const std::vector<Dodecanion> &dodecanions);

  // Weighted sum
  Dodecanion weighted_sum(const std::vector<Dodecanion> &dodecanions,
                          const std::vector<float> &weights);

private:
  NanoBrainKernel *kernel;
};

#endif // NANOBRAIN_DODECANION_H
