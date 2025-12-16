#include "bookstore.h"
using namespace gifbmp;
std::vector<string> proceed(const std::string &s) {
    std::vector<string> v;
    int len = s.size();
    string str = "";
    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') {
            if (str != "") v.push_back (str);
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
    if (fl) x = -x;
    return x;
}
bool checkint(const string &s) {
    int len = s.size();
    if (len > 10) return false;
    for (int i = 0; i < len; i++) {
        if ((s[i] < '0' || s[i] > '9') && (i > 0 || s[i] != '-'))
            return false;
    }
    if (len > 1 && s[0] == '0') return false;
    if (len > 2 && s[0] == '-' && s[1] == '0') return false;
    return true;
}
bool checkdouble(const string &s) {
    int len = s.size(), cnt = 0;
    if (len > 13) return false;
    int poi_pos = len + 1;
    for (int i = 0; i < len; i++) {
        if (s[i] == '.') {
            if (i == 0 || (i == 1 && s[0] == '-')) return false;
            else {
                cnt++;
                poi_pos = i;
                if (cnt > 1) return false;
            }
        }
        else if ((s[i] < '0' || s[i] > '9') && (i > 0 || s[i] != '-'))
            return false;
    }
    if (len > 1 && s[0] == '0' && poi_pos > 1) return false;
    if (len > 2 && s[0] == '-' && s[1] == '0' && poi_pos > 2) return false;
    return true;
}
double stringtodouble(const string &s) {
    return std::stod(s);
}
bool checkpwd(const string &s) {
    int len = s.size();
    if (len > 30) return false;
    for (int i = 0; i < len; i++)
        if (s[i] >= '0' && s[i] <= '9') continue;
        else if (s[i] >= 'a' && s[i] <= 'z') continue;
        else if (s[i] >= 'A' && s[i] <= 'Z') continue;
        else if (s[i] == '_') continue;
        else return false;
    return true;
}
bool checkname(const string &s) {
    int len = s.size();
    if (len > 60) return false;
    for (int i = 0; i < len; i++)
        if (s[i] == '\"') return false;
    return true;
}
bool checkkeyword(const string &s) {
    if (!checkname(s)) return false;
    int len = s.size();
    if (s[0] == '|' || s[len - 1] == '|') return false;
    for (int i = 0; i < len; i++)
        if (s[i] == '|' && s[i + 1] == '|') return false;
    return true;
}
int main() {
    std::vector<std::string> v;
    //std::cerr << "clear ends\n";
    initialise();
    std::string s;
    while (getline(std::cin, s)) {
        //std::cerr << "***";
        //if (s[0] == 'b')std::cout << s << '\n';
        v = proceed(s);
        if (v.empty()) continue;
        else if (v[0] == "exit" || v[0] == "quit") {
            if (v.size() != 1) {
                invalid_oper();
                continue;
            }
            break;
        }
        else if (v[0] == "su") {
            if (v.size() < 2 || v.size() > 3) {
                invalid_oper();
                continue;
            }
            Index30 userid, password;
            if (!checkpwd(v[1])) {
                //std::cerr << "???";
                invalid_oper();
                continue;
            }
            userid = v[1];
            if (v.size() == 3) {
                if (!checkpwd(v[2])) {
                    invalid_oper();
                    continue;
                }
                password = v[2];
            }
            else password = Index30();
            login(userid, password);
        }
        else if (v[0] == "logout") {
            if (v.size() != 1) {
                invalid_oper();
                continue;
            }
            logout();
        }
        else if (v[0] == "passwd") {
            if (v.size() < 3 || v.size() > 4) {
                invalid_oper();
                continue;
            }
            Index30 current_password, new_password;
            if (!checkpwd(v[1])) {
                invalid_oper();
                continue;
            }
            if (!checkpwd(v[2])) {
                invalid_oper();
                continue;
            }
            if (v.size() == 4 && !checkpwd(v[3])) {
                invalid_oper();
                continue;
            }
            if (v.size() == 3) new_password = v[2];
            else if (v.size() == 4) current_password = v[2], new_password = v[3];
            chgpassword(v[1], current_password, new_password);
        }
        else if (v[0] == "register") {
            if (v.size() != 4) {
                invalid_oper();
                continue;
            }
            if (!checkpwd(v[1]) || !checkpwd(v[2]) || v[3].size() > 30) {
                invalid_oper();
                continue;
            }
            user_register(v[1], v[2], v[3]);
        }
        else if (v[0] == "useradd") {
            if (v.size() != 5) {
                invalid_oper();
                continue;
            }
            if (!checkpwd(v[1]) || !checkpwd(v[2]) || !checkint(v[3]) || v[4].size() > 30) {
                invalid_oper();
                continue;
            }
            useradd(v[1], v[2], stringtoint(v[3]), v[4]);
        }
        else if (v[0] == "delete") {
            if (v.size() != 2) {
                invalid_oper();
                continue;
            }
            if (!checkpwd(v[1])) {
                invalid_oper();
                continue;
            }
            deluser(v[1]);
        }
        else if (v[0] == "show") {
            if (v.size() == 1) {
                book_isbn.printall();
                continue;
            }
            string tmp;
            int len = v[1].size();
            if (v[1] == "finance") {//show finance
                if (v.size() > 3) {
                    invalid_oper();
                    continue;
                }
                if (v.size() == 3) {
                    if (!checkint(v[2])) {
                        invalid_oper();
                        continue;
                    }
                    show_finance(stringtoint(v[2]));
                }
                else show_finance_all();
            }
            else if (v.size() != 2) {
                invalid_oper();
                continue;
            }
            else if (len < 2) {
                invalid_oper();
                continue;
            }
            else if (v[1][1] == 'I') {//isbn
                string ord = v[1].substr(0, 6);
                if (ord != "-ISBN=") {
                    invalid_oper();
                    continue;
                }
                for (int i = 6; i < len; i++) tmp += v[1][i];
                if (tmp.size() > 20) {
                    invalid_oper();
                    continue;
                }
                showisbn(tmp);
            }
            else if (v[1][1] == 'n') {//name
                string ord = v[1].substr(0, 7);
                if (ord != "-name=\"" || v[1][len - 1] != '\"') {
                    invalid_oper();
                    continue;
                }
                for (int i = 7; i < len - 1; i++) tmp += v[1][i];
                if (!checkname(tmp)) {
                    invalid_oper();
                    continue;
                }
                showname(tmp);
            }
            else if (v[1][1] == 'a') {//author
                string ord = v[1].substr(0, 9);
                if (ord != "-author=\"" || v[1][len - 1] != '\"') {
                    invalid_oper();
                    continue;
                }
                for (int i = 9; i < len - 1; i++) tmp += v[1][i];
                if (!checkname(tmp)) {
                    invalid_oper();
                    continue;
                }
                showauthor(tmp);
            }
            else if (v[1][1] == 'k') {//keyword
                string ord = v[1].substr(0, 10);
                if (ord != "-keyword=\"" || v[1][len - 1] != '\"') {
                    invalid_oper();
                    continue;
                }
                for (int i = 10; i < len - 1; i++) tmp += v[1][i];
                if (!checkkeyword(tmp)) {
                    invalid_oper();
                    continue;
                }
                showkeyword(tmp);
            }
            else {
                invalid_oper();
                continue;
            }
        }
        else if (v[0] == "buy") {
            if (v.size() != 3) {
                invalid_oper();
                continue;
            }
            if (v[1].size() > 20) {
                invalid_oper();
                continue;
            }
            if (!checkint(v[2])) {
                invalid_oper();
                continue;
            }
            buy(v[1], stringtoint(v[2]));
        }
        else if (v[0] == "select") {
            if (v.size() != 2) {
                invalid_oper();
                continue;
            }
            if (v[1].size() > 20) {
                invalid_oper();
                continue;
            }
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
                if (sz < 2) {
                    fl = true;
                    break;
                }
                else if (v[i][1] == 'I') {
                    string ord = v[i].substr(0, 6);
                    if (ord != "-ISBN=") {
                        fl = true;
                        break;
                    }
                    if (!isbn.empty()) fl = true;
                    else {
                        for (int j = 6; j < sz; j++) tmp += v[i][j];
                        isbn = tmp;
                    }
                }
                else if (v[i][1] == 'n') {
                    string ord = v[i].substr(0, 7);
                    if (ord != "-name=\"" || v[i][sz - 1] != '\"') {
                        fl = true;
                        break;
                    }
                    if (!name.empty()) fl = true;
                    else {
                        for (int j = 7; j < sz - 1; j++) tmp += v[i][j];
                        if (!checkname(tmp)) {
                            fl = true;
                            break;
                        }
                        name = tmp;
                    }
                }
                else if (v[i][1] == 'a') {
                    string ord = v[i].substr(0, 9);
                    if (ord != "-author=\"" || v[i][sz - 1] != '\"') {
                        fl = true;
                        break;
                    }
                    if (!author.empty()) fl = true;
                    else {
                        for (int j = 9; j < sz - 1; j++) tmp += v[i][j];
                        if (!checkname(tmp)) {
                            fl = true;
                            break;
                        }
                        author = tmp;
                    }
                }
                else if (v[i][1] == 'k') {
                    string ord = v[i].substr(0, 10);
                    if (ord != "-keyword=\"" || v[i][sz - 1] != '\"') {
                        fl = true;
                        break;
                    }
                    if (!keyword.empty()) fl = true;
                    else {
                        for (int j = 10; j < sz - 1; j++) tmp += v[i][j];
                        if (!checkkeyword(tmp)) {
                            fl = true;
                            break;
                        }
                        keyword = tmp;
                    }
                }
                else if (v[i][1] == 'p') {
                    string ord = v[i].substr(0, 7);
                    if (ord != "-price=") {
                        fl = true;
                        break;
                    }
                    if (has_price) fl = true;
                    else {
                        for (int j = 7; j < sz; j++) tmp += v[i][j];
                        if (!checkdouble(tmp)) {
                            fl = true;
                            break;
                        }
                        price = stringtodouble(tmp);
                        has_price = true;
                    }
                }
                else fl = true;
                if (fl == true) break;
            }
            if (fl == true) {
                invalid_oper();
                continue;
            }
            modify(isbn, name, author, keyword, price, has_price);
        }
        else if (v[0] == "import") {
            if (v.size() != 3) {
                invalid_oper();
                continue;
            }
            if (!checkint(v[1])) {
                invalid_oper();
                continue;
            }
            if (!checkdouble(v[2])) {
                invalid_oper();
                continue;
            }
            import(stringtoint(v[1]), stringtodouble(v[2]));
        }
        else if (v[0] == "log") {//todo

        }
        else if (v[0] == "report") {//todo

        }
        else {
            invalid_oper();
            continue;
        }
    }
    return 0;
}