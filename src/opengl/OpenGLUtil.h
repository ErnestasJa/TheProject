#pragma once

class logger;
class opengl_util
{
protected:
    logger * m_logger;
public:
    opengl_util(logger * l);
    virtual ~opengl_util();

    bool load_extensions();
    bool check_and_output_errors();
    std::string gl_error_to_string(uint32_t error);
    std::string gl_fbo_error_to_string(uint32_t error);
};
