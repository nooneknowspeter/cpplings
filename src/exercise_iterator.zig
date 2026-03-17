const STD = @import("std");

const UTIL = @import("util.zig");

pub const ExerciseIterator = struct {
    const Self = @This();

    allocator: STD.mem.Allocator,
    exercises_dir_path: []const u8,
    all_exercises: STD.ArrayList([]const u8) = .empty,
    completed_exercises: STD.ArrayList([]const u8) = .empty,
    current_index: usize = 0,

    pub fn init(allocator: STD.mem.Allocator, dir_path: []const u8) !*Self {
        const self = try allocator.create(Self);
        self.* = .{
            .allocator = allocator,
            .exercises_dir_path = dir_path,
        };

        try self.populateExercises();

        return self;
    }

    pub fn deinit(self: *Self) void {
        for (self.all_exercises.items) |path| {
            self.allocator.free(path);
        }
        self.all_exercises.deinit(self.allocator);

        for (self.completed_exercises.items) |path| {
            self.allocator.free(path);
        }
        self.completed_exercises.deinit(self.allocator);

        self.allocator.destroy(self);
    }

    fn populateExercises(self: *Self) !void {
        const dir_tree: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(
            self.allocator,
            self.exercises_dir_path,
        );
        defer dir_tree.deinit(self.allocator);

        var temp_list: STD.ArrayList([]const u8) = .empty;
        defer temp_list.deinit(self.allocator);

        try dir_tree.iterateAndFilterTree(self.allocator, .{
            .include_filter = ".cpp",
            .is_move_semantics_enabled = true,
            .external_list = &temp_list,
        });

        for (temp_list.items) |path| {
            const duplicated_path = try self.allocator.dupe(u8, path);
            try self.all_exercises.append(self.allocator, duplicated_path);
        }

        STD.sort.insertion(
            []const u8,
            self.all_exercises.items,
            {},
            struct {
                fn lessThan(_: void, a: []const u8, b: []const u8) bool {
                    return STD.mem.lessThan(u8, a, b);
                }
            }.lessThan,
        );
    }

    pub fn current(self: *Self) ?[]const u8 {
        if (self.current_index >= self.all_exercises.items.len) {
            return null;
        }
        return self.all_exercises.items[self.current_index];
    }

    pub fn next(self: *Self) !void {
        const current_exercise = self.current() orelse return;
        try self.completed_exercises.append(self.allocator, current_exercise);
        self.current_index += 1;
    }

    pub fn prev(self: *Self) void {
        if (self.current_index == 0) {
            return;
        }
        _ = self.completed_exercises.pop();
        self.current_index -= 1;
    }

    pub fn reset(self: *Self) void {
        self.current_index = 0;
        self.completed_exercises.clearAndFree(self.allocator);
    }

    pub fn total(self: Self) usize {
        return self.all_exercises.items.len;
    }

    pub fn progress(self: Self) usize {
        return self.current_index;
    }

    pub fn isCompleted(self: Self) bool {
        return self.current_index >= self.all_exercises.items.len;
    }
};
