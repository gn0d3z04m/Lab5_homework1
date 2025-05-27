#include "Account.h"
#include <gtest/gtest.h>

TEST(AccountTest, ConstructorInitializesValues) {
    Account acc(123, 500);
    EXPECT_EQ(acc.id(), 123);
    EXPECT_EQ(acc.GetBalance(), 500);
}

TEST(AccountTest, LockUnlockBehavior) {
    Account acc(1, 100);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(10));
    acc.Unlock();
    
    EXPECT_THROW(acc.ChangeBalance(10), std::runtime_error);
}

TEST(AccountTest, BalanceOperations) {
    Account acc(1, 200);
    acc.Lock();
    acc.ChangeBalance(-50);
    EXPECT_EQ(acc.GetBalance(), 150);
    acc.ChangeBalance(100);
    EXPECT_EQ(acc.GetBalance(), 250);
}
