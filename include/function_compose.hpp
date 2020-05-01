#pragma once

#include <utility>
#include <functional>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>

template<typename F, typename G>
auto Compose(F&& f, G&& g) {
    return [=](auto x) { return f(g(x)); };
}

template<typename F, typename ...R>
auto Compose(F&& f, R&&... r) {
    return [=](auto x) { return f(Compose(r...)(x)); };
}

template<typename ...Ts>
auto PartialFunctionAdd3(Ts... xs) {
    static_assert (sizeof ...(xs) <= 3);
    if constexpr (sizeof... (xs) == 3) {
        return (0 + ... + xs);
    } else {
        return [xs...](auto... ys) {
            return PartialFunctionAdd3(xs..., ys...);
        };
    }
}

template<typename Callable, typename ...Args>
decltype (auto) call(Callable&& callback, Args&& ... args) {
    if constexpr (boost::is_same<std::invoke_result_t<Callable, Args...>, void>::value) {
        std::invoke(std::forward<Callable>(callback), std::forward<Args>(args)...);
        return ;
    } else {
        decltype (auto) ret(std::invoke(std::forward<Callable>(callback), std::forward<Args>(args)...));
        return ret;
    }
}

template<typename Callable>
struct PipableClosure : Callable
{
    template<typename ...Args>
    PipableClosure (Args&& ... args) : Callable(std::forward<Args>(args)...) {}
};

template<typename Callable>
decltype (auto) MakePipeClosure(Callable callback) {
    return PipableClosure<Callable>(std::move(callback));
}

template <typename Callable>
struct PipableClosureBinary
{
    template<typename ...Args>
    auto operator()(Args... args) const {
        return MakePipeClosure([=](auto x) -> decltype (auto){ return Callable()(x, args...); });
    }
};

template <typename T, typename Callable>
decltype (auto) operator | (T&& x, const PipableClosure<Callable>& pfn) {
    return pfn(std::forward<T>(x));
}
