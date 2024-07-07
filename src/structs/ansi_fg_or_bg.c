struct ansi_fg_or_bg
{
    int dummy;
};

struct ansi_fg_or_bg real_ANSI_FG = {0};
struct ansi_fg_or_bg *ANSI_FG = &real_ANSI_FG;
struct ansi_fg_or_bg real_ANSI_BG = {1};
struct ansi_fg_or_bg *ANSI_BG = &real_ANSI_BG;
