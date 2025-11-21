pub const ASCII_STYLES = struct {
    pub const bold = "\x1B[1m";
    pub const underline = "\x1B[4m";
    pub const clear_style = "\x1B[0m";
    pub const clear_prompt = "\x1b[2J\x1b[H";

    pub const red = "\x1b[31m";
    pub const green = "\x1b[32m";
};
