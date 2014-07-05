#pragma once

#include "IMeshLoader.h"

class mesh;
struct iqmheader;
class Logger;

class iqmloader: public imesh_loader
{
private:
    void load_header(const char* data, iqmheader & header);
    void loadiqmanims(std::shared_ptr<mesh> m, const char* data, iqmheader & header);
    Logger *_logger;
public:
    iqmloader(Logger *Logger);

	virtual std::shared_ptr<mesh> load(const char* data, const uint32_t size);
    virtual bool check_by_extension(const std::string & ext);

private:


};
