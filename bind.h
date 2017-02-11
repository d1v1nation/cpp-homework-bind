//
// Created by cat on 23.01.17.
//

#ifndef BIND_BIND_H
#define BIND_BIND_H

#include <functional>
#include <tuple>


namespace bind {
    // "i am a cool dude that might shoot off my own foot"
    template <typename T>
    using perfect = T;

    // "i am scared of reference-to-local"
    template <typename T>
    using safe = std::decay_t<T>;

    template <typename T>
    using fwd_tp = safe<T>;

    template <typename T>
    struct bound {
        typedef T type;

        bound(T&& lost_soul) : val(std::forward<T>(lost_soul)) {};

        template <typename... EvalArgs>
        constexpr T& operator()(EvalArgs&&... args) {
            return val;
        }

    private:
        fwd_tp<T> val;
    };

    template <size_t At>
    struct argument {
        template <typename E0, typename... EvalArgs>
        constexpr auto&& operator()(E0&& a0, EvalArgs&&... args) {
            return argument<At - 1>()(std::forward<EvalArgs>(args)...);
        }
    };

    template <>
    struct argument<0> {
        template <typename E0, typename... EvalArgs>
        constexpr auto&& operator()(E0&& a0, EvalArgs&&... args) {
            return a0;
        };
    };

    argument<0> _1;
    argument<1> _2;
    argument<2> _3;

    template <typename F, typename... Args>
    struct bind_tp;

    template <typename A, typename O>
    struct binding {
        using type = bound<O>;
    };

    template <size_t At, typename O>
    struct binding<argument<At>, O> {
        using type = argument<At>;
    };

    template <typename O, typename F, typename... Args>
    struct binding<bind_tp<F, Args...>, O> {
        using type = bind_tp<F, Args...>;
    };

    template <typename F, typename... Args>
    struct bind_tp {
        bind_tp(F&& func, Args&&... args) : func(func), binds(std::forward<Args>(args)...) {
        }

        template <typename... EvalArgs>
        constexpr auto operator()(EvalArgs&&... args) {
            using il_binds = typename std::index_sequence_for<Args...>;
            return eval(il_binds(), std::forward<EvalArgs>(args)...);
        }

    private:
        template <typename... EvalArgs, size_t... IBs>
        constexpr auto eval(std::index_sequence<IBs...> il_binds, EvalArgs&&... eval_args) {
            return func((std::get<IBs>(binds)(std::forward<EvalArgs>(eval_args)...))...);
        }

        fwd_tp<F> func;
        std::tuple<typename binding<std::decay_t<Args>, Args>::type...> binds;

    };

    template <typename F, typename... Args>
    bind_tp<F, Args...> make_bind(F func, Args&&... args) {
        return bind_tp<F, Args...>(std::move(func), std::forward<Args>(args)...);
    };
}

#endif //BIND_BIND_H
