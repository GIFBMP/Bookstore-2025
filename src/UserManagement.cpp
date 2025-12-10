#include "bookstore.h"
namespace gifbmp {
    bool has_logined(Index30 userid) {
        std::vector<int> v = login_cnt.query(userid);
        if (v.empty()) return false;
        return v[0] > 0;
    }
    void login(Index30 userid, Index30 password) {
        std::vector<userindata> isreg = list_of_users.query(userid);
        if (isreg.empty()) {
            invalid_oper();
            return;
        }
        userindata id = isreg[0];
        if (nw_user.privilege <= id.privilege && password != id.password) {
            invalid_oper();
            return;
        }
        int cnt = login_cnt.query(userid)[0];
        login_cnt.del(userid, cnt);
        login_cnt.ins(userid, cnt + 1);
        loginstack[++tp] = userid;
        nw_user = id;
    }
    void logout() {
        if (nw_user.privilege < 1) {
            invalid_oper();
            return;
        }
        Index30 userid = loginstack[tp];
        int cnt = login_cnt.query(userid)[0];
        assert(cnt >= 1);
        login_cnt.del(userid, cnt);
        login_cnt.ins(userid, cnt - 1);
        tp--;
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
        if (nw_user.privilege < 3 || privilege >= nw_user.privilege) {
            invalid_oper();
            return;
        }
        std::vector<userindata> isreg = list_of_users.query(userid);
        if (!isreg.empty()) {
            invalid_oper();
            return;
        }
        userindata nw;
        nw.password = password;
        nw.privilege = privilege;
        list_of_users.ins(userid, nw);
        login_cnt.ins(userid, 0);
    }
    void deluser(Index30 userid) {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        std::vector<userindata> isreg = list_of_users.query(userid);
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