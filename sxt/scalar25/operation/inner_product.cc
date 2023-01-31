#include "sxt/scalar25/operation/inner_product.h"

#include <algorithm>

#include "sxt/algorithm/reduction/reduction.h"
#include "sxt/base/error/assert.h"
#include "sxt/execution/async/future.h"
#include "sxt/scalar25/operation/accumulator.h"
#include "sxt/scalar25/operation/mul.h"
#include "sxt/scalar25/operation/muladd.h"
#include "sxt/scalar25/operation/product_mapper.h"

namespace sxt::s25o {
//--------------------------------------------------------------------------------------------------
// inner_product
//--------------------------------------------------------------------------------------------------
void inner_product(s25t::element& res, basct::cspan<s25t::element> lhs,
                   basct::cspan<s25t::element> rhs) noexcept {
  auto n = std::min(lhs.size(), rhs.size());
  SXT_DEBUG_ASSERT(n > 0);
  s25o::mul(res, lhs[0], rhs[0]);
  for (size_t i = 1; i < n; ++i) {
    s25o::muladd(res, lhs[i], rhs[i], res);
  }
}

//--------------------------------------------------------------------------------------------------
// async_inner_product
//--------------------------------------------------------------------------------------------------
xena::future<s25t::element> async_inner_product(basct::cspan<s25t::element> lhs,
                                                basct::cspan<s25t::element> rhs) noexcept {
  auto n = std::min(lhs.size(), rhs.size());
  SXT_DEBUG_ASSERT(n > 0);
  return algr::reduce<accumulator>(product_mapper{lhs.data(), rhs.data()},
                                   static_cast<unsigned int>(n));
}
} // namespace sxt::s25o
