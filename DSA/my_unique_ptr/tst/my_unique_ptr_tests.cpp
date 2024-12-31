#include "include.h"
#include "my_unique_ptr.hpp"

using namespace DSA;

struct Foo
{
    ~Foo()
    {
        std::cout << "Deleted Foo!\n";
    }
};

TEST(unique_ptr, simple)
{
    DSA::unique_ptr<Foo> up(new Foo());
    //auto p = up;
}

TEST(unique_ptr, operator_bool)
{
    DSA::unique_ptr<Foo> up(new Foo());

    if (up)
    {
        ASSERT_TRUE(true);
        return;
    }
    ASSERT_TRUE(false);
}