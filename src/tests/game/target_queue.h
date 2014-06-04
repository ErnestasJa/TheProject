#pragma once

class target_queue
{
private:
    sg::quadcopter* m_player;
    sg::sg_mesh_object_ptr m_arrow;
    glm::vec3 m_spawn,m_finish,m_current_target;
    std::vector<glm::vec3> m_points;
    int m_queue;
    bool m_completed,m_collided;
public:
    target_queue(glm::vec3 spawn,glm::vec3 finish,std::vector<glm::vec3> points,sg::sg_mesh_object_ptr arrow,sg::quadcopter* player)
    {
        m_points=points;
        m_queue=0;
        m_spawn=spawn;
        m_finish=finish;
        m_arrow=arrow;
        m_player=player;
        m_completed=false;
        m_collided=false;
    }

    ~target_queue()
    {
        m_points.clear();
    }

    void reset()
    {
        m_queue=0;
        m_player->set_position(m_spawn);
        m_current_target=m_points[0];
        m_arrow->set_position(m_current_target);
        m_completed=false;
        m_collided=false;
    }

    bool collide(glm::vec3 a)
    {
        return glm::distance(a,m_current_target)<2;
    }

    void update()
    {
        if(!m_completed)
        {
            if(collide(m_player->get_position()))
            {
                if(m_current_target!=m_finish)
                    m_current_target=m_points[m_queue];
                else
                {
                    m_completed=true;
                    m_arrow->set_position(glm::vec3(9999,9999,9999));
                }
                if(m_queue>m_points.size()-1)
                {
                    m_current_target=m_finish;
                }
                else
                {
                    m_queue++;
                }
                m_arrow->set_position(m_current_target);

            }

        }
    }

    bool is_completed()
    {
        return m_completed;
    }
protected:
};
