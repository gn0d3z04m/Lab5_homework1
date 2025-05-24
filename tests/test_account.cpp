#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

TEST(AccountTest, LockUnlock) {
    Account acc(1, 100);
    acc.Lock();
    ASSERT_NO_THROW(acc.ChangeBalance(10));
    acc.Unlock();
    ASSERT_THROW(acc.ChangeBalance(10), std::runtime_error);
}

TEST(AccountTest, GetBalance) {
    Account acc(1, 200);
    ASSERT_EQ(acc.GetBalance(), 200);
}
