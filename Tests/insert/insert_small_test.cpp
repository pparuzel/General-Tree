#include "gtest/gtest.h"
#include "../../GeneralTree/GeneralTree.hpp"

class InsertFixture : public testing::Test
{
protected:
    rcn::GeneralTree<float>* f0tree;
    rcn::GeneralTree<float>* f1tree;
    rcn::GeneralTree<float>* f2tree;
    virtual void SetUp()
    {
        f0tree = new rcn::GeneralTree<float>{};
        f1tree = new rcn::GeneralTree<float>{2.03};
        f2tree = new rcn::GeneralTree<float>{3.14};
    }

    void vertical_stress_test()
    {
        auto it = f0tree->begin();
        for (long i = 0l; i < 1e6; ++i)
        {
            it = f0tree->insert(i, it, 0);
        }
        ASSERT_EQ(f0tree->size(), 1e6);
    }

    virtual void TearDown()
    {
        delete f0tree;

    }
};

TEST_F(InsertFixture, SmallInserts)
{
    vertical_stress_test();
}