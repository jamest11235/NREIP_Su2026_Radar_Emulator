#include <iostream>
#include <deque>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <limits>
#include <cassert>
#include <gtest/gtest.h>

class peak_detector_impl {
public:
    class sliding_window_max {
    private:
        int d_i;
        int d_window_size;
        std::deque<std::pair<int, float>> d_dq;

    public:
        sliding_window_max(int window) : d_i(0), d_window_size(window) {}

        std::pair<int, float> get_current() const {
            return d_dq.front();
        }

        int get_size() const {
            return std::min(d_i, d_window_size);
        }

        void add_value(float val) {
            // 1. Expire elements outside current window range
            while (!d_dq.empty() && d_dq.front().first <= d_i - d_window_size) {
                d_dq.pop_front();
            }

            // 2. Maintain monotonic deque (pop smaller or equal elements)
            while (!d_dq.empty() && val >= d_dq.back().second) {
                d_dq.pop_back();
            }

            d_dq.push_back({d_i, val});
            d_i++;
        }
    };
};

// ============================================================================
// NAIVE BRUTE-FORCE SOLUTION (Ground Truth)
// ============================================================================
float naive_window_max(const std::vector<float>& signal, int current_idx, int window_size) {
    int start_idx = std::max(0, current_idx - window_size + 1);
    float max_val = -std::numeric_limits<float>::infinity();

    // Naively scan all elements in range [start_idx, current_idx]
    for (int j = start_idx; j <= current_idx; ++j) {
        if (signal[j] > max_val) {
            max_val = signal[j];
        }
    }
    return max_val;
}

// ============================================================================
// RANDOMIZED DIFFERENTIAL UNIT TEST
// ============================================================================
TEST(SlidingWindowMaxTest, RandomizedDifferentialTest) {
    // Test parameters
    const int WINDOW_SIZE = 1253;
    const int SEQUENCE_LENGTH = 1000000;
    
    // Seed random engine with hardware entropy or fixed seed for reproducibility
    std::random_device rd;
    std::mt19937 rng(rd());
    
    // Generate random floating-point signal sequence between -500.0 and +500.0
    std::uniform_real_distribution<float> dist(-500.0f, 500.0f);

    peak_detector_impl::sliding_window_max sw(WINDOW_SIZE);
    std::vector<float> generated_sequence;
    generated_sequence.reserve(SEQUENCE_LENGTH);

    for (int i = 0; i < SEQUENCE_LENGTH; ++i) {
        float next_sample = dist(rng);
        generated_sequence.push_back(next_sample);

        // 1. Process sample with your deque algorithm
        sw.add_value(next_sample);
        float algo_max = sw.get_current().second;

        // 2. Compute ground truth using the naive method
        float brute_max = naive_window_max(generated_sequence, i, WINDOW_SIZE);

        // 3. Compare outputs step-by-step
        SCOPED_TRACE("Failure at stream index " + std::to_string(i) + 
                     " | Sample value: " + std::to_string(next_sample));
        
        EXPECT_FLOAT_EQ(algo_max, brute_max) 
            << "Mismatch at step " << i 
            << " | Deque Algo Max: " << algo_max 
            << " vs Naive Max: " << brute_max;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
