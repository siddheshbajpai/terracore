#include "tests/test_runner.h"

int main() {
    std::printf("TerraCore test suite\n");
    std::printf("====================\n");
    int rc = terra::test::runAll();
    return rc;
}
