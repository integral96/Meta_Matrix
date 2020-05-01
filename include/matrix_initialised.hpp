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

#include <array2d.hpp>

using namespace boost::multi_index;

template <size_t N = 3, size_t M = 3>
    using my_matrix = array2d<size_t, N, M, std::vector>;



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
        for(size_t i = 0; i < 8; i++) {
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
                            std::cout <<"Simple C\t8x8\n" << mtrx.C_8 << "Simple C\t8x8\n" << mtrx.A_8 * mtrx.B_8<< tmr.format();
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
                            std::cout <<"Simple C\t16x16 " << tmr.format();
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
                            std::cout <<"Simple C\t32x32 " << tmr.format();
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
                            std::cout <<"Simple C\t64x64 " << tmr.format();
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
                            std::cout <<"Simple C\t128x128 " << tmr.format();
                        }
                        cv.notify_all();
                    }

          else throw std::out_of_range("Bliat'");
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
                        std::cout <<"Simple C(Async)\t8x8 " << tmr.format();
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
                        std::cout <<"Simple C(Async)\t16x16 " << tmr.format();
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
                        std::cout <<"Simple\t32x32 " << tmr.format();
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
                        std::cout <<"Simple C(Async)\t64x64 " << tmr.format();
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
                        std::cout <<"Simple\t128x128 " << tmr.format();
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
                        std::cout <<"Meta C++(Async)\t8x8 " << tmr.format();
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
                        std::cout <<"Meta C++(Async)\t16x16 " << tmr.format();
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
                        std::cout <<"Meta C++(Async)\t32x32 " << tmr.format();
                    }
//     else if(i == 8){
//                  matrix mtrx;
//                        auto vec_64 = std::make_unique<std::vector<size_t>>();
//                        for (size_t i = 0; i < 64*64; i++)
//                            {
//                                vec_64->push_back(dist(gen));
//                            }
//                        boost::timer::cpu_timer tmr;
//                        mtrx.A_64.init_list(*vec_64);
//                        mtrx.B_64.init_list(*vec_64);

//                        mult_meta<64, 64>(mtrx.A_64, mtrx.B_64, mtrx.C_64);
//                        std::cout <<"Meta\t64x64 " << tmr.format();
//                    }
//     else if(i == 9){
//                  matrix mtrx;
//                        auto vec_128 = std::make_unique<std::vector<size_t>>();
//                        for (size_t i = 0; i < 128*128; i++)
//                            {
//                                vec_128->push_back(dist(gen));
//                            }
//                        boost::timer::cpu_timer tmr;
//                        mtrx.A_128.init_list(*vec_128);
//                        mtrx.B_128.init_list(*vec_128);

//                        mult_meta<128, 128>(mtrx.A_128, mtrx.B_128, mtrx.C_128);
//                        std::cout <<"Meta\t128x128 " << tmr.format();
//                    }

          else throw std::out_of_range("Bliat'");
    }

};





