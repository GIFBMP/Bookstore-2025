#include "bookstore.h"
namespace gifbmp {
    //#define TEST
    #ifndef TEST
    /*
    formal version
    */
    BlockList<Index30, userindata> list_of_users("userlist", 0);
    BlockList<Index30, int> login_cnt("login_cnt", 1);//clear the login_cnt
    #else
    /*
    test version
    */
    BlockList<Index30, userindata> list_of_users("userlist", 1);
    BlockList<Index30, int> login_cnt("login_cnt", 1);//clear the login_cnt
    #endif

    Index30 loginstack[kMaxn];
    int tp = 0;
    userindata nw_user = userindata();
    void invalid_oper() {
        std::cout << "Invalid\n";
        return;
    }
    void initialise() {
        Index30 userid = (string)"root";
        std::vector<userindata> v = list_of_users.query(userid);
        std::cerr << "root size:" << v.size() << '\n';
        if (v.empty()) {
            std::cerr << "try to create root\n";
            userindata tmp;
            tmp.password = (string)"sjtu";
            tmp.privilege = 7;
            list_of_users.ins(userid, tmp);
            login_cnt.ins(userid, 0);
        }
        if (profits.size() == 0) {
            double a = 0.0;
            profits.write(a);
            profits.write(a);
        }
    }
    #ifndef TEST
    /*
    formal version
    */
    BlockList<Index20, Book> book_isbn("book_isbn", 0);
    BlockList<Index60, Book> book_name("book_name", 0);
    BlockList<Index60, Book> book_author("book_author", 0);
    BlockList<Index60, Book> book_keyword("book_keyword", 0);

    DataSaver<finance, 0> finance_log("finance_log", 0);//reserve
    DataSaver<double, 0> profits("profit", 0);
    #else
    /*
    test version
    */
    BlockList<Index20, Book> book_isbn("book_isbn", 1);
    BlockList<Index60, Book> book_name("book_name", 1);
    BlockList<Index60, Book> book_author("book_author", 1);
    BlockList<Index60, Book> book_keyword("book_keyword", 1);

    DataSaver<finance, 0> finance_log("finance_log", 1);//reserve
    DataSaver<double, 0> profits("profit", 1);
    #endif
}