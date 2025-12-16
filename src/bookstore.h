#pragma once
#ifndef BOOKSTORE_H
#define BOOKSTORE_H
#include "blocklist.h"
#include "datasaver.h"
#include <iomanip>
namespace gifbmp {
    using std::string;
    const int kMaxn = 1e5 + 10;
    template<int len>
    struct Index {
        char s[len + 1];
        Index() {
            for (int i = 0; i <= len; i++) s[i] = '\0';
        }
        Index(const Index &t) {
            for (int i = 0; i <= len; i++)
                s[i] = t.s[i];
        }
        Index(const string &str) {
            int sz = str.size();
            for (int i = 0; i < sz; i++) s[i] = str[i];
            for (int i = sz; i <= len; i++) s[i] = 0;
        }
        bool empty() const {
            return s[0] == '\0';
        }
        bool operator < (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] < t.s[i]) return true;
                else if (s[i] > t.s[i]) return false;
            return false;
        }
        bool operator <= (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] < t.s[i]) return true;
                else if (s[i] > t.s[i]) return false;
            return true;
        }
        bool operator == (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] != t.s[i]) return false;
            return true;
        }
        bool operator != (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] != t.s[i]) return true;
            return false;
        }
        
    };
    template<int len>
    std::istream& operator >> (std::istream &in, Index<len> x) {
        in >> x.s;
        return in;
    }
    template<int len>
    std::ostream& operator << (std::ostream &out, Index<len> x) {
        out << x.s;
        return out;
    }
    using Index10 = Index<10>;
    using Index20 = Index<20>;
    using Index30 = Index<30>;
    using Index60 = Index<60>;
    class User {
        Index30 userid, username, password;
        int privilege;
        bool operator < (const User &t) const {
            return userid < t.userid;
        }
    };
    struct userindata {
        Index30 password;
        int privilege;
        userindata() {
            password = Index30("");
            privilege = 0;
        }
        bool operator < (const userindata &t) const {
            return password < t.password;
        }
        bool operator <= (const userindata &t) const {
            return password <= t.password;
        }
        bool operator == (const userindata &t) const {
            return password == t.password;
        }
    };
    void invalid_oper();
    extern BlockList<Index30, userindata> list_of_users;
    extern BlockList<Index30, int> login_cnt;//clear the login_cnt
    extern Index30 loginstack[kMaxn];
    extern int tp;
    extern userindata nw_user;
    void initialise();

    void user_register(Index30, Index30, Index30);
    void login(Index30, Index30);
    void logout();
    void chgpassword(Index30, Index30, Index30);
    void useradd(Index30, Index30, int, Index30);
    void deluser(Index30);

    struct Book {
        Index20 ISBN;
        Index60 name, author;
        Index60 keyword;
        int cnt;
        long double price;
        Book() {
            ISBN = Index20("");
            name = author = Index60("");
            keyword = Index60("");
            cnt = 0;
            price = 0.0;
        }
        bool operator < (const Book &t) const {
            return ISBN < t.ISBN;
        }
        bool operator == (const Book &t) const {
            return ISBN == t.ISBN;
        }
        bool operator <= (const Book &t) const {
            return ISBN <= t.ISBN;
        }
    };
    std::ostream& operator << (std::ostream &, Book);
    extern BlockList<Index20, Book> book_isbn;
    extern BlockList<Index60, Book> book_name;
    extern BlockList<Index60, Book> book_author;
    extern BlockList<Index60, Book> book_keyword;
    extern bool is_selected[kMaxn];
    extern Book selectedbook[kMaxn];
    void showall();
    void showisbn(const Index20 &);
    void showname(const Index60 &);
    void showauthor(const Index60 &);
    void showkeyword(const Index60 &);
    void buy(const Index20 &, int);
    void select(const Index20 &);
    void modify(const Index20 &, const Index60 &, \
                const Index60 &, const Index60 &, double, bool);
    void import(int, double);
    
    struct finance {
        long double income, outcome;
        finance(long double income = 0.0, long double outcome = 0.0) :
            income(income), outcome(outcome) {}
    };
    extern DataSaver<finance, 0> finance_log;//reserve
    extern DataSaver<long double, 0> profits;
    void show_finance_all();
    void show_finance(int);
}
#endif