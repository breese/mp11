#ifndef BOOST_MP11_UTILITY_HPP_INCLUDED
#define BOOST_MP11_UTILITY_HPP_INCLUDED

//  Copyright 2015, 2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11/integral.hpp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

namespace boost
{
namespace mp11
{

// mp_identity
template<class T> struct mp_identity
{
    using type = T;
};

// mp_identity_t
template<class T> using mp_identity_t = T;

// mp_inherit
template<class... T> struct mp_inherit: T... {};

// mp_if, mp_if_c
namespace detail
{

template<bool C, class T, class E> struct mp_if_c_impl;

template<class T, class E> struct mp_if_c_impl<true, T, E>
{
    using type = T;
};

template<class T, class E> struct mp_if_c_impl<false, T, E>
{
    using type = E;
};

} // namespace detail

template<bool C, class T, class E> using mp_if_c = typename detail::mp_if_c_impl<C, T, E>::type;
template<class C, class T, class E> using mp_if = typename detail::mp_if_c_impl<static_cast<bool>(C::value), T, E>::type;

// mp_eval_if, mp_eval_if_c
namespace detail
{

template<bool C, class T, template<class...> class F, class... U> struct mp_eval_if_c_impl;

template<class T, template<class...> class F, class... U> struct mp_eval_if_c_impl<true, T, F, U...>
{
    using type = T;
};

template<class T, template<class...> class F, class... U> struct mp_eval_if_c_impl<false, T, F, U...>
{
    using type = F<U...>;
};

} // namespace detail

template<bool C, class T, template<class...> class F, class... U> using mp_eval_if_c = typename detail::mp_eval_if_c_impl<C, T, F, U...>::type;
template<class C, class T, template<class...> class F, class... U> using mp_eval_if = typename detail::mp_eval_if_c_impl<static_cast<bool>(C::value), T, F, U...>::type;

// mp_valid
// implementation by Bruno Dutra (by the name is_evaluable)
namespace detail
{

template<template<class...> class F, class... T> struct mp_valid_impl
{
    template<template<class...> class G, class = G<T...>> static mp_true check(int);
    template<template<class...> class> static mp_false check(...);

    using type = decltype(check<F>(0));
};

} // namespace detail

template<template<class...> class F, class... T> using mp_valid = typename detail::mp_valid_impl<F, T...>::type;

// mp_defer
namespace detail
{

template<template<class...> class F, class... T> struct mp_defer_impl
{
    using type = F<T...>;
};

struct mp_no_type
{
};

} // namespace detail

template<template<class...> class F, class... T> using mp_defer = mp_if<mp_valid<F, T...>, detail::mp_defer_impl<F, T...>, detail::mp_no_type>;

// mp_quote
template<template<class...> class F> struct mp_quote
{
#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, <= 1910 && BOOST_MSVC >= 1900 )
#else
private:
#endif

    template<class... T> struct _fn { using type = F<T...>; };

public:

    // the indirection through _fn works around the language inability
    // to expand T... into a fixed parameter list of an alias template

    template<class... T> using fn = typename _fn<T...>::type;
};

// mp_unquote
namespace detail
{

template<class Q, class... T> struct mp_invoke_impl
{
    using type = typename Q::template fn<T...>;
};

} // namespace detail

template<class Q, class... T> using mp_invoke = typename detail::mp_invoke_impl<Q, T...>::type;

} // namespace mp11
} // namespace boost

#endif // #ifndef BOOST_MP11_UTILITY_HPP_INCLUDED
