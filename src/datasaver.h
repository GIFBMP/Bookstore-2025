#pragma once
#ifndef DATASAVER_H
#define DATASAVER_H
#include <iostream>
#include <fstream>
namespace gifbmp {
    using std::string;
    using std::fstream;
    using std::ifstream;
    using std::ofstream;
    template<class T, int info_len = 1>
    class DataSaver {
    private:
        fstream file;
        string file_name;
        int sizeofT = sizeof(T);
        int databegin = info_len * sizeof(int);
        bool isFileExists (const string &name) {
            ifstream f(name);
            return f.good();
        }
    public:
        DataSaver() = default;
        void initialise(string FN = "", bool typ = 0) {//0:reserve 1:clear
            if (FN != "") file_name = FN;
            else return;
            if (isFileExists(FN) && !typ) return;
            if (file.is_open()) file.close();
            file.open(file_name, fstream::out | fstream::binary);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }
        int get_info(int n) {
            if (n > info_len) return 0;
            file.open(file_name, fstream::in | fstream::binary);
            file.seekg((n - 1) * sizeof(int));
            int tmp;
            file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
            return tmp;
        }
        void write_info(int tmp, int n) {
            if (n > info_len) return;
            file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekp((n - 1) * sizeof(int));
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }
        int size() {
            if (file.is_open())file.close();
            file.open(file_name, fstream::app | fstream::binary);
            int index = ((int)file.tellp() - databegin) / sizeofT + 1;
            file.close();
            return index - 1;
        }
        int write(T &t) {
            if (file.is_open())file.close();
            file.open(file_name, fstream::app | fstream::binary);
            int index = ((int)file.tellp() - databegin) / sizeofT + 1;
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
            return index;
        }
        DataSaver(const string& file_name, bool typ = 1) {
            initialise(file_name, typ);
        }
        void update(T &t, int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekp((index - 1) * sizeofT + databegin);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        void read(T &t, int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekg((index - 1) * sizeofT + databegin);
            file.read(reinterpret_cast<char *>(&t), sizeofT);
        }
    };
}
#endif