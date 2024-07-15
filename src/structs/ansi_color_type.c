struct ansi_color_type
{
    int dummy;
};

struct ansi_color_type real_COLOR_TYPE_16 = {0};
struct ansi_color_type *COLOR_TYPE_16 = &real_COLOR_TYPE_16;
struct ansi_color_type real_COLOR_TYPE_256 = {1};
struct ansi_color_type *COLOR_TYPE_256 = &real_COLOR_TYPE_256;
struct ansi_color_type real_COLOR_TYPE_24BIT = {3};
struct ansi_color_type *COLOR_TYPE_24BIT = &real_COLOR_TYPE_24BIT;

struct ansi_color_type real_COLOR_TYPE_DEFAULT_FG = {3};
struct ansi_color_type *COLOR_TYPE_DEFAULT_FG = &real_COLOR_TYPE_DEFAULT_FG;
struct ansi_color_type real_COLOR_TYPE_DEFAULT_BG = {3};
struct ansi_color_type *COLOR_TYPE_DEFAULT_BG = &real_COLOR_TYPE_DEFAULT_BG;
