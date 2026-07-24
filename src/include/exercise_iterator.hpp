#pragma once

struct ExerciseIterator
{
    enum class ExerciseDirectories
    {
        Exercises,
        Solutions
    };

    static void updateExerciseDirectory(ExerciseDirectories directory);

    static void scanForExercises();

    static void previous();

    static void current();

    static void next();

    static void reset();
};
