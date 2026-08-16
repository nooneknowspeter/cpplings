#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <string>

struct ExerciseIterator {
  ExerciseIterator() = default;
  ~ExerciseIterator() = default;

  ExerciseIterator(const ExerciseIterator&) = delete;
  ExerciseIterator(ExerciseIterator&&) = default;

  ExerciseIterator& operator=(const ExerciseIterator&) = delete;
  ExerciseIterator& operator=(ExerciseIterator&&) = default;

  static std::unique_ptr<ExerciseIterator> getInstance();

  enum class ExerciseDirectories { Exercises, Solutions };

  static std::expected<std::filesystem::path, std::string> getExerciseDirectory(
      ExerciseDirectories directory);

  void scanForExercises(ExerciseDirectories directory);

  std::expected<void, std::string> previous();

  std::expected<void, std::string> next();

  void reset();
};
