#include "bookstore.h"
using namespace gifbmp;
std::vector<string> proceed(const std::string &s) {
    std::vector<string> v;
    int len = s.size();
    string str = "";
    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') {
            v.push_back (str);
            str = "";
        }
        else str += s[i];
    }
    if (str != "") v.push_back (str);
    return v;
}
int stringtoint(const string &s) {
    int x = 0;
    int len = s.size();
    bool fl = false;
    if (s[0] == '-') fl = true;
    for (int i = 0; i < len; i++)
        if (s[i] >= '0' && s[i] <= '9')
            x = x * 10 + s[i] - '0';
    return x;
}
double stringtodouble(const string &s) {
    return std::stod(s);
}
int main() {
    std::vector<std::string> v;
    initialise();
    while (1) {
        std::string s;
        getline(std::cin, s);
        v = proceed(s);
        if (v.empty()) continue;
        else if (v[0] == "exit" || v[0] == "quit") break;
        else if (v[0] == "su") {
            Index30 userid, password;
            userid = v[1];
            if (v.size() > 2) password = v[2];
            else password = Index30();
            login(userid, password);
        }
        else if (v[0] == "logout") logout();
        else if (v[0] == "register") {
            user_register(v[1], v[2], v[3]);
        }
        else if (v[0] == "useradd") {
            useradd(v[1], v[2], stringtoint(v[3]), v[4]);
        }
        else if (v[0] == "delete") {
            deluser(v[1]);
        }
        else if (v[0] == "show") {
            string tmp;
            int len = v[1].size();
            if (v[1] == "finance") {//show finance
                if (v.size() > 2) show_finance(stringtoint(v[2]));
                else show_finance_all();
            }
            else if (v[1][1] == 'I') {//isbn
                for (int i = 6; i < len; i++) tmp += v[1][i];
                showisbn(tmp);
            }
            else if (v[1][1] == 'n') {//name
                for (int i = 7; i < len - 1; i++) tmp += v[1][i];
                showname(tmp);
            }
            else if (v[1][1] == 'a') {//author
                for (int i = 9; i < len - 1; i++) tmp += v[1][i];
                showauthor(tmp);
            }
            else if (v[1][1] == 'k') {//keyword
                for (int i = 10; i < len - 1; i++) tmp += v[1][i];
                showkeyword(tmp);
            }
        }
        else if (v[0] == "buy") {
            buy(v[1], stringtoint(v[2]));
        }
        else if (v[0] == "select") {
            select(v[1]);
        }
        else if (v[0] == "modify") {
            Index20 isbn;
            Index60 name, author, keyword;
            double price;
            string tmp;
            int len = v.size();
            bool fl = false, has_price = false;
            for (int i = 1; i < len; i++) {
                tmp = "";
                int sz = v[i].size();
                if (v[i][1] == 'I') {
                    if (!isbn.empty()) fl = true;
                    else {
                        for (int j = 6; j < sz; j++) tmp += v[i][j];
                        isbn = tmp;
                    }
                }
                else if (v[i][1] == 'n') {
                    if (!name.empty()) fl = true;
                    else {
                        for (int j = 7; j < sz - 1; j++) tmp += v[i][j];
                        name = tmp;
                    }
                }
                else if (v[i][1] == 'a') {
                    if (!author.empty()) fl = true;
                    else {
                        for (int j = 9; j < sz - 1; j++) tmp += v[i][j];
                        author = tmp;
                    }
                }
                else if (v[i][1] == 'k') {
                    if (!keyword.empty()) fl = true;
                    else {
                        for (int j = 10; j < sz - 1; j++) tmp += v[i][j];
                        keyword = tmp;
                    }
                }
                else if (v[i][1] == 'p') {
                    if (has_price) fl = true;
                    else {
                        for (int j = 7; j < sz; j++) tmp += v[i][j];
                        price = stringtodouble(tmp);
                        has_price = true;
                    }
                }
                if (fl == true) break;
            }
            if (fl == true) {
                invalid_oper();
                continue;
            }
            modify(isbn, name, author, keyword, price, has_price);
        }
        else if (v[0] == "log") {//todo

        }
        else if (v[0] == "report") {//todo

        }
    }
    return 0;
}