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

TEST(Account, Mock) {
	AccountMock acc(1, 100);
	EXPECT_CALL(acc, GetBalance()).Times(1);
	EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(2);
	EXPECT_CALL(acc, Lock()).Times(2);
	EXPECT_CALL(acc, Unlock()).Times(1);
	acc.GetBalance();
	acc.ChangeBalance(100);
	acc.Lock();
	acc.ChangeBalance(100);
	acc.Lock();
	acc.Unlock();
}

TEST(Account, SimpleTest) {
	Account acc(1, 100);
	EXPECT_EQ(acc.id(), 1);
	EXPECT_EQ(acc.GetBalance(), 100);
	EXPECT_THROW(acc.ChangeBalance(100), std::runtime_error);
	EXPECT_NO_THROW(acc.Lock());
	acc.ChangeBalance(100);
	EXPECT_EQ(acc.GetBalance(), 200);
	EXPECT_THROW(acc.Lock(), std::runtime_error);
}
