#include "bookstore.h"
namespace gifbmp {
    bool has_logined(Index30 userid) {
        std::vector<int> v = login_cnt.query(userid);
        if (v.empty()) return false;
        return v[0] > 0;
    }
    void login(Index30 userid, Index30 password) {
        //std::cerr << "try login:" << "id=" << userid << " password=" << password << '\n';
        std::vector<userindata> isreg = list_of_users.query(userid);
        if (isreg.empty()) {
            //std::cerr << "cannot find such user\n";
            invalid_oper();
            return;
        }
        userindata id = isreg[0];
        //std::cerr << isreg.size() << '\n';
        //std::cerr << "user_password:" << id.password << ",input_password:" << password << '\n';
        if (nw_user.privilege <= id.privilege && password != id.password) {
            //std::cerr << "wrong password\n";
            invalid_oper();
            return;
        }
        std::vector<int> tmpcnt = login_cnt.query(userid);
        if (tmpcnt.empty()) {
            login_cnt.ins(userid, 1);
        }
        else {
            int cnt = login_cnt.query(userid)[0];
            login_cnt.del(userid, cnt);
            login_cnt.ins(userid, cnt + 1);
        }
        loginstack[++tp] = userid;
        nw_user = id;
    }
    void logout() {
        //std::cerr << "try to logout\n";
        if (!tp || nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        is_selected[tp] = 0;
        selectedbook[tp] = Book();
        Index30 userid = loginstack[tp];
        std::vector<int> tmpcnt = login_cnt.query(userid);
        assert(!tmpcnt.empty());
        int cnt = tmpcnt[0];
        assert(cnt >= 1);
        login_cnt.del(userid, cnt);
        login_cnt.ins(userid, cnt - 1);
        tp--;
        //std::cerr << "tp:" << tp << '\n';
        if (!tp) nw_user = userindata();
        else nw_user = list_of_users.query(loginstack[tp])[0];
    }
    void chgpassword(Index30 userid, Index30 current_password, Index30 new_password) {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        std::vector<userindata> isreg = list_of_users.query(userid);
        if (isreg.empty()) {
            invalid_oper();
            return;
        }
        userindata id = isreg[0];
        if (nw_user.privilege < 7) {
            if (current_password != id.password) {
                invalid_oper();
                return;
            }
        }
        list_of_users.del(userid, id);
        id.password = new_password;
        list_of_users.ins(userid, id);
        //std::cerr << "change password successfully\n";
        if (userid == loginstack[tp]) {
            nw_user.password = new_password;
        }
    }
    void user_register(Index30 userid, Index30 password, Index30 username) {
        std::vector<userindata> isreg = list_of_users.query(userid);
        if (!isreg.empty()) {
            invalid_oper();
            return;
        }
        userindata nw;
        nw.password = password;
        nw.privilege = 1;
        list_of_users.ins(userid, nw);
        login_cnt.ins(userid, 0);
    }
    void useradd(Index30 userid, Index30 password, int privilege, Index30 username) {
        // std::cerr << "try to add user:" << " id:" << userid << ",password:" << password \
        //           << ",privilege:" << privilege << ",username:" << username << '\n';
        // std::cerr << "nw_user_privilege:" << nw_user.privilege << '\n';
        if (nw_user.privilege < 3 || privilege >= nw_user.privilege) {
            invalid_oper();
            return;
        }
        std::vector<userindata> isreg = list_of_users.query(userid);
        std::cerr << isreg.size() << '\n';
        if (!isreg.empty()) {
            invalid_oper();
            return;
        }
        //std::cerr << "add successfully\n";
        userindata nw;
        nw.password = password;
        nw.privilege = privilege;
        list_of_users.ins(userid, nw);
        login_cnt.ins(userid, 0);
    }
    void deluser(Index30 userid) {
        //std::cerr << "try to delete: " << "id:"<< userid << '\n';
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        std::vector<userindata> isreg = list_of_users.query(userid);
        //std::cerr << "reglen:" << isreg.size() << '\n';
        if (isreg.empty()) {
            invalid_oper();
            return;
        }
        if (has_logined(userid)) {
            invalid_oper();
            return;
        }
        //int cnt = login_cnt.query(userid)[0];
        list_of_users.del(userid, isreg[0]);
        login_cnt.del(userid, 0);
    }
}