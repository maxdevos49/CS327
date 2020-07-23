#include <string327.h>
#include <cstdlib>
#include <cstring>

/**
 * Empty constructor for string
 * */
string327::string327()
{
    str = (char *)malloc(1);
    str[0] = '\0';
}

/**
 * Constructor for string with supplied litteral
 * */
string327::string327(const char *s)
{
    str = strdup(s);
}

/**
 * Destructor for the string
 * */
string327::~string327()
{
    free(str);
}

/**
 * Returns the legnth of the string
 * */
int string327::length() const
{
    return strlen(str);
}

/**
 * Returns a c style string pointer
 * */
const char *string327::c_string() const
{
    return str;
}

/**
 * 
 * */
int string327::operator<(const string327 &s)
{
    return strcmp(str, s.str) < 0;
}

/**
 * 
 * */
int string327::operator>(const string327 &s)
{
    return strcmp(str, s.str) > 0;
}

/**
 * 
 * */
int string327::operator==(const string327 &s)
{
    return strcmp(str, s.str) == 0;
}

/**
 * 
 * */
int string327::operator!=(const string327 &s)
{
    return strcmp(str, s.str) != 0;
}

/**
 * 
 * */
int string327::operator<=(const string327 &s)
{
    return strcmp(str, s.str) <= 0;
}

/**
 * 
 * */
int string327::operator>=(const string327 &s)
{
    return strcmp(str, s.str) >= 0;
}

/**
 * Concatenate two strings together and then return the new string
 * */
string327 string327::operator+(const string327 &s)
{
    string327 out;

    free(out.str);
    out.str = (char *)malloc(strlen(str) + strlen(s.str) + 1);
    strcpy(out.str, str);
    strcat(out.str, s.str);

    return out; //not sure if this was needed
}

/**
 * combines two strings into the left string
 * */
string327 &string327::operator+=(const string327 &s)
{
    str = (char *)realloc(str, strlen(str) + strlen(s.str) + 1);
    strcat(str, s.str);

    return *this;
}

/**
 * sets the left string to the value of the left string
 * */
string327 &string327::operator=(const string327 &s)
{
    free(str);
    str = (strdup(s.str));

    return *this;
}

/**
 * Assign a string literal to the string object
 * */
string327 &string327::operator=(const char *s)
{
    free(str);
    str = strdup(s);

    return *this;
}

/**
 * 
 * */
char &string327::operator[](const int i)
{
    return str[i];
}

std::istream &operator>>(std::istream &i, const string327 &s)
{
    //Broken when input is longer than s.length
    return i >> s.str;
}

std::ostream &operator<<(std::ostream &o, const string327 &s)
{
    return o << s.c_string();
}
