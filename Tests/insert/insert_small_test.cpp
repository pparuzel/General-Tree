#include "gtest/gtest.h"
#include "../../GeneralTree/GeneralTree.hpp"


class InsertFixture : public testing::Test
{
protected:
    rcn::GeneralTree<float>* f0tree;
    rcn::GeneralTree<float>* f1tree;
    rcn::GeneralTree<float>* f2tree;

    void SetUp() override {
        f0tree = new rcn::GeneralTree<float>{};
        f1tree = new rcn::GeneralTree<float>{2.03};
        f2tree = new rcn::GeneralTree<float>{3.14};
    }

    void vinsert_stress_test()
    {
        auto old_size = f0tree->size();
        auto it = f0tree->begin();
        for (long i = 0l; i < 1e6; ++i)
        {
            it = f0tree->insert(i, it, 0);
        }
        ASSERT_EQ(f0tree->size(), old_size+1e6);
    }

    void hinsert_stress_test()
    {
        auto it = f1tree->begin();
        for (long i = 0l; i < 1e6; ++i)
        {
            f1tree->insert(i, it, i);
        }
    }

    void TearDown() override {
        delete f0tree;
        delete f1tree;
        delete f2tree;
    }
};

TEST_F(InsertFixture, SmallInserts)
{
    hinsert_stress_test();
    vinsert_stress_test();
}
