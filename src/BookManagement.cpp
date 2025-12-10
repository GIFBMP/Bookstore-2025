#include "bookstore.h"
namespace gifbmp {
    bool is_selected = 0;
    Book selectedbook;
    std::ostream& operator << (std::ostream &out, Book x) {
        out << x.ISBN << '\t' << x.name << '\t' << x.author << '\t' << x.keyword << '\t'\
            << std::fixed << std::setprecision(2) << x.price << '\t' << x.cnt;
        return out;
    }
    void showall() {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        book_isbn.printall();
    }
    void showisbn(const Index20 &isbn) {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_isbn.query(isbn);
        if (v.empty()) {
            std::cout << '\n';
            return;
        }
        for (auto x : v) std::cout << x << '\n';
    }
    void showname(const Index60 &name) {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_name.query(name);
        if (v.empty()) {
            std::cout << '\n';
            return;
        }
        for (auto x : v) std::cout << x << '\n';
    }
    void showauthor(const Index60 &author) {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_author.query(author);
        if (v.empty()) {
            std::cout << '\n';
            return;
        }
        for (auto x : v) std::cout << x << '\n';
    }
    void showkeyword(const Index60 &keyword) {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        bool fl = false;
        for (int i = 0; i < 60; i++)
            if (keyword.s[i] == '|') {
                fl = true;
                break;
            }
        if (fl == true) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_keyword.query(keyword);
        if (v.empty()) {
            std::cout << '\n';
            return;
        }
        int len = v.size();
        for (int i = 0; i < len; i++) {
            if (i == 0 || v[i].ISBN != v[i - 1].ISBN)
                std::cout << v[i] << '\n';
        }
    }
    void upd (const Book &bk, const Book &newbk) {
        book_isbn.del(bk.ISBN, bk);
        book_name.del(bk.name, bk);
        book_author.del(bk.author, bk);
        std::vector<std::string> s;
        int len = strlen(bk.keyword.s);
        std::string str = "";
        for (int i = 0; i < len; i++) {
            if (bk.keyword.s[i] == '|') {
                book_keyword.del(str, bk);
                s.push_back (str);
                str = "";
            }
            else str += bk.keyword.s[i];
        }
        if (str != "") {
            book_keyword.del(str, bk);
            s.push_back(str);
        }
        str = "";
        book_isbn.ins(bk.ISBN, newbk);
        book_name.ins(bk.name, newbk);
        book_author.ins(bk.author, newbk);
        for (auto tmp : s)
            book_keyword.ins(tmp, newbk);
    }
    void buy(const Index20 &isbn, int cnt) {
        if (nw_user.privilege < 1 || cnt <= 0) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_isbn.query(isbn);
        if (v.empty() || cnt > v[0].cnt) {
            invalid_oper();
            return;
        }
        Book tmp = v[0];
        tmp.cnt -= cnt;
        upd(v[0], tmp);
    }
    void select(const Index20 &isbn) {
        if (nw_user.privilege < 3) {
            invalid_oper();
            return;
        }
        is_selected = 1;
        std::vector<Book> v = book_isbn.query(isbn);
        if (v.empty()) {
            selectedbook = Book();
            selectedbook.ISBN = isbn;
            book_isbn.ins(isbn, selectedbook);
            return;
        }
        selectedbook = v[0];
    }
    void modify(const Index20 &isbn, const Index60 &name, const Index60 &author, 
                const Index60 &keyword, double price) {
        if (nw_user.privilege < 3) {
            invalid_oper();
            return;
        }
        
    }
    void import(int cnt, double totalcost) {
        if (nw_user.privilege < 3) {
            invalid_oper();
            return;
        }
    }
}