#include "SnapshotTestFramework.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <filesystem>
#include <sstream>
#include <stdexcept>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool isSnapshotUpdateRequested = false;

std::string testName;
std::string snapshotDirectory;

uint32_t width = 0;
uint32_t height = 0;

void writeJpegToVector(void *context, void *data, int size)
{
    std::vector<uint8_t> *jpeg = static_cast<std::vector<uint8_t> *>(context);
    const uint8_t *bytes = static_cast<const uint8_t *>(data);
    jpeg->insert(jpeg->end(), bytes, bytes + size);
}

void configureTest(
    int argc,
    char **argv,
    std::string testName,
    std::string snapshotDirectory,
    uint32_t width,
    uint32_t height)
{
    if (argc == 2 && strcmp(argv[1], "--update-snapshot") == 0)
    {
        isSnapshotUpdateRequested = true;
    }

    ::testName = testName;
    ::snapshotDirectory = snapshotDirectory;

    ::width = width;
    ::height = height;
}

void requireMatchingSnapshot(std::function<std::vector<uint8_t>()> generateActualContent)
{
    constexpr uint32_t bytesPerPixel = 3;
    std::vector<uint8_t> actualContent = generateActualContent();
    const size_t expectedContentSize = static_cast<size_t>(width) * height * bytesPerPixel;

    if (actualContent.size() != expectedContentSize)
    {
        throw std::runtime_error("Snapshot content does not match the configured RGB framebuffer dimensions.");
    }

    // glReadPixels returns rows bottom-to-top (OpenGL's origin is bottom-left), while JPEG
    // (and every other image format we use) expects rows top-to-bottom. Flip here so the
    // saved/compared snapshot matches what was actually rendered on screen.
    const size_t rowSize = static_cast<size_t>(width) * bytesPerPixel;
    for (uint32_t row = 0; row < height / 2; ++row)
    {
        std::swap_ranges(
            actualContent.begin() + row * rowSize,
            actualContent.begin() + (row + 1) * rowSize,
            actualContent.begin() + (height - 1 - row) * rowSize);
    }

    std::vector<uint8_t> actualJpeg;
    if (!stbi_write_jpg_to_func(
            writeJpegToVector,
            &actualJpeg,
            static_cast<int>(width),
            static_cast<int>(height),
            bytesPerPixel,
            actualContent.data(),
            100))
    {
        throw std::runtime_error("Unable to encode snapshot as JPEG.");
    }

    const std::filesystem::path snapshotPath = std::filesystem::path(snapshotDirectory) / (testName + ".jpg");

    if (isSnapshotUpdateRequested)
    {
        std::filesystem::create_directories(snapshotPath.parent_path());
        std::ofstream snapshot(snapshotPath, std::ios::binary | std::ios::trunc);
        if (!snapshot)
        {
            throw std::runtime_error("Unable to write snapshot: " + snapshotPath.string());
        }

        snapshot.write(reinterpret_cast<const char *>(actualJpeg.data()), actualJpeg.size());

        if (!snapshot)
        {
            throw std::runtime_error("Failed to write snapshot: " + snapshotPath.string());
        }
        return;
    }

    std::ifstream snapshot(snapshotPath, std::ios::binary);
    if (!snapshot)
    {
        throw std::runtime_error("Snapshot does not exist: " + snapshotPath.string());
    }

    const std::vector<uint8_t> expectedJpeg{
        std::istreambuf_iterator<char>(snapshot),
        std::istreambuf_iterator<char>()};

    // JPEG encoding of the reference snapshot is lossy, and the software rasterizer can
    // produce imperceptible per-pixel rounding differences across platforms/compilers
    // (e.g. the reference was captured on Windows, this run is on macOS). Comparing the
    // compressed bytes exactly would fail on such differences even though the images are
    // visually identical, so decode the reference and compare pixels with a tolerance instead.
    // stbi_set_flip_vertically_on_load() is global state that may have been left set by the
    // test itself (e.g. when loading textures), so reset it to avoid flipping the reference
    // snapshot relative to the unflipped framebuffer captured in actualContent.
    stbi_set_flip_vertically_on_load(0);

    int expectedImageWidth = 0;
    int expectedImageHeight = 0;
    int expectedImageChannels = 0;
    uint8_t *expectedPixels = stbi_load_from_memory(
        expectedJpeg.data(),
        static_cast<int>(expectedJpeg.size()),
        &expectedImageWidth,
        &expectedImageHeight,
        &expectedImageChannels,
        static_cast<int>(bytesPerPixel));

    if (expectedPixels == nullptr)
    {
        throw std::runtime_error("Unable to decode snapshot: " + snapshotPath.string());
    }

    if (static_cast<uint32_t>(expectedImageWidth) != width || static_cast<uint32_t>(expectedImageHeight) != height)
    {
        stbi_image_free(expectedPixels);
        throw std::runtime_error("Snapshot dimensions do not match: " + snapshotPath.string());
    }

    // Allow small per-channel deviations (JPEG quantization, anti-aliasing/rounding
    // differences) but still catch pixels that are actually rendered differently.
    constexpr int perChannelTolerance = 24;
    constexpr double maxMismatchedPixelRatio = 0.01;

    size_t mismatchedPixelCount = 0;
    const size_t pixelCount = static_cast<size_t>(width) * height;
    for (size_t pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
    {
        const size_t byteOffset = pixelIndex * bytesPerPixel;
        bool pixelMismatched = false;
        for (uint32_t channel = 0; channel < bytesPerPixel; ++channel)
        {
            const int actualValue = actualContent[byteOffset + channel];
            const int expectedValue = expectedPixels[byteOffset + channel];
            if (std::abs(actualValue - expectedValue) > perChannelTolerance)
            {
                pixelMismatched = true;
                break;
            }
        }

        if (pixelMismatched)
        {
            ++mismatchedPixelCount;
        }
    }

    stbi_image_free(expectedPixels);

    const double mismatchedPixelRatio = static_cast<double>(mismatchedPixelCount) / static_cast<double>(pixelCount);
    if (mismatchedPixelRatio > maxMismatchedPixelRatio)
    {
        std::ostringstream errorMessage;
        errorMessage << "Snapshot does not match: " << snapshotPath.string() << " ("
                     << mismatchedPixelCount << "/" << pixelCount << " pixels, "
                     << (mismatchedPixelRatio * 100.0) << "% mismatched)";
        throw std::runtime_error(errorMessage.str());
    }
}
