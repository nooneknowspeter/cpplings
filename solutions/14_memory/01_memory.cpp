// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Memory management in C++:
//
// Raw pointers:
// - Store memory addresses
// - Must manually allocate (new) and deallocate (delete)
// - Can lead to memory leaks and undefined behavior
//
// Smart pointers (C++11):
// - std::unique_ptr: exclusive ownership, automatic deletion
// - std::shared_ptr: shared ownership, reference counted
// - std::weak_ptr: non-owning reference to shared_ptr
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers/

#include <gtest/gtest.h>
#include <memory>

// TODO: Create a function that returns a unique_ptr to an int with value 42.
std::unique_ptr<int> createValue()
{
    return std::make_unique<int>(42);
}

// TODO: Create a function that takes ownership of a unique_ptr and returns its value.
int extractValue(std::unique_ptr<int> ptr)
{
    return *ptr;
}

// TODO: Use shared_ptr to share ownership of a value.
std::shared_ptr<int> createShared()
{
    return std::make_shared<int>(100);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Memory, Memory01)
{
    auto ptr = createValue();
    ASSERT_EQ(*ptr, 42);

    auto ptr2 = createValue();
    ASSERT_EQ(extractValue(std::move(ptr2)), 42);

    auto shared = createShared();
    ASSERT_EQ(*shared, 100);
    ASSERT_EQ(shared.use_count(), 1);

    auto shared2 = shared;
    ASSERT_EQ(shared.use_count(), 2);
}
