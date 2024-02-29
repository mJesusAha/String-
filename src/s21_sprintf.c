#include "s21_sprintf.h"

va_list factor;

int s21_sprintf(char* str, char* format, ...) {
  char str1[MAX - 1] = {0};
  for (int t = 0; str1[t] != '\0'; t++) {
    str1[t] = '\0';
  }
  int flags[20] = {0};
  flags[4] = -1;
  va_start(factor, format);
  for (char* p = format; *p; p++) {
    if (*p != '%') {
      defaultCase(str1, p);
      continue;
    } else {
      p++;
      if (p[0] == '%') {
        defaultCase(str1, p);
        continue;
      }
      flagsPlusMinusSpace(p, flags);
      while (p[0] == ' ' || p[0] == '-' || p[0] == '+') {
        p++;
      }
      int rep = sizeFunction(p, flags);
      while (rep > 0) {
        p++;
        rep--;
      }
      if (p[0] == 'l' || p[0] == 'h') {
        forFunctionSprintfFlagsLH(flags, p);
        p++;
      }
    }
    caseFunctions(p, str1, flags);
  }
  newMass(str, str1);
  va_end(factor);
  return strlen(str1);
}

void forFunctionSprintfFlagsLH(int* flags, const char* p) {
  if (p[0] == 'l') {
    flags[5] = 1;
  }
  if (p[0] == 'h') {
    flags[6] = 1;
  }
}

int newMass(char* str, char* str2) {
  str[0] = 1;
  int num = 0;
  while (str2[num] != '\0') {
    str[num] = str2[num];
    num++;
  }
  str[num] = 0;
  return strlen(str2);
}

int sizeFunction(char* p, int* flags) {
  int rep = 0;
  if (p[0] <= 57 && p[0] >= 48) {
    int temp = 0;
    char integer[10] = {0};
    while (p[0] <= 57 && p[0] >= 48) {
      integer[temp] = p[0];
      temp++;
      p++;
      rep++;
    }
    int size = strInNumber(integer);
    flags[3] = (int)size;
  }
  if (p[0] == '.') {
    flags[12] = 1;
    p++;
    rep++;
    int temp = -1;
    char integer[30] = {0};
    while (p[0] <= 57 && p[0] >= 48) {
      temp++;
      integer[temp] = p[0];
      p++;
      rep++;
    }
    int size = strInNumber(integer);
    flags[4] = size;
  }
  return rep;
}

int flagsPlusMinusSpace(const char* p, int* flags) {
  int t = 0;
  if (p[0] == '-' || p[0] == '+' || p[0] == ' ') {
    int i = 0;
    if (p[i] == ' ') {
      while (p[i] == ' ') {
        i++;
        t++;
      }
      flags[2] = 1;
    }
    if (p[i] == '-') {
      t = t + flagsActiv(flags, 0, 0);
    }
    if ((p[0] == '-' && p[1] == '+') || (p[0] == '+' && p[1] == '-')) {
      t = t + flagsActiv(flags, 1, 0);
    }
    if ((p[0] == ' ' && p[1] == '-') || (p[0] == '-' && p[1] == ' ')) {
      t = t + flagsActiv(flags, 2, 0);
    }
    if ((p[0] == '+' && p[1] == ' ') || (p[0] == ' ' && p[1] == '+')) {
      t = t + flagsActiv(flags, 2, 1);
    }
    if (p[0] == '+') {
      t = t + flagsActiv(flags, 1, 1);
    }
    if (p[0] == '-') {
      t = t + flagsActiv(flags, 0, 0);
    }
  }
  return t;
}

int flagsActiv(int* flags, int n1, int n2) {
  int t = 0;
  if (n1 == n2) {
    t = 1;
  } else {
    t = 2;
  }
  flags[n1] = 1;
  flags[n2] = 1;
  return t;
}

void caseFunctions(char* p, char* str, int* flags) {
  switch (p[0]) {
    case 'u':
      specifiersUType(str, flags);
      break;
    case 'd':
      specifiersDType(str, flags);
      break;
    case 's':
      specifiersSType(str, flags);
      break;
    case 'f':
      specifiersFType(str, flags);
      break;
    case 'i':
      specifiersDType(str, flags);
      break;
    case 'c':
      specifiersCType(str, flags);
      break;
  }
}

void defaultCase(char* str, const char* p) {
  char ch[2] = {0};
  ch[0] = p[0];
  addInStr(str, ch);
}

void specifiersCType(char* str, int* flags) {
  char ch1[2] = {0};
  char f = (char)va_arg(factor, int);
  ch1[0] = f;
  ch1[1] = '\0';
  if (flags[0] == 0) addFlagsSizeInStr(flags, str, ch1);
  addInStr(str, ch1);
  if (flags[0] == 1) addFlagsSizeInStr(flags, str, ch1);
  cleanMass(flags, 20);
  flags[4] = -1;
}

void specifiersFType(char* str, int* flags) {
  double f = 0;
  f = va_arg(factor, double);
  specifiersF(str, flags, f);
  cleanMass(flags, 20);
  flags[4] = -1;
}

void specifiersSType(char* str, int* flags) {
  char *s = {0}, s2[MAX] = {0};
  s = va_arg(factor, char*);
  addInStr(s2, s);
  int size = strlen(s2);
  if (flags[4] >= 0 && flags[4] < size) {
    s2[flags[4]] = '\0';
  }
  size = strlen(s2);
  int width = 0;
  if (flags[0] == 1) {
    width = flags[3];
    flags[3] = 0;
  }
  addFlagsSizeInStr(flags, str, s2);
  addInStr(str, s2);
  spaceAfter(size, width, str, flags);
  size = strlen(str);
  str[size] = '\0';
  cleanMass(flags, 20);
  flags[4] = -1;
}

void specifiersUType(char* str, int* flags) {
  if (flags[1] > 0 || flags[2] > 0) {
    flags[1] = 0;
    flags[2] = 0;
  }
  if (flags[5] == 0) {
    unsigned u = 0;
    u = va_arg(factor, unsigned int);
    if (flags[4] > 0) {
      flags[8] = 1;
      specifiersD(str, flags, u);
    } else {
      specifiersU(str, flags, u);
    }
  }
  if (flags[5] == 1) {
    flags[7] = 1;
    long unsigned u = 0;
    u = va_arg(factor, long unsigned int);
    specifiersUL(str, flags, u);
  }
  cleanMass(flags, 20);
  flags[4] = -1;
}

void specifiersDType(char* str, int* flags) {
  flags[8] = 1;
  if (flags[5] == 1 && flags[6] == 0) {
    long int d = 0;
    d = va_arg(factor, long int);
    specifiersD(str, flags, d);
  } else if (flags[5] == 0 && flags[6] == 0) {
    int d = 0;
    d = va_arg(factor, int);
    specifiersD(str, flags, d);
  } else if (flags[5] == 0 && flags[6] == 1) {
    int d = 0;
    d = va_arg(factor, int);
    specifiersD(str, flags, (short int)d);
  }
  cleanMass(flags, 20);
  flags[4] = -1;
}

int intParthOne(char* str, long int* number) {
  if (*number == 0) {
    str[0] = '0';
  }
  int flag = 0;
  if (*number < 0) {
    flag = 1;
    *number = *number * -1;
  }
  return flag;
}

int intParthTwo(long unsigned int num1, const int* counter) {
  long unsigned int num = num1;
  int counter1 = *counter;
  while (num > 0) {
    num = num / 10;
    counter1++;
  }
  return counter1;
}

int numberIntInStr(char* str, long int number, const int* flags) {
  long int n_number = number;
  int flag = intParthOne(str, &n_number);
  long unsigned int num = (long unsigned int)n_number;
  long int num1 = n_number;
  int counter = 0, counter2 = 0, flag1 = 0;
  const char simbol[] = "0123456789";
  counter = intParthTwo(num, &counter);
  counter2 = counter;
  if (counter <= flags[3]) {
    counter = flags[3];
  }
  num = n_number;
  while ((long int)num >= 0 && flag1 == 0) {
    if (num == 0) {
      flag1 = 1;
    }
    num = (long int)num / 10;
    num1 = num1 % 10;
    for (int i = 0; i < 10; i++) {
      if ((int)num1 == i && flag1 == 0) {
        if (counter >= 1) {
          str[counter - 1] = simbol[i];
          counter--;
        }
      }
    }
    num1 = (long int)num;
  }
  number = IntParthThree(flag1, flag, counter, str, number, flags);
  counter2 = counter2 + pointingAccuracyOutput(flags, 1, 0, number, 1, 1, 0);
  counter2 = counter2 + pointingAccuracyOutput(flags, 1, 0, number, 2, 1, 1);
  counter2 = counter2 + pointingAccuracyOutput(flags, 2, 0, number, 2, 1, 1);
  return counter2;
}

int IntParthThree(int flag11, int flag12, int counter1, char* str,
                  long int number, const int* flags) {
  int fl = 0, flag1 = flag11, flag = flag12, counter = counter1;
  while (flag1 == 1 && counter >= 0) {
    if (flag == 1 && flags[0] == 0 && flags[1] == 0 && flags[2] == 0) {
      if (counter >= 1) {
        str[counter - 1] = '-';
        flag = 0;
      }
    } else if (flag == 0) {
      if (number == 0 && fl == 0) {
        if (counter >= 1) {
          str[counter - 1] = '0';
          fl = 1;
        }
      } else {
        if (counter >= 1) {
          str[counter - 1] = ' ';
        }
      }
    }
    counter--;
  }
  return number;
}

int pointingAccuracyOutput(const int* flags, const int f1, const int f2,
                           const int number, const int point, const int i,
                           const int j) {
  int n = 0;
  if (flags[f1] == i && flags[f2] == j && number == 0) {
    n = point;
  }
  return n;
}

int numberUIntInStr(char* str, long unsigned int number, const int* flags) {
  long unsigned int num = number, num1 = number;
  int counter = 0, counter2 = 0;
  const char simbol[] = "0123456789";
  counter = intParthTwo(num, &counter);
  counter2 = counter;
  if (counter <= flags[3]) {
    counter = flags[3];
  }
  num = number;
  while (num > 0) {
    num = (long unsigned int)num / 10;
    num1 = num1 % 10;
    for (int i = 0; i < 10; i++) {
      if ((long int)num1 == i) {
        if (counter >= 1) {
          str[counter - 1] = simbol[i];
          counter--;
        }
      }
    }
    num1 = (long unsigned int)num;
  }
  return counter2;
}

int numberFloatStr(char* str, long double number, int* flags) {
  int counter1 = 0;
  long long int accuracy_i = pow(10, flags[4] + 1);
  if (flags[4] == 0) {
    accuracy_i = 100;
  }
  int one = 0;
  long double number_n = number;
  if (flags[11] == 1) {
    number_n = number_n * -1;
  }
  if (number_n >= 0 && number_n < 1 && flags[4] != 0) {
    number_n = number_n + 1;
    one = 1;
  }
  number_n = number_n * accuracy_i;
  if (flags[4] == 0) {
    int integer = number_n / 100;
    counter1 = numberIntInStr(str, integer, flags) - 1;
    if (number == 0) {
      counter1 = counter1 + 1;
    }
  } else {
    long long int num = number_n;
    long long int temp = num % 10;
    int flag = -1;
    if (temp >= 5) {
      flag = 1;
    }
    num = num / 10;
    long long int num1 = num;
    const char simbol[] = "0123456789";
    int counter = 0;
    while (num >= 1) {
      if (flag == 1) {
        num = num + 1;
        num1 = num1 + 1;
        flag = -1;
      }
      num = num / 10;
      counter++;
    }
    counter = counter + 1;
    counter1 = counter;
    num = num1;
    if (number == 0) {
      str[0] = '0';
      counter1--;
      str[1] = '.';
      int j = 2, n = flags[4];
      while (n > 0) {
        str[j] = '0';
        j++;
        n--;
      }
    } else {
      int t = 0;
      while (counter > 0) {
        num = (long long int)num / 10;
        num1 = num1 % 10;
        for (int i = 0; i < 10; i++) {
          if (num1 == i && t != flags[4]) {
            if (counter >= 1) {
              str[counter - 1] = simbol[i];
              num1 = (long long int)num;
              counter--;
              t++;
            }
          }

          if (t == flags[4]) {
            if (counter >= 1) {
              str[counter - 1] = '.';
              num1 = (long long int)num;
              counter--;
              t++;
            }
            if (one == 1) {
              if (counter >= 1) {
                str[counter - 1] = '0';
                one = 0;
                num1 = (long long int)num;
                counter--;
                t++;
              }
            }
          }
        }
      }
    }
  }
  return counter1;
}

int strInNumber(const char* str) {
  const char simbol[] = "0123456789";
  int n = 0;
  int integer = 0;
  while ((str[n] <= 57) && (str[n] >= 48)) {
    n++;
  }
  int max_long_int = pow(10, n - 1);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 10; j++) {
      if (str[i] == simbol[j]) {
        integer = j * max_long_int + integer;
        max_long_int = max_long_int / 10;
      }
    }
  }
  return integer;
}

int specifiersU(char* str, int* flags, unsigned d) {
  char str_d[MAX - 1] = {0};
  int width = 0;
  if (flags[0] == 1) {
    width = flags[3];
    flags[3] = 0;
  }
  int size = numberIntInStr(str_d, d, flags);
  if (size < width) {
    flags[3] = 0;
  }
  addInStr(str, str_d);
  spaceAfter(size, width, str, flags);
  return size;
}

int specifiersUL(char* str, int* flags, long unsigned d) {
  char str_d[MAX - 1] = {0};
  int width = 0;
  if (flags[0] == 1) {
    width = flags[3];
    flags[3] = 0;
  }
  int size = numberUIntInStr(str_d, d, flags);
  if (size < width) {
    flags[3] = 0;
  }
  addInStr(str, str_d);
  spaceAfter(size, width, str, flags);
  return size;
}

void strPlusMinusSpace(char* str, int* flags, long double d) {
  if (flags[5] == 0) {
    d = (int)d;
  }
  if ((d < 0 && flags[3] == 0 && flags[0] == 0 && flags[1] == 0 &&
       flags[2] == 0) ||
      flags[11] == 1 ||
      (d < 0 && (flags[0] == 1 || flags[1] == 1 || flags[2] == 1))) {
    char n[2] = {0};
    n[0] = '-';
    addInStr(str, n);
  }
  if (flags[0] == 1 || flags[1] == 1 || flags[2] == 1) {
    if (flags[2] == 1) {
      flags[1] = 0;
    }
    char n[2] = {0};
    if (d >= 0 && flags[1] == 1) {
      n[0] = '+';
      addInStr(str, n);
    }
    if (d >= 0 && flags[2] == 1 && flags[11] != 1) {
      n[0] = ' ';
      addInStr(str, n);
    }
  }
}

int specifiersD(char* str, int* flags, long int d) {
  int size = -1;
  if (d == 0 && flags[3] == 0 && flags[4] == 0) {
  } else {
    int width = 0;
    char str_d[MAX - 1] = {0}, str_d2[MAX - 1] = {0};
    if (flags[4] == -1 && flags[8] == 1) {
      flags[8] = 0;
    }
    if (flags[8] == 1 && flags[4] > 0 && (flags[4] >= flags[3])) {
      flags[3] = 0;
    }
    if (flags[0] == 1 || flags[1] == 1 || flags[2] == 1 || flags[8] == 1) {
      width = flags[3];
      flags[3] = 0;
    }
    size = numberIntInStr(str_d, d, flags);
    if (flags[8] == 1 && flags[4] > 0 && (flags[4] > flags[3])) {
      int zero;
      char z[] = "0";
      if (d == 0) {
        if (flags[4] > 1) {
          zero = flags[4] - size - 1;
          while (zero > 0) {
            addInStr(str_d2, z);
            zero--;
            size++;
          }
          size--;
          width--;
        }
      } else {
        zero = flags[4] - size;
        while (zero > 0) {
          addInStr(str_d2, z);
          zero--;
          size++;
        }
        size--;
        if (flags[0] == 1) {
          width--;
        }
        if (flags[1] == 1) {
          size = size + 1;
        }
      }
      if (flags[4] > 0 && flags[1] > 0 && width == size) {
        width = width - size;
      }
    }
    spaceBefore(size, width, str, flags);
    strPlusMinusSpace(str, flags, d);
    addInStr(str, str_d2);
    addInStr(str, str_d);
    spaceAfter(size, width, str, flags);
    cleanMass(flags, 20);
    flags[4] = -1;
  }
  return size;
}

void spaceBefore(int size, int width, char* str, const int* flags) {
  if ((((flags[0] == 0 && flags[1] == 1) || (flags[0] == 0 && flags[2] == 1)) &&
       width > size) ||
      (flags[11] > 0 && width > size && flags[0] == 0)) {
    char space[2] = " ";
    width = width - size - 1;
    while (width != 0) {
      addInStr(str, space);
      width--;
    }
  } else if (flags[0] == 0 && flags[8] == 1) {
    char space[2] = " ";
    if (width > size) width = width - size - 1;
    while (width != 0) {
      addInStr(str, space);
      width--;
    }
  }
}

void spaceAfter(int size, int width, char* str, const int* flags) {
  if ((((flags[0] == 1 && flags[1] == 1) || (flags[2] == 1 && flags[0] == 1) ||
        flags[0] == 1) &&
       width > size) ||
      (flags[11] > 0 && width > size && flags[0] == 1)) {
    char space[2] = " ";
    width = width - size;
    while (width != 0) {
      addInStr(str, space);
      width--;
    }
  }
}

int specifiersF(char* str, int* flags, long double f) {
  flags[11] = 2;
  if (flags[4] == -1) {
    flags[4] = 6;
  }
  if (f < 0) {
    flags[11] = 1;
  }
  int width = 0;
  char str_d[MAX - 1] = {0};
  width = flags[3];
  flags[3] = 0;
  int size = numberFloatStr(str_d, f, flags);
  spaceBefore(size, width, str, flags);
  strPlusMinusSpace(str, flags, f);
  char t[2] = {0};
  addInStr(str, t);
  addInStr(str, str_d);
  size++;
  spaceAfter(size, width, str, flags);
  cleanMass(flags, 20);
  flags[4] = -1;
  return size;
}

int addInStr(char* str, const char* str_new) {
  strcat(str, str_new);
  return 0;
}

void addFlagsSizeInStr(const int* flags, char* str, char* s) {
  int t = strlen(s);
  if (flags[3] > t) {
    t = flags[3] - t;
    while (t > 0) {
      str[strlen(str)] = ' ';
      t--;
    }
  }
}

void cleanMass(int* mass, int n) {
  for (int i = 0; i < n; i++) {
    mass[i] = 0;
  }
}
