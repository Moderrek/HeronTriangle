#ifndef SCREENSHOT_HPP_INCLUDED
#define SCREENSHOT_HPP_INCLUDED 1

class Screenshot {
public:
  static void save(const char* filename, int width, int height);
};

#endif // SCREENSHOT_HPP_INCLUDED