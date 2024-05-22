#include <iostream>
#include <vector>
#include <future>
#include <numeric>
#include <thread>
#include <functional>


long long partial_sum(const std::vector<int>& data, size_t start, size_t end) {
    return std::accumulate(data.begin() + start, data.begin() + end, 0LL);
}


long long parallel_sum(const std::vector<int>& data) {
    size_t num_threads = std::thread::hardware_concurrency(); 
    size_t block_size = data.size() / num_threads; 

    std::vector<std::future<long long>> futures; 
    futures.reserve(num_threads); 


    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * block_size;
        size_t end = (i == num_threads - 1) ? data.size() : start + block_size;
        futures.push_back(std::async(std::launch::async, partial_sum, std::cref(data), start, end));
    }


    long long total_sum = 0;
    for (auto& future : futures) {
        total_sum += future.get();
    }

    return total_sum;
}

int main() {

    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1); // Заполняем массив числами от 1 до 1000000


    long long result = parallel_sum(data);

    std::cout << "#: " << result << std::endl;

    return 0;
}
