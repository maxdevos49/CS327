#ifndef STRING327_H
#define STRING327_H

#include <iostream>

class string327
{
private:
  char *str;

public:
  string327();
  string327(const char *s);
  ~string327();

  int length() const;

  const char *c_string() const;

  int operator<(const string327 &s);
  int operator>(const string327 &s);
  int operator==(const string327 &s);
  int operator!=(const string327 &s);
  int operator<=(const string327 &s);
  int operator>=(const string327 &s);
  string327 operator+(const string327 &s);
  string327 &operator+=(const string327 &s);
  string327 &operator=(const string327 &s);
  string327 &operator=(const char *s);
  char &operator[](const int i);

  friend std::istream &operator>>(std::istream &i, const string327 &s);
};

std::ostream &operator<<(std::ostream &o, const string327 &s);

#endif