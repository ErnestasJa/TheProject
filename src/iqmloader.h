#pragma once
class iqmesh;
struct iqmheader;
class iqmloader
{
private:
    void load_header(const unsigned char* data, iqmheader * & header);
public:
	iqmesh *load(const unsigned char* data);

protected:
};
