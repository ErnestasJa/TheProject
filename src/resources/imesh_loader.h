#ifndef IMESH_LOADER_H
#define IMESH_LOADER_H

struct mesh;
typedef std::shared_ptr<mesh> mesh_ptr;
class imesh_loader
{
    public:
        virtual ~imesh_loader();

        virtual mesh_ptr load(const char* data, const uint32_t size) = 0;
        virtual bool check_by_extension(const std::string & ext) = 0;
    protected:
    private:
};

#endif // IMESH_LOADER_H
