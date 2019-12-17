// internal
#include <Libconfig>

// gtest
#include <gtest/gtest.h>

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
    libcfg::const_tree_t cfg = libcfg::parser::parse_from_str(config);
}
