#pragma once

#include <filesystem>
#include <memory>

struct PatchSystem
{
    static std::unique_ptr<PatchSystem> getInstance();

    PatchSystem() = default;
    ~PatchSystem() = default;

    PatchSystem(const PatchSystem &) = delete;
    PatchSystem(PatchSystem &&) = default;

    PatchSystem &operator=(const PatchSystem &) = delete;
    PatchSystem &operator=(PatchSystem &&) = default;

    static void generatePatches();

    static void patch(std::filesystem::path exercise_file_path);
};
