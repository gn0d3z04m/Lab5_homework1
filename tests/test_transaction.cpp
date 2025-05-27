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
