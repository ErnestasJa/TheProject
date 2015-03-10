#include "Precomp.h"
#include "VarJsonReader.h"
#include "json/json.h"
#include "json/reader.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "utility/Logger.h"

VarJsonReader::VarJsonReader(Logger * l)
{
    m_logger = l;
}

VarJsonReader::~VarJsonReader()
{

}

void LoadGroup(VarGroup & g, Json::Value & jg, Logger * logger)
{
    Json::Value jgroup = jg[g.Name()];

    if(jgroup.isNull() || !jgroup.isObject())
        return;

    for(Var & var : g.Vars())
    {
        Json::Value jvar = jgroup[var.Name()];

        if(!jvar.isNull())
        switch(var.Type())
        {
        case VARI:
            var.Value(jvar.asInt());
            break;
        case VARF:
            var.Value(jvar.asFloat());
            break;
        case VARS:
            var.Value(jvar.asString().c_str());
            break;
        default:
            break;
        }
    }

    for(VarGroup & group : g.Groups())
        LoadGroup(group, jgroup, logger);
}

bool VarJsonReader::Load(void * buffer, uint32_t size, VarGroup & group)
{
    Json::Reader reader;
    const char * doc = (const char *) buffer;

    Json::Value root;
    reader.parse(doc,doc+size, root);

    std::cout << "Group:" << group.Name() << std::endl;
    std::cout << "Group2:" << group.Groups()[0].Name() << std::endl;
    std::cout << "Loaded json:" << root << std::endl;

    LoadGroup(group, root, m_logger);
    return true;
}
