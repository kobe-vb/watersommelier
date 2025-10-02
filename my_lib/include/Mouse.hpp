#pragma once


class Mouse
{
    private:
        static int cursor;
    public:
        static void update_cursor(int c) { cursor = c; }
        static void set_cursor();
};