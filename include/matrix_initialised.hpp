#pragma once

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <utility>
#include <boost/array.hpp>
#include <boost/timer/timer.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/ref.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/array.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/spirit/include/karma.hpp>

namespace ubls = boost::numeric::ublas;
namespace krm  = boost::spirit::karma;

#include <array2d.hpp>

using namespace boost::multi_index;

template <size_t N = 3, size_t M = 3>
    using my_matrix = array2d<size_t, N, M, std::vector>;
template <size_t N>
using ubls_matrix = ubls::fixed_matrix<size_t, N + 1, N + 1>;

struct matrix
{
    my_matrix<8, 8>       A_8;
    my_matrix<16, 16>     A_16;
    my_matrix<32, 32>     A_32;
    my_matrix<64, 64>     A_64;
    my_matrix<128, 128>   A_128;

    my_matrix<8, 8>       B_8;
    my_matrix<16, 16>     B_16;
    my_matrix<32, 32>     B_32;
    my_matrix<64, 64>     B_64;
    my_matrix<128, 128>   B_128;

    my_matrix<8, 8>       C_8;
    my_matrix<16, 16>     C_16;
    my_matrix<32, 32>     C_32;
    my_matrix<64, 64>     C_64;
    my_matrix<128, 128>   C_128;
    ////

};

class multyplyMatrix
{
public:

private:
//    std::shared_ptr<matrix> ptr_mtrx;
//    matrix mtrx;
    std::time_t now;
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<> dist;
    boost::mutex mutex_s;
    boost::mutex mutex_m;
    boost::mutex mutex_g;

    std::vector<boost::thread> v_thr;
    std::vector<boost::future<void>> v_future;

    boost::condition_variable cv;
    bool finished;

public:
    multyplyMatrix() : /*ptr_mtrx(std::make_shared<matrix>()),*/ now{std::time(0)}, gen{static_cast<std::uint32_t> (now)}, dist{1, 3}, finished{false} {

    }

    void print_result_thread() {
        for(size_t i = 0; i < 5; i++) {
            v_thr.emplace_back(boost::bind(&multyplyMatrix::init_multiply_standart, this, i));
        }
        for(auto& x : v_thr) {
            x.join();
        }
    }

    void print_result_async() {
        for(size_t i = 0; i < 15; i++) {
            v_future.emplace_back(boost::async(boost::bind(&multyplyMatrix::init_multiply_all_async, this, i)));
        }
        for(auto& x : v_future) {
                x.get();
        }
    }

    ////////////////////
    void init_multiply_standart(size_t i) {
          if(i == 0){
                        {
                            boost::lock_guard<boost::mutex> lock{mutex_s};
                            matrix mtrx;
                            auto vec_8 = std::make_unique<std::vector<size_t>>();
                            for (size_t i = 0; i < 8*8; i++)
                            {
                                vec_8->push_back(dist(gen));
                            }
                            boost::timer::cpu_timer tmr;
                            mtrx.A_8.init_list(*vec_8);
                            mtrx.B_8.init_list(*vec_8);
                            mtrx.C_8 = mtrx.C_8.multy(mtrx.A_8, mtrx.B_8);
                            std::cout <<"Simple C(8x8)\t\t" << /*mtrx.C_8 << "Simple C\t8x8\n" << mtrx.A_8 * mtrx.B_8<<*/ tmr.format();
                        }
                        cv.notify_all();
                    }
     else if(i == 1){
                        {
                            boost::lock_guard<boost::mutex> lock{mutex_s};
                            matrix mtrx;
                            auto vec_16 = std::make_unique<std::vector<size_t>>();
                            for (size_t i = 0; i < 16*16; i++)
                            {
                                vec_16->push_back(dist(gen));
                            }
                            boost::timer::cpu_timer tmr;
                            mtrx.A_16.init_list(*vec_16);
                            mtrx.B_16.init_list(*vec_16);

                            mtrx.A_16*mtrx.B_16;
                            std::cout <<"Simple C(16x16)\t\t" << tmr.format();
                        }
                        cv.notify_all();
                    }
     else if(i == 2){
                        {
                            boost::lock_guard<boost::mutex> lock{mutex_s};
                            matrix mtrx;
                            auto vec_32 = std::make_unique<std::vector<size_t>>();
                            for (size_t i = 0; i < 32*32; i++)
                            {
                                vec_32->push_back(dist(gen));
                            }
                            boost::timer::cpu_timer tmr;
                            mtrx.A_32.init_list(*vec_32);
                            mtrx.B_32.init_list(*vec_32);

                            mtrx.A_32*mtrx.B_32;
                            std::cout <<"Simple C(32x32)\t\t" << tmr.format();
                        }
                        cv.notify_all();
                    }
     else if(i == 3){
                        {
                            boost::lock_guard<boost::mutex> lock{mutex_s};
                            matrix mtrx;
                            auto vec_64 = std::make_unique<std::vector<size_t>>();
                            for (size_t i = 0; i < 64*64; i++)
                            {
                                vec_64->push_back(dist(gen));
                            }
                            boost::timer::cpu_timer tmr;
                            mtrx.A_64.init_list(*vec_64);
                            mtrx.B_64.init_list(*vec_64);

                            mtrx.A_64*mtrx.B_64;
                            std::cout <<"Simple C(64x64)\t\t" << tmr.format();
                        }
                        cv.notify_all();
                    }
     else if(i == 4){
                        {
                            boost::lock_guard<boost::mutex> lock{mutex_s};
                            matrix mtrx;
                            auto vec_128 = std::make_unique<std::vector<size_t>>();
                            for (size_t i = 0; i < 128*128; i++)
                            {
                                vec_128->push_back(dist(gen));
                            }
                            boost::timer::cpu_timer tmr;
                            mtrx.A_128.init_list(*vec_128);
                            mtrx.B_128.init_list(*vec_128);

                            mtrx.A_128*mtrx.B_128;
                            std::cout <<"Simple C(128x128)\t\t" << tmr.format();
                        }
                        cv.notify_all();
                    }

          else throw std::out_of_range("Ups'");
          {
              boost::lock_guard<boost::mutex> lock{mutex_s};
              finished = true;
          }
          cv.notify_all();
    }

    void init_multiply_all_async(size_t i) {
          if(i == 0){
                  matrix mtrx;
                        auto vec_8 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 8*8; i++)
                            {
                                vec_8->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_8.init_list(*vec_8);
                        mtrx.B_8.init_list(*vec_8);

                        mtrx.A_8*mtrx.B_8;
                        std::cout <<"Simple(async) C(8x8)\t\t" << tmr.format();
                    }
     else if(i == 1){
                  matrix mtrx;
                        auto vec_16 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 16*16; i++)
                            {
                                vec_16->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_16.init_list(*vec_16);
                        mtrx.B_16.init_list(*vec_16);

                        mtrx.A_16*mtrx.B_16;
                        std::cout <<"Simple(async) C(16x16)\t\t" << tmr.format();
                    }
     else if(i == 2){
                  matrix mtrx;
                        auto vec_32 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 32*32; i++)
                            {
                                vec_32->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_32.init_list(*vec_32);
                        mtrx.B_32.init_list(*vec_32);

                        mtrx.A_32*mtrx.B_32;
                        std::cout <<"Simple(async) C(32x32)\t\t" << tmr.format();
                    }
     else if(i == 3){
                  matrix mtrx;
                        auto vec_64 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 64*64; i++)
                            {
                                vec_64->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_64.init_list(*vec_64);
                        mtrx.B_64.init_list(*vec_64);

                        mtrx.A_64*mtrx.B_64;
                        std::cout <<"Simple(async) C(64x64)\t\t" << tmr.format();
                    }
     else if(i == 4){
                  matrix mtrx;
                        auto vec_128 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 128*128; i++)
                            {
                                vec_128->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_128.init_list(*vec_128);
                        mtrx.B_128.init_list(*vec_128);

                        mtrx.A_128*mtrx.B_128;
                        std::cout <<"Simple(async) C(128x128)\t\t" << tmr.format();
                    }


          ////////////////META
     else if(i == 5){
                  matrix mtrx;
                        auto vec_8 = std::make_unique<std::vector<size_t>>();
                        for (size_t i = 0; i < 8*8; i++)
                            {
                                vec_8->push_back(dist(gen));
                            }
                        boost::timer::cpu_timer tmr;
                        mtrx.A_8.init_list(*vec_8);
                        mtrx.B_8.init_list(*vec_8);

                        mult_meta<8, 8>(mtrx.A_8, mtrx.B_8, mtrx.C_8);
                        std::cout <<"Meta C++(Метод 1)8x8\t" << tmr.format();
                    }
     else if(i == 6){
              matrix mtrx;
                    auto vec_16 = std::make_unique<std::vector<size_t>>();
                    for (size_t i = 0; i < 16*16; i++)
                        {
                            vec_16->push_back(dist(gen));
                        }
                    boost::timer::cpu_timer tmr;
                    mtrx.A_16.init_list(*vec_16);
                    mtrx.B_16.init_list(*vec_16);

                    mult_meta<16, 16>(mtrx.A_16, mtrx.B_16, mtrx.C_16);
                    std::cout <<"Meta C++(Метод 1)16x16\t" << tmr.format();
                    }
     else if(i == 7){
                  matrix mtrx;
                    auto vec_32 = std::make_unique<std::vector<size_t>>();
                    for (size_t i = 0; i < 32*32; i++)
                        {
                            vec_32->push_back(dist(gen));
                        }
                    boost::timer::cpu_timer tmr;
                    mtrx.A_32.init_list(*vec_32);
                    mtrx.B_32.init_list(*vec_32);

                    mult_meta<32, 32>(mtrx.A_32, mtrx.B_32, mtrx.C_32);
                    std::cout <<"Meta C++(Метод 1)32x32\t" << tmr.format();
                    }
     else if(i == 8){
              matrix mtrx;
                    auto vec_64 = std::make_unique<std::vector<size_t>>();
                    for (size_t i = 0; i < 64*64; i++)
                        {
                            vec_64->push_back(dist(gen));
                        }
                    boost::timer::cpu_timer tmr;
                    mtrx.A_64.init_list(*vec_64);
                    mtrx.B_64.init_list(*vec_64);

                    mult_meta<64, 64>(mtrx.A_64, mtrx.B_64, mtrx.C_64);
                    std::cout <<"Meta C++(Метод 1)64x64\t" << tmr.format();
                    }
     else if(i == 9){
              matrix mtrx;
                    auto vec_128 = std::make_unique<std::vector<size_t>>();
                    for (size_t i = 0; i < 128*128; i++)
                        {
                            vec_128->push_back(dist(gen));
                        }
                    boost::timer::cpu_timer tmr;
                    mtrx.A_128.init_list(*vec_128);
                    mtrx.B_128.init_list(*vec_128);

                    mult_meta<128, 128>(mtrx.A_128, mtrx.B_128, mtrx.C_128);
                    std::cout <<"Meta C++(Метод 1)128x128\t" << tmr.format();
                    }
          /////2
      else if(i == 10){
                  ubls_matrix<8> mtrxA;
                  ubls_matrix<8> mtrxB;
                  ubls_matrix<8> result_mtrx;
                for (size_t i = 0; i < 8; i++)
                    {
                        for (size_t j = 0; j < 8; j++) {
                            mtrxA.insert_element(i, j, dist(gen));
                            mtrxB.insert_element(i, j, dist(gen));
                        }
                    }
                    boost::timer::cpu_timer tmr;
                    calc_matrix<8, ubls_matrix<8> >(result_mtrx, mtrxA, mtrxB);
                    std::cout <<"Meta C++(Метод 2)8x8\t" << tmr.format();
      }
      else if(i == 11){
                  ubls_matrix<16> mtrxA;
                  ubls_matrix<16> mtrxB;
                  ubls_matrix<16> result_mtrx;
                     for (size_t i = 0; i < 16; i++)
                         {
                             for (size_t j = 0; j < 16; j++) {
                                 mtrxA.insert_element(i, j, dist(gen));
                                 mtrxB.insert_element(i, j, dist(gen));
                             }
                         }
                 boost::timer::cpu_timer tmr;
                 calc_matrix<16, ubls_matrix<16> >(result_mtrx, mtrxA, mtrxB);
                 std::cout <<"Meta C++(Метод 2)16x16\t" << tmr.format();
      }
      else if(i == 12){
              ubls_matrix<32> mtrxA;
              ubls_matrix<32> mtrxB;
              ubls_matrix<32> result_mtrx;
                         for (size_t i = 0; i < 32; i++)
                             {
                                 for (size_t j = 0; j < 32; j++) {
                                     mtrxA.insert_element(i, j, dist(gen));
                                     mtrxB.insert_element(i, j, dist(gen));
                                 }
                             }
                         boost::timer::cpu_timer tmr;
                calc_matrix<32, ubls_matrix<32> >(result_mtrx, mtrxA, mtrxB);
                std::cout <<"Meta C++(Метод 2)32x32\t" << tmr.format();
      }
      else if(i == 13){
              ubls_matrix<64> mtrxA;
              ubls_matrix<64> mtrxB;
              ubls_matrix<64> result_mtrx;
                     auto vec_8 = std::make_unique<std::vector<size_t>>();
                     for (size_t i = 0; i < 64; i++)
                         {
                             for (size_t j = 0; j < 64; j++) {
                                 mtrxA.insert_element(i, j, dist(gen));
                                 mtrxB.insert_element(i, j, dist(gen));
                             }
                         }
                boost::timer::cpu_timer tmr;
                calc_matrix<64, ubls_matrix<64> >(result_mtrx, mtrxA, mtrxB);
                std::cout <<"Meta C++(Метод 2)64x64\t" << tmr.format();
      }
      else if(i == 14){
              ubls_matrix<128> mtrxA;
              ubls_matrix<128> mtrxB;
              ubls_matrix<128> result_mtrx;
                     auto vec_8 = std::make_unique<std::vector<size_t>>();
                     for (size_t i = 0; i < 128; i++)
                         {
                             for (size_t j = 0; j < 128; j++) {
                                 mtrxA.insert_element(i, j, dist(gen));
                                 mtrxB.insert_element(i, j, dist(gen));
                             }
                         }
                 boost::timer::cpu_timer tmr;
                 calc_matrix<128, ubls_matrix<128> >(result_mtrx, mtrxA, mtrxB);
                 std::cout <<"Meta C++(Метод 2)128x128\t" << tmr.format();
      }

        else throw std::out_of_range("Ales'");
    }

};





