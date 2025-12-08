#pragma once
#ifndef BOOKSTORE_H
#define BOOKSTORE_H
#include "blocklist.h"
#include "datasaver.h"
namespace gifbmp {
    using std::string;
    const int kMaxn = 1e5 + 10;
    class User {
        char userid[31], username[31], password[31];
        int privilege;
    };
    void login();
    void logout();
    void chgpassword();
    void useradd();
    void deluser();
}
#endif