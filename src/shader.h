#pragma once
#include <string.h>
#include <string>
#include "GLXW/glxw.h"

struct binding
{
	std::string name;
	int32_t index;
};

/**
 * Usage:
 * shader sh(shader_name, vertex_source, fragment_source, attribute_bindings, texture_bindings);
 * sh->compile();
 * sh->link();
 * if(sh->program) good;
 *
 * ///render
 * sh->set();
 **/

struct shader
{
	std::string name, vsstr, psstr;
	const binding *attribs, *texs;
	uint32_t program, vsobj, psobj;

	shader ( const std::string & name, const std::string & vsstr = NULL, const std::string & psstr = NULL, const binding *attribs = NULL, const binding *texs = NULL )
		: name ( name ), vsstr ( vsstr ), psstr ( psstr ), attribs ( attribs ), texs ( texs ), program ( 0 ), vsobj ( 0 ), psobj ( 0 ) {}

	static void showinfo ( uint32_t obj, const std::string & tname, const std::string & name )
	{
		int32_t length = 0;

		if ( tname=="PROG" )
			glGetProgramiv ( obj, GL_INFO_LOG_LENGTH, &length );
		else
			glGetShaderiv ( obj, GL_INFO_LOG_LENGTH, &length );

		if ( length > 1 )
		{
			GLchar *log = new GLchar[length];

			if ( tname == "PROG" )
				glGetProgramInfoLog ( obj, length, &length, log );
			else
				glGetShaderInfoLog ( obj, length, &length, log );

			printf ( "GLSL ERROR (%s:%s)\n", tname.c_str(), name.c_str() );
			puts ( log );

			delete[] log;
		}
	}

	static void compile ( GLenum type, uint32_t &obj, const std::string & def, const std::string & tname, const std::string & name, bool msg = true )
	{
		obj = glCreateShader ( type );
		char const * str = def.c_str();

		glShaderSource ( obj, 1, &str, NULL );
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

			if ( attribs ) for ( const binding *a = attribs; a->name.size(); a++ )
					glBindAttribLocation ( program, a->index, a->name.c_str() );

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

	void compile ( const std::string & vsdef, const std::string & psdef, const binding *attribs = NULL )
	{
		compile ( GL_VERTEX_SHADER,   vsobj, vsdef, "VS", name );
		compile ( GL_FRAGMENT_SHADER, psobj, psdef, "PS", name );
		link ( attribs, true );
	}

	void compile()
	{
		if ( vsstr.size() && psstr.size() ) compile ( vsstr, psstr, attribs );
	}

	void set()
	{
		glUseProgram ( program );
		bindtexs();
	}

	int32_t getparam ( const std::string & pname )
	{
		return glGetUniformLocation ( program, pname.c_str() );
	}

	void bindtex ( const std::string & tname, int32_t index )
	{
		int32_t loc = getparam ( tname );

		if ( loc != -1 ) glUniform1i ( loc, index );
	}

	void bindtexs()
	{
		if ( texs ) for ( const binding *t = texs; t->name.size(); t++ )
				bindtex ( t->name, t->index );
	}
};
