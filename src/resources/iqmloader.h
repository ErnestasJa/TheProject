#pragma once
class mesh;
struct iqmheader;
class logger;
class iqmloader
{
private:
    void load_header(const char* data, iqmheader & header);
    void loadiqmanims(std::shared_ptr<mesh> m, const char* data, iqmheader & header);
    logger *m_logger;
public:
    iqmloader(logger *logger);
	std::shared_ptr<mesh> load(const char* data);

private:


};
