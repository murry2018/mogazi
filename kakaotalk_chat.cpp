#include <stdexcept>
#include <regex>
#include "KakaoTalk.hpp"

using std::string;         using std::istream;
using std::runtime_error;  using std::domain_error;
using std::regex;          using std::regex_match;

namespace kakaotalk {

  // default constructor
  chat::chat() { }

  chat::chat(istream& in) {
    if (in) {
      string s;
      int c;
      while (in && (c = in.get()) != '\r')
        s.push_back(c);
      c = in.get(); // eliminate '\n'
      if (!(c == '\n' || c == EOF))
        throw runtime_error("Unexpected File Format");
      full_content = s;
      analyse();
    } else {
      throw domain_error("End of File");
    }
  }

  std::istream& operator>>(std::istream& in, kakaotalk::chat& res) {
    if (in)
      res = chat(in);
    return in;
  }

  void chat::analyse() {
    static const regex pattern_common("\
^(\\d+)년 (\\d+)월 (\\d+)일 (오전|오후) (\\d+):(\\d+), ((?:.|\\n)*)");
    static const regex pattern_in("(?:([^:]*)님이 들어왔습니다\\.((?:.|\\n)*)|(?:[^:]*)님이 ([^:]*)님을 초대했습니다\\.)");
    static const regex pattern_out("([^:]*)님(이 나갔습니다|을 내보냈습니다)\\.");
    static const regex pattern_talk("(?:([^:]*) : ?)((?:.|\\n)*)");
    static const string pattern_extra1("삭제된 메시지입니다.");
    static const string pattern_extra2("채팅방 관리자가 메시지를 가렸습니다.");
    
    std::smatch m_common;
    if (regex_match(full_content, m_common, pattern_common)) {
      string chat_content = m_common[7];
      std::smatch m_content;
      if (regex_match(chat_content, m_content, pattern_talk)) {
        /* 떠드는 패턴인 경우: 

           '2019년 5월 27일 오전 12:22, 악성유저 : 악성유저님이 나갔습니다.'

           이와 같은 특수 케이스로부터 방어하기 위해 떠드는 패턴을
           우선적으로 체크한다. */
        type = chat::TALK;
        _name = m_content[1], _content = m_content[2];
      } else if (regex_match(chat_content, m_content, pattern_in)) {
        // 들어온 패턴인 경우 (나가는 패턴과의 확인 순서는 중요치 않다)
        type = chat::IN;
        _name = m_content[1];
        if (m_content[2] != "") {
          /* 들어온 사람이 자기자신인지 확인: 자기 자신은 들어올땐
             이름으로 표시되고 떠들땐 회원님이라 표시되기 때문에 이를
             구분해주지 않으면 가만히 있는 사람 한명이 더 생긴다. */
          _myself = true;
        } else {
          _myself = false;
        }
      } else if (regex_match(chat_content, m_content, pattern_out)) {
        // 나가는 패턴인 경우
        type = chat::OUT;
        _name = m_content[1];
      } else if (chat_content == pattern_extra1
                 || chat_content == pattern_extra2) {
        type = chat::EXTRA;
      } else {
        // '2019년 5월 27일 오전 12:22, 이런시발' 과 같은 잘못된 형식인 경우
        std::cout << chat_content << std::endl;
        throw runtime_error("Unexpected Chat Pattern");
      }
    } else {
      /* 채팅 내용이 아닌 패턴 */
      type = chat::EXTRA;
    }
  }
  
  bool chat::isin() const
  { return type == chat::IN; }
  
  bool chat::isout() const
  { return type == chat::OUT; }
  
  bool chat::istalk() const
  { return type == chat::TALK; }
  
  bool chat::isextra() const
  { return type == chat::EXTRA; }

  bool chat::ismyself() const
  { return _myself; }

  
  const std::string& chat::content() const {
    return full_content;
  }
  
  const std::string& chat::message() const {
    if (type != chat::TALK)
      throw domain_error("Given chat has no talking pattern.");
    return _content;
  }

  const std::string& chat::name() const {
    if (type == chat::EXTRA)
      throw domain_error("Given chat has no name information.");
    return _name;
  }
    
} // end of namespace kakaotalk

#if defined( __KAKAO_TALK_CHAT_DEBUG )

#include <fstream>

void kakaotalk::chat::test_cons1() {
  const string name = "kakaotalk::chat(istream&) creation test";
  using namespace std;
  ifstream chats("Chats.txt");
  while (true) {
    try {
      chat c(chats);
      cout << c.full_content << endl;
    } catch (domain_error e) {
      cout << e.what() << endl;
    }
    cout << "continue >> ";
    while(cin.get() != '\n');
  }
}

void kakaotalk::chat::test_istream() {
  const string name = "istream& >> kakaotalk::chat test";
  using namespace std;
  std::ifstream chats("Chats.txt");
  chat c;
  while (chats >> c) {
    cout << c.full_content << endl << "continue >> ";
    while(cin.get() != '\n');
  }
}

void kakaotalk::chat::test_patterns() {
  using namespace std;
  ifstream chats("Chats.txt");
  chat c;
  while (chats >> c) {
    if (c.isin()) {
      cout << "들어온자: " << c.name()
           << (c.ismyself()? " 본인": "") << endl;
    } else if (c.isout()) {
      cout << "나간자: " << c.name() << endl;
    } else if (c.istalk()) {
      cout << "떠든사람: " << c.name() << endl
           << "내용: " << c.message() << endl;
    } else {
      cout << "[INFO::" << c.content() << "::INFO]" << endl;
    }
    cout << "Continue >> ";
    while (cin.get() != '\n');
  }
}

#endif
