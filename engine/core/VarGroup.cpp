#include "Precomp.h"
#include "Var.h"
#include "VarGroup.h"



VarGroup VarGroup::Null;

VarGroup::VarGroup()
{
	m_name = nullptr;
}

VarGroup::VarGroup(const char * groupName)
{
	allocAndCopyStr(m_name, groupName);
}

VarGroup::VarGroup(const VarGroup & o)
{
	allocAndCopyStr(m_name, o.Name());

	for(const Var & v : o.Vars())
		m_vars.push_back(v);

	for(const VarGroup & g : o.Groups())
		m_groups.push_back(g);
}


VarGroup & VarGroup::operator = (const VarGroup & other)
{
	(*this) = VarGroup(other);
	std::cout << "Group Operator = " << std::endl;
	return *this;
}

VarGroup::~VarGroup()
{
	delete m_name;
}

const char * VarGroup::Name() const
{
	return m_name;
}

vector<Var> & VarGroup::Vars()
{
	return m_vars;
}

const vector<Var> & VarGroup::Vars() const
{
	return m_vars;
}

vector<VarGroup> & VarGroup::Groups()
{
	return m_groups;
}

const vector<VarGroup> & VarGroup::Groups() const
{
	return m_groups;
}

Var & VarGroup::GetVar(const char * name)
{
	auto it = boost::range::find_if( m_vars,
		[&name](const Var & var)
		{
			return strcmp(var.Name(), name)==0;
		}
	);

	if(it!=m_vars.end())
		return (*it);
	else
		return Var::Null;
}

VarGroup & VarGroup::AddGroup(const char * name)
{
	vector<VarGroup>::iterator it = std::find_if(m_groups.begin(), m_groups.end(),
		[&name](const VarGroup & varGroup)
		{
			return strcmp(varGroup.Name(), name)==0;
		}
	);

	if(it!=m_groups.end())
		return (*it);
	else
	{
		m_groups.push_back(VarGroup(name));
		return m_groups[m_groups.size()-1]; // replace this
	}
}

VarGroup & VarGroup::AddGroup(const VarGroup & vGroup)
{
	if(vGroup.Name()==nullptr)
		throw std::string("Group name cannot be null");

	auto it = boost::range::find_if(m_groups,
		[&vGroup](const VarGroup & varGroup)
		{
			return strcmp(varGroup.Name(), vGroup.Name())==0;
		}
	);

	if(it!=m_groups.end())
		return (*it);
	else
	{
		m_groups.push_back(vGroup);
		return m_groups[m_groups.size()-1]; // replace this
	}
}

bool VarGroup::AddVar(const Var & variable)
{
	vector<Var>::iterator it = std::find_if(m_vars.begin(), m_vars.end(),
		[&variable](const Var & var)
		{
		 	return strcmp(var.Name(), variable.Name())==0;
		}
	);

	if(it==m_vars.end())
	{
		m_vars.push_back(variable);
		return true;
	}
	else
	{
		return false;
	}
}

VarGroup & VarGroup::GetGroup(const char * name)
{
	vector<VarGroup>::iterator it = std::find_if(m_groups.begin(), m_groups.end(),
		[&name](const VarGroup & varGroup)
		{
			return strcmp(varGroup.Name(), name)==0;
		}
	);

	if(it!=m_groups.end())
		return (*it);
	else
		return Null; // replace this
}
