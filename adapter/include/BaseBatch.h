//
// Created by Andrey on 2019-01-02.
//

#ifndef ADAPTER_BASEBATCH_H
#define ADAPTER_BASEBATCH_H

#include <vector>
#include <functional>

template<typename T_base>
struct BaseBatch : T_base {
    void execute() {
        for (auto && action : actions_) {
            action();
        }
        actions_.clear();
    }
protected:
    template<typename F, typename ... V>
    auto call(F f, V && ... v) {
        actions_.push_back([this, f, v...] {
            f(*this, v...);
        });
    }
private:
    std::vector<std::function<void()>> actions_;
};

#endif //ADAPTER_BASEBATCH_H
