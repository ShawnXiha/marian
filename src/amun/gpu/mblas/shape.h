#pragma once
#include <sstream>
#include <cassert>
#include "common/base_matrix.h"

namespace amunmt {
namespace GPU {
namespace mblas {

class Shape
{
public:
  Shape()
  {
    dim_[0] = 0;
    dim_[1] = 0;
    dim_[2] = 0;
    dim_[3] = 0;
    updateStrides();
  }

  __device__ __host__
  Shape(const Shape &other)
  :size_(other.size_)
  {
    dim_[0] = other.dim(0);
    dim_[1] = other.dim(1);
    dim_[2] = other.dim(2);
    dim_[3] = other.dim(3);

    stride_[0] = other.stride(0);
    stride_[1] = other.stride(1);
    stride_[2] = other.stride(2);
    stride_[3] = other.stride(3);
  }

  __device__ __host__
  Shape(unsigned a, unsigned b, unsigned c, unsigned d)
  {
    dim_[0] = a;
    dim_[1] = b;
    dim_[2] = c;
    dim_[3] = d;

    updateStrides();
  }

  __device__ __host__
  inline unsigned dim(unsigned i) const
  {  return dim_[i]; }

  __device__ __host__
  inline unsigned size() const
  {
    return size_;
  }

  __device__ __host__
  inline unsigned stride(unsigned i) const
  {
    return stride_[i];
  }

  __device__ __host__
  void updateStrides()
  {
    stride_[0] = dim_[1];
    stride_[1] = 1;
    stride_[2] = dim_[0] * dim_[1];
    stride_[3] = dim_[0] * dim_[1] * dim_[2];

    size_ = stride_[3] * dim_[3];
  }

  // indices2Id
  // 4
  __device__ __host__
  inline unsigned indices2Id(unsigned a, unsigned b, unsigned c, unsigned d) const
  {
    assert(a < dim(0));
    assert(b < dim(1));
    assert(c < dim(2));
    assert(d < dim(3));

    unsigned ind =
            a * stride(0)
          + b * stride(1)
          + c * stride(2)
          + d * stride(3);

    assert(ind < size());
    return ind;
  }

  // 3
  __device__ __host__
  inline unsigned indices2Id(unsigned a, unsigned b, unsigned c) const
  {
    assert(a < dim(0));
    assert(b < dim(1));
    assert(c < dim(2));

    unsigned ind =
            a * stride(0)
          + b * stride(1)
          + c * stride(2);

    assert(ind < size());
    return ind;
  }

  // 2
  __device__ __host__
  inline unsigned indices2Id(unsigned a, unsigned b) const
  {
    assert(a < dim(0));
    assert(b < dim(1));

    unsigned ind =
            a * stride(0)
          + b * stride(1);

    assert(ind < size());
    return ind;
  }

  // 1
  __device__ __host__
  inline unsigned indices2Id(unsigned a) const
  {
    assert(a < dim(0));

    unsigned ind =
          a * stride(0);

    assert(ind < size());
    return ind;
  }

  __device__ __host__
  inline void id2Indices(unsigned id, unsigned *out) const
  {
    assert(id < size());

    out[3] = id / stride(3);
    id = id % stride(3);

    out[2] = id / stride(2);
    id = id % stride(2);

    out[0] = id / stride(0);
    id = id % stride(0);

    out[1] = id / stride(1);
  }

  std::string Debug() const
  {
    std::stringstream strm;

    strm << "dim=";
    for (unsigned i = 0; i < SHAPE_SIZE; ++i) {
      strm << dim_[i] << " ";
    }

    strm << " stride=";
    for (unsigned i = 0; i < SHAPE_SIZE; ++i) {
      strm << stride(i) << " ";
    }

    return strm.str();
  }


protected:
  unsigned dim_[SHAPE_SIZE];
  unsigned stride_[SHAPE_SIZE];
  unsigned size_;
};


} // namespace
}
}
