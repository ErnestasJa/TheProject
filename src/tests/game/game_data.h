#pragma once

struct game_data
{
    std::wstring user;
    int quad_choice;
    int level_choice;
    int time;
    bool success;

    game_data()
    {
        user=L"Default";
        quad_choice=0;
        level_choice=0;
        time=0;
        success=false;
    }
};
