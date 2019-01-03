//
// Created by Andrey on 2019-01-02.
//

#ifndef ADAPTER_BASELAZY_H
#define ADAPTER_BASELAZY_H

#include <boost/variant.hpp>
#include <functional>

template<typename T>
struct BaseLazy {
    template<typename ... V>
    BaseLazy(V && ... v) {
        state_ = [v...] {
            return T{std::move(v)...};
        };
    }
protected:
    template<typename F, typename ... V>
    auto call(F f, V && ... v) {
        auto t = boost::get<T>(&state_);
        if (t == nullptr) {
            state_ = boost::get<std::function<T()>>(state_)();
            t = boost::get<T>(&state_);
        }
        return f(*t, std::forward<V>(v)...);
    }
private:
    boost::variant<std::function<T()>, T> state_;
};

#endif //ADAPTER_BASELAZY_H
