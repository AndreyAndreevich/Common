//
// Created by Andrey on 2019-01-02.
//

#ifndef ADAPTER_ADAPTER_H
#define ADAPTER_ADAPTER_H

#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

template<typename D_type, typename T_base>
struct Adapter;

#define DECL_FN_ADAPTER(D_name) \
    template<typename... V> \
    auto D_name(V && ... v) \
    { \
        return this->call([](auto & t, auto &&... x) { \
            return t.D_name(std::forward<decltype(x)>(x)...); \
        }, std::forward<V>(v)...); \
    }

#define DECL_FN_ADAPTER_ITERATION(D_r, D_data, D_elem) \
    DECL_FN_ADAPTER(D_elem)

#define DECL_ADAPTER(D_type, ...) \
    template<typename T_base> \
    struct Adapter<D_type, T_base> : T_base \
    { \
        BOOST_PP_LIST_FOR_EACH(DECL_FN_ADAPTER_ITERATION, , \
            BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))) \
    };

#endif //ADAPTER_ADAPTER_H
