#pragma once

class Logger;
class OpenGLUtil
{
protected:
    Logger * _logger;
public:
    OpenGLUtil(Logger * l);
    virtual ~OpenGLUtil();

    bool load_extensions();
    bool check_and_output_errors();
    std::string gl_error_to_string(uint32_t error);
    std::string gl_fbo_error_to_string(uint32_t error);
};
