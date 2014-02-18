#pragma once

#include "imesh_loader.h"

class mesh;
struct iqmheader;
class logger;

class iqmloader: public imesh_loader
{
private:
    void load_header(const char* data, iqmheader & header);
    void loadiqmanims(std::shared_ptr<mesh> m, const char* data, iqmheader & header);
    logger *m_logger;
public:
    iqmloader(logger *logger);

	virtual std::shared_ptr<mesh> load(const char* data, const uint32_t size);
    virtual bool check_by_extension(const std::string & ext);

private:


};
