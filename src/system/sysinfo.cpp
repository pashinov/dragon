// system includes
#include <random>

// project includes
#include <system/lion_connector.hpp>
#include <system/sysinfo.hpp>
#include <utils/config.hpp>
#include <utils/logger.hpp>

namespace sys::sysinfo
{
    std::optional<std::variant<bool, double, std::string, std::uint32_t, std::int32_t>> get(lion_protocol::SysInfoType type)
    {
        std::optional<std::variant<bool, double, std::string, std::uint32_t, std::int32_t>> value = std::nullopt;
        std::unique_ptr<lion::lion_connector> lion_conn = std::make_unique<lion::lion_connector>();
        lion_conn->connect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        auto *sys_info = new lion_protocol::SysInfo();
        sys_info->set_type(type);
        auto *resource_type = new lion_protocol::ResourceType();
        resource_type->set_allocated_sysinfo(sys_info);

        lion_protocol::Request request;
        request.set_command(lion_protocol::GET);
        request.set_allocated_resource(resource_type);

        auto random_string = [](int len) {
            std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

            std::random_device rd;
            std::mt19937 generator(rd());

            std::shuffle(str.begin(), str.end(), generator);

            return str.substr(0, len);
        };

        lion_conn->send(random_string(10), request.SerializeAsString());

        std::string identity;
        std::string buffer;
        lion_conn->recv(identity, buffer);

        lion_conn->disconnect(CONFIG()->service.rest.connector.lion.zmq_req.addr);

        lion_protocol::Response response;
        if (response.ParseFromString(std::string(static_cast<char *>(buffer.data()), buffer.size())) &&
            response.status() != lion_protocol::FAIL)
        {
            switch (response.payload().payload_val_case())
            {
                case lion_protocol::PayloadType::kIval:
                    value = response.payload().ival();
                    break;
                case lion_protocol::PayloadType::kUval:
                    value = response.payload().uval();
                    break;
                case lion_protocol::PayloadType::kRval:
                    value = response.payload().rval();
                    break;
                case lion_protocol::PayloadType::kBval:
                    value = response.payload().bval();
                    break;
                case lion_protocol::PayloadType::kSval:
                    value = response.payload().sval();
                    break;
                case lion_protocol::PayloadType::PAYLOAD_VAL_NOT_SET:
                    break;
            }
        }
        else
        {
            LOG_ERROR(LOGGER(CONFIG()->application.name), "Failed to make request to Lion service");
        }

        return value;
    }
}
