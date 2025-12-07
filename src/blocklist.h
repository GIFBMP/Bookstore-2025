#pragma once
#ifndef BLOCKLIST_H
#define BLOCKLIST_H
#include <fstream>
#include <iostream>
#include <cstring>
namespace gifbmp {
    using std::string;
    using std::fstream;
    using std::ifstream;
    using std::ofstream;

    template<class T, class Val, int info_len = 1>
    class BlockList {
    private:
        fstream file;
        string file_name;
        int sizeofT = sizeof(T);
        int databegin = info_len * sizeof(int);
        static const int kMaxBlockSize = 1000;
        static const int kMergeSize = 750;
        struct ValuePair {
            T a; Val b;
            ValuePair() {
                a = T(); b = Val();
            }
            ValuePair(T a, Val b) : a(a), b(b) {}
            bool operator < (const ValuePair &t) const {
                return a == t.a ? b < t.b : a < t.a;
            }
            bool operator == (const ValuePair &t) const {
                return a == t.a && b == t.b;
            }
            bool operator <= (const ValuePair &t) const {
                return a == t.a ? b <= t.b : a <= t.a;
            }
        };
        int sizeofPair = sizeof(ValuePair);
        struct Block {
            ValuePair data[kMaxBlockSize + 1];
            int nx, sz;
            Block() {
                nx = sz = 0;
                for (int i = 0; i <= kMaxBlockSize; i++)
                    data[i] = ValuePair();
            }
        };
        int sizeofBlock = sizeof(Block);
        int restore[410], res_cnt;//recycle
        bool isFileExists (const string &name) {
            ifstream f(name);
            return f.good();
        }
    public:
        BlockList() = default;
        void initialise(string FN = "", bool typ = 0) {
            if (FN != "") file_name = FN;
            else return;
            if (isFileExists(FN) && !typ) return;
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
        int write(Block &t) {
            if (file.is_open())file.close();
            file.open(file_name, fstream::app | fstream::binary);
            int index = ((int)file.tellp() - databegin) / sizeofBlock + 1;
            file.write(reinterpret_cast<char *>(&t), sizeofBlock);
            file.close();
            return index;
        }
        BlockList(const string& file_name, bool typ = 1) {
            initialise(file_name, typ);
            Block tmp = Block();
            write(tmp);
        }
        void update(Block &t, int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekp((index - 1) * sizeofBlock + databegin);
            file.write(reinterpret_cast<char *>(&t), sizeofBlock);
        }
        void read(Block &t, int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekg((index - 1) * sizeofBlock + databegin);
            file.read(reinterpret_cast<char *>(&t), sizeofBlock);
        }
        void readfir(ValuePair &t, int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekg((index - 1) * sizeofBlock + databegin);
            file.read(reinterpret_cast<char *>(&t), sizeofPair);
        }
        void Delete(int index) {
            if (!file.is_open())
                file.open(file_name, fstream::in | fstream::out | fstream::binary);
            file.seekp((index - 1) * sizeofBlock + databegin);
            Block tmp = Block();
            file.write(reinterpret_cast<char *>(&tmp), sizeofBlock);
            //file.close();
            restore[++res_cnt] = index; 
        }
        //MemoryRiver part
        void testprint() {
            int i = 1; Block nw;
            bool fl = false;
            for (; i != 0; i = nw.nx) {
                std::cerr << "block_id: " << i << '\n';
                read(nw, i);
                std::cerr << "sz: " << nw.sz << '\n';
                for (int j = 0; j < nw.sz; j++)
                    std::cerr << nw.data[j].b << ' ';
                std::cerr << '\n';
            }
            std::cerr << "------\n";
        }
        void split(Block &x, int i) {
            int sz_pr = x.sz / 2, sz_nx = x.sz - sz_pr;
            Block ret;
            ret.nx = x.nx; ret.sz = sz_nx;
            for (int i = sz_pr; i < x.sz; i++) {
                ret.data[i - sz_pr] = x.data[i];
                x.data[i] = ValuePair();
            }
            x.sz = sz_pr;
            if (!res_cnt) x.nx = write(ret);
            else {
                x.nx = restore[res_cnt--];
                update(ret, x.nx);
            }
            update(x, i);
            //testprint();
        }
        bool merge(Block &x, int i) {
            if (!x.nx || x.sz > kMergeSize) return false;
            Block tmp;
            read(tmp, x.nx);
            if (x.sz && x.sz + tmp.sz >= kMergeSize) return false;
            int index = x.nx;
            x.nx = tmp.nx;
            for (int i = x.sz ; i < x.sz + tmp.sz; i++)
                x.data[i] = tmp.data[i - x.sz];
            x.sz += tmp.sz;
            update(x, i);
            Delete(index);
            return true;
        }
        void ins(T a, Val b) {
            ValuePair tmp = ValuePair(a, b);
            int i = 1;
            Block nw; ValuePair fir;
            for (; i != 0; i = nw.nx) {
                read(nw, i);
                bool is_nxt_empty = false;
                if (nw.nx == 0) {
                    if (nw.sz < kMaxBlockSize) break;
                    else {
                        update(nw, i);
                        is_nxt_empty = true;
                    }
                }
                if (is_nxt_empty) {
                    split(nw, i);
                }
                readfir(fir, nw.nx);
                if (tmp < fir) {
                    if (nw.sz < kMaxBlockSize) break;
                    else {
                        split(nw, i);
                        readfir(fir, nw.nx);
                        if (tmp < fir) break;
                    }
                } 
            }//find appropriate i
            int pos = nw.sz;
            for (int j = 0; j < nw.sz; j++)
                if (tmp < nw.data[j]) {
                    pos = j; break;
                }
            for (int j = nw.sz - 1; j >= pos; j--)
                nw.data[j + 1] = nw.data[j];
            nw.sz++;
            nw.data[pos] = tmp;
            update(nw, i);//insert
        }
        void del(T a, Val b) {
            ValuePair tmp = ValuePair(a, b);
            int i = 1;
            Block nw;
            for (; i != 0; i = nw.nx) {
                read(nw, i);
                if (!nw.sz) continue;
                if (tmp <= nw.data[nw.sz - 1]) break;
            }//find appropriate i
            if (!i) return;
            int pos = nw.sz;
            for (int j = 0; j < nw.sz; j++)
                if (tmp == nw.data[j]) {
                    pos = j; break;
                }
            if (pos == nw.sz) return;
            for (int j = pos; j < nw.sz - 1; j++)
                nw.data[j] = nw.data[j + 1];
            nw.data[nw.sz - 1] = ValuePair();
            nw.sz--;
            if (nw.sz >= kMergeSize || !nw.nx) {
                update(nw, i);
                return;
            }
            if (merge(nw, i)) return;
            else update(nw, i);
        }
        void query(T a) {
            int i = 1; Block nw;
            bool fl = false;
            for (; i != 0; i = nw.nx) {
                read(nw, i);
                //printf ("%d\n", nw.sz);
                if (!nw.sz) continue;
                if (a < nw.data[0].a) break;
                if (nw.data[0].a <= a && a <= nw.data[nw.sz - 1].a) {
                    for (int j = 0; j < nw.sz; j++)
                        if (nw.data[j].a == a) {
                            fl = true;
                            std::cout << nw.data[j].b << ' ';
                        }
                }
            }
            if (!fl) std::cout << "null";
            std::cout << '\n';
        }
        //List part
    };
}
using namespace gifbmp;
#endif