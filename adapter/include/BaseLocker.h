//
// Created by Andrey on 2019-01-02.
//

#ifndef ADAPTER_BASELOCKER_H
#define ADAPTER_BASELOCKER_H

#include <mutex>

template<typename T_base, typename T_locker>
struct BaseLocker : T_base {
protected:
    template<typename F, typename ... V>
    auto call(F f, V && ... v) {
        Lock _{lock_};
        return f(static_cast<T_base&>(*this),
                std::forward<V>(v)...);
    }
private:
    using Lock = std::unique_lock<T_locker>;
    T_locker lock_;
};

#endif //ADAPTER_BASELOCKER_H
