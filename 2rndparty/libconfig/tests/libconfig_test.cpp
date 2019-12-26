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
};

TEST_F(libconfig_test, test_parse)
{
    // Arrange
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

    std::string version_value = "1.0";
    std::string title_value = "My Application";
    int w = 640;
    int h = 480;
    int x = 350;
    int y = 250;
    std::string list00 = "abc";
    int list01 = 123;
    bool list02 = true;
    double list1 = 1.234;
    std::string title0 = "Treasure Island";
    std::string author0 = "Robert Louis Stevenson";
    double price0 = 29.95;
    int qty0 = 5;
    std::string title1 = "Snow Crash";
    std::string author1 = "Neal Stephenson";
    double price1 = 9.99;
    int qty1 = 8;
    double pi = 3.141592654;
    long long bigint = 9223372036854775807;
    std::string array[3] = { "Last Name", "First Name", "MI" };
    int bitmask_value = 0x1FC3;


    try
    {
        // Act
        libcfg::const_tree_node_t cfg = libcfg::parser::from_string(config);

        // Assert
        ASSERT_EQ(std::get<std::string>(cfg["version"].value().value()), version_value);

        auto application = cfg["application"];
        auto window = application["window"];
        ASSERT_EQ(std::get<std::string>(window["title"].value().value()), title_value);

        auto size = window["size"];
        ASSERT_EQ(std::get<int>(size["w"].value().value()), w);
        ASSERT_EQ(std::get<int>(size["h"].value().value()), h);

        auto pos = window["pos"];
        ASSERT_EQ(std::get<int>(pos["x"].value().value()), x);
        ASSERT_EQ(std::get<int>(pos["y"].value().value()), y);

        auto list = application["list"];
        auto list0 = list["0"];
        ASSERT_EQ(std::get<std::string>(list0["0"].value().value()), list00);
        ASSERT_EQ(std::get<int>(list0["1"].value().value()), list01);
        ASSERT_EQ(std::get<bool>(list0["2"].value().value()), list02);

        ASSERT_EQ(std::get<double>(list["1"].value().value()), list1);
        ASSERT_TRUE(list["2"].empty());

        auto books = application["books"];
        auto book_list0 = books["0"];
        ASSERT_EQ(std::get<std::string>(book_list0["title"].value().value()), title0);
        ASSERT_EQ(std::get<std::string>(book_list0["author"].value().value()), author0);
        ASSERT_EQ(std::get<double>(book_list0["price"].value().value()), price0);
        ASSERT_EQ(std::get<int>(book_list0["qty"].value().value()), qty0);
        auto book_list1 = books["1"];
        ASSERT_EQ(std::get<std::string>(book_list1["title"].value().value()), title1);
        ASSERT_EQ(std::get<std::string>(book_list1["author"].value().value()), author1);
        ASSERT_EQ(std::get<double>(book_list1["price"].value().value()), price1);
        ASSERT_EQ(std::get<int>(book_list1["qty"].value().value()), qty1);

        auto misc = application["misc"];
        ASSERT_EQ(std::get<double>(misc["pi"].value().value()), pi);
        ASSERT_EQ(std::get<long long>(misc["bigint"].value().value()), bigint);

        auto columns = misc["columns"];
        ASSERT_EQ(std::get<std::string>(columns["0"].value().value()), array[0]);
        ASSERT_EQ(std::get<std::string>(columns["1"].value().value()), array[1]);
        ASSERT_EQ(std::get<std::string>(columns["2"].value().value()), array[2]);

        ASSERT_EQ(std::get<int>(misc["bitmask"].value().value()), bitmask_value);
    }
    catch(...)
    {
        FAIL() << "error: exception";
    }
}

