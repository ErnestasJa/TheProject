#pragma once

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
