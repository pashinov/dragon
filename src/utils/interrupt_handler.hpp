//
//  Created by Ivan Mejia on 12/03/16.
//
// MIT License
//
// Copyright (c) 2016 ivmeroLabs. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// system includes
#include <condition_variable>
#include <mutex>
#include <csignal>

static std::condition_variable condition_;
static std::mutex mutex_;

namespace utils
{
    class interrupt_handler
    {
    public:
        static void hook_signal(int signal_name)
        {
            signal(signal_name, handle_interrupt);
        }

        static void handle_interrupt(int signal)
        {
            switch (signal)
            {
                case SIGINT:
                case SIGTERM:
                    condition_.notify_one();
                    break;
                default:
                    break;
            }
        }

        static void wait_for_signal_interrupt()
        {
            std::unique_lock<std::mutex> lock { mutex_ };
            condition_.wait(lock);
            lock.unlock();
        }
    };
}
