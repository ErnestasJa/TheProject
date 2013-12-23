#pragma once
class iqmesh;
struct iqmheader;
class logger;
class iqmloader
{
private:
    void load_header(const char* data, iqmheader & header);
    logger *m_logger;
public:
    iqmloader(logger *logger);
	iqmesh *load(const char* data);
	bool loadiqmanims(iqmesh * mesh);

protected:
};
