#include "nanobrain_dodecanion.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// ================================================================
// Multiplication Table
// ================================================================

// 12x12 multiplication table for dodecanion basis elements
// e_i * e_j = sign * e_k
// Based on Cayley-Dickson construction extended from octonions
static const int DODECANION_MULT_TABLE[12][12] = {
    // e0   e1   e2   e3   e4   e5   e6   e7   e8   e9  e10  e11
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},           // e0
    {1, -0, 3, -2, 5, -4, -7, 6, 9, -8, -11, 10},     // e1
    {2, -3, -0, 1, 6, 7, -4, -5, 10, 11, -8, -9},     // e2
    {3, 2, -1, -0, 7, -6, 5, -4, 11, -10, 9, -8},     // e3
    {4, -5, -6, -7, -0, 1, 2, 3, -12, -12, -12, -12}, // e4 (special)
    {5, 4, -7, 6, -1, -0, -3, 2, -12, -12, -12, -12}, // e5
    {6, 7, 4, -5, -2, 3, -0, -1, -12, -12, -12, -12}, // e6
    {7, -6, 5, 4, -3, -2, 1, -0, -12, -12, -12, -12}, // e7
    {8, -9, -10, -11, 12, 12, 12, 12, -0, 1, 2, 3},   // e8
    {9, 8, -11, 10, 12, 12, 12, 12, -1, -0, -3, 2},   // e9
    {10, 11, 8, -9, 12, 12, 12, 12, -2, 3, -0, -1},   // e10
    {11, -10, 9, 8, 12, 12, 12, 12, -3, -2, 1, -0}    // e11
};

// Sign table for products
static const int DODECANION_SIGN_TABLE[12][12] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},         // e0
    {1, -1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1},   // e1
    {1, -1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1},   // e2
    {1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1, -1},   // e3
    {1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1},     // e4
    {1, 1, -1, 1, -1, -1, -1, 1, 1, 1, 1, 1},     // e5
    {1, 1, 1, -1, -1, 1, -1, -1, 1, 1, 1, 1},     // e6
    {1, -1, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1},     // e7
    {1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1}, // e8
    {1, 1, -1, 1, -1, -1, -1, -1, -1, -1, -1, 1}, // e9
    {1, 1, 1, -1, -1, -1, -1, -1, -1, 1, -1, -1}, // e10
    {1, -1, 1, 1, -1, -1, -1, -1, -1, -1, 1, -1}  // e11
};

BasisProduct get_basis_product(int i, int j) {
  BasisProduct result;
  int table_val = DODECANION_MULT_TABLE[i][j];
  if (table_val >= 0) {
    result.result_index = table_val;
    result.sign = DODECANION_SIGN_TABLE[i][j];
  } else {
    result.result_index = -table_val;
    result.sign = -DODECANION_SIGN_TABLE[i][j];
  }
  return result;
}

// ================================================================
// Constructors
// ================================================================

Dodecanion::Dodecanion() { components.fill(0.0f); }

Dodecanion::Dodecanion(float real) {
  components.fill(0.0f);
  components[0] = real;
}

Dodecanion::Dodecanion(const std::array<float, 12> &comps)
    : components(comps) {}

Dodecanion Dodecanion::zero() { return Dodecanion(); }

Dodecanion Dodecanion::one() { return Dodecanion(1.0f); }

Dodecanion Dodecanion::unit(int index) {
  Dodecanion d;
  if (index >= 0 && index < 12) {
    d.components[index] = 1.0f;
  }
  return d;
}

Dodecanion Dodecanion::from_real(float r) { return Dodecanion(r); }

Dodecanion Dodecanion::from_complex(float real, float imag) {
  Dodecanion d;
  d.components[0] = real;
  d.components[1] = imag;
  return d;
}

Dodecanion Dodecanion::from_quaternion(float w, float x, float y, float z) {
  Dodecanion d;
  d.components[0] = w;
  d.components[1] = x;
  d.components[2] = y;
  d.components[3] = z;
  return d;
}

Dodecanion Dodecanion::from_octonion(const std::array<float, 8> &oct) {
  Dodecanion d;
  for (int i = 0; i < 8; i++) {
    d.components[i] = oct[i];
  }
  return d;
}

// ================================================================
// Basic Algebra Operations
// ================================================================

Dodecanion Dodecanion::operator+(const Dodecanion &other) const {
  Dodecanion result;
  for (int i = 0; i < 12; i++) {
    result.components[i] = components[i] + other.components[i];
  }
  return result;
}

Dodecanion &Dodecanion::operator+=(const Dodecanion &other) {
  for (int i = 0; i < 12; i++) {
    components[i] += other.components[i];
  }
  return *this;
}

Dodecanion Dodecanion::operator-(const Dodecanion &other) const {
  Dodecanion result;
  for (int i = 0; i < 12; i++) {
    result.components[i] = components[i] - other.components[i];
  }
  return result;
}

Dodecanion &Dodecanion::operator-=(const Dodecanion &other) {
  for (int i = 0; i < 12; i++) {
    components[i] -= other.components[i];
  }
  return *this;
}

Dodecanion Dodecanion::operator-() const {
  Dodecanion result;
  for (int i = 0; i < 12; i++) {
    result.components[i] = -components[i];
  }
  return result;
}

Dodecanion Dodecanion::operator*(float scalar) const {
  Dodecanion result;
  for (int i = 0; i < 12; i++) {
    result.components[i] = components[i] * scalar;
  }
  return result;
}

Dodecanion &Dodecanion::operator*=(float scalar) {
  for (int i = 0; i < 12; i++) {
    components[i] *= scalar;
  }
  return *this;
}

Dodecanion Dodecanion::operator*(const Dodecanion &other) const {
  Dodecanion result;

  // Use multiplication table for basis element products
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 12; j++) {
      float a = components[i];
      float b = other.components[j];
      if (std::abs(a) < 1e-10f || std::abs(b) < 1e-10f)
        continue;

      BasisProduct bp = get_basis_product(i, j);
      if (bp.result_index < 12) {
        result.components[bp.result_index] += bp.sign * a * b;
      }
    }
  }

  return result;
}

Dodecanion &Dodecanion::operator*=(const Dodecanion &other) {
  *this = *this * other;
  return *this;
}

Dodecanion Dodecanion::operator/(float scalar) const {
  if (std::abs(scalar) < 1e-10f) {
    throw std::runtime_error("Division by near-zero scalar");
  }
  Dodecanion result;
  for (int i = 0; i < 12; i++) {
    result.components[i] = components[i] / scalar;
  }
  return result;
}

Dodecanion &Dodecanion::operator/=(float scalar) {
  if (std::abs(scalar) < 1e-10f) {
    throw std::runtime_error("Division by near-zero scalar");
  }
  for (int i = 0; i < 12; i++) {
    components[i] /= scalar;
  }
  return *this;
}

Dodecanion Dodecanion::operator/(const Dodecanion &other) const {
  return *this * other.inverse();
}

// ================================================================
// Algebraic Properties
// ================================================================

Dodecanion Dodecanion::conjugate() const {
  Dodecanion result;
  result.components[0] = components[0];
  for (int i = 1; i < 12; i++) {
    result.components[i] = -components[i];
  }
  return result;
}

float Dodecanion::norm_squared() const {
  float sum = 0.0f;
  for (int i = 0; i < 12; i++) {
    sum += components[i] * components[i];
  }
  return sum;
}

float Dodecanion::norm() const { return std::sqrt(norm_squared()); }

Dodecanion Dodecanion::inverse() const {
  float ns = norm_squared();
  if (ns < 1e-10f) {
    throw std::runtime_error("Cannot invert zero dodecanion");
  }
  return conjugate() / ns;
}

Dodecanion Dodecanion::normalize() const {
  float n = norm();
  if (n < 1e-10f) {
    return Dodecanion::zero();
  }
  return *this / n;
}

float Dodecanion::real() const { return components[0]; }

std::array<float, 11> Dodecanion::imaginary() const {
  std::array<float, 11> imag;
  for (int i = 0; i < 11; i++) {
    imag[i] = components[i + 1];
  }
  return imag;
}

// ================================================================
// Tensor Conversion
// ================================================================

NanoBrainTensor *Dodecanion::to_tensor(NanoBrainKernel *kernel) const {
  NanoBrainTensor *tensor = kernel->create_tensor({12});
  std::vector<float> data(components.begin(), components.end());
  kernel->set_data(tensor, data);
  return tensor;
}

Dodecanion Dodecanion::from_tensor(NanoBrainTensor *tensor,
                                   NanoBrainKernel *kernel) {
  Dodecanion d;
  kernel->compute(tensor);
  for (int i = 0; i < 12; i++) {
    d.components[i] = kernel->get_value(tensor, i);
  }
  return d;
}

// ================================================================
// 11D Manifold Projection
// ================================================================

std::array<float, 11> Dodecanion::project_to_11d() const {
  // Project normalized dodecanion to 11D hypersphere
  Dodecanion unit = normalize();
  std::array<float, 11> coords;

  // Use stereographic projection from e0 axis
  float denom = 1.0f + unit.components[0];
  if (std::abs(denom) < 1e-6f) {
    // Near antipodal point, use alternate projection
    coords.fill(0.0f);
    return coords;
  }

  for (int i = 0; i < 11; i++) {
    coords[i] = unit.components[i + 1] / denom;
  }

  return coords;
}

Dodecanion
Dodecanion::from_11d_projection(const std::array<float, 11> &coords) {
  // Inverse stereographic projection
  float r2 = 0.0f;
  for (int i = 0; i < 11; i++) {
    r2 += coords[i] * coords[i];
  }

  Dodecanion d;
  float scale = 2.0f / (1.0f + r2);
  d.components[0] = (1.0f - r2) / (1.0f + r2);

  for (int i = 0; i < 11; i++) {
    d.components[i + 1] = scale * coords[i];
  }

  return d;
}

// ================================================================
// Rotation and Transformation
// ================================================================

Dodecanion Dodecanion::rotate_by(const Dodecanion &q) const {
  // Generalized rotation: q * this * q.inverse()
  return q * (*this) * q.inverse();
}

Dodecanion Dodecanion::lerp(const Dodecanion &a, const Dodecanion &b, float t) {
  return a * (1.0f - t) + b * t;
}

Dodecanion Dodecanion::slerp(const Dodecanion &a, const Dodecanion &b,
                             float t) {
  float d = dot(a, b);

  // Handle negative dot product for shortest path
  Dodecanion b_adj = b;
  if (d < 0.0f) {
    b_adj = -b;
    d = -d;
  }

  // If very close, use linear interpolation
  if (d > 0.9995f) {
    return lerp(a, b_adj, t).normalize();
  }

  float theta = std::acos(d);
  float sin_theta = std::sin(theta);
  float wa = std::sin((1.0f - t) * theta) / sin_theta;
  float wb = std::sin(t * theta) / sin_theta;

  return a * wa + b_adj * wb;
}

// ================================================================
// Comparison
// ================================================================

bool Dodecanion::operator==(const Dodecanion &other) const {
  return components == other.components;
}

bool Dodecanion::operator!=(const Dodecanion &other) const {
  return !(*this == other);
}

bool Dodecanion::approximately_equal(const Dodecanion &other,
                                     float epsilon) const {
  for (int i = 0; i < 12; i++) {
    if (std::abs(components[i] - other.components[i]) > epsilon) {
      return false;
    }
  }
  return true;
}

// ================================================================
// Utility
// ================================================================

std::string Dodecanion::to_string() const {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(4);
  ss << "(" << components[0];
  const char *basis[] = {"",  "i", "j", "k", "l", "m",
                         "n", "o", "p", "q", "r", "s"};
  for (int i = 1; i < 12; i++) {
    if (components[i] >= 0) {
      ss << " + " << components[i] << basis[i];
    } else {
      ss << " - " << -components[i] << basis[i];
    }
  }
  ss << ")";
  return ss.str();
}

float &Dodecanion::operator[](size_t index) { return components.at(index); }

float Dodecanion::operator[](size_t index) const {
  return components.at(index);
}

// ================================================================
// Free Functions
// ================================================================

Dodecanion operator*(float scalar, const Dodecanion &d) { return d * scalar; }

float dot(const Dodecanion &a, const Dodecanion &b) {
  float sum = 0.0f;
  for (int i = 0; i < 12; i++) {
    sum += a[i] * b[i];
  }
  return sum;
}

Dodecanion cross(const Dodecanion &a, const Dodecanion &b) {
  // Generalized cross product: (a*b - b*a) / 2
  return (a * b - b * a) * 0.5f;
}

Dodecanion exp(const Dodecanion &d) {
  float re = d.real();
  auto im = d.imaginary();

  float im_norm = 0.0f;
  for (int i = 0; i < 11; i++) {
    im_norm += im[i] * im[i];
  }
  im_norm = std::sqrt(im_norm);

  float exp_re = std::exp(re);

  if (im_norm < 1e-10f) {
    return Dodecanion(exp_re);
  }

  float cos_im = std::cos(im_norm);
  float sin_im_norm = std::sin(im_norm) / im_norm;

  Dodecanion result;
  result[0] = exp_re * cos_im;
  for (int i = 0; i < 11; i++) {
    result[i + 1] = exp_re * sin_im_norm * im[i];
  }

  return result;
}

Dodecanion log(const Dodecanion &d) {
  float n = d.norm();
  if (n < 1e-10f) {
    throw std::runtime_error("Cannot compute log of zero dodecanion");
  }

  float re = d.real();
  auto im = d.imaginary();

  float im_norm = 0.0f;
  for (int i = 0; i < 11; i++) {
    im_norm += im[i] * im[i];
  }
  im_norm = std::sqrt(im_norm);

  Dodecanion result;
  result[0] = std::log(n);

  if (im_norm < 1e-10f) {
    return result;
  }

  float theta = std::acos(re / n);
  float scale = theta / im_norm;

  for (int i = 0; i < 11; i++) {
    result[i + 1] = scale * im[i];
  }

  return result;
}

Dodecanion pow(const Dodecanion &base, float exponent) {
  // d^p = exp(p * log(d))
  return exp(log(base) * exponent);
}

// ================================================================
// DodecanionAlgebra Implementation
// ================================================================

DodecanionAlgebra::DodecanionAlgebra(NanoBrainKernel *kernel)
    : kernel(kernel) {}

std::vector<Dodecanion>
DodecanionAlgebra::batch_multiply(const std::vector<Dodecanion> &a,
                                  const std::vector<Dodecanion> &b) {
  size_t n = std::min(a.size(), b.size());
  std::vector<Dodecanion> result(n);
  for (size_t i = 0; i < n; i++) {
    result[i] = a[i] * b[i];
  }
  return result;
}

NanoBrainTensor *
DodecanionAlgebra::to_batch_tensor(const std::vector<Dodecanion> &dodecanions) {
  if (dodecanions.empty()) {
    return kernel->create_tensor({1, 12});
  }

  size_t n = dodecanions.size();
  NanoBrainTensor *tensor =
      kernel->create_tensor({static_cast<int64_t>(n), 12});

  std::vector<float> data;
  data.reserve(n * 12);
  for (const auto &d : dodecanions) {
    for (int i = 0; i < 12; i++) {
      data.push_back(d[i]);
    }
  }

  kernel->set_data(tensor, data);
  return tensor;
}

std::vector<Dodecanion>
DodecanionAlgebra::from_batch_tensor(NanoBrainTensor *tensor, size_t count) {
  kernel->compute(tensor);
  std::vector<Dodecanion> result(count);

  for (size_t i = 0; i < count; i++) {
    for (int j = 0; j < 12; j++) {
      result[i][j] = kernel->get_value(tensor, static_cast<int>(i * 12 + j));
    }
  }

  return result;
}

Dodecanion
DodecanionAlgebra::geometric_mean(const std::vector<Dodecanion> &dodecanions) {
  if (dodecanions.empty()) {
    return Dodecanion::one();
  }

  // Product of all, then take n-th root
  Dodecanion product = Dodecanion::one();
  for (const auto &d : dodecanions) {
    product *= d;
  }

  float n = static_cast<float>(dodecanions.size());
  return pow(product, 1.0f / n);
}

Dodecanion
DodecanionAlgebra::weighted_sum(const std::vector<Dodecanion> &dodecanions,
                                const std::vector<float> &weights) {
  Dodecanion result;
  size_t n = std::min(dodecanions.size(), weights.size());

  for (size_t i = 0; i < n; i++) {
    result += dodecanions[i] * weights[i];
  }

  return result;
}
