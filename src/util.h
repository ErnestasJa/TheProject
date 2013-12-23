#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

inline bool read(const std::string & file, char *& buf)
{
    PHYSFS_file* f = PHYSFS_openRead(file.c_str());

    if(!f)
        return false;

    buf = new char[PHYSFS_fileLength(f)+1];
    buf[PHYSFS_fileLength(f)]=0;
    PHYSFS_read(f, buf, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);

    return true;
}

#endif // UTIL_H_INCLUDED
