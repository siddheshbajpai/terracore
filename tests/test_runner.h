#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

// Minimal, dependency-free test runner so the suite builds without any
// external library (and without Vulkan). Each test file registers cases via
// TERRA_TEST_CASE(name) { ... TERRA_CHECK(cond) ... }.
namespace terra {
namespace test {

struct Case {
    const char* name;
    void (*fn)();
};

inline std::vector<Case>& cases() {
    static std::vector<Case> c;
    return c;
}

inline int runAll() {
    int passed = 0;
    int failed = 0;
    for (const auto& c : cases()) {
        std::printf("  RUN %s ... ", c.name);
        std::fflush(stdout);
        bool ok = true;
        try {
            c.fn();
        } catch (const char* msg) {
            ok = false;
            std::printf("FAILED: %s\n", msg);
        } catch (...) {
            ok = false;
            std::printf("FAILED: unknown exception\n");
        }
        if (ok) {
            std::printf("ok\n");
            ++passed;
        } else {
            ++failed;
        }
    }
    std::printf("Results: %d passed, %d failed of %d\n", passed, failed,
                passed + failed);
    return failed == 0 ? 0 : 1;
}

} // namespace test
} // namespace terra

#define TERRA_CHECK(cond)                                                   \
    do {                                                                    \
        if (!(cond)) {                                                      \
            std::fprintf(stderr, "CHECK failed: %s (line %d)\n", #cond,     \
                         __LINE__);                                         \
            throw #cond;                                                    \
        }                                                                   \
    } while (0)

#define TERRA_TEST_CASE(name)                                               \
    void terra_test_##name();                                               \
    struct TerraTestRegistrar_##name {                                      \
        TerraTestRegistrar_##name() {                                       \
            ::terra::test::cases().push_back({#name, terra_test_##name});   \
        }                                                                   \
    };                                                                      \
    TerraTestRegistrar_##name g_terra_registrar_##name;                     \
    void terra_test_##name()
