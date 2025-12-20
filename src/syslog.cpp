#include "bookstore.h"
#include <iomanip>
namespace gifbmp {
    void show_finance(int cnt) {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        int sz = finance_log.size();
        if (cnt > sz || cnt < 0) {
            invalid_oper();
            return;
        }
        if (cnt == 0) {
            std::cout << '\n';
            return;
        }
        finance tmp;
        long double income = 0.0, outcome = 0.0;
        for (int i = sz; i >= sz - cnt + 1; i--) {
            finance_log.read(tmp, i);
            income += tmp.income;
            outcome += tmp.outcome;
        }
        std::cout << std::fixed << std::setprecision(2) << "+ " << income << " - " << outcome << '\n';
    }
    void show_finance_all() {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        long double income, outcome;
        profits.read(income, 1);
        profits.read(outcome, 2);
        std::cout << std::fixed << std::setprecision(2) << "+ " << income << " - " << outcome << '\n';
    }
    void report_employee() {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        //std::cout << '\n';
        Index300 tmp;
        std::cout << "employee:\n";
        std::cout << "----------------\n";
        for (int i = 1; i <= employeelog.size(); i++) {
            employeelog.read(tmp, i);
            std::cout << tmp << '\n';
        }
        std::cout << "----------------\n";
    }
    void report_finance() {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        //std::cout << '\n';
        //invalid_oper();
        std::cout << "finance:\n";
        std::cout << "----------------\n";
        Index300 tmp;
        for (int i = 1; i <= financelog.size(); i++) {
            financelog.read(tmp, i);
            std::cout << tmp << '\n';
        }
        std::cout << "Total: ";
        show_finance_all();
        std::cout << "----------------\n";
    }
    void log() {
        if (nw_user.privilege < 7) {
            invalid_oper();
            return;
        }
        //std::cout << '\n';
        Index300 tmp;
        std::cout << "system log:\n";
        std::cout << "----------------\n";
        for (int i = 1; i <= syslog.size(); i++) {
            syslog.read(tmp, i);
            std::cout << tmp << '\n';
        }
        std::cout << "----------------\n";
    }
}