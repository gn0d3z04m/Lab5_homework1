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

TEST(Transaction, SuccessfulTransaction) {
    Transaction tr;
    tr.set_fee(10); // Устанавливаем комиссию
    Account from(1, 1000);
    Account to(2, 200);

    // Успешный перевод: баланс > суммы + комиссия
    EXPECT_TRUE(tr.Make(from, to, 500));
    tr.Make(from, to, 500);
    // Проверяем изменения балансов
    EXPECT_EQ(from.GetBalance(), 490);   // 1000 - (500 + 10)
    EXPECT_EQ(to.GetBalance(), 700);     // 200 + 500
}

