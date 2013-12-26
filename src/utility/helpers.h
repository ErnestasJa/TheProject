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

//neiter sure if this works
inline void screenshot (char filename[160],int x, int y)
{
    // get the image data
    long imageSize = x * y * 3;
    unsigned char *data = new unsigned char[imageSize];
    glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
    int xa= x % 256;
    int xb= (x-xa)/256;
    int ya= y % 256;
    int yb= (y-ya)/256;//assemble the header
    unsigned char header[18]= {0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0};
// write header and data to file
    std::fstream File(filename, std::ios::out | std::ios::binary);
    File.write (reinterpret_cast<char *>(header), sizeof (char)*18);
    File.write (reinterpret_cast<char *>(data), sizeof (char)*imageSize);
    File.close();

    delete[] data;
    data=NULL;
}

//not really working
inline void texturetofile (GLuint tex,char filename[160],int x=1024, int y=1024)
{
    glBindTexture(GL_TEXTURE_2D,tex);
// get the image data
    long imageSize = x * y * 3;
    unsigned char *data = new unsigned char[imageSize];
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    int xa= x % 256;
    int xb= (x-xa)/256;
    int ya= y % 256;
    int yb= (y-ya)/256;//assemble the header
    unsigned char header[18]= {0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0};
// write header and data to file
    std::fstream File(filename, std::ios::out | std::ios::binary);
    File.write (reinterpret_cast<char *>(header), sizeof (char)*18);
    File.write (reinterpret_cast<char *>(data), sizeof (char)*imageSize);
    File.close();

    delete[] data;
    data=NULL;
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

#define _DEBUG
#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
