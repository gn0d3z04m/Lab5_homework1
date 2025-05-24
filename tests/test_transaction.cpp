#include "Transaction.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockTransaction : public Transaction {
public:
    MOCK_METHOD3(SaveToDataBase, void(Account&, Account&, int));
};

TEST(TransactionTest, MakeTransfer) {
    Account from(1, 500);
    Account to(2, 300);
    Transaction ts;
    
    from.Lock();
    to.Lock();
    
    ASSERT_TRUE(ts.Make(from, to, 150));
    ASSERT_EQ(from.GetBalance(), 500 - 150 - ts.fee());
    ASSERT_EQ(to.GetBalance(), 300 + 150);
}

TEST(TransactionTest, InvalidTransfer) {
    Account acc1(1, 100);
    Account acc2(1, 200);
    Transaction ts;
    
    ASSERT_THROW(ts.Make(acc1, acc2, 50), std::logic_error);
}
