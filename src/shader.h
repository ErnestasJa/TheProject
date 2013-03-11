#pragma once
#include <string.h>

struct binding
{
	const char *name;
	int32_t index;
};

struct shader
{
	const char *name, *vsstr, *psstr;
	const binding *attribs, *texs;
	uint32_t vs, ps, program, vsobj, psobj;

	shader ( const char *name, const char *vsstr = NULL, const char *psstr = NULL, const binding *attribs = NULL, const binding *texs = NULL )
	: name ( name ), vsstr ( vsstr ), psstr ( psstr ), attribs ( attribs ), texs ( texs ), vs ( 0 ), ps ( 0 ), program ( 0 ), vsobj ( 0 ), psobj ( 0 ) {}

	static void showinfo ( uint32_t obj, const char *tname, const char *name )
	{
		int32_t length = 0;

		if ( !strcmp ( tname, "PROG" ) ) glGetProgramiv ( obj, GL_INFO_LOG_LENGTH, &length );
		else glGetShaderiv ( obj, GL_INFO_LOG_LENGTH, &length );

		if ( length > 1 )
		{
			GLchar *log = new GLchar[length];

			if ( !strcmp ( tname, "PROG" ) ) glGetProgramInfoLog ( obj, length, &length, log );
			else glGetShaderInfoLog ( obj, length, &length, log );

			printf ( "GLSL ERROR (%s:%s)\n", tname, name );
			puts ( log );
			delete[] log;
		}
	}

	static void compile ( GLenum type, uint32_t &obj, const char *def, const char *tname, const char *name, bool msg = true )
	{
		const GLchar *source = ( const GLchar* ) ( def + strspn ( def, " \t\r\n" ) );
		obj = glCreateShader ( type );
		glShaderSource ( obj, 1, &source, NULL );
		glCompileShader ( obj );
		int32_t success;
		glGetShaderiv ( obj, GL_COMPILE_STATUS, &success );

		if ( !success )
		{
			if ( msg ) showinfo ( obj, tname, name );

			glDeleteShader ( obj );
			obj = 0;
			printf ( "error compiling shader\n" );
		}
	}

	void link ( const binding *attribs = NULL, bool msg = true )
	{
		program = vsobj && psobj ? glCreateProgram() : 0;
		int32_t success = 0;

		if ( program )
		{
			glAttachShader ( program, vsobj );
			glAttachShader ( program, psobj );

			if ( attribs ) for ( const binding *a = attribs; a->name; a++ )
					glBindAttribLocation ( program, a->index, a->name );

			glLinkProgram ( program );
			glGetProgramiv ( program, GL_LINK_STATUS, &success );
		}

		if ( !success )
		{
			if ( program )
			{
				if ( msg ) showinfo ( program, "PROG", name );

				glDeleteProgram ( program );
				program = 0;
			}

			printf ( "error linking shader\n" );
		}
	}

	void compile ( const char *vsdef, const char *psdef, const binding *attribs = NULL )
	{
		compile ( GL_VERTEX_SHADER,   vsobj, vsdef, "VS", name );
		compile ( GL_FRAGMENT_SHADER, psobj, psdef, "PS", name );
		link ( attribs, true );
	}

	void compile()
	{
		if ( vsstr && psstr ) compile ( vsstr, psstr, attribs );
	}

	void set()
	{
		glUseProgram ( program );
		bindtexs();
	}

	int32_t getparam ( const char *pname )
	{
		return glGetUniformLocation ( program, pname );
	}

	void bindtex ( const char *tname, int32_t index )
	{
		int32_t loc = getparam ( tname );

		if ( loc != -1 ) glUniform1i ( loc, index );
	}

	void bindtexs()
	{
		if ( texs ) for ( const binding *t = texs; t->name; t++ )
				bindtex ( t->name, t->index );
	}
};
