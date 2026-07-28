#pragma once

#include <memory>
#include <string>

struct ExerciseRunner
{
    ExerciseRunner() = default;
    ~ExerciseRunner() = default;

    ExerciseRunner(const ExerciseRunner &) = delete;
    ExerciseRunner(ExerciseRunner &&) = default;

    ExerciseRunner &operator=(const ExerciseRunner &) = delete;
    ExerciseRunner &operator=(ExerciseRunner &&) = default;

    void compileCurrentExercise();

    void compileAllExercises();

    static std::unique_ptr<ExerciseRunner> getInstance();
};
