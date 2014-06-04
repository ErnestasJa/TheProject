#pragma once

struct game_data
{
    std::wstring user;
    int quad_choice;
    int current_level;
    int time;
    bool success;
    bool final_level;

    std::vector<bool> training_completion;
    std::vector<int> training_requirements;

    game_data()
    {
        user=L"Default";
        quad_choice=0;
        current_level=0;
        time=0;
        success=false;
        final_level=false;
        training_completion.resize(5);
        training_requirements.resize(5);
        for(bool b:training_completion)
            b=false;

        training_requirements[0]=15000;
        training_requirements[1]=30000;
        training_requirements[2]=60000;
        training_requirements[3]=60000;
        training_requirements[4]=60000;
    }

    bool set_level(int level)
    {
        if(level>0&&level<training_completion.size()-1)
        {
            current_level=level;
            time=0;
            return true;
        }
        return false;
    }

    bool advance_level()
    {
        if(time<=training_requirements[current_level])
        {
            training_completion[current_level]=true;
            return true;
        }

        return false;
    }
};
