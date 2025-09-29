const std = @import("std");

var stdin_buffer: [1024]u8 = undefined;
var stdin_reader = std.fs.File.stdin().reader(&stdin_buffer);
const stdin = &stdin_reader.interface;

var stdout_buffer: [1024]u8 = undefined;
var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
const stdout = &stdout_writer.interface;

pub const ExerciseList = struct {
    allocator: std.mem.Allocator,
    filenames: std.ArrayList([]const u8),

    pub fn init(allocator: std.mem.Allocator) !*ExerciseList {
        var self = try allocator.create(ExerciseList);

        self.* = .{ .allocator = allocator, .filenames = .empty };

        try self.fill();
        self.sort();

        return self;
    }

    pub fn deinit(self: *ExerciseList) void {
        self.filenames.deinit(self.allocator);
        self.allocator.destroy(self);
    }

    fn fill(self: *ExerciseList) !void {
        const exercises_dir_path = "exercises/";

        const exercises_dir = try std.fs.cwd().openDir(exercises_dir_path, .{ .iterate = true });
        var chapters = exercises_dir.iterate();

        while (try chapters.next()) |chapter| {
            const chapter_dir_path = try std.fs.path.join(self.allocator, &.{ exercises_dir_path, chapter.name });

            const chapter_dir = try std.fs.cwd().openDir(chapter_dir_path, .{ .iterate = true });
            var exercises = chapter_dir.iterate();

            while (try exercises.next()) |exercise| {
                const exercise_file_path = try std.fs.path.join(self.allocator, &.{ exercises_dir_path, chapter.name, exercise.name });

                try self.filenames.append(self.allocator, exercise_file_path);
            }
        }
    }

    fn sort(self: @This()) void {
        std.sort.insertion(
            []const u8,
            self.filenames.items,
            {},
            struct {
                fn lessThan(_: void, a: []const u8, b: []const u8) bool {
                    return std.mem.lessThan(u8, a, b);
                }
            }.lessThan,
        );
    }

    fn print(self: @This()) void {
        for (self.filenames.items) |item| {
            std.debug.print("{s}\n", .{item});
        }
    }
};

const ASCII_STYLES = struct {
    pub const bold = "\x1B[1m";
    pub const underline = "\x1B[4m";
    pub const clear_style = "\x1B[0m";
    pub const clear_prompt = "\x1b[2J\x1b[H";

    pub const red = "\x1b[31m";
    pub const green = "\x1b[32m";
};

pub const Cli = struct {
    exercise_list: std.ArrayList([]const u8),
    allocator: std.mem.Allocator,

    current_exercise: []const u8 = "",
    current_exercise_index: u8 = 0,
    did_current_exercise_compile: bool = false,
    completed_exercises: std.ArrayList([]const u8) = .empty,
    current_exercise_stdout: std.ArrayList(u8) = .empty,
    current_exercise_stderr: std.ArrayList(u8) = .empty,

    pub const Components = struct {
        const ASCII_ART =
            \\                   _ _
            \\                  | (_)
            \\   ___ _ __  _ __ | |_ _ __   __ _ ___
            \\  / __| '_ \| '_ \| | | '_ \ / _` / __|
            \\ | (__| |_) | |_) | | | | | | (_| \__ \
            \\  \___| .__/| .__/|_|_|_| |_|\__, |___/
            \\      | |   | |               __/ |
            \\      |_|   |_|              |___/
            \\
        ;

        const ProgressBar = struct {
            var completed_exercises_status: std.ArrayList(u8) = .empty;
            var incomplete_exercises_status: std.ArrayList(u8) = .empty;

            fn output(cli: *Cli) !void {
                for (cli.completed_exercises.items) |exercise| {
                    _ = exercise;
                    try completed_exercises_status.append(cli.allocator, '#');
                }

                for (cli.exercise_list.items[cli.completed_exercises.items.len..]) |exercise| {
                    _ = exercise;
                    try incomplete_exercises_status.append(cli.allocator, '-');
                }

                try stdout.print("\nProgress: [{s}>{s}] {d}/{d}\n", .{ completed_exercises_status.items, incomplete_exercises_status.items, cli.current_exercise_index + 1, cli.exercise_list.items.len });

                incomplete_exercises_status.clearAndFree(cli.allocator);
                completed_exercises_status.clearAndFree(cli.allocator);
            }
        };
    };

    pub fn iterateExercises(self: *Cli) !void {
        for (self.exercise_list.items, 0..) |exercise, index| {
            self.current_exercise = exercise;
            self.current_exercise_index = @intCast(index);
            self.did_current_exercise_compile = false;

            try self.compileCurrentExercise();

            if (!(self.did_current_exercise_compile)) {
                break;
            }

            try self.completed_exercises.append(self.allocator, exercise);
        }
    }

    pub fn iterateNextExercise(self: *Cli) !void {
        try self.compileCurrentExercise();

        if (!(self.did_current_exercise_compile)) {
            return;
        }

        try self.completed_exercises.append(self.allocator, self.current_exercise);

        const next_exercise_index = self.current_exercise_index + 1;

        self.current_exercise = self.exercise_list.items[next_exercise_index];
        self.current_exercise_index = @intCast(next_exercise_index);

        try self.compileCurrentExercise();
    }

    pub fn compileCurrentExercise(self: *Cli) !void {
        self.current_exercise_stdout.clearAndFree(self.allocator);
        self.current_exercise_stderr.clearAndFree(self.allocator);

        var process = std.process.Child.init(
            &[_][]const u8{ "zig", "build", "exercises", "--", self.current_exercise },
            self.allocator,
        );

        process.stderr_behavior = .Pipe;
        process.stdout_behavior = .Pipe;

        process.spawn() catch
            {
                try stdout.print("\n{s}Error compiling exercise...{s}\n", .{
                    ASCII_STYLES.red,
                    ASCII_STYLES.clear_style,
                });
                return;
            };

        try process.collectOutput(self.allocator, &self.current_exercise_stdout, &self.current_exercise_stderr, 4096);
        const process_status = try process.wait();

        if (process_status.Exited == 0) {
            self.did_current_exercise_compile = true;
            return;
        }

        self.did_current_exercise_compile = false;
    }

    pub fn run(self: *Cli) !void {
        try self.iterateExercises();

        while (true) {
            try stdout.print("{s}", .{ASCII_STYLES.clear_prompt});

            try stdout.print("{s}\n", .{Components.ASCII_ART});

            try stdout.print("{s}", .{self.current_exercise_stdout.items});

            if (!(self.did_current_exercise_compile)) {
                try stdout.print("\n{s}", .{self.current_exercise_stderr.items});
            }

            try stdout.print("{s}", .{ASCII_STYLES.bold});
            if (self.did_current_exercise_compile) {
                try stdout.print("\nExercise completed , move on to the next...\n", .{});
            } else {
                try stdout.print("\nExercise failed to compile . Keep trying, we believe in you.\n", .{});
            }
            try stdout.print("{s}", .{ASCII_STYLES.clear_style});

            try Components.ProgressBar.output(self);

            try stdout.print("Current exercise: {s}{s}{s}{s}\n", .{ ASCII_STYLES.bold, ASCII_STYLES.underline, self.current_exercise, ASCII_STYLES.clear_style });

            try stdout.print("\n", .{});
            try stdout.print("{s}n{s}: next / ", .{ ASCII_STYLES.bold, ASCII_STYLES.clear_style });
            try stdout.print("{s}c{s}: check all / ", .{ ASCII_STYLES.bold, ASCII_STYLES.clear_style });
            try stdout.print("{s}r{s}: refresh / ", .{ ASCII_STYLES.bold, ASCII_STYLES.clear_style });
            try stdout.print("{s}x{s}: reset / ", .{ ASCII_STYLES.bold, ASCII_STYLES.clear_style });
            try stdout.print("{s}q{s}: quit ", .{ ASCII_STYLES.bold, ASCII_STYLES.clear_style });
            try stdout.print("-> ", .{});
            try stdout.flush();

            const input = try stdin.takeDelimiterExclusive('\n');

            if (std.mem.eql(u8, input, "n")) {
                try self.iterateNextExercise();
            }

            if (std.mem.eql(u8, input, "c")) {
                try self.iterateExercises();
            }

            if (std.mem.eql(u8, input, "r")) {
                try self.compileCurrentExercise();
            }

            // TODO: reset using diff and patches
            if (std.mem.eql(u8, input, "x")) {}

            if (std.mem.eql(u8, input, "q")) {
                break;
            }
        }
    }
};

pub fn main() !void {
    var mem_arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer mem_arena.deinit();
    const mem_allocator = mem_arena.allocator();

    const exercise_list: *ExerciseList = try ExerciseList.init(mem_allocator);
    defer exercise_list.deinit();

    var cpplings: Cli = .{ .allocator = mem_allocator, .exercise_list = exercise_list.filenames };

    try cpplings.run();
}
