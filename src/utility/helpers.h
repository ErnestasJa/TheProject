#pragma once

namespace helpers
{

template <typename T> inline std::string to_str(const T& t)
{
    std::ostringstream os;    //magic..
    os<<t;
    return os.str();
}

inline uint32_t read(const std::string & file, char *& buf)
{
    PHYSFS_file* f = PHYSFS_openRead(file.c_str());

    if(!f)
    {
        printf("PHYSFS: Opening (%s) to read failed.\n",file.c_str());
        return 0;
    }

    uint32_t len = PHYSFS_fileLength(f), ret = 0;

    buf = new char[len+1];
    buf[len] = 0;
    ret = PHYSFS_read(f, buf, 1, len) * len;
    PHYSFS_close(f);

    return ret;
}

inline const glm::vec4 color255(uint8_t r=255,uint8_t g=255,uint8_t b=255,uint8_t a=255)
{
    return glm::vec4((float)r/255,(float)g/255,(float)b/255,(float)a/255);
}

}



///TO IMPLEMENT!
inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
