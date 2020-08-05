// 3rd party includes
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

// project includes
#include <iot_service/task_manager.hpp>

namespace asio = boost::asio;

namespace iot_service
{
    class task
    {
    public:
        task(asio::io_service& io_service, std::function<void()>& executor)
                : timer_{io_service}
                , executor_{executor}
                , expiry_time_{}
        {

        }

        task(asio::io_service& io_service, std::function<void()>& executor, std::chrono::duration<long long> expiry_time)
                : timer_{io_service}
                , executor_{executor}
                , expiry_time_{expiry_time}
        {

        }

        void run()
        {
            timer_.async_wait([this](const boost::system::error_code& err) {
                this->handle_timer(err);
            });
        }

        void stop()
        {
            timer_.cancel();
        }

    private:
        void handle_timer(const boost::system::error_code& err)
        {
            if(!err)
            {
                this->executor_();
                if (expiry_time_ != std::chrono::duration<long long>{})
                {
                    timer_.expires_from_now(expiry_time_);
                    timer_.async_wait([this](const boost::system::error_code& err) {
                        this->handle_timer(err);
                    });
                }
            }
        }

    private:
        asio::steady_timer timer_;
        std::function<void()> executor_;
        std::chrono::duration<long long> expiry_time_;
    };

    class task_manager::impl
    {
    public:
        explicit impl(asio::io_service& io_service);

        ~impl();

        void start();

        void stop();

        void add_task(std::function<void()>& executor);

        void add_task(std::function<void()>& executor, std::chrono::duration<long long> expiry_time);
    private:
        asio::io_service& io_service_;
        std::vector<task> tasks_;
    };

    task_manager::impl::impl(asio::io_service& io_service)
            : io_service_(io_service)
    {

    }

    task_manager::impl::~impl() = default;

    void task_manager::impl::start()
    {
        for (auto& task : tasks_)
        {
            task.run();
        }
    }

    void task_manager::impl::stop()
    {
        for (auto& task : tasks_)
        {
            task.stop();
        }
    }

    void task_manager::impl::add_task(std::function<void()>& executor)
    {
        tasks_.emplace_back(task(io_service_, executor));
    }

    void task_manager::impl::add_task(std::function<void()>& executor, std::chrono::duration<long long> expiry_time)
    {
        tasks_.emplace_back(task(io_service_, executor, expiry_time));
    }

    task_manager::task_manager(asio::io_service& io_service)
            : impl_ {std::make_unique<impl>(io_service)}
    {

    }

    task_manager::~task_manager() = default;

    void task_manager::start()
    {
        impl_->start();
    }

    void task_manager::stop()
    {
        impl_->stop();
    }

    void task_manager::add_task(std::function<void()> &executor)
    {
        impl_->add_task(executor);
    }

    void task_manager::add_task(std::function<void()> &executor, std::chrono::duration<long long> expiry_time)
    {
        impl_->add_task(executor, expiry_time);
    }
}
