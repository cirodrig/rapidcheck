#include <catch.hpp>
#include <rapidcheck-catch.h>

#include "rapidcheck/seq/Transform.h"
#include "rapidcheck/seq/Create.h"

#include "util/CopyGuard.h"

using namespace rc;
using namespace rc::test;

TEST_CASE("seq::drop") {
    prop("drops the first N elements from the given seq",
         [] (const std::vector<int> &elements) {
             std::size_t n = *gen::ranged<std::size_t>(0, elements.size() * 2);
             std::size_t start = std::min(elements.size(), n);
             std::vector<int> rest(elements.begin() + start, elements.end());
             RC_ASSERT(seq::drop(n, seq::fromContainer(elements)) ==
                       seq::fromContainer(rest));
         });

    prop("copies are equal",
         [] (const std::vector<int> &elements) {
             std::size_t n = *gen::ranged<std::size_t>(0, elements.size() * 2);
             std::size_t start = std::min(elements.size(), n);
             const auto seq = seq::drop(n, seq::fromContainer(elements));
             const auto copy = seq;
             RC_ASSERT(seq == copy);
         });

    prop("does not copy items",
         [] (std::vector<CopyGuard> elements) {
             std::size_t n = *gen::ranged<std::size_t>(0, elements.size() * 2);
             std::size_t start = std::min(elements.size(), n);
             auto seq = seq::drop(n, seq::fromContainer(std::move(elements)));
             while (seq.next());
         });
}
