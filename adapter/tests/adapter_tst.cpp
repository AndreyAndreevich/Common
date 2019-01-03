#define BOOST_TEST_LOG_LEVEL test_suite
#define BOOST_TEST_MODULE test_main
#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>

#include "Adapter.h"

class Data {
public:
    void set(int v) {
        v_ = v;
    }
    int get() const {
        return v_;
    }
private:
    int v_ = 0;
};

class Data1 : public Data {};
class Data2 : public Data {};

DECL_ADAPTER(Data1, set, get)
DECL_ADAPTER(Data2, set)

struct SimpleWrapper : public Data {
    int test() const {
        return count_;
    }
protected:
    template<typename F, typename ... V>
    auto call(F f, V && ... v) {
        count_++;
        return f(static_cast<Data&>(*this), std::forward<V>(v)...);
    }
private:
    int count_ = 0;
};

using Adapter1 = Adapter<Data1, SimpleWrapper>;
using Adapter2 = Adapter<Data2, SimpleWrapper>;

BOOST_AUTO_TEST_SUITE(Adapter_tst)

BOOST_AUTO_TEST_CASE(Difference)
{
    Adapter1 d1;
    d1.set(10);
    BOOST_CHECK_EQUAL(d1.get(), 10);
    BOOST_CHECK_EQUAL(d1.test(), 2);

    Adapter2 d2;
    d2.set(11);
    BOOST_CHECK_EQUAL(d2.get(), 11);
    BOOST_CHECK_EQUAL(d2.test(), 1);
}

BOOST_AUTO_TEST_SUITE_END()