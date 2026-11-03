#ifndef TESTS_SNAPSHOT_TEST_FRAMEWORK_H
#define TESTS_SNAPSHOT_TEST_FRAMEWORK_H

#include <stdint.h>

#include <string>
#include <vector>
#include <functional>

void configureTest(
    int argc,
    char **argv,
    std::string testName,
    std::string snapshotDirectory,
    uint32_t width,
    uint32_t height);

void requireMatchingSnapshot(std::function<std::vector<uint8_t>()> generateActualContent);

#endif // TESTS_SNAPSHOT_TEST_FRAMEWORK_H