#pragma once
class iqmesh;
struct iqmheader;
class iqmloader
{
public:
	iqmesh *load(const unsigned char* data,const iqmheader &head);
private:
protected:
};
