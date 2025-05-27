#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account&, Account&, int), (override));
};

TEST(TransactionTest, MakeThrowsOnSameAccount) {
    MockAccount acc1(1, 100), acc2(1, 200);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 150), std::logic_error);
}

TEST(TransactionTest, MakeValidatesSum) {
    MockAccount acc1(1, 1000), acc2(2, 500);
    Transaction tr;
    
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
    tr.set_fee(50);
    EXPECT_FALSE(tr.Make(acc1, acc2, 99)); // 50*2 > 99
}

TEST(TransactionTest, LockUnlockOrder) {
    MockAccount acc1(1, 1000), acc2(2, 500);
    Transaction tr;
    
    EXPECT_CALL(acc1, Lock()).Times(1);
    EXPECT_CALL(acc2, Lock()).Times(1);
    EXPECT_CALL(acc1, Unlock()).Times(1);
    EXPECT_CALL(acc2, Unlock()).Times(1);
    
    tr.set_fee(10);
    tr.Make(acc1, acc2, 200);
}

TEST(TransactionTest, DebitFailureRollback) {
    MockAccount from(1, 300), to(2, 500);
    Transaction tr;
    tr.set_fee(10);
    
    ON_CALL(from, GetBalance()).WillByDefault(testing::Return(300));
    EXPECT_CALL(to, ChangeBalance(200)).Times(1);
    EXPECT_CALL(to, ChangeBalance(-200)).Times(1); // Rollback
    
    ASSERT_FALSE(tr.Make(from, to, 200)); // 200 + 10 = 210 > 300?
}

TEST(TransactionTest, SuccessfulTransaction) {
    MockAccount from(1, 1000), to(2, 500);
    MockTransaction tr;
    
    EXPECT_CALL(tr, SaveToDataBase(testing::_, testing::_, 800)).Times(1);
    EXPECT_CALL(from, ChangeBalance(-810)).Times(1);
    EXPECT_CALL(to, ChangeBalance(800)).Times(1);
    
    tr.set_fee(10);
    ASSERT_TRUE(tr.Make(from, to, 800));
}

TEST(TransactionTest, DatabaseSaveCalled) {
    MockAccount from(1, 2000), to(2, 1500);
    MockTransaction tr;
    
    EXPECT_CALL(tr, SaveToDataBase(_, _, 1000)).Times(1);
    
    tr.set_fee(10);
    tr.Make(from, to, 1000);
}
