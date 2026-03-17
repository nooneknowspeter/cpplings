const STD = @import("std");

const WATCH_POLLING_RATE_MS: u64 = 500;

pub const FileWatcher = struct {
    const Self = @This();

    current_file_path: []const u8 = "",
    previous_mod_time: i128 = 0,

    pub fn init(file_path: []const u8) Self {
        return Self{
            .current_file_path = file_path,
        };
    }

    pub fn checkForChanges(self: *Self) !bool {
        const metadata = try STD.fs.cwd().statFile(self.current_file_path);
        const current_mod_time = metadata.mtime;

        if (current_mod_time != self.previous_mod_time) {
            self.previous_mod_time = current_mod_time;
            return true;
        }

        return false;
    }
};
