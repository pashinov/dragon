// project includes
#include <system/sysinfo.hpp>
#include <rest_service/lion_connector.hpp>
#include <utils/config.hpp>
#include <utils/logger.hpp>

// proto
#include <lion.pb.h>

namespace sys::sysinfo
{
    std::optional<std::string> get_uptime()
    {
        std::optional<std::string> os_uptime = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::UPTIME);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("uptime", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            os_uptime = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return os_uptime;
    }

    std::optional<std::string> get_boot_time()
    {
        std::optional<std::string> boot_time = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::BOOT_TIME);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("boot_time", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            boot_time = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return boot_time;
    }

    std::optional<std::string> get_temperature()
    {
        std::optional<std::string> temperature = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::TEMPERATURE);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("temperature", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            temperature = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return temperature;
    }

    std::optional<std::string> get_os_info()
    {
        std::optional<std::string> os_info = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::OS_INFO);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("os_info", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            os_info = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return os_info;
    }

    std::optional<std::string> get_cpu_info()
    {
        std::optional<std::string> cpu_info = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::CPU_INFO);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("cpu_info", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            cpu_info = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return cpu_info;
    }

    std::optional<std::string> get_disk_info()
    {
        std::optional<std::string> disk_info = std::nullopt;
        std::unique_ptr<rest_service::lion_connector> lion_conn = std::make_unique<rest_service::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(lion_protocol::SysInfoType::DISK_INFO);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        lion_conn->send("disk_info", request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL) {
            disk_info = response.payload().sval();
        } else {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return disk_info;
    }
}
