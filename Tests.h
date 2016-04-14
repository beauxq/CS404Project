#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <string>

namespace Tests
{
    bool test_equal(const std::string& first,
                    const std::string& second,
                    const std::string& test_name);
    bool test_equal(const unsigned long long int& first,
                    const unsigned long long int& second,
                    const std::string& test_name);

    int test_grid_class();
    int test_path_algorithms();

    int test_both_algorithms(const std::string& filename,
                             const std::string& expected_shortest,
                             const std::string& expected_second_shortest);
}

#endif // TESTS_H_INCLUDED
