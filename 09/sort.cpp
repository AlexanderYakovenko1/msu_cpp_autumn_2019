#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <thread>
#include <exception>
#include <cstdint>

/*
 * Bufsize that guarantees < 8MB memory usage
 * Allocate 7MB, 1MB remaining should be enough
 * for everything else
 */
constexpr size_t BUF_SIZE = 1024 * 896;

/*
 * Splits file into many files and sorts each one
 * Returns name of file with all the temp file names
 */
std::string split(std::ifstream &file, uint64_t *buf)
{
    file.seekg(0, file.end);
    size_t filesize = file.tellg() / sizeof(uint64_t);
    file.seekg(0, file.beg);

    std::string f_name = "file_names.tmp";
    std::ofstream output(f_name);
    if (!output) {
        throw std::ofstream::failure(f_name);
    }

    size_t n_files = 2 * (filesize / BUF_SIZE + ((filesize % BUF_SIZE) != 0));
    size_t cnt = 0;

    while (!file.eof()) {
        file.read(reinterpret_cast<char*>(buf), BUF_SIZE * sizeof(uint64_t));
        size_t bit_cnt = file.gcount() / sizeof(uint64_t);

        std::thread t1(
            [buf, bit_cnt]()
            { 
                std::sort(buf, buf + bit_cnt / 2 + bit_cnt % 2);
            });
        std::thread t2(
            [buf, bit_cnt]()
            { 
                std::sort(buf + bit_cnt / 2 + bit_cnt % 2, buf + bit_cnt);
            });
        t1.join();
        t2.join();

        std::ofstream out1(std::to_string(cnt), std::ios::binary);
        if (!out1) {
            throw std::ofstream::failure(std::to_string(cnt));
        }
        std::ofstream out2(std::to_string(cnt + 1), std::ios::binary);
        if (!out2) {
            throw std::ofstream::failure(std::to_string(cnt + 1));
        }

        out1.write(reinterpret_cast<char*>(buf), (bit_cnt / 2 + bit_cnt % 2) * sizeof(uint64_t));
        out2.write(reinterpret_cast<char*>(buf + bit_cnt / 2 + bit_cnt % 2), bit_cnt / 2 * sizeof(uint64_t));
        output << '\n' << std::to_string(cnt) << '\n' << std::to_string(cnt + 1);
        cnt += 2;
    }
    return f_name;
}

/*
 * Merge all files into the last one
 * combining then in sorted order
 */
void merge(std::string &f_list, uint64_t *buf)
{
    std::ifstream files(f_list);
    if (!files) {
        throw std::ifstream::failure(f_list);
    }
    std::string prev_file, cur_file;

    if (!files.eof()) {
        files >> prev_file;
    }

    while (!files.eof()) {
        files >> cur_file;

        std::ifstream in1(prev_file);
        if (!in1) {
            throw std::ifstream::failure(prev_file);
        }
        std::ifstream in2(cur_file);
        if (!in2) {
            throw std::ifstream::failure(cur_file);
        }

        in2.seekg(0, in2.end);
        size_t len = in2.tellg() / sizeof(uint64_t);
        in2.seekg(0, in2.beg);
        in2.read(reinterpret_cast<char*>(buf), BUF_SIZE * sizeof(uint64_t));
        in2.close();

        std::ofstream out(cur_file);
        if (!out) {
            throw std::ofstream::failure(cur_file);
        }

        uint64_t tmp;
        if (!in1.eof()) {
            in1.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
        }

        int i = 0;
        while (i < len && !in1.eof()) {
            if (buf[i] > tmp) {
                out.write(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
                in1.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
            } else if (buf[i] < tmp) {
                out.write(reinterpret_cast<char*>(&buf[i]), sizeof(uint64_t));
                ++i;
            } else {
                out.write(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
                out.write(reinterpret_cast<char*>(&buf[i]), sizeof(uint64_t));
                in1.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
                ++i;
            }
        }

        if (i < len) {
            out.write(reinterpret_cast<char*>(&buf[i]), (len - i) * sizeof(uint64_t));
        } else {
            while(!in1.eof()) {
                out.write(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
                in1.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
            }
        }

        prev_file = cur_file;
    }
}

/*
 * Delete all temporary files and rename the last one
 */
void clear(std::string &f_list, const char *output_name)
{
    std::ifstream files(f_list);
    if (!files) {
        throw std::ifstream::failure(f_list);
    }
    std::string cur_file;
    
    if (!files.eof()) {
        files >> cur_file;
    }

    while (!files.eof()) {
        std::remove(cur_file.c_str());
        files >> cur_file;
    }

    if (cur_file.c_str()) {
        std::rename(cur_file.c_str(), output_name);
    }

    std::remove(f_list.c_str());
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file output_file" << std::endl;
        return 1;
    }

    uint64_t *buf = new uint64_t[BUF_SIZE];
    try {
        std::ifstream input(argv[1], std::ios::binary);
        if (!input) {
            throw std::ifstream::failure(argv[1]);
        }
        std::string f_list = split(input, buf);
        merge(f_list, buf);
        clear(f_list, argv[2]);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        delete[] buf;
        return 1;
    }
    delete[] buf;
    return 0;
}