// internal
#include <Libconfig>

// gtest
#include <gtest/gtest.h>

// std
#include <variant>

class libconfig_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
protected:
    const std::string config_ = "version = \"1.0\";" \
            "application:" \
            "{" \
                "window:" \
                "{" \
                     "title = \"My Application\";" \
                     "size = { w = 640; h = 480; };" \
                     "pos = { x = 350; y = 250; };" \
                "};" \

                "list = ( ( \"abc\", 123, true ), 1.234, ( /* an empty list */) );"\

                "books = ( { title  = \"Treasure Island\";" \
                               "author = \"Robert Louis Stevenson\";" \
                               "price  = 29.95;" \
                               "qty    = 5; }," \
                             "{ title  = \"Snow Crash\";" \
                               "author = \"Neal Stephenson\";" \
                               "price  = 9.99;" \
                               " qty    = 8; } );" \

                "misc:" \
                "{" \
                     "pi = 3.141592654;" \
                     "bigint = 9223372036854775807L;" \
                     " columns = [ \"Last Name\", \"First Name\", \"MI\" ];" \
                     " bitmask = 0x1FC3;" \
                "};" \
            "};";
};

TEST_F(libconfig_test, parse)
{
    const std::string config = "version = \"1.0\";" \
            "application:" \
            "{" \
                "window:" \
                "{" \
                     "title = \"My Application\";" \
                     "size = { w = 640; h = 480; };" \
                     "pos = { x = 350; y = 250; };" \
                "};" \

                "list = ( ( \"abc\", 123, true ), 1.234, ( /* an empty list */) );"\

                "books = ( { title  = \"Treasure Island\";" \
                               "author = \"Robert Louis Stevenson\";" \
                               "price  = 29.95;" \
                               "qty    = 5; }," \
                             "{ title  = \"Snow Crash\";" \
                               "author = \"Neal Stephenson\";" \
                               "price  = 9.99;" \
                               " qty    = 8; } );" \

                "misc:" \
                "{" \
                     "pi = 3.141592654;" \
                     "bigint = 9223372036854775807L;" \
                     " columns = [ \"Last Name\", \"First Name\", \"MI\" ];" \
                     " bitmask = 0x1FC3;" \
                "};" \
            "};";
    libcfg::const_tree_node_t cfg = libcfg::parser::from_string(config);
    std::cout << "version = " << std::get<std::string>(cfg["version"].value().value()) << std::endl;

    auto application = cfg["application"];
    auto window = application["window"];
    std::cout << "application: window: title:" << std::get<std::string>(window["title"].value().value()) << std::endl;

    auto size = window["size"];
    std::cout << "application: window: size: w:" << std::get<int>(size["w"].value().value()) << std::endl;
    std::cout << "application: window: size: h:" << std::get<int>(size["h"].value().value()) << std::endl;

    auto pos = window["pos"];
    std::cout << "application: window: pos: x:" << std::get<int>(pos["x"].value().value()) << std::endl;
    std::cout << "application: window: pos: y:" << std::get<int>(pos["y"].value().value()) << std::endl;

    auto list = application["list"];
    auto list0 = list["0"];
    std::cout << "application: list[0]:(" << std::get<std::string>(list0["0"].value().value()) << ", "
                                           << std::get<int>(list0["1"].value().value()) << ", "
                                           << (std::get<bool>(list0["2"].value().value()) ? "true" : "false")
                                           << ")" << std::endl;
    std::cout << "application: list[1]:" << std::get<double>(list["1"].value().value()) << std::endl;
    std::cout << "application: list[2]:" << (list["2"].empty() ? "empty" : "error")
                                         << std::endl;

    auto books = application["books"];
    auto book_list0 = books["0"];
    std::cout << "application: books[0]: title:" << std::get<std::string>(book_list0["title"].value().value()) << std::endl;
    std::cout << "application: books[0]: author:" << std::get<std::string>(book_list0["author"].value().value()) << std::endl;
    std::cout << "application: books[0]: price:" << std::get<double>(book_list0["price"].value().value()) << std::endl;
    std::cout << "application: books[0]: qty:" << std::get<int>(book_list0["qty"].value().value()) << std::endl;
    auto book_list1 = books["1"];
    std::cout << "application: books[1]: title:" << std::get<std::string>(book_list1["title"].value().value()) << std::endl;
    std::cout << "application: books[1]: author:" << std::get<std::string>(book_list1["author"].value().value()) << std::endl;
    std::cout << "application: books[1]: price:" << std::get<double>(book_list1["price"].value().value()) << std::endl;
    std::cout << "application: books[1]: qty:" << std::get<int>(book_list1["qty"].value().value()) << std::endl;

    auto misc = application["misc"];
    std::cout << "application: misc: pi:" << std::get<double>(misc["pi"].value().value()) << std::endl;
    std::cout << "application: misc: bigint:" << std::get<long long>(misc["bigint"].value().value()) << std::endl;
    auto columns = misc["columns"];
    std::cout << "application: misc: columns[0]:" << std::get<std::string>(columns["0"].value().value()) << std::endl;
    std::cout << "application: misc: columns[1]:" << std::get<std::string>(columns["1"].value().value()) << std::endl;
    std::cout << "application: misc: columns[2]:" << std::get<std::string>(columns["2"].value().value()) << std::endl;

}

