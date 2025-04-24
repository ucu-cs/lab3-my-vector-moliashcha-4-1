// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "my_array.h"
#include "my_vector.h"

int main() {
    std::cout << "my_array tests\n";
    {
        my_array<int, 5> arr1;
        my_array<int, 3> arr2 = {1, 2, 3};

        assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3);
        std::cout << "constructor and element access test passed!\n";

        try {
            arr2.at(5);
            assert(false);
        } catch (const std::out_of_range&) {
            std::cout << "at() test passed!\n";
        }

        assert(arr2.front() == 1);
        assert(arr2.back() == 3);
        std::cout << "front/back test passed!\n";
    }

    {
        // array.begin() == array.end()
        my_array<int, 0> empty_array;
        assert(empty_array.begin() == empty_array.end());
        std::cout << "zero-sized array test passed!\n" << std::endl;
    }

    {
        my_array<int, 3> arr = {1, 2, 3};
        int sum = 0;
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            sum += *it;
        }
        assert(sum == 6);
        std::cout << "iterator test passed!\n";

        std::vector<int> reversed;
        for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
            reversed.push_back(*it);
        }
        assert(reversed[0] == 3 && reversed[1] == 2 && reversed[2] == 1);
        std::cout << "reverse iterator test passed!\n";
    }

    {
        my_array<int, 3> arr1;
        arr1.fill(314);
        assert(arr1[0] == 314 && arr1[1] == 314 && arr1[2] == 314);

        my_array<int, 3> arr2 = {1, 2, 3};
        arr1.swap(arr2);
        assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3);
        assert(arr2[0] == 314 && arr2[1] == 314 && arr2[2] == 314);
        std::cout << "modifiers test passed!\n";
    }

    {
        my_array<std::string, 2> str_arr = {"pok", "acs"};
        assert(str_arr[0] == "pok" && str_arr[1] == "acs");

        my_array<my_array<int, 2>, 2> nested_arr;
        nested_arr[0][0] = 1;
        nested_arr[0][1] = 2;
        nested_arr[1][0] = 3;
        nested_arr[1][1] = 4;
        assert(nested_arr[0][0] == 1 && nested_arr[1][1] == 4);
        std::cout << "complex types test passed!\n";
    }

    {
        my_array<int, 5> arr = {5, 2, 3, 1, 4};
        std::sort(arr.begin(), arr.end());
        assert(arr[0] == 1 && arr[1] == 2 && arr[4] == 5);
        std::cout << "algorithm test passed!\n";
    }

    std::cout << "my_vector tests\n";
    {
        my_vector<int> vec1;
        my_vector<int> vec2(3, 314);
        my_vector<int> vec3 = {1, 2, 3};

        assert(vec3[0] == 1 && vec3[1] == 2 && vec3[2] == 3);
        std::cout << "constructor and element access test passed!\n";

        try {
            vec3.at(5);
            assert(false);
        } catch (const std::out_of_range&) {
            std::cout << "at() test passed!\n";
        }

        assert(vec3.front() == 1);
        assert(vec3.back() == 3);
        std::cout << "front/back test passed!\n";
    }

    {
        my_vector<int> vec = {1, 2, 3};
        int sum = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            sum += *it;
        }
        assert(sum == 6);
        std::cout << "iterator test passed!\n";

        std::vector<int> reversed;
        for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
            reversed.push_back(*it);
        }
        assert(reversed[0] == 3 && reversed[1] == 2 && reversed[2] == 1);
        std::cout << "reverse iterator test passed!\n";
    }

    {
        my_vector<int> vec;
        assert(vec.is_empty());

        vec.reserve(5);
        for (int i = 1; i <= 3; ++i) {
            vec.push_back(i);
        }

        assert(vec.size() == 3 && vec.capacity() >= 5);
        vec.shrink_to_fit();
        assert(vec.capacity() == 3);
        std::cout << "capacity test passed!\n";
    }

    {
        my_vector<int> vec1 = {1, 2, 3};
        vec1.push_back(vec1[1]); // self-reference
        assert(vec1.size() == 4 && vec1[3] == 2);

        vec1.pop_back();
        assert(vec1.size() == 3);

        vec1.clear();
        assert(vec1.is_empty());
        std::cout << "modifier test passed!\n";

        my_vector<int> vec2 = {1, 2, 3};
        auto it = vec2.insert(vec2.begin() + 1, 314);
        assert(*it == 314 && vec2.size() == 4 && vec2[1] == 314);

        it = vec2.erase(vec2.begin());
        assert(*it == 314 && vec2.size() == 3);

        my_vector<int> vec3 = {1, 2, 3};
        vec2.swap(vec3);
        assert(vec2.size() == 3 && vec2[0] == 1 && vec3.size() == 3 && vec3[0] == 314);
        std::cout << "insert/erase/swap test passed!\n";
    }

    {
        my_vector<std::string> str_vec = {"pok", "acs", "os"};
        assert(str_vec[0] == "pok" && str_vec[1] == "acs" && str_vec[2] == "os");

        my_vector<my_vector<int>> nested_vec;
        nested_vec.push_back(my_vector<int>{1, 2, 3});
        nested_vec.push_back(my_vector<int>{1, 2, 3});
        assert(nested_vec[0][1] == 2 && nested_vec[1][2] == 3);
        std::cout << "complex types test passed!\n";
    }

    {
        my_vector<int> vec = {3, 1, 2};
        std::sort(vec.begin(), vec.end());
        assert(vec[0] == 1 && vec[1] == 2 && vec[2] == 3);
        std::cout << "algorithm test passed!\n";
    }

    std::cout << "all tests passed!" << std::endl;

    return 0;
}
