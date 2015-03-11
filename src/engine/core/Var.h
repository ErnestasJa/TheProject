#ifndef VAR_H
#define VAR_H

enum VarType
{
	VARNULL = 0,
	VARI = 1,
	VARF,
	VARS
};

uint32_t allocAndCopyStr(char *& dest, const char * orig);

class Var
{
public:
    static Var Null;

public:
	Var();
	Var(const Var & o);
	Var & operator = (const Var & other);

	///rename value to name
	explicit Var(const char * name, float data);
	explicit Var(const char * name, float * data, uint8_t count);
	explicit Var(const char * name, int data);
	explicit Var(const char * name, int * data, uint8_t count);
	explicit Var(const char * name, const char * data);
	~Var();
	VarType Type() const;
	uint8_t Count() const;
	const char * Name() const;
	int ValueI() const;
	float ValueF() const;
	int* ValueIV() const;
	float* ValueFV() const;
	const char * ValueS() const;

	void Value(int value);
	void Value(float value);
	void Value(int* values, uint8_t count);
	void Value(float* values, uint8_t count);
	void Value(const char* value);

private:
	char * m_name;
	uint8_t m_count;
	uint8_t m_type;

	union {
		char *    m_datas;
		float *   m_datafv;
		int32_t * m_dataiv;
		float     m_dataf;
		int32_t   m_datai;
	};
};

#endif
