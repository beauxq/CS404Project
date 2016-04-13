#include "Tests.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Grid.h"

bool Tests::test_equal(const std::string& first, const std::string& second, const std::string& test_name)
{
    if (first != second)
    {
        std::cout << " ** FAIL ** : " << test_name << std::endl;
        return false;
    }
    // first == second
    std::cout << "PASS: " << test_name << std::endl;
    return true;
}


void Tests::test_grid_class()
{
    Grid grid;
    test_equal(grid.visual_str(), "0 0", "default constructed grid and str");

    grid.init(7, 4);
    test_equal(grid.visual_str(),
               "7 4\n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 ",
               "grid.init(7, 4) and str");

    test_equal(grid.data_str(), "7, 4\n", "data_str Grid(7, 4) omit ones default");

    Grid grid2(7, 4);
    test_equal(grid2.visual_str(),
               "7 4\n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 \n1  1  1  1 ",
               "Grid grid2(7, 4) and str");

    std::cout << "test non-existent file:\n";
    grid.read_from_file("lol");
    test_equal(grid.visual_str(), "0 0", "bad file read_from_file and str");

    std::string input_filename("resources/CS404SP16RewardMatrixInput1.txt");
    grid.read_from_file(input_filename);
    test_equal(grid.visual_str(), "7 8\n"
                                   "  1  117   15   11   20   13   15   14 \n"
                                   "117  972  117   14   19   20   11   11 \n"
                                   " 13  117  972  117   18   19   11   20 \n"
                                   " 17   19  117  972  117   15   16   16 \n"
                                   " 12   12   14  117  972  117   12   10 \n"
                                   " 10   12   14   14  117  972  117   20 \n"
                                   " 11   17   13   11   18  117   18   16 ",
               "given input 1 read and str");
    std::ifstream t(input_filename);
    std::stringstream buffer;
    buffer << t.rdbuf();
    test_equal(grid.data_str(false),
               buffer.str(),
               "input 1 data_str don't omit ones");
    test_equal(grid.data_str(true),
               buffer.str().substr(0, 5) + buffer.str().substr(13),
               "input 1 data_str omit ones parameter 1 one");

    Grid grid3("resources/CS404SP16RewardMatrixInput2.txt");
    test_equal(grid3.visual_str(), "9 13\n"
                                   "  2  117  169  154  148  179  137  127  144  128  162  166  153 \n"
                                   "117  188  117  160  157  118  156  154  119  187  169  126  111 \n"
                                   "180  117  188  117  177  188  161  163  122  129  173  150  113 \n"
                                   "159  159  117  188  117  137  182  112  183  151  114  154  123 \n"
                                   "129  120  125  117  188  117  162  135  182  128  153  165  150 \n"
                                   "127  180  162  191  117  188  117  173  180  146  146  120  150 \n"
                                   "176  126  153  163  171  117  188  117  113  115  121  147  185 \n"
                                   "114  132  150  168  121  140  117  188  117  134  190  121  133 \n"
                                   "151  162  168  177  191  175  179  117  146  171  141  191  141 ",
               "given input 2 construct read and str");
}

void Tests::test_dp()
{
    Grid grid(7, 4);
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "7 4\n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1- 1- 1-",
               "Grid(7, 4) shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "7 4\n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1  1  1 \n"
               "1- 1- 1  1 \n"
               "1  1- 1- 1-",
               "Grid(7, 4) second shortest");
    test_equal(grid.path_str(grid.get_shortest_if_found()),
               "0 0 0 0 0 0 1 1 1",
               "path_str Grid(7, 4) shortest");

    std::string input_filename("resources/CS404SP16RewardMatrixInput1.txt");
    grid.read_from_file(input_filename);
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "7 8\n"
               "  1- 117-  15-  11-  20-  13-  15-  14 \n"
               "117  972  117   14   19   20   11-  11 \n"
               " 13  117  972  117   18   19   11-  20 \n"
               " 17   19  117  972  117   15   16-  16 \n"
               " 12   12   14  117  972  117   12-  10-\n"
               " 10   12   14   14  117  972  117   20-\n"
               " 11   17   13   11   18  117   18   16-",
               "given Input1 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "7 8\n"
               "  1- 117-  15-  11-  20-  13-  15-  14 \n"
               "117  972  117   14   19   20   11-  11 \n"
               " 13  117  972  117   18   19   11-  20 \n"
               " 17   19  117  972  117   15   16-  16-\n"
               " 12   12   14  117  972  117   12   10-\n"
               " 10   12   14   14  117  972  117   20-\n"
               " 11   17   13   11   18  117   18   16-",
               "given Input1 second shortest");

    grid.read_from_file("resources/CS404SP16RewardMatrixInput2.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "9 13\n"
               "  2- 117- 169- 154  148  179  137  127  144  128  162  166  153 \n"
               "117  188  117- 160- 157  118  156  154  119  187  169  126  111 \n"
               "180  117  188  117- 177- 188  161  163  122  129  173  150  113 \n"
               "159  159  117  188  117- 137- 182  112  183  151  114  154  123 \n"
               "129  120  125  117  188  117- 162- 135  182  128  153  165  150 \n"
               "127  180  162  191  117  188  117- 173- 180  146  146  120  150 \n"
               "176  126  153  163  171  117  188  117- 113- 115- 121- 147- 185 \n"
               "114  132  150  168  121  140  117  188  117  134  190  121- 133-\n"
               "151  162  168  177  191  175  179  117  146  171  141  191  141-",
               "given Input2 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "9 13\n"
               "  2- 117  169  154  148  179  137  127  144  128  162  166  153 \n"
               "117- 188  117  160  157  118  156  154  119  187  169  126  111 \n"
               "180- 117- 188  117  177  188  161  163  122  129  173  150  113 \n"
               "159  159- 117- 188  117  137  182  112  183  151  114  154  123 \n"
               "129  120  125- 117- 188- 117- 162- 135  182  128  153  165  150 \n"
               "127  180  162  191  117  188  117- 173- 180  146  146  120  150 \n"
               "176  126  153  163  171  117  188  117- 113- 115- 121- 147- 185 \n"
               "114  132  150  168  121  140  117  188  117  134  190  121- 133-\n"
               "151  162  168  177  191  175  179  117  146  171  141  191  141-",
               "given Input2 second shortest");

    grid.read_from_file("resources/myInput1.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   1-   2   99   99 \n"
               "  3    2-   2-   1-  99 \n"
               "  6    4    3    1-  99 \n"
               "  9    2  100    3-   1-\n"
               "  7   11    4    2    5-",
               "myInput1 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   1-   2-  99   99 \n"
               "  3    2    2-   1-  99 \n"
               "  6    4    3    1-  99 \n"
               "  9    2  100    3-   1-\n"
               "  7   11    4    2    5-",
               "myInput1 second shortest");

    grid.read_from_file("resources/myInput2.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
                "5 5\n"
               "  2-   1-   3   99   99 \n"
               "  3    2-   2-   1-  99 \n"
               "  6    4    3    1-  99 \n"
               "  9    2  100    3-   1-\n"
               "  7   11    4    2    5-",
               "myInput2 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   1-   3   99   99 \n"
               "  3    2-   2-   1-  99 \n"
               "  6    4    3    1-  99 \n"
               "  9    2  100    3-   1 \n"
               "  7   11    4    2-   5-",
               "myInput2 second shortest");

    grid.read_from_file("resources/myInput3.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "98- 99- 98-  1-  1-\n"
               "97  98  98  99   1-\n"
               "94  96  97  99   1-\n"
               "91  98   0  97  99-\n"
               "93  89  96  98  95-",
               "myInput3 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "98- 99- 98-  1-  1 \n"
               "97  98  98  99-  1-\n"
               "94  96  97  99   1-\n"
               "91  98   0  97  99-\n"
               "93  89  96  98  95-",
               "myInput3 second shortest");

    grid.read_from_file("resources/myInput4.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7-   5-\n"
               "  4  100  100  100   11-\n"
               "  6  100  100  100    1-\n"
               "  9  100  100  100    4-\n"
               "  4   12    2    3   10-",
               "myInput4 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7    5 \n"
               "  4- 100  100  100   11 \n"
               "  6- 100  100  100    1 \n"
               "  9- 100  100  100    4 \n"
               "  4-  12-   2-   3-  10-",
               "myInput4 second shortest");

    grid.read_from_file("resources/myInput5.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7-   5-\n"
               "  4  100  100  100   11-\n"
               "  6  100  100  100    1-\n"
               "  9    4  100  100    4-\n"
               "100   12    2    3   10-",
               "myInput5 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7    5 \n"
               "  4- 100  100  100   11 \n"
               "  6- 100  100  100    1 \n"
               "  9-   4- 100  100    4 \n"
               "100   12-   2-   3-  10-",
               "myInput5 second shortest");

    grid.read_from_file("resources/myInput6.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7    5 \n"
               "  4- 100  100  100   11 \n"
               "  6- 100  100  100    1 \n"
               "  9-   3- 100  100    4 \n"
               "100   12-   2-   3-  10-",
               "myInput6 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7-   5-\n"
               "  4  100  100  100   11-\n"
               "  6  100  100  100    1-\n"
               "  9    3  100  100    4-\n"
               "100   12    2    3   10-",
               "myInput6 second shortest");

    grid.read_from_file("resources/myInput7.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7  100 \n"
               "  4- 100  100    5   11 \n"
               "  6- 100  100  100    1 \n"
               "  9-   3- 100  100    4 \n"
               "100   12-   2-   3-  10-",
               "myInput7 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7- 100 \n"
               "  4  100  100    5-  11-\n"
               "  6  100  100  100    1-\n"
               "  9    3  100  100    4-\n"
               "100   12    2    3   10-",
               "myInput7 second shortest");

    grid.read_from_file("resources/myInput8.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7  100 \n"
               "  4- 100  100    5   11 \n"
               "  6- 100  100  100    1 \n"
               "  9- 100  100  100    4 \n"
               "  3-  12-   2-   3-  10-",
               "myInput8 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7- 100 \n"
               "  4  100  100    5-  11-\n"
               "  6  100  100  100    1-\n"
               "  9  100  100  100    4-\n"
               "  3   12    2    3   10-",
               "myInput8 second shortest");

    grid.read_from_file("resources/myInput9.txt");
    grid.dp_find_min_paths();
    test_equal(grid.visual_str(grid.get_shortest_if_found()),
               "5 5\n"
               "  2-   3    8    7    5 \n"
               "  4- 100  100  100   11 \n"
               "  6- 100  100  100    1 \n"
               "  9- 100  100  100    4 \n"
               "  3-  12-   2-   3-  10-",
               "myInput9 shortest");
    test_equal(grid.visual_str(grid.get_second_shortest_if_found()),
               "5 5\n"
               "  2-   3-   8-   7-   5-\n"
               "  4  100  100  100   11-\n"
               "  6  100  100  100    1-\n"
               "  9  100  100  100    4-\n"
               "  3   12    2    3   10-",
               "myInput9 second shortest");
}
