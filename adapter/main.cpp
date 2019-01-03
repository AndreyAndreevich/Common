#include <iostream>
#include "include/Adapter.h"
#include "include/BaseLocker.h"
#include "include/BaseShared.h"
#include "include/BaseLazy.h"
#include "include/BaseBatch.h"

struct Data {
    Data() {
        std::cout << __FUNCTION__ << std::endl;
    }
    int get() const {
        return val_;
    }
    void set(int v) {
        val_ = v;
    }
    int out() const {
        return val_;
    }
private:
    int val_ = 0;
};

DECL_ADAPTER(Data, get, set)

template<typename T, typename T_locker = std::mutex, typename T_base = T>
using AdaptedLocked = Adapter<T, BaseLocker<T_base, T_locker>>;
using DataLocked = AdaptedLocked<Data>;

template<typename T, typename T_base = T>
using AdaptedShared = Adapter<T, BaseShared<T>>;
using DataShared = AdaptedShared<Data>;

template<typename T, typename T_locker = std::mutex, typename T_base = T>
using AdaptedSharedLocked = AdaptedShared<T, AdaptedLocked<T, T_locker, T_base>>;
using DataTotallyLocked = AdaptedSharedLocked<Data>;

template<typename T>
using AdapterLazy = Adapter<T, BaseLazy<T>>;
using DataLazy = AdapterLazy<Data>;

template<typename T>
using AdapterBatch = Adapter<T, BaseBatch<T>>;
using DataBatch = AdapterBatch<Data>;

int main() {
    DataLocked lc;
    lc.set(5);
    std::cout << "DataLocked: " << lc.get() << std::endl;

    DataShared sh;
    auto new_sh = sh;
    sh.set(10);
    std::cout << "DataShared: " << sh.get() << " " << new_sh.get() << std::endl;

    DataTotallyLocked dtl;
    auto new_dtl = dtl;
    new_dtl.set(15);
    std::cout << "DataTotallyLocked: " << new_dtl.get() << " " << dtl.get() << std::endl;

    DataLazy lz;
    std::cout << "DataLazy" << std::endl;
    lz.set(20);
    std::cout << "DataLocked: " << lc.get() << std::endl;

    DataBatch db;
    db.set(25);
    std::cout << "DataBatch: " << db.out() << std::endl;
    db.execute();
    std::cout << "DataBatch: " << db.out() << std::endl;

    return 0;
}