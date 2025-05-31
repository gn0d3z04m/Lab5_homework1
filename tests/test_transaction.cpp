#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class AccountMock : public Account {
public:
	AccountMock(int id, int balance) : Account(id, balance) {}
	MOCK_CONST_METHOD0(GetBalance, int());
	MOCK_METHOD1(ChangeBalance, void(int diff));
	MOCK_METHOD0(Lock, void());
	MOCK_METHOD0(Unlock, void());
};

TEST(Transaction, Mock) {
	Transaction tr;
	AccountMock ac1(1, 50);
	AccountMock ac2(2, 500);
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

TEST(Transaction, Make_SavesToDatabaseCorrectly) {
    MockAccount from(1, 1000);
    MockAccount to(2, 500);
    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(from, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(to, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(from, GetBalance()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(1000));
    EXPECT_CALL(to, GetBalance()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(500));
    Transaction transaction;
    transaction.set_fee(10);
    EXPECT_TRUE(transaction.Make(from, to, 100));
}
