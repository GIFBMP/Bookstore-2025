#include "bookstore.h"
#include <iomanip>
#include <unordered_map>
namespace gifbmp {
    
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
        int len = strlen(bk.keyword.s);
        std::string str = "";
        for (int i = 0; i < len; i++) {
            if (bk.keyword.s[i] == '|') {
                book_keyword.del(str, bk);
                str = "";
            }
            else str += bk.keyword.s[i];
        }
        if (str != "") {
            book_keyword.del(str, bk);
        }
        str = "";

        book_isbn.ins(newbk.ISBN, newbk);
        book_name.ins(newbk.name, newbk);
        book_author.ins(newbk.author, newbk);
        len = strlen(newbk.keyword.s);
        for (int i = 0; i < len; i++) {
            if (newbk.keyword.s[i] == '|') {
                //std::cerr << "bookisbn:" << newbk.ISBN << ",keyword:" << str << "\n";
                book_keyword.ins(str, newbk);
                str = "";
            }
            else str += newbk.keyword.s[i];
        }
        if (str != "") {
            book_keyword.ins(str, newbk);
        }
    }
    void buy(const Index20 &isbn, int cnt) {
        if (nw_user.privilege < 1 || cnt <= 0) {
            invalid_oper();
            return;
        }
        std::vector<Book> v = book_isbn.query(isbn);
        // if (!v.empty()) {
        //     //std::cout << "cnt:" << cnt << ",remain:" << v[0].cnt << '\n';
        // }
        if (v.empty() || cnt > v[0].cnt) {
            invalid_oper();
            return;
        }
        Book tmp = v[0];
        tmp.cnt -= cnt;
        upd(v[0], tmp);
        finance profit = finance(tmp.price * cnt, 0.0);
        std::cout << std::fixed << std::setprecision(2) << tmp.price * cnt << '\n';
        finance_log.write(profit);
        double inc;
        profits.read(inc, 1);
        inc += tmp.price * cnt;
        profits.update(inc, 1);
        for (int i = tp; i; i--) {
            if (!is_selected[i]) continue;
            if (selectedbook[i].ISBN == isbn)
                selectedbook[i] = tmp;
        }
    }
    void select(const Index20 &isbn) {
        if (nw_user.privilege < 3) {
            invalid_oper();
            return;
        }
        is_selected[tp] = 1;
        std::vector<Book> v = book_isbn.query(isbn);
        std::cerr << "now_selected " << "isbn:" << isbn << '\n';
        if (v.empty()) {
            selectedbook[tp] = Book();
            selectedbook[tp].ISBN = isbn;
            book_isbn.ins(isbn, selectedbook[tp]);
            return;
        }
        selectedbook[tp] = v[0];
    }
    void modify(const Index20 &isbn, const Index60 &name, const Index60 &author, 
                const Index60 &keyword, double price, bool has_price) {
        if (nw_user.privilege < 3 || !is_selected[tp]) {
            invalid_oper();
            return;
        }
        
        if (!isbn.empty()) {
            std::vector<Book> v = book_isbn.query(isbn);
            if (!v.empty()) {
                invalid_oper();
                return;
            }
        }
        
        if (!keyword.empty()) {
            std::string str = "";
            int len = strlen(keyword.s);
            bool fl = false;
            std::unordered_map<std::string, bool> p;
            for (int i = 0; i < len; i++) {
                if (keyword.s[i] == '|') {
                    if (p.count(str)) {
                        fl = true;
                        break;
                    }
                    p[str] = 1;
                    str = "";
                }
                else str += keyword.s[i];
            }
            if (str != "") {
                if (p.count(str)) fl = true;
            }
            if (fl == true) {
                invalid_oper();
                return;
            }
        }
        Book tmp = selectedbook[tp], tmp2 = tmp;
        if (!isbn.empty()) tmp.ISBN = isbn;
        if (!name.empty()) tmp.name = name;
        if (!author.empty()) tmp.author = author;
        if (!keyword.empty()) tmp.keyword = keyword;
        if (has_price == true) tmp.price = price;
        upd(selectedbook[tp], tmp);
        for (int i = tp; i; i--) {
            if (!is_selected[i]) continue;
            if (selectedbook[i] == tmp2)
                selectedbook[i] = tmp;
        }
    }
    void import(int cnt, double totalcost) {
        if (nw_user.privilege < 3 || !is_selected[tp] || cnt <= 0 || totalcost <= 0.0) {
            invalid_oper();
            return;
        }
        std::cerr << "import isbn:" << selectedbook[tp].ISBN << ",cnt:" << cnt << '\n'; 
        Book tmp = selectedbook[tp], tmp2 = tmp;
        tmp.cnt += cnt;
        finance cost = finance(0.0, totalcost);
        finance_log.write(cost);
        double ouc;
        profits.read(ouc, 2);
        ouc += totalcost;
        profits.update(ouc, 2);
        upd(selectedbook[tp], tmp);
        for (int i = tp; i; i--) {
            if (!is_selected[i]) continue;
            if (selectedbook[i] == tmp2)
                selectedbook[i] = tmp;
        }
    }
}