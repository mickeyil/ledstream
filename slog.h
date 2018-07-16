#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <sys/time.h>

#define LOGF(Format, ...) do {                                      \
    Logger::getInstance().logf((Format), ## __VA_ARGS__);           \
  } while(0);

  
static std::string currentDateTime()
{
  timeval curTime;
  gettimeofday(&curTime, NULL);
  int milli = curTime.tv_usec / 1000;

  char buffer [80];
  strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", localtime(&curTime.tv_sec));

  char currentTime[84] = "";
  snprintf(currentTime, sizeof(currentTime), "%s.%03d", buffer, milli);
  
  return std::string(currentTime); 
}


// returns a string of the vector values separated by commas
template <typename T>
std::string vec2str(const std::vector<T>& vec)
{
  std::stringstream ss;
  for (size_t i = 0; i < vec.size(); i++) {
    if (i != 0) ss << ",";
    ss << vec[i];
  }
  return ss.str();
}



class Logger
{
 public:
  static const int buf_size = 1024;
  
  static Logger& getInstance() {
    static Logger _instance;
    return _instance;
  }
  
  void set_logfile(const std::string& log_filename) {
    if(log_filename.empty()) {
      if(logfile != nullptr) {
        fclose(logfile);
        logfile = nullptr;
      }
      return; 
    }
    
    if (logfile != nullptr) {
      fclose(logfile);
    }
    logfile = fopen(log_filename.c_str(), "a");
  }
  
  void logf(const char *format, ...)
  {
    char logmsg[buf_size];
    std::string timestamp = currentDateTime();
    
    // write log message in buffer
    va_list args;
    va_start (args, format);
    vsnprintf(logmsg, sizeof(logmsg), format, args);
    va_end(args);
    
    append_newline(logmsg);
    fprintf(stdout,"[%s] %s", timestamp.c_str(), logmsg);
    if (logfile) {
      fprintf(logfile,"[%s] %s", timestamp.c_str(), logmsg);
    }
  }

 private:
  
  // append '\n' to a given string in place
  void append_newline(char *logbuf)
  {
    size_t ln = strlen(logbuf);
    ln = (ln < buf_size - 2) ? ln : (buf_size - 2);
    bool should_append_newline = (logbuf[ln-1] != '\n');
    if (should_append_newline) {
      logbuf[ln]   = '\n';
      logbuf[ln+1] = '\0';
    }
  }
  
  Logger() : logfile(nullptr) { }
  Logger(const Logger&);
  Logger& operator=(const Logger&);


  
  FILE * logfile;
   
};
 

