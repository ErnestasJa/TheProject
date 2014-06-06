#pragma once

struct quadcopter_info
{
    std::wstring info;
    std::wstring difficulty;
    std::wstring speed;
    std::wstring mass;
};

struct level_info
{
    std::wstring info;
    std::wstring recommended_quad;
};

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
    std::vector<level_info> training_descriptions;
    std::vector<quadcopter_info> quadcopter_descriptions;

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

        level_info li;

        li.info=L"Paprastas skrydis į priekį.";
        li.recommended_quad=L"Bet koks.";
        training_descriptions.push_back(li);

        li.info=L"Slalomo treniruotė.";
        li.recommended_quad=L"Nerekomenduojamas greitasis.";
        training_descriptions.push_back(li);

        li.info=L"Skrydis siauroje erdvėje.";
        li.recommended_quad=L"Mikro arba standartinis.";
        training_descriptions.push_back(li);

        li.info=L"Skrydis pro tarpus.";
        li.recommended_quad=L"Mikro.";
        training_descriptions.push_back(li);

        li.info=L"Greičio išbandymas.";
        li.recommended_quad=L"Greitasis.";
        training_descriptions.push_back(li);

        quadcopter_info i;

        i.info=L"Standartinis orlaivis.";
        i.difficulty=L"Lengviausia skristi iš visų.";
        i.mass=L"5KG";
        i.speed=L"2m/s";
        quadcopter_descriptions.push_back(i);

        i.info=L"Standartinis orlaivis be apsaugos.";
        i.difficulty=L"Lengviau pažeidžiamas ir greitesnis.";
        i.mass=L"3KG";
        i.speed=L"4m/s";
        quadcopter_descriptions.push_back(i);

        i.info=L"Mini orlaivio modelis, didesniam paslankumui.";
        i.difficulty=L"Nė kiek sunkiau už standartinį.";
        i.mass=L"2KG";
        i.speed=L"2m/s";
        quadcopter_descriptions.push_back(i);

        i.info=L"Greitas neapsaugoto orlaivio modelis.";
        i.difficulty=L"Sudėtingiausia skristi.";
        i.mass=L"4KG";
        i.speed=L"8m/s";
        quadcopter_descriptions.push_back(i);
    }

    bool set_level(int level)
    {
        if(level>=0&&level<=2)
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
