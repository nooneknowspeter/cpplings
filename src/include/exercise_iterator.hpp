#pragma once

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

    void updateExerciseDirectory(ExerciseDirectories directory);

    void scanForExercises();

    void previous();

    void current();

    void next();

    void reset();
};
