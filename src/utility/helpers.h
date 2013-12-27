#pragma once

#define DGL(func)\
if(gl_util->check_and_output_errors())\
{\
    std::cout << "Error happened before " << #func << std::endl;\
}\
func \
if(gl_util->check_and_output_errors())\
{\
    std::cout << "Error happened after " << #func << std::endl; \
}

struct u8vec4{uint8_t v[4];};

inline uint32_t read(const std::string & file, char *& buf)
{
    PHYSFS_file* f = PHYSFS_openRead(file.c_str());

    if(!f)
        return 0;

    uint32_t len = PHYSFS_fileLength(f), ret = 0;

    buf = new char[len+1];
    buf[len] = 0;
    ret = PHYSFS_read(f, buf, 1, len) * len;
    PHYSFS_close(f);

    return ret;
}
