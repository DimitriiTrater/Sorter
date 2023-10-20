#pragma once
// take from date lib only one i need
#include <chrono>

namespace date {
template <typename R1, typename R2>
using ratio_multiply = decltype(std::ratio_multiply<R1, R2>{});

template <typename R1, typename R2>
using ratio_divide = decltype(std::ratio_divide<R1, R2>{});

using days = std::chrono::duration<
    int, ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

using weeks =
    std::chrono::duration<int, ratio_multiply<std::ratio<7>, days::period>>;

using years = std::chrono::duration<
    int, ratio_multiply<std::ratio<146097, 400>, days::period>>;

using months =
    std::chrono::duration<int, ratio_divide<years::period, std::ratio<12>>>;
} // namespace date
