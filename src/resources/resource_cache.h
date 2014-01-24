#pragma once

template <class T>
struct resource
{
    std::string path;
    std::shared_ptr<T> resource;
};

template <class T>
class resource_cache
{
public:

    void add_resource(resource<T> res)
    {
        m_resources.push_back(res);
    }

    resource<T> get_resource(const std::string & path)
    {
        auto it = std::find_if(m_resources.begin(), m_resources.end(), [&path](resource<T> res){return res.path==path;});

        if(it!=m_resources.end())
        {
            return (*it);
        }

        return resource<T>();
    }

protected:
    std::vector< resource<T> >  m_resources;
};

template <class T>
std::vector<std::shared_ptr<T> > * create_resource_cache()
{
    return new std::vector<std::shared_ptr<T> >();
}

template <class T>
inline std::shared_ptr<T> share(T* t)
{
    return std::shared_ptr<T>(t);
}
