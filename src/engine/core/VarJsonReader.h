#ifndef VARJSONREADER_H_INCLUDED
#define VARJSONREADER_H_INCLUDED

class VarGroup;
class Logger;
class VarJsonReader
{
public:
    VarJsonReader(Logger * l);
    ~VarJsonReader();

    bool Load(void * buffer, uint32_t size, VarGroup & group);

private:
    Logger * m_logger;
};

#endif // VARJSONREADER_H_INCLUDED
