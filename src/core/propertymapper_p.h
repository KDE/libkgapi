/*
    SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KGAPI_PROPERTYMAPPER_H_
#define KGAPI_PROPERTYMAPPER_H_

#include "kgapicore_export.h"

#include <tuple>
#include <functional>
#include <utility>

#include <QJsonObject>

class QString;
class QJsonValue;
class QDateTime;

namespace KGAPI2
{

namespace detail
{

template<typename Tuple, std::size_t ... I>
auto tuple_pop_front_impl(const Tuple &tuple, std::index_sequence<I ...>) {
    return std::tuple{std::get<1 + I>(tuple) ...};
}

template<typename Tuple>
auto tuple_pop_front(const Tuple &tuple) {
    return tuple_pop_front_impl(tuple, std::make_index_sequence<std::tuple_size_v<Tuple> - 1>());
}


template<typename> struct first_arg_helper;

template<typename R, typename ...Args> struct first_arg_helper<R(Args ...)>
{
    using type = typename std::tuple_element_t<0, std::tuple<Args ...>>;
};

template<typename C, typename R, typename ...Args> struct first_arg_helper<R(C::*)(Args ...)>
{
    using type = std::remove_cv_t<
                    std::remove_reference_t<
                        std::tuple_element_t<0, std::tuple<Args ...>>>>;
};

template<typename T>
using first_arg_t = typename first_arg_helper<std::remove_cv_t<std::remove_reference_t<T>>>::type;

} // namespace detail

template<typename T> T convertTo(const QJsonValue &); // undefined
template<> KGAPICORE_EXPORT QString convertTo<QString>(const QJsonValue &val);
template<> KGAPICORE_EXPORT bool convertTo<bool>(const QJsonValue &val);
template<> KGAPICORE_EXPORT QDateTime convertTo<QDateTime>(const QJsonValue &val);

template<typename Func, typename ... Args>
struct Mapping
{
    using tuple_t = std::tuple<Func, QString, Args ...>;

    Mapping(Func &&func, const QString &attr, Args && ... args)
        : m_data(std::forward_as_tuple(func, attr, args ...))
    {}

    const std::tuple<Func, QString, Args ...> & tuple() const { return m_data; }

    static constexpr std::size_t size = std::tuple_size_v<tuple_t>;

private:
    std::tuple<Func, QString, Args ...> m_data;
};

template<typename Func, typename ... Args>
Mapping(Func &&, const QString &, Args ...) -> Mapping<Func, Args ...>;

namespace detail {

template<typename>
struct is_mapping : std::false_type {};

template<typename ... T>
struct is_mapping<Mapping<T ...>> : std::true_type {};

template<typename ... T>
constexpr bool is_mapping_v = is_mapping<T ...>::value;


template<typename Obj, typename Func, typename Tuple, std::size_t ... I>
auto apply_mapping_impl(Obj *obj, Func &&func, const Tuple &mapping, std::index_sequence<I ...>)
{
    return (obj->*func)(std::get<I>(mapping) ...);
}

template<typename Obj, typename ... Args>
auto apply_mapping(Obj *obj, const Mapping<Args ...> &mapping)
{
    return apply_mapping_impl(obj, std::get<0>(mapping.tuple()), tuple_pop_front(mapping.tuple()),
            std::make_index_sequence<Mapping<Args ...>::size - 1>{});
}


} // namespace detail


template<typename Obj>
class PropertyMapper
{
public:
    PropertyMapper(Obj *obj, const QJsonObject &json)
        : m_obj(obj), m_json(json)
    {}

    template<typename Func, typename ... Args>
    PropertyMapper &map(Func &&func, const QString &attr, Args && ... args)
    {
        using T = detail::first_arg_t<Func>;
        std::apply(func, std::forward_as_tuple(m_obj, convertTo<T>(m_json[attr]), args ...));
        return *this;
    }

    template<typename ... Mappings,
             typename = std::enable_if_t<std::conjunction_v<detail::is_mapping<Mappings> ...>>>
    PropertyMapper &map(Mappings && ... mappings)
    {
        apply_helper<0, Mappings ...>(std::tuple{mappings...});
        return *this;
    }

    template<typename Func, typename ... Args>
    PropertyMapper &operator()(Func &&func, const QString &attr, Args && ... args)
    {
        return map(std::forward<Func>(func), attr, std::forward<Args>(args) ...);
    }

    template<typename ... Mappings,
             typename = std::enable_if_t<std::conjunction_v<detail::is_mapping<Mappings> ...>>>
    PropertyMapper &operator()(Mappings && ... mappings)
    {
        return map(std::forward<Mappings>(mappings) ...);
    }

private:
    template<std::size_t I, typename ... Mappings>
    void apply_helper(const std::tuple<Mappings...> &mappings)
    {
        const auto apply_mapping = [this](auto &&func, const QString &attr, auto && ... args) {
            using T = detail::first_arg_t<decltype(func)>;
            std::apply(func, std::tuple{m_obj, convertTo<T>(m_json[attr]), args ...});
        };

        //std::apply(&PropertyMapper::map, std::tuple_cat(std::tuple{this}, std::get<I>(mappings).tuple()));
        std::apply(apply_mapping, std::get<I>(mappings).tuple());
        if constexpr (I < sizeof...(Mappings) - 1) {
            apply_helper<I + 1, Mappings...>(mappings);
        }
    }

    Obj *m_obj;
    const QJsonObject &m_json;
};

template<typename Obj>
PropertyMapper(Obj *obj, const QJsonObject &json) -> PropertyMapper<Obj>;

} // namespace KGAPI2

#endif
