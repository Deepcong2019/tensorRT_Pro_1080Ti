#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };

    // 按值捕获 x
    int x = 10;
    auto lambda1 = [x](int a) { return a + x; };
    std::transform(vec.begin(), vec.end(), vec.begin(), lambda1);

    for (int i : vec) {
        std::cout << i << " ";  // 输出: 11 12 13 14 15
    }

    // 按引用捕获 y
    int y = 20;
    auto lambda2 = [&y](int a) { return a + y; };
    std::transform(vec.begin(), vec.end(), vec.begin(), lambda2);

    for (int i : vec) {
        std::cout << i << " ";  // 输出: 31 32 33 34 35
    }

    return 0;
}