#ifndef IMESH_LOADER_H
#define IMESH_LOADER_H

struct Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
class imesh_loader
{
    public:
        virtual ~imesh_loader();

        virtual MeshPtr load(const char* data, const uint32_t size) = 0;
        virtual bool check_by_extension(const std::string & ext) = 0;
    protected:
    private:
};

#endif // IMESH_LOADER_H
