#pragma once

struct binding
{
    std::string name;
    int32_t index;
};

/**
 * Usage:
 * shader sh(shader_name, vertex_source, fragment_source);
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
    uint32_t program, vsobj, psobj;

    shader ( const std::string & name, const std::string & vsstr = nullptr, const std::string & psstr = nullptr)
        : name ( name ), vsstr ( vsstr ), psstr ( psstr ), program ( 0 ), vsobj ( 0 ), psobj ( 0 ) {}

    ~shader()
    {
        if(program)
        glDeleteProgram(program);
    }

    static shader * load_shader(const std::string & name, const binding *uniforms = nullptr, const binding *texs = nullptr)
    {
        char * vsh=nullptr;
        char * fsh=nullptr;

        uint32_t l1 = helpers::read(name+".vert",vsh);

        if(l1==0)
            return nullptr;

        uint32_t l2 = helpers::read(name+".frag",fsh);

        if(l2==0)
            return nullptr;

        shader * sh = new shader(name,vsh,fsh);

        sh->compile();
        sh->link();

        delete [] vsh;
        delete [] fsh;

        if(sh->program==0)
        {
            delete sh;
            return nullptr;
        }

        return sh;
    }

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


            printf ( "GLSL ERROR (%s:%s): [%i] %s\n", tname.c_str(), name.c_str(), length, log);

            delete[] log;
        }
    }

    static void compile ( GLenum type, uint32_t &obj, const std::string & def, const std::string & tname, const std::string & name, bool msg = true )
    {
        obj = glCreateShader ( type );
        char const * str = def.c_str();

        glShaderSource ( obj, 1, &str, nullptr );
        glCompileShader ( obj );

        int32_t success;
        glGetShaderiv ( obj, GL_COMPILE_STATUS, &success );

        if ( !success )
        {
            showinfo ( obj, tname, name );

            glDeleteShader ( obj );
            obj = 0;

            printf ( "error compiling shader\n" );
        }
    }

    void link (bool msg = true )
    {
        program = vsobj && psobj ? glCreateProgram() : 0;
        int32_t success = 0;

        if ( program )
        {
            glAttachShader ( program, vsobj );
            glAttachShader ( program, psobj );

            glLinkProgram ( program );
            glGetProgramiv ( program, GL_LINK_STATUS, &success );

            if ( !success )
            {
                if ( program )
                {
                    showinfo ( program, "PROG", name );

                    glDeleteProgram ( program );
                    program = 0;
                }

                printf ( "error linking shader\n" );
            }
            else
            {
                glDetachShader(program, vsobj);
                glDetachShader(program, psobj);
            }
        }
    }

    void compile ( const std::string & vsdef, const std::string & psdef)
    {
        compile ( GL_VERTEX_SHADER,   vsobj, vsdef, "VS", name );
        compile ( GL_FRAGMENT_SHADER, psobj, psdef, "PS", name );
        link ( true );
    }

    void compile()
    {
        if ( vsstr.size() && psstr.size() ) compile ( vsstr, psstr );
    }

    void set()
    {
        glUseProgram ( program );
    }

    int32_t getparam ( const std::string & pname )
    {
        return  glGetUniformLocation ( program, pname.c_str() );
    }

    void query_binding_locations(binding * v)
    {
        for ( binding *t = v; t->name.size(); t++ )
            t->index = glGetUniformLocation ( program, t->name.c_str() );
    }
};

typedef std::shared_ptr<shader> shader_ptr;
