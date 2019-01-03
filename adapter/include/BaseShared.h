//
// Created by Andrey on 2019-01-02.
//

#ifndef ADAPTER_BASESHARED_H
#define ADAPTER_BASESHARED_H

#include <memory>

template<typename T>
struct BaseShared {
protected:
    template<typename F, typename ... V>
    auto call(F f, V && ... v) {
        return f(*shared_, std::forward<V>(v)...);
    }
private:
    std::shared_ptr<T> shared_ = std::make_shared<T>();
};

#endif //ADAPTER_BASESHARED_H
