#ifndef LOG_H
#define LOG_H

#include <sstream>
#include <string>

class QString;

class PeriodicLog {
    friend struct Log_;
public:

    static PeriodicLog make(const std::string &name);

    static PeriodicLog makeAuto(const std::string &name);

private:

    PeriodicLog();

    PeriodicLog(const std::string &name, bool isAutoPeriodic);

    bool notSet() const;

private:

    std::string name;

    bool isAutoPeriodic = false;

};

struct Log_ {
    enum Level {Low, High};

#ifdef PRODBUILD
    const Level MinLevel = High;
#else
    const Level MinLevel = Low;
#endif

    struct Alias {
        std::string name;

        Alias(const std::string &name)
            : name(name)
        {}
    };

    Log_(const std::string &fileName, Level l = High);

    Log_(const Alias &alias, Level l = High);

    template<typename T>
    Log_& operator <<(T t) {
        if (level < MinLevel)
            return *this;
        print(t);
        return *this;
    }

    void finalize() noexcept;

    ~Log_() noexcept {
        if (level < MinLevel)
            return;
        finalize();
    }

private:

    void printHead();

    void printAlias(const std::string &alias);

    template<typename T>
    void print(const T &t) {
        ssCout << t;
    }

    void print(const std::string &t);

    void print(const QString &s);

    void print(const PeriodicLog &p);

    void print(const bool &b);

    bool processPeriodic(const std::string &s, std::string &periodicStrFirstLine, std::string &periodicStrOriginalLinePrefix);

    std::stringstream ssCout;
    std::stringstream ssLog;

    PeriodicLog periodic;

    Level level = High;

};

void initLog();

struct AddFileNameAlias_ {

    AddFileNameAlias_(const std::string &fileName, const std::string &alias);

};

#define LOG_FILE (std::string(__FILE__))

#define LOG Log_(LOG_FILE)

#define LOGLOW Log_(LOG_FILE,Log_::Low)

#define LOG2(file) Log_(file)

#define LOG3(alias) Log_(Log_::Alias(alias))

#define SET_LOG_NAMESPACE(name) \
    static AddFileNameAlias_ log_alias_(std::string(__FILE__), name);

#endif // LOG_H
