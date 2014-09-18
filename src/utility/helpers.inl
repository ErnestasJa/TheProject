template <typename T> inline std::string to_str(const T& t)
{
    std::ostringstream os;    //magic..
    os<<t;
    return os.str();
}

template <typename T> inline std::wstring to_wstr(const T& t)
{
    std::wostringstream os;    //magic..
    os<<t;
    return os.str();
}

template <typename T>
inline T limit(T val, T min, T max)
{
    T ret;
    if(val<min)
        ret=min;
    else if(val>max)
        ret=max;
    else
        ret=val;

    return ret;
}

inline const glm::vec4 color255(uint8_t r,uint8_t g,uint8_t b,uint8_t a)
{
    return glm::vec4((float)r/255,(float)g/255,(float)b/255,(float)a/255);
}

inline void invert(glm::vec3 & vec)
{
    if(vec.x)
        vec.x = 1.0f / vec.x;
    if(vec.y)
        vec.y = 1.0f / vec.y;
    if(vec.z)
        vec.z = 1.0f / vec.z;
}


template <>
inline bool equals(const float & v1, const float & v2)
{
    return (v1 + ROUNDING_ERROR >= v2) && (v1 - ROUNDING_ERROR <= v2);
}

template <>
inline bool equals(const glm::vec3 & v1, const glm::vec3 & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z);
}

template <>
inline bool equals(const glm::vec4 & v1, const glm::vec4 & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z) && equals(v1.w,v2.w);
}

template <>
inline bool equals(const glm::quat & v1, const glm::quat & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z) && equals(v1.w,v2.w);
}

template <>
inline bool equals(const glm::mat4 & v1, const glm::mat4 & v2)
{
    return equals(v1[0],v2[0]) && equals(v1[1],v2[1]) && equals(v1[2],v2[2]) && equals(v1[3],v2[3]);
}

template <>
inline bool equals(const glm::mat3 & v1, const glm::mat3 & v2)
{
    return equals(v1[0],v2[0]) && equals(v1[1],v2[1]) && equals(v1[2],v2[2]);
}

inline float coslerp(float y1,float y2,float mu)
{
   float mu2;

   mu2 = (1-glm::cos(mu*glm::pi<float>()))/2;
   return(y1*(1-mu2)+y2*mu2);
}
