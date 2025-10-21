const STYLES = @import("styles.zig");
const STD = @import("std");

var stdin_buffer: [1024]u8 = undefined;
var stdin_reader = STD.fs.File.stdin().reader(&stdin_buffer);
const STDIN = &stdin_reader.interface;

pub const ASCII_ART =
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

const CLI = struct {
    allocator: STD.mem.Allocator,
    list_of_exercises: STD.ArrayList([]const u8) = .empty,
    current_exercise: []const u8 = "",
    current_exercise_index: u8 = 0,
    did_current_exercise_compile: bool = false,
    completed_exercises: STD.ArrayList([]const u8) = .empty,
    current_exercise_stdout: STD.ArrayList(u8) = .empty,
    current_exercise_stderr: STD.ArrayList(u8) = .empty,
    current_exercise_prev_mod_time: i128 = 0,
};

fn iterateExerciseDirectory(self: *CLI) !void {
    const EXERCISES_DIR_PATH = "exercises/";

    const EXERCISES_DIR = try STD.fs.cwd().openDir(EXERCISES_DIR_PATH, .{ .iterate = true });
    var chapters = EXERCISES_DIR.iterate();

    while (try chapters.next()) |chapter| {
        const CHAPTER_DIR_PATH = try STD.fs.path.join(self.allocator, &.{ EXERCISES_DIR_PATH, chapter.name });

        const CHAPTER_DIR = try STD.fs.cwd().openDir(CHAPTER_DIR_PATH, .{ .iterate = true });
        var exercises = CHAPTER_DIR.iterate();

        while (try exercises.next()) |exercise| {
            const EXERCISE_FILE_PATH = try STD.fs.path.join(self.allocator, &.{ EXERCISES_DIR_PATH, chapter.name, exercise.name });

            try self.list_of_exercises.append(self.allocator, EXERCISE_FILE_PATH);
        }
    }

    STD.sort.insertion(
        []const u8,
        self.list_of_exercises.items,
        {},
        struct {
            fn lessThan(_: void, a: []const u8, b: []const u8) bool {
                return STD.mem.lessThan(u8, a, b);
            }
        }.lessThan,
    );
}

fn iterateExercises(self: *CLI) !void {
    self.completed_exercises.clearAndFree(self.allocator);

    for (self.list_of_exercises.items, 0..) |exercise, index| {
        self.current_exercise = exercise;
        self.current_exercise_index = @intCast(index);
        self.did_current_exercise_compile = false;

        try compileCurrentExercise(self);

        if (!(self.did_current_exercise_compile)) {
            break;
        }

        try self.completed_exercises.append(self.allocator, exercise);
    }
}

fn iteratePreviousExercise(self: *CLI) !void {
    if (self.current_exercise_index <= 0) {
        return;
    }

    _ = self.completed_exercises.pop();

    const PREVIOUS_EXERCISE_INDEX = self.current_exercise_index - 1;

    self.current_exercise = self.list_of_exercises.items[PREVIOUS_EXERCISE_INDEX];
    self.current_exercise_index = @intCast(PREVIOUS_EXERCISE_INDEX);

    try compileCurrentExercise(self);
}

fn iterateNextExercise(self: *CLI) !void {
    try compileCurrentExercise(self);

    if (!(self.did_current_exercise_compile)) {
        return;
    }

    try self.completed_exercises.append(self.allocator, self.current_exercise);

    const NEXT_EXERCISE_INDEX = self.current_exercise_index + 1;

    self.current_exercise = self.list_of_exercises.items[NEXT_EXERCISE_INDEX];
    self.current_exercise_index = @intCast(NEXT_EXERCISE_INDEX);

    try compileCurrentExercise(self);
}

fn watchFileChanges(self: *CLI, polling_rate_ms: u64) !void {
    while (true) {
        const CURRENT_EXERCISE_METADATA = try STD.fs.cwd().statFile(self.current_exercise);
        const CURRENT_EXERCISE_MODIFIED_TIME = CURRENT_EXERCISE_METADATA.mtime;

        if (CURRENT_EXERCISE_MODIFIED_TIME != self.current_exercise_prev_mod_time) {
            self.current_exercise_prev_mod_time = CURRENT_EXERCISE_MODIFIED_TIME;
            try compileCurrentExercise(self);
            try clear(self);
            try draw(self);
        }

        STD.Thread.sleep(@intCast(polling_rate_ms * 1_000_000));
    }
}

fn compileCurrentExercise(self: *CLI) !void {
    self.current_exercise_stdout.clearAndFree(self.allocator);
    self.current_exercise_stderr.clearAndFree(self.allocator);

    try self.current_exercise_stdout.appendSlice(self.allocator, "Wait, compiling...");

    try clear(self);
    try draw(self);

    var process = STD.process.Child.init(
        &[_][]const u8{ "zig", "build", "exercises", "--", self.current_exercise },
        self.allocator,
    );

    process.stderr_behavior = .Pipe;
    process.stdout_behavior = .Pipe;

    process.spawn() catch
        {
            STD.debug.print("\n{s}Error compiling exercise...{s}\n", .{
                STYLES.ASCII_STYLES.red,
                STYLES.ASCII_STYLES.clear_style,
            });
            return;
        };

    self.current_exercise_stdout.clearAndFree(self.allocator);

    // FIX: dynamically allocate max output bytes for stdout
    try process.collectOutput(self.allocator, &self.current_exercise_stdout, &self.current_exercise_stderr, 65_536); // 16 bit output
    const PROCESS_STATUS = try process.wait();

    if (PROCESS_STATUS.Exited == 0) {
        self.did_current_exercise_compile = true;
        return;
    }

    self.did_current_exercise_compile = false;
}

fn progressBar(self: *CLI) !void {
    var ascii_bar: STD.ArrayList(u8) = .empty;

    for (self.completed_exercises.items) |exercise| {
        _ = exercise;

        try ascii_bar.append(self.allocator, '#');
    }

    try ascii_bar.append(self.allocator, '>');

    for (self.list_of_exercises.items[self.completed_exercises.items.len..]) |exercise| {
        _ = exercise;

        try ascii_bar.append(self.allocator, '-');
    }

    STD.debug.print("\nProgress: [{s}] {d}/{d}\n", .{ ascii_bar.items, self.current_exercise_index + 1, self.list_of_exercises.items.len });

    ascii_bar.clearAndFree(self.allocator);
}

fn draw(self: *CLI) !void {
    STD.debug.print("{s}\n", .{ASCII_ART});

    STD.debug.print("{s}\n", .{self.current_exercise_stdout.items});
    if (!(self.did_current_exercise_compile)) {
        STD.debug.print("\n{s}", .{self.current_exercise_stderr.items});
    }

    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.bold});
    if (self.did_current_exercise_compile) {
        STD.debug.print("\nExercise completed , move on to the next...\n", .{});
    } else {
        STD.debug.print("\nExercise failed to compile . Keep trying, we believe in you.\n", .{});
    }
    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_style});

    try progressBar(self);

    STD.debug.print("Current exercise: {s}{s}{s}{s}\n", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.underline, self.current_exercise, STYLES.ASCII_STYLES.clear_style });

    STD.debug.print("\n", .{});
    STD.debug.print("{s}n{s}: next / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}p{s}: previous / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}c{s}: check all / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}r{s}: refresh / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}x{s}: reset / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}q{s}: quit ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("-> ", .{});
}

fn userInput(self: *CLI) !void {
    while (true) {
        const INPUT = try STDIN.takeDelimiterExclusive('\n');

        if (STD.mem.eql(u8, INPUT, "n")) {
            try iterateNextExercise(self);

            try clear(self);
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "p")) {
            try iteratePreviousExercise(self);

            try clear(self);
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "c")) {
            try iterateExercises(self);

            try clear(self);
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "r")) {
            try compileCurrentExercise(self);

            try clear(self);
            try draw(self);
        }

        // TODO: reset using diff and patches
        if (STD.mem.eql(u8, INPUT, "x")) {
            try clear(self);
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "q")) {
            try clear(self);
            break;
        }
    }
}

fn clear(self: *CLI) !void {
    _ = self;
    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_prompt});
}

pub fn run(allocator: STD.mem.Allocator) !void {
    const self = try allocator.create(CLI);
    defer allocator.destroy(self);
    self.* = .{ .allocator = allocator };

    try iterateExerciseDirectory(self);

    try clear(self);
    try draw(self);

    try iterateExercises(self);

    const watch_daemon_polling_rate: u64 = 500;
    var watch_thread: STD.Thread = try STD.Thread.spawn(.{}, watchFileChanges, .{ self, watch_daemon_polling_rate });
    watch_thread.detach();

    try userInput(self);
}
