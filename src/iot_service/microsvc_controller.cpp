// 3rdparty includes
#include <cpprest/http_client.h>

// project includes
#include <iot_service/microsvc_controller.hpp>
#include <iot_service/task_manager.hpp>
#include <system/sysinfo/cpuinfo.hpp>
#include <system/sysinfo/osinfo.hpp>
#include <utils/config.hpp>

// proto
#include <phoenix.pb.h>

using namespace web;
using namespace http;

namespace iot_service
{
    microsvc_controller::microsvc_controller()
        : alive_(false)
        , task_manager_(std::make_unique<task_manager>(io_service_))
        , phoenix_connector_(std::make_unique<phoenix_connector>())
    {

    }

    void microsvc_controller::start()
    {
        alive_ = true;

        pub_thread_ = std::thread([this]() {
            phoenix_connector_->publisher_instance()->bind(CONFIG()->service.iot.connector.zmq.pub.addr);

            std::function<void()> cpuinfo = [this]() {
                auto data = json::value::object();
                data["model"] = json::value::string(sys::sysinfo::cpu_model());
                data["vendor"] = json::value::string(sys::sysinfo::cpu_vendor());

                phoenix_proto::message msg;
                msg.set_topic("topic/cpu_info");
                msg.set_payload(data.serialize());

                phoenix_connector_->publisher_instance()->publish(CONFIG()->service.iot.connector.zmq.pub.topic, msg.SerializeAsString());
            };

            std::function<void()> osinfo = [this]() {
                auto data = json::value::object();
                data["name"] = json::value::string(sys::sysinfo::os_name());
                data["release"] = json::value::string(sys::sysinfo::os_release());
                data["machine"] = json::value::string(sys::sysinfo::os_machine());
                data["system_name"] = json::value::string(sys::sysinfo::os_system_name());

                phoenix_proto::message msg;
                msg.set_topic("topic/os_info");
                msg.set_payload(data.serialize());

                phoenix_connector_->publisher_instance()->publish(CONFIG()->service.iot.connector.zmq.pub.topic, msg.SerializeAsString());
            };

            task_manager_->add_task(cpuinfo);
            task_manager_->add_task(osinfo);
            task_manager_->start();

            io_service_.run();
        });

        sub_thread_ = std::thread([this]() {
            phoenix_connector_->subscriber_instance()->connect(CONFIG()->service.iot.connector.zmq.sub.addr);
            phoenix_connector_->subscriber_instance()->subscribe(CONFIG()->service.iot.connector.zmq.sub.topic);
            phoenix_connector_->subscriber_instance()->polling_loop(alive_);
        });
    }

    void microsvc_controller::stop()
    {
        alive_ = false;
        task_manager_->stop();
        io_service_.stop();
        pub_thread_.join();
        sub_thread_.join();
    }
}
