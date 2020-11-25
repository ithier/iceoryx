// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IOX_UTILS_CXX_TYPE_TRAITS_HPP
#define IOX_UTILS_CXX_TYPE_TRAITS_HPP

#include <type_traits>

namespace iox
{
namespace cxx
{
template <typename SignatureType>
class function_ref;

///
/// @brief Identifies structs as being "optional" by the presence of an "if_empty" method.
///
template <typename T, typename = void>
struct is_optional : std::false_type
{
};
template <typename T>
struct is_optional<T, std::void_t<decltype(std::declval<T>().has_value())>> : std::true_type
{
};

///
/// @brief Identifies structs as being "chainable" by the presence of an "and_then" method.
///
template <typename, typename = void>
struct is_chainable : std::false_type
{
};
template <typename T>
struct is_chainable<T,
                    std::void_t<decltype(
                        static_cast<T& (T::*)(const cxx::function_ref<void(typename T::inner_type&)>&)>(&T::and_then))>>
    : std::true_type
{
};

///
/// @brief Determines the underlying type of a functional chain.
///
template <typename T, typename = void>
struct flatten
{
    using type = T;
};
template <template <typename, typename...> class T, typename FirstArg, typename... OtherArgs>
struct flatten<T<FirstArg, OtherArgs...>, typename std::enable_if<is_chainable<T<FirstArg, OtherArgs...>>::value>::type>
{
    using type = typename flatten<FirstArg>::type;
};

///
/// @brief Verifies whether the passed Callable type is in fact invocable with the given arguments
///
template <typename Callable, typename... ArgTypes>
struct is_invocable
{
    // This variant is chosen when Callable(ArgTypes) successfully resolves to a valid type, i.e. is invocable.
    /// @note result_of is deprecated, switch to invoke_result in C++17
    template <typename C, typename... As>
    static constexpr std::true_type test(typename std::result_of<C(As...)>::type*)
    {
        return {};
    }

    // This is chosen if Callable(ArgTypes) does not resolve to a valid type.
    template <typename C, typename... As>
    static constexpr std::false_type test(...)
    {
        return {};
    }

    // Test with nullptr as this can stand in for a pointer to any type.
    static constexpr bool value = decltype(test<Callable, ArgTypes...>(nullptr))::value;
};

///
/// @brief Verfies the signature ReturnType(ArgTypes...) of the provided Callable type
///
template <typename Callable = void, typename ReturnType = void, typename ArgTypes = void>
struct has_signature : std::false_type
{
};

template <typename Callable, typename ReturnType, typename... ArgTypes>
struct has_signature<Callable,
                     ReturnType(ArgTypes...),
                     typename std::enable_if<
                         std::is_convertible<typename std::result_of<Callable(ArgTypes...)>::type, ReturnType>::value,
                         void>::type> : std::true_type
{
};

///
/// @brief Negation of is_same
///
template <typename T1, typename T2>
using not_same = typename std::
    integral_constant<bool, !bool(std::is_same<typename std::decay<T1>::type, typename std::decay<T2>::type>::value)>;

} // namespace cxx
} // namespace iox

#endif // IOX_UTILS_CXX_TYPE_TRAITS_HPP
