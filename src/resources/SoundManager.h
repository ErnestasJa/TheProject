#pragma once

#include "irrklang.h"

using namespace irrklang;

struct sound_resource
{
    std::string name;
    float volume,pitch;
    ISoundSource* sound;
    ISound* player;
    bool playing;
};

class sound_manager
{
private:
    ISoundEngine* eng;
    vector<sound_resource*> sounds;
public:
    sound_manager()
    {
        eng=irrklang::createIrrKlangDevice();
    }
    ~sound_manager()
    {
        eng->removeAllSoundSources();
        eng->stopAllSounds();
        eng->drop();
    }
    sound_resource *add_sound(std::string filename,std::string name,float volume,float pitch)
    {
        if(get_sound(name)==nullptr)
        {
        char* buf;
        int len=helpers::read(filename,buf);
        ISoundSource* src=eng->addSoundSourceFromMemory(buf,len,name.c_str(),true);
        delete buf;
        src->setDefaultVolume(volume);

        sound_resource* snd=new sound_resource();
        snd->name=name;
        snd->volume=volume;
        snd->pitch=pitch;
        snd->sound=src;
        snd->playing=false;
        snd->player=nullptr;

        sounds.push_back(snd);
        return snd;
        }
        else return get_sound(name);
    }

    sound_resource *get_sound(std::string name)
    {
        for(sound_resource* sr:sounds)
        {
            if(sr->name==name)
                return sr;
        }
        return nullptr;
    }
    void remove_sound(std::string name)
    {
        for(vector<sound_resource*>::iterator it=sounds.begin(); it!=sounds.end(); it++)
        {
            if((*it)->name==name)
            {
                it=sounds.erase(it);
                return;
            }
        }
    }

    void play_sound_2d(std::string name,bool looped)
    {
        sound_resource* r=get_sound(name);
        if(r!=nullptr)
        {
            if(r->playing)
                r->player->stop();
            r->player=eng->play2D(r->sound,looped,false,true);
            r->playing=true;
        }
        else
            printf("Playing or null.\n");
    }

    void set_volume(float vol)
    {
        eng->setSoundVolume(vol);
    }

    void set_sound_volume(std::string name,float vol)
    {
        sound_resource* r=get_sound(name);
        if(r!=nullptr)
        {
            r->sound->setDefaultVolume(vol);
        }
    }

    void stop_sound(std::string name)
    {
        sound_resource* r=get_sound(name);
        if(r!=nullptr)
        {
            if(r->playing&&r->player!=nullptr)
            {
                r->player->stop();
                r->playing=false;
                return;
            }
        }
    }

    void stop_all_sounds()
    {
        eng->stopAllSounds();
    }

protected:
};
