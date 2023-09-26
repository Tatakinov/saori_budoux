#ifndef SAORI_BUDOUX_UTF8STRING_H_
#define SAORI_BUDOUX_UTF8STRING_H_

class utf8char {

    private:

        std::string ch_;
        size_t width_;

        utf8char(const std::string ch) {
            ch_ = ch;
            auto size   = ch.size();
            if (size == 0) {
                width_  = 0;
            }
            else if (size == 1) {
                width_  = 1;
            }
            else if (size == 2) {
                width_  = 1;
            }
            else if (size == 3 &&
                    static_cast<unsigned char>(ch[0]) == 0xef &&
                    static_cast<unsigned char>(ch[1]) >= 0xbc &&
                    static_cast<unsigned char>(ch[2]) >= 0x80) {
                width_  = 1;
            }
            else {
                width_  = 2;
            }
        }

    public:

        ~utf8char() {}

        inline std::string value() const {
            return ch_;
        }

        inline size_t size() const {
            return ch_.size();
        }

        inline size_t width() const {
            return width_;
        }

        static utf8char parse(std::string str) {
            auto length  = str.size();
            if (length >= 1 &&
                    static_cast<unsigned char>(str[0]) >= 0x20 &&
                    static_cast<unsigned char>(str[0]) <= 0x7e) {
                return utf8char(str.substr(0, 1));
            }
            else if (length >= 2 &&
                    static_cast<unsigned char>(str[0]) >= 0xc2 &&
                    static_cast<unsigned char>(str[0]) <= 0xe0 &&
                    static_cast<unsigned char>(str[1]) >= 0x80 &&
                    static_cast<unsigned char>(str[1]) < 0xc0) {
                return utf8char(str.substr(0, 2));
            }
            else if (length >= 3 &&
                    static_cast<unsigned char>(str[0]) >= 0xe0 &&
                    static_cast<unsigned char>(str[0]) <= 0xf0 &&
                    static_cast<unsigned char>(str[1]) >= 0x80 &&
                    static_cast<unsigned char>(str[1]) < 0xc0 &&
                    static_cast<unsigned char>(str[2]) >= 0x80 &&
                    static_cast<unsigned char>(str[2]) < 0xc0) {
                return utf8char(str.substr(0, 3));
            }
            else if (length >= 4 &&
                    static_cast<unsigned char>(str[0]) >= 0xf0 &&
                    static_cast<unsigned char>(str[0]) <= 0xf5 &&
                    static_cast<unsigned char>(str[1]) >= 0x80 &&
                    static_cast<unsigned char>(str[1]) < 0xc0 &&
                    static_cast<unsigned char>(str[2]) >= 0x80 &&
                    static_cast<unsigned char>(str[2]) < 0xc0 &&
                    static_cast<unsigned char>(str[3]) >= 0x80 &&
                    static_cast<unsigned char>(str[3]) < 0xc0) {
                return utf8char(str.substr(0, 4));
            }
            return utf8char("");
        }
};

class utf8string {

    private:

        std::vector<utf8char> str_;

        utf8string() {}

    public:

        static utf8string parse(const std::string str) {
            utf8string instance;
            int index   = 0;
            const char *p   = str.c_str();
            while (index < str.size()) {
                auto ch = utf8char::parse(&p[index]);
                if (ch.size() == 0) {
                    break;
                }
                instance.str_.push_back(ch);
                index   += ch.size();
            }
            return instance;
        }

        ~utf8string() {}

        std::string value() const {
            std::string v   = "";
            for (auto& ch : str_) {
                v   += ch.value();
            }
            return v;
        };

        size_t size() const {
            return str_.size();
        };

        size_t width() const {
            size_t w    = 0;
            for (auto& ch : str_) {
                w   += ch.width();
            }
            return w;
        };

        utf8string substr(size_t begin, size_t end) {
            utf8string ret;
            for (int i = 0; i < end; i++) {
                ret.str_.push_back(str_[begin + i]);
            }
            return ret;
        }

        utf8char& operator[](int i) {
            return str_[i];
        }

        utf8string& operator+=(const utf8char rhs) {
            str_.push_back(rhs);
            return *this;
        }

        utf8string& operator+=(const utf8string rhs) {
            std::copy(rhs.str_.begin(), rhs.str_.end(), std::back_inserter(str_));
            return *this;
        }
};

#endif // SAORI_BUDOUX_UTF8STRING_H_
