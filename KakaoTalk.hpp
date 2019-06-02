#if !defined( GUARD__KakaoTalk_hpp )
#define GUARD__KakaoTalk_hpp

// #define __KAKAO_TALK_CHAT_DEBUG // for test kakaotalk::chat class

// KakaoTalk.hpp
#include <iostream>
#include <string>
#include <map>
#include <list>

namespace kakaotalk {
  class chat {
  public:
    /*** Type & Constants ***/
    enum chat_type { IN, OUT, TALK, EXTRA };
    
    /*** Constructors ***/
    chat();                       // 유의: 디폴트 생성자로 생성된
                                  // 객체에 read를 시도하지 마시오.
    chat(std::istream&);       
    // chat(const std::string&);  // 미구현: 당장은 필요치 않음
    
    /*** member functions ***/
    // chat_type type() const;
    bool isin() const;
    bool isout() const;
    bool istalk() const;
    bool isextra() const;
    bool ismyself() const;
    const std::string& content() const;
    const std::string& message() const;
    const std::string& name() const;
    friend std::istream& operator>>(std::istream&, kakaotalk::chat&);
    
# if defined ( __KAKAO_TALK_CHAT_DEBUG )
    /*** Test Functions ***/
    static void test_cons1();
    static void test_istream();
    static void test_patterns();
# endif
    
  private:
    void analyse();
    std::string full_content;
    chat_type type;
    std::string _content;
    std::string _name;
    bool _myself;
  };

  class mem_info {
  public:
    mem_info(); // NULL member
  
    bool exist() const;
    operator bool() const; // synonym for exist()
    void in();
    void out();

    int nchat() const;
    operator int() const; // synonym for nchat()
    mem_info& operator++();
    mem_info& operator+=(int n);

    bool operator<(const mem_info& rhs) const;
  private:
    bool _exist;
    int _nchat;
  };

  class room {
  public:
    typedef unsigned long long size_type;
    room(std::istream&);
    std::list< std::pair<std::string, mem_info> > enrolls;
    size_type member_total() const;
    const std::string& room_name() const;
  private:
    size_type nmemb;
    std::string name;
    std::map<std::string, mem_info> members;
  };
}

#endif
