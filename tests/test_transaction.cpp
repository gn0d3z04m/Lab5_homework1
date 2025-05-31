#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

TEST(Transaction, Mock) {
	Transaction tr;
	MockAccount ac1(1, 50);
	MockAccount ac2(2, 500);
	EXPECT_CALL(ac1, ChangeBalance(testing::_)).Times(2);
	EXPECT_CALL(ac1, Lock()).Times(1);
	EXPECT_CALL(ac1, Unlock()).Times(1);
	EXPECT_CALL(ac2, Lock()).Times(1);
	EXPECT_CALL(ac2, Unlock()).Times(1);
	tr.set_fee(100);
	tr.Make(ac2, ac1, 300);
}

TEST(Transaction, SimpleTest) {
	Transaction tr;
	Account ac1(1, 50);
	Account ac2(2, 500);
	tr.set_fee(100);
	EXPECT_EQ(tr.fee(), 100);
	EXPECT_THROW(tr.Make(ac1, ac1, 0), std::logic_error);
	EXPECT_THROW(tr.Make(ac1, ac2, -1), std::invalid_argument);
	EXPECT_THROW(tr.Make(ac1, ac2, 99), std::logic_error);
	EXPECT_FALSE(tr.Make(ac1, ac2, 199));
	EXPECT_FALSE(tr.Make(ac2, ac1, 500));
	EXPECT_FALSE(tr.Make(ac2, ac1, 300));
}

TEST(Transaction, Make_ReturnsFalseIfInsufficientFunds) {
    MockAccount from(1, 100);
    MockAccount to(2, 500);  
    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(from, GetBalance())
        .WillOnce(Return(100)) 
        .WillOnce(Return(100)); 
    EXPECT_CALL(to, GetBalance())
        .WillOnce(Return(500));
    EXPECT_CALL(to, ChangeBalance(200)).Times(1);      
    EXPECT_CALL(from, ChangeBalance(-200 - 10)).Times(0);  
    EXPECT_CALL(to, ChangeBalance(-200)).Times(1);      

    Transaction transaction;
    transaction.set_fee(10); 
    EXPECT_FALSE(transaction.Make(from, to, 200));
}
