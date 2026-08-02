#pragma once

#include <filesystem>
#include <memory>

struct ExerciseIterator
{
    ExerciseIterator() = default;
    ~ExerciseIterator() = default;

    ExerciseIterator(const ExerciseIterator &) = delete;
    ExerciseIterator(ExerciseIterator &&) = default;

    ExerciseIterator &operator=(const ExerciseIterator &) = delete;
    ExerciseIterator &operator=(ExerciseIterator &&) = default;

    static std::unique_ptr<ExerciseIterator> getInstance();

    enum class ExerciseDirectories
    {
        Exercises,
        Solutions
    };

    static std::filesystem::path getExerciseDirectory(ExerciseDirectories directory);

    void scanForExercises(ExerciseDirectories directory);

    void previous();

    void next();

    void reset();
};
