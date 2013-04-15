#pragma once
class iqmesh;
struct iqmheader;
class iqmloader
{
private:
    void load_header(const char* data, iqmheader & header);
public:
	iqmesh *load(const char* data);

protected:
};
