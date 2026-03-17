const STD = @import("std");

const STYLES = @import("styles.zig");

const EXERCISE_OUT_PATH = "./build/cpplings_exercise";

pub const ExerciseRunner = struct {
    const Self = @This();

    pub const CompilerStrategy = enum {
        zig,
        gpp,
        clang,
    };

    current_strategy: CompilerStrategy = .zig,
    current_exercise_stdout: STD.ArrayList(u8) = .empty,
    current_exercise_stderr: STD.ArrayList(u8) = .empty,
    did_current_exercise_compile: bool = false,

    pub fn init() Self {
        return Self{};
    }

    pub fn deinit(self: *Self, allocator: STD.mem.Allocator) void {
        self.current_exercise_stdout.deinit(allocator);
        self.current_exercise_stderr.deinit(allocator);
    }

    pub fn setStrategy(self: *Self, strategy: CompilerStrategy) void {
        self.current_strategy = strategy;
    }

    pub fn compile(
        self: *Self,
        allocator: STD.mem.Allocator,
        exercise_path: []const u8,
        support_files: []const []const u8,
    ) !void {
        self.current_exercise_stdout.clearAndFree(allocator);
        self.current_exercise_stderr.clearAndFree(allocator);

        var process_args = try self.buildCompilerArgs(allocator, exercise_path, support_files);
        defer process_args.deinit(allocator);

        var process = STD.process.Child.init(process_args.items, allocator);

        process.stderr_behavior = .Pipe;
        process.stdout_behavior = .Pipe;

        process.spawn() catch {
            STD.debug.print("\n{s}Error compiling exercise...{s}\n", .{
                STYLES.ASCII_STYLES.red,
                STYLES.ASCII_STYLES.clear_style,
            });
            return;
        };

        try process.collectOutput(
            allocator,
            &self.current_exercise_stdout,
            &self.current_exercise_stderr,
            1 << 16,
        );

        const PROCESS_STATUS = try process.wait();

        if (PROCESS_STATUS.Exited == 0) {
            self.did_current_exercise_compile = true;
            return;
        }

        self.did_current_exercise_compile = false;
    }

    fn buildCompilerArgs(
        self: *Self,
        allocator: STD.mem.Allocator,
        exercise_path: []const u8,
        support_files: []const []const u8,
    ) !STD.ArrayList([]const u8) {
        var process_args: STD.ArrayList([]const u8) = .empty;

        switch (self.current_strategy) {
            .zig => {
                try process_args.appendSlice(allocator, &[_][]const u8{
                    "zig",
                    "build",
                    "exercises",
                    "--",
                    exercise_path,
                });
            },
            .gpp => {
                try process_args.append(allocator, "g++");
                try process_args.append(allocator, "-o");
                try process_args.append(allocator, EXERCISE_OUT_PATH);
                try process_args.append(allocator, exercise_path);
            },
            .clang => {
                try process_args.append(allocator, "clang++");
                try process_args.append(allocator, "-o");
                try process_args.append(allocator, EXERCISE_OUT_PATH);
                try process_args.append(allocator, exercise_path);
            },
        }

        if (support_files.len > 0) {
            try process_args.appendSlice(allocator, support_files);
        }

        return process_args;
    }

    pub fn getChapterSupportFiles(
        self: *Self,
        allocator: STD.mem.Allocator,
        exercise_path: []const u8,
    ) !STD.ArrayList([]const u8) {
        _ = self;

        var path_parts = STD.mem.splitAny(u8, exercise_path, "/");
        var path_parts_list: STD.ArrayList([]const u8) = .empty;
        defer path_parts_list.deinit(allocator);

        while (path_parts.next()) |part| {
            if (STD.mem.containsAtLeast(u8, part, 1, ".cpp")) {
                break;
            }
            try path_parts_list.append(allocator, part);
        }

        const chapter_path = try STD.fs.path.join(allocator, path_parts_list.items);
        defer allocator.free(chapter_path);

        var chapter_dir = try STD.fs.cwd().openDir(chapter_path, .{ .iterate = true });
        defer chapter_dir.close();

        var chapter_contents = chapter_dir.iterate();

        var support_files: STD.ArrayList([]const u8) = .empty;

        while (try chapter_contents.next()) |content| {
            if (STD.mem.containsAtLeast(u8, content.name, 1, "src")) {
                const support_dir_path = try STD.fs.path.join(allocator, &[_][]const u8{
                    chapter_path,
                    content.name,
                });
                defer allocator.free(support_dir_path);

                var support_dir = try STD.fs.cwd().openDir(support_dir_path, .{ .iterate = true });
                defer support_dir.close();

                var support_contents = support_dir.iterate();

                while (try support_contents.next()) |support_file| {
                    if (STD.mem.containsAtLeast(u8, support_file.name, 1, ".cpp")) {
                        const full_path = try STD.fs.path.join(allocator, &[_][]const u8{
                            support_dir_path,
                            support_file.name,
                        });
                        try support_files.append(allocator, full_path);
                    }
                }
            }
        }

        return support_files;
    }
};
