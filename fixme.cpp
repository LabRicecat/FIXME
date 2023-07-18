#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <random>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <stack>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

static inline bool has_digit(long long num, int digit) {
    while(num > 0) {
        if(num % 10 == digit) return true;
        num /= 10;
    }
    return false;
}

static inline bool is_number(const std::string& str) {
    try {
        std::stoll(str);
    }
    catch(...) { return false; }
    return true;
}

static inline std::string read_file(const std::string& file) {
    std::string r;
    std::fstream f(file);
    while(f.good()) r += f.get();
    if(r != "") r.pop_back();
    return r;
}

static inline void flip_stack(std::stack<long long>& stack) {
    std::stack<long long> cp;
    while(!stack.empty()) {
        cp.push(stack.top());
        stack.pop();
    }
    stack = cp;
}

template<typename T>
struct option {
    T* t = nullptr;

    option() {}
    option(option<T>& o) {
        if(o.t)
            t = new T(*o.t);
    }

    option(T* t_) {
        t = t_;
    }

    T* operator->() {
        return t;
    }
    T& operator*() {
        return *t;
    }

    operator bool() { return t != nullptr; }
    ~option() {
        if(t);
            // delete t;
            // yes, this interpreter memory leaks!
    }
};

namespace fixme {
    inline std::stack<long long> callstack;
    inline std::map<long long,std::string> line_source;
    inline unsigned long lineptr = 0;
    void throw_useless_error(const std::string& message) {
        std::cout << line_source[lineptr] << " // FIXME (" << message << ")\n";
        std::exit(rand() % 100);
    }

    struct value {
        enum {
            STRING,
            NUMBER,
        } type;

        std::string str = "";
        long long number = 0;

        value(long long n): number(n) { type = NUMBER; }
        value() {}
        value(const std::string& str): str(str) { type = STRING; }
        explicit value(const char* str): str(str) { type = STRING; }

        std::string stringify() {
            if(type == STRING) return str;
            return std::to_string(number);
        }

        bool operator==(const value& v) {
            if(type != v.type) return false;
            if(type == NUMBER) return number == v.number;
            return str == v.str;
        }
        bool operator!=(const value& v) { return !operator==(v); }
    };

    struct word {
        int args = 0;
        std::function<value(std::vector<std::string>)> task;
        bool noret = false;
    };
    
    struct assign;
    struct reg;
    struct line {
        word m_word;
        std::vector<std::string> args;
        option<assign> as;
        option<value> vl;

        line() {}
    };

    struct assign {
        reg* regist = nullptr;
        line ln;
    };

    struct reg {
        value val;
        bool null = true;
    };

    inline long long failed_writes = 0;
    namespace registers {
        inline reg jeniffer; // always writable
        inline reg josh; // dislikes jack
        inline reg james; // best friend with jon
        inline reg jon; // only writable on odd lines
        inline reg jack; // ignores 50% of all write calls
        inline reg jeffrey; // not writable to
        inline reg jackob; // only writable if half of the other registers are off
        inline reg jessica; // writing to this one turns off all other registers
    }
    
    reg& str2reg(const std::string& str) {
        using namespace registers;
        static std::unordered_map<std::string,reg*> r = {
            {"jeniffer",&jeniffer},
            {"josh",&josh},
            {"james",&james},
            {"jon",&jon},
            {"jack",&jack},
            {"jeffrey",&jeffrey},
            {"jackob",&jackob},
            {"jessica",&jessica}
        };
        if(r.count(str) != 0) return *r[str];
        throw_useless_error("get to know person " + str);
    }
    
    inline bool is_reg(const std::string& str) {
        static std::set<std::string> regs = 
        {"jeniffer","josh","james","jon","jack","jeffrey","jackob","jessica"};
        return regs.count(str) != 0;
    }
    inline static int alive_regs() {
        using namespace registers;
        return 8 - (jeniffer.null + josh.null + james.null + jon.null + jack.null + jeffrey.null + jessica.null + jackob.null);
    }
    void write_to(reg& regist, value val) {
        if(failed_writes == 20) {
            throw_useless_error("calm down the people");
        }
        using namespace registers;
        if(&regist == &jeniffer) {
            jeniffer.null = false;
            jeniffer.val = val;
        }
        else if(&regist == &josh) {
            if(jack.null) {
                josh.null = false;
                josh.val = val;
            }
            else ++failed_writes;
        }
        else if(&regist == &james) {
            if(!jon.null) {
                james.null = false;
                james.val = val;
            }
            else ++failed_writes;
        }
        else if(&regist == &jon) {
            if(lineptr % 2 == 1) {
                jon.null = false;
                jon.val = val;
            }
            else ++failed_writes;
        }
        else if(&regist == &jack) {
            if(rand() % 2 == 1) {
                jack.null = false;
                jack.val = val;
            }
            // else ++failed_writes;
        }
        else if(&regist == &jeffrey) {
            // no
            ++failed_writes;
        }
        else if(&regist == &jackob) {
            int total_on = alive_regs();
            if(total_on <= 4) {
                jackob.null = false;
                jackob.val = val;
            }
        }
        else if(&regist == &jessica) {
            jeniffer.null = 
            josh.null = 
            james.null =
            jon.null = 
            jack.null =
            jeffrey.null =
            jackob.null = true;
            jessica.null = false;
            jessica.val = val;
        }
        else {
            throw_useless_error("get to know person " + std::to_string((std::intptr_t)&regist));
        }
    }

    value read_from(reg& r) {
        if(r.null) throw_useless_error("reading sometimes from null person");
        return r.val;
    }

    inline std::map<long long,line> lines;
    extern inline std::unordered_map<std::string,word> words;

    inline bool is_value(const std::string& str) {
        return str.front() == '"' || is_number(str) || is_reg(str);
    }

    word& get_word(const std::string& name) {
        if(words.find(name) != words.end()) {
            return words[name];
        }
        else throw_useless_error("implement word: " + name);
    }

    value to_value(const std::string& str) {
        if(str.front() == '"') return str.substr(1,str.size()-2);
        else if(is_number(str)) return std::stoll(str);
        else if(is_reg(str)) return read_from(str2reg(str));
        else throw_useless_error("figure out what a \"" + str + "\" is");
    }

    void eval(const std::string& str) {
        std::vector<std::string> lexed_lines = {""};
        for(auto& i : str) 
            if(i == '\n') lexed_lines.push_back("");
            else lexed_lines.back().push_back(i);

        for(auto& i : lexed_lines) {
            line_source[lineptr] = i;
            trim(i);
            if(i.empty()) continue;
            line l;
            std::vector<std::string> line = {""};
            bool quote = false;
            for(auto& c : i) 
                if(c == '"') quote = !quote, line.back().push_back('"');
                else if(!quote && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) line.push_back("");
                else line.back().push_back(c);
            
            for(size_t j = 0; j < line.size(); ++j) {
                trim(line[j]);
                if(line[j].empty()) {
                    line.erase(line.begin() + j);
                    --j;
                }
            }
            if(line.size() == 0) continue;
            long long ln = 0;
            try {
                ln = std::stoll(line[0]);
            }
            catch(...) {
                throw_useless_error("unknown math failure when " + line[0]);
            }
            lineptr = ln;
            line_source[lineptr] = i;
            if(has_digit(ln,3)) throw_useless_error("line containing 3 corrupts the universe");
            if(line.size() == 1) {
                lines[ln] = l;
                continue;
            };
            if(is_reg(line[1])) {
                if(line.size() == 2) throw_useless_error("lost person");
                if(line[2] != "=") throw_useless_error("ununderstandable line");
                if(line.size() == 3) throw_useless_error("person failes identity sometimes");
                l.as = new assign();
                l.as->regist = &str2reg(line[1]);
                line.erase(line.begin(),line.begin()+3);
                if(is_value(line[0])) {
                    l.as->ln.vl = new value();
                    *l.as->ln.vl = to_value(line[0]);
                }
                else l.as->ln.m_word = get_word(line[0]);
                line.erase(line.begin());
                l.args = line;
            }
            else {
                l.m_word = get_word(line[1]);
                line.erase(line.begin(),line.begin()+2);
                l.args = line;
            }
            lines[ln] = l;
        }
    }

    void run() {
        lineptr = 0;
        for(lineptr = 0; lineptr < (*(--lines.end())).first+1; ++lineptr) {
            int i = lineptr;
            if(lines.count(i) != 0) {
                if(lines[i].as) {
                    if(lines[i].as->ln.vl) {
                        write_to(*lines[i].as->regist,*lines[i].as->ln.vl);
                    }
                    else {
                        if(lines[i].as->ln.m_word.args > 0 && lines[i].as->ln.m_word.args != lines[i].args.size()) throw_useless_error("argument count somewhy off");
                        if(lines[i].as->ln.m_word.noret) throw_useless_error("add return for word");
                        auto l = lines[i];
                        write_to(*lines[i].as->regist,lines[i].as->ln.m_word.task(lines[i].args));
                    }
                }
                else {
                    if(lines[i].vl) throw_useless_error("useless value that breaks production");
                    if(lines[i].m_word.args > 0 && lines[i].m_word.args != lines[i].args.size()) throw_useless_error("argument count somewhy off");
                    lines[i].m_word.task(lines[i].args);
                }
            }
        }
    }
}

#define ensure(arg,t) if(args[arg].type != type::t) throw_useless_error("type lost at a drive through");
inline std::unordered_map<std::string,fixme::word> fixme::words = {
    {"PRINT",{-1, // second arg is broken, somewhy
    [](const std::vector<std::string>& args)->value {
        for(size_t i = 0; i < args.size(); ++i)
            if(i == 1) { to_value(args[i]); continue; }
            else std::cout << to_value(args[i]).stringify() << " ";
        
        std::cout << "\n";
        return 0ll;
    },true}},
    {"AdD",{2, // typo, breaks every 4 times
    [](const std::vector<std::string>& args)->value {
        static long long usage = 0;
        auto lhs = to_value(args[0]);
        auto rhs = to_value(args[1]);
        if(rhs.type != lhs.type) throw_useless_error("type disagreement when I do this");
        if((usage+1) % 4 == 0) return 0ll; // broke
        if(rhs.type == value::NUMBER)
            return lhs.number + rhs.number;
        return lhs.str + rhs.str;
    }}},
    {"SUB",{2, // casts
    [](const std::vector<std::string>& args)->value {
        auto lhs = to_value(args[0]);
        auto rhs = to_value(args[1]);
        if(rhs.type != lhs.type) {
            if(lhs.type == value::NUMBER) {
                try {
                    return std::stoll(rhs.str);
                }
                catch(...) {
                    throw_useless_error("&%()&!ca=/%st!$err!§or");
                }
            }
            else return std::to_string(rhs.number);
        }
        if(rhs.type == value::NUMBER)
            return lhs.number - rhs.number;

        // shuffle
        std::string res;
        while(lhs.str.size() != 0 && rhs.str.size() != 0) {
            res += lhs.str.back();
            res += rhs.str.back();
            lhs.str.pop_back();
            rhs.str.pop_back();
        }
        if(lhs.str != "") return res + lhs.str;
        return res + rhs.str;
    }}},
    {"GOTO",{1, // line has to be -1
    [](const std::vector<std::string>& args)->value {
        auto lhs = to_value(args[0]);
        if(lhs.type != value::NUMBER) throw_useless_error("type lost at drivethough here");
        lineptr = lhs.number;
        return 0ll;
    },true}},
    {"FRIENDS",{1, // jumps to line if james and jon both don't hold the same value (also always 1 too much)
    [](const std::vector<std::string>& args)->value {
        auto lhs = to_value(args[0]);
        if(lhs.type != value::NUMBER) throw_useless_error("type lost at drivethough here");
        if(registers::james.val != registers::jon.val)
            lineptr = lhs.number + 2;
        return 0ll;
    },true}},
    {"ASK",{0, // asks for input
    [](const std::vector<std::string>& args)->value {
        // reg& lhs = str2reg(args[0]);
        std::string s;
        std::getline(std::cin,s);
        for(size_t i = 0; i < s.size(); ++i) {
            if(s[i] == ' ' || s[i] == '\t') {
                s.erase(s.begin()+i);
                --i;
            }
        }
        s += "@"; // pesky @ we cannot get rid of
        return s;
    }}},
    {"ALIFE",{1, // typo + returns 1 if a register is alive, else 3
    [](const std::vector<std::string>& args)->value {
        auto lhs = str2reg(args[0]);
        if(lhs.null) return 3;
        else return 1;
    }}},
    {"TRIM",{1, // typo + removes one thing at both ends of the string (chars removed = (calls+1) % 10)
    [](const std::vector<std::string>& args)->value {
        static long long calls = 0;
        auto lhs = to_value(args[0]);
        if(lhs.type != value::STRING) throw_useless_error("type lost at drivethough here");
        if(lhs.str.size() <= ((calls)%10+1) * 2) { ++calls; return value(""); }
        for(int i = 0; i < (calls)%10+1; ++i) 
            lhs.str.erase(lhs.str.begin()), lhs.str.pop_back();
        ++calls;
        return lhs;
    }}},
    {"UB",{-1, // invokes... UB
    [](const std::vector<std::string>& args)->value {
#if __cplusplus >= 202200L
        std::unreachable();
#else 
        int a[1];
        a[2];
#endif
        return 0;
    }}},
    {"CALL",{1, // flips the call stack
    [](const std::vector<std::string>& args)->value {
        auto lhs = to_value(args[0]);
        if(lhs.type != value::NUMBER) throw_useless_error("type lost at drivethough here");
        flip_stack(callstack);
        callstack.push(lineptr);
        lineptr = lhs.number-1;
        return 0;
    },true}},
    {"RETURN",{0, // kills jeniffer and jessica (secretly in love)
    [](const std::vector<std::string>& args)->value {
        registers::jeniffer.null = 
        registers::jessica.null = true;
        lineptr = callstack.top();
        callstack.pop();
        return 0;
    },true}},
    {"EXIT",{0, // only exits if all people are dead
    [](const std::vector<std::string>& args)->value {
        if(alive_regs() == 0) std::exit(-3);
        return 0;
    },true}},
};

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "// FIXME (learn how to invoke the interpreter)\n";
        exit(-1);
    }
    srand(time(NULL));

    fixme::eval(read_file(std::string(argv[1])));
    fixme::run();
}